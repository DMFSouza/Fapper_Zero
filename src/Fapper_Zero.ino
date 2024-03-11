  #ifndef BOARD_HAS_PSRAM
#error "PSRAM is not ENABLE, Please set ArduinoIDE PSRAM option to OPI!"
#endif

#include "Arduino.h"
#include "FS.h"
#include "SD_MMC.h"
#include "SPIFFS.h"
#include "driver/i2s.h"
#include "global_flags.h"
#include "pin_config.h"
#include "ui.h"
#include "wifi_menu.h"
/* external library */
#include "APA102.h"     // https://github.com/pololu/apa102-arduino
#include "Audio.h"      // https://github.com/schreibfaul1/ESP32-audioI2S
#include "TFT_eSPI.h"   // https://github.com/Bodmer/TFT_eSPI
#include "arduinoFFT.h" // https://github.com/kosme/arduinoFFT
// #include <NimBLEDevice.h>  // https://github.com/h2zero/NimBLE-Arduino
#include <OneButton.h>     // https://github.com/mathertel/OneButton
#include <RotaryEncoder.h> // https://github.com/mathertel/RotaryEncoder
#include "lv_conf.h"
#include <lvgl.h> // https://github.com/lvgl/lvgl
#include <RadioLib.h>
#include <Adafruit_PN532.h>

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Fapper_zero.h"
#include <nvs_flash.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


String text;
#define WIFI_CHANNEL_SWITCH_INTERVAL  (500)
#define WIFI_CHANNEL_MAX               (13)

uint8_t level = 0, channel = 1;

static wifi_country_t wifi_country = {.cc="BR", .schan = 1, .nchan = 13};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
typedef struct {
    uint8_t cmd;
    uint8_t data[14];
    uint8_t len;
} lcd_cmd_t;

lcd_cmd_t lcd_st7789v[] = {
    {0x11, {0}, 0 | 0x80},
    {0x3A, {0X05}, 1},
    {0xB2, {0X0B, 0X0B, 0X00, 0X33, 0X33}, 5},
    {0xB7, {0X75}, 1},
    {0xBB, {0X28}, 1},
    {0xC0, {0X2C}, 1},
    {0xC2, {0X01}, 1},
    {0xC3, {0X1F}, 1},
    {0xC6, {0X13}, 1},
    {0xD0, {0XA7}, 1},
    {0xD0, {0XA4, 0XA1}, 2},
    {0xD6, {0XA1}, 1},
    {0xE0, {0XF0, 0X05, 0X0A, 0X06, 0X06, 0X03, 0X2B, 0X32, 0X43, 0X36, 0X11, 0X10, 0X2B, 0X32}, 14},
    {0xE1, {0XF0, 0X08, 0X0C, 0X0B, 0X09, 0X24, 0X2B, 0X22, 0X43, 0X38, 0X15, 0X16, 0X2F, 0X37}, 14},
};

TFT_eSPI tft = TFT_eSPI(170, 320);
RotaryEncoder encoder(PIN_ENCODE_A, PIN_ENCODE_B, RotaryEncoder::LatchMode::TWO03);
OneButton button(PIN_ENCODE_BTN, true);

APA102<PIN_APA102_DI, PIN_APA102_CLK> ledStrip;
int wifi_init = false; // When the WIFI initialization is completed, switch the mic spectrum mode.
EventGroupHandle_t global_event_group;

/*
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
           |  mode |brightness
*/

int batteryLevel =100;
int batteryLevelAnt =0;
int rawValue;
bool sniff_init = false;
lv_obj_t *labelBattery =nullptr;

bool lBattery = true;
void drawBatteryMeter();
static EventGroupHandle_t lv_input_event;

static bool isCoderOnline = false;

void ui_task(void *param);

static lv_obj_t *create_btn(lv_obj_t *parent,const char *text);
lv_obj_t *log_label=nullptr;
void SD_init(void);
typedef struct {
  unsigned frame_ctrl:16;
  unsigned duration_id:16;
  uint8_t addr1[6]; /* receiver address */
  uint8_t addr2[6]; /* sender address */
  uint8_t addr3[6]; /* filtering address */
  unsigned sequence_ctrl:16;
  uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

static esp_err_t event_handler(void *ctx, system_event_t *event);
void wifi_sniffer_init(void);
static void wifi_sniffer_set_channel(uint8_t channel);
static const char *wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type);
static void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type);

esp_err_t event_handler(void *ctx, system_event_t *event)
{
  return ESP_OK;
}

void wifi_sniffer_init(void)
{

  nvs_flash_init();
  lv_obj_clean(lv_scr_act());
  log_label = lv_label_create(lv_scr_act());
  lv_obj_set_size(log_label,260,140);

  lv_obj_set_width(log_label, LV_PCT(100));
  
  lv_label_set_long_mode(log_label, LV_LABEL_LONG_SCROLL);
  lv_label_set_recolor(log_label, true);
  sniff_init=true;
  tcpip_adapter_init();
  ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
  ESP_ERROR_CHECK( esp_wifi_set_country(&wifi_country) ); /* set country for channel range [1, 13] */
  ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_NULL) );
  ESP_ERROR_CHECK( esp_wifi_start() );
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
}

void wifi_sniffer_set_channel(uint8_t channel)
{
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
}

const char * wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type)
{
  switch(type) {
  case WIFI_PKT_MGMT: return "MGMT";
  case WIFI_PKT_DATA: return "DATA";
  default:  
  case WIFI_PKT_MISC: return "MISC";
  }
}

void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type)
{

  if (type != WIFI_PKT_MGMT)
    return;

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buff;
  const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;
  std::string packetTypeStr = wifi_sniffer_packet_type2str(type);
  std::ostringstream text;

text <<"Scan WiFi"<< "\n"
     << "  PACKET TYPE=" << packetTypeStr<< "\n"
     << "  CHAN=" << std::setw(2) << std::setfill('0') << ppkt->rx_ctrl.channel<< "\n"
     << "  RSSI=" << std::setw(2) << std::setfill('0') << ppkt->rx_ctrl.rssi<< "\n"
     << "  ADDR1=" << std::hex << std::setw(2) << std::setfill('0')
     << static_cast<int>(hdr->addr1[0]) << ":" << std::setw(2) << static_cast<int>(hdr->addr1[1])
     << ":" << std::setw(2) << static_cast<int>(hdr->addr1[2]) << ":"
     << std::setw(2) << static_cast<int>(hdr->addr1[3]) << ":" << std::setw(2) << static_cast<int>(hdr->addr1[4])
     << ":" << std::setw(2) << static_cast<int>(hdr->addr1[5]) << "\n"
     << "  ADDR2=" << std::hex << std::setw(2) << std::setfill('0')
     << static_cast<int>(hdr->addr2[0]) << ":" << std::setw(2) << static_cast<int>(hdr->addr2[1])
     << ":" << std::setw(2) << static_cast<int>(hdr->addr2[2]) << ":"
     << std::setw(2) << static_cast<int>(hdr->addr2[3]) << ":" << std::setw(2) << static_cast<int>(hdr->addr2[4])
     << ":" << std::setw(2) << static_cast<int>(hdr->addr2[5]) << "\n"
     << "  ADDR3=" << std::hex << std::setw(2) << std::setfill('0')
     << static_cast<int>(hdr->addr3[0]) << ":" << std::setw(2) << static_cast<int>(hdr->addr3[1])
     << ":" << std::setw(2) << static_cast<int>(hdr->addr3[2]) << ":"
     << std::setw(2) << static_cast<int>(hdr->addr3[3]) << ":" << std::setw(2) << static_cast<int>(hdr->addr3[4])
     << ":" << std::setw(2) << static_cast<int>(hdr->addr3[5]) << "\n";

std::string resultString = text.str();
lv_label_set_text(log_label, resultString.c_str());
timeClient.update();
String dateTimeString =timeClient.getFormattedTime();
File f = SD_MMC.open("/wifi/sniffer_logs/logs.txt", FILE_APPEND);
  if (f) {
    f.println(dateTimeString.c_str());
    f.println(resultString.c_str());
    f.close();
  }  
}
static void lv_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h);
    lv_disp_flush_ready(disp);
}

static void lv_encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{

    EventGroupHandle_t *lv_input_event = (EventGroupHandle_t *)indev_drv->user_data;
    EventBits_t bit = xEventGroupGetBits(lv_input_event);
    data->state = LV_INDEV_STATE_RELEASED;
    if (bit & LV_BUTTON) {
        xEventGroupClearBits(lv_input_event, LV_BUTTON);
        data->state = LV_INDEV_STATE_PR;
    } else if (bit & LV_ENCODER_CW) {
        xEventGroupClearBits(lv_input_event, LV_ENCODER_CW);
        data->enc_diff = 1;
    } else if (bit & LV_ENCODER_CCW) {
        xEventGroupClearBits(lv_input_event, LV_ENCODER_CCW);
        data->enc_diff = -1;
    }
}

void setup()
{
    global_event_group = xEventGroupCreate(); 

    lv_input_event = xEventGroupCreate();
    
    pinMode(PIN_POWER_ON, OUTPUT);
    digitalWrite(PIN_POWER_ON, HIGH);

    Serial.begin(115200);

    SPIFFS.begin(true);

   
   button.attachLongPressStart(
    []() {
    
      digitalWrite(PIN_APA102_CLK, LOW);
      digitalWrite(PIN_APA102_DI, LOW);
      digitalWrite(RADIO_CS_PIN, LOW);  // Desativa rádio (exemplo)
      digitalWrite(PIN_RF_MISO, LOW);
      digitalWrite(PIN_RF_RST, LOW);
      delay(100);
      digitalWrite(PIN_LCD_BL, LOW);
      digitalWrite(PIN_POWER_ON, LOW);
      esp_deep_sleep_start();
    }
   );
    
    SD_init();

    xTaskCreatePinnedToCore(ui_task, "ui_task", 1024 * 40, NULL, 3, NULL, 1);

}


void drawBatteryMeter() {

  
  
  batteryLevelAnt = batteryLevel;

   if (labelBattery!=nullptr) {
    lv_obj_del(labelBattery);
  }
  labelBattery = lv_label_create(lv_scr_act());
  lv_obj_set_width(labelBattery, 100);
  lv_obj_set_style_text_align(labelBattery, LV_TEXT_ALIGN_RIGHT, 0);
  lv_obj_align(labelBattery, LV_ALIGN_TOP_RIGHT, 0, 0);
 
  // Preenche o medidor com a cor indicando o nível de carga
  if (batteryLevel > 90) {
    lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_FULL);
    lv_obj_set_style_text_color(labelBattery,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
    
  } else if (batteryLevel > 70 && batteryLevel<90) {
   lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_3);
   lv_obj_set_style_text_color(labelBattery,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
    
  } else if (batteryLevel> 50 && batteryLevel<70) {
    lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_2);
    lv_obj_set_style_text_color(labelBattery, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_MAIN);
    
  } else if (batteryLevel> 20 && batteryLevel<50) {
    lv_label_set_text(labelBattery,LV_SYMBOL_BATTERY_1);
    lv_obj_set_style_text_color(labelBattery, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_MAIN);
    
  }else if (batteryLevel< 20 ) {
    lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_EMPTY);
    lv_obj_set_style_text_color(labelBattery,lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

 }
}


void loop()
{  
    rgb_color colors[7];
    uint8_t brightness = 1;
    static uint64_t time;
    time++;
    dnsServer.processNextRequest();
    server.handleClient();
    for (uint16_t i = 0; i < 7; i++) {
        colors[i] = hsvToRgb((uint32_t)time * 359 / 256, 255, 255);
    }
    ledStrip.write(colors, 7, brightness);
    delay(1000);
    if (sniff_init){
       wifi_sniffer_set_channel(channel);
      channel = (channel % WIFI_CHANNEL_MAX) + 1;
      delay(2000);
    }
}


void ui_task(void *param)
{
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t *buf1, *buf2;

    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    // Update Embed initialization parameters
    for (uint8_t i = 0; i < (sizeof(lcd_st7789v) / sizeof(lcd_cmd_t)); i++) {
        tft.writecommand(lcd_st7789v[i].cmd);
        for (int j = 0; j < (lcd_st7789v[i].len & 0x7f); j++) {
            tft.writedata(lcd_st7789v[i].data[j]);
        }

        if (lcd_st7789v[i].len & 0x80) {
            delay(120);
        }
    }

    button.attachClick(
        [](void *param) {
        if (lBattery){
          labelBattery = lv_label_create(lv_scr_act());
          lv_obj_set_width(labelBattery, 100);
          lv_obj_set_style_text_align(labelBattery, LV_TEXT_ALIGN_RIGHT, 0);
          lv_obj_align(labelBattery, LV_ALIGN_TOP_RIGHT, 0, 0);
          lv_label_set_text(labelBattery, LV_SYMBOL_BATTERY_FULL);
          lv_obj_set_style_text_color(labelBattery,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
          lBattery=false;
      }
    rawValue = analogRead(PIN_BAT_VOLT);
    batteryLevel = map(rawValue, 95, 2840, 0, 100);
    
    if (batteryLevel!=batteryLevelAnt){
        drawBatteryMeter();
    }
        EventGroupHandle_t *lv_input_event = (EventGroupHandle_t *)param;
        xEventGroupSetBits(lv_input_event, LV_BUTTON);
    }, 
    lv_input_event);

    lv_init();
    buf1 = (lv_color_t *)ps_malloc(LV_BUF_SIZE * sizeof(lv_color_t));
    assert(buf1);
    buf2 = (lv_color_t *)ps_malloc(LV_BUF_SIZE * sizeof(lv_color_t));
    assert(buf2);
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LV_BUF_SIZE);
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = LV_SCREEN_WIDTH;
    disp_drv.ver_res = LV_SCREEN_HEIGHT;
    disp_drv.flush_cb = lv_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = lv_encoder_read;
    indev_drv.user_data = lv_input_event;
    static lv_indev_t *lv_encoder_indev = lv_indev_drv_register(&indev_drv);
    lv_group_t *g = lv_group_create();
    lv_indev_set_group(lv_encoder_indev, g);
    lv_group_set_default(g);

    LV_IMG_DECLARE(image_hunter_0047);
    lv_obj_t *fcc = lv_img_create(lv_scr_act());
    lv_img_set_src(fcc, &image_hunter_0047);
    lv_task_handler();
    delay(3000);
    lv_obj_del(fcc);
    
    //Show lilyg embed fcc image
    lv_obj_clean(lv_scr_act());
    
    xEventGroupSetBits(lv_input_event, LV_UI_DEMO_START);
  
    attachInterrupt( 
        digitalPinToInterrupt(PIN_ENCODE_A), []() {
        encoder.tick();
    }, CHANGE);
    attachInterrupt( 
        digitalPinToInterrupt(PIN_ENCODE_B), []() {
        encoder.tick();
    }, CHANGE);

    while (1) {
        delay(1);
        button.tick();
        lv_timer_handler();

        RotaryEncoder::Direction dir = encoder.getDirection();
        if (dir != RotaryEncoder::Direction::NOROTATION) {
            if (dir != RotaryEncoder::Direction::CLOCKWISE) {
                xEventGroupSetBits(lv_input_event, LV_ENCODER_CW);
            } else {
                xEventGroupSetBits(lv_input_event, LV_ENCODER_CCW);
            }
        }

        EventBits_t bit = xEventGroupGetBits(lv_input_event);
        if (bit & LV_UI_DEMO_START) {
            xEventGroupClearBits(lv_input_event, LV_UI_DEMO_START);
            ui_init();
        }
    }

    vTaskDelete(NULL);
}

static lv_obj_t *create_btn(lv_obj_t *parent,const char *text)
{

    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 120, 100);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, text);
    return btn;
}

// clang-format off
/* Converts a color from HSV to RGB.
 * h is hue, as a number between 0 and 360.
 * s is the saturation, as a number between 0 and 255.
 * v is the value, as a number between 0 and 255. */
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
  {
      // Define um valor de matiz fixo para vermelho (0°)
      uint16_t fixedHue = 0;
      
      uint8_t f = ((fixedHue % 60) * 255) / 60;
      uint8_t p = (255 - s) * static_cast<uint16_t>(v) / 255;
      uint8_t q = (255 - f * static_cast<uint16_t>(s) / 255) * static_cast<uint16_t>(v) / 255;
      uint8_t t = (255 - (255 - f) * static_cast<uint16_t>(s) / 255) * static_cast<uint16_t>(v) / 255;
      uint8_t r = 0, g = 0, b = 0;
      
      switch ((fixedHue / 60) % 6) {
          case 0: r = v; g = t; b = p; break;
          case 1: r = q; g = v; b = p; break;
          case 2: r = p; g = v; b = t; break;
          case 3: r = p; g = q; b = v; break;
          case 4: r = t; g = p; b = v; break;
          case 5: r = v; g = p; b = q; break;
      }
      
      return rgb_color(r, g, b);
  }



void uint8_to_hexstr(uint8_t *in_data,int len,char *out_data)
{
    int i = 0, out_cont = 0;
    for(i = 0; i < len; i++)
    {
        //Serial.printf("in_data[%d] = %d ", i, in_data[i]);
        if(in_data[i] > 15)
        {
            out_data[out_cont++] = (in_data[i]/16) >= 10?('A'+(in_data[i]/16)-10):('0'+in_data[i]/16);
        }
        else 
            out_data[out_cont++] = '0';
        out_data[out_cont++] = (in_data[i]%16) >= 10?('A'+(in_data[i]%16)-10):('0'+in_data[i]%16);
        out_data[out_cont++] = ' ';
    }
    out_data[out_cont] = '\0';
}



void SD_init(void)
{
    pinMode(PIN_SD_CS, OUTPUT);
    digitalWrite(PIN_SD_CS, 1);
    SD_MMC.setPins(PIN_SD_SCK, PIN_SD_MOSI, PIN_SD_MISO);
    if (!SD_MMC.begin("/sdcard", true)) {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD_MMC.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }
    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    delay(500);
}



// flag to indicate that a packet was received
//volatile bool receivedFlag = false;
// disable interrupt when it's not needed
volatile bool enableInterrupt = true;
// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void) {
  // check if the interrupt is enabled
  if(!enableInterrupt) {
    return;
  }

   
}


void float_to_str(float in_data, int decimal_place, char *out_data)
{
    int out_len_cont = 0;
    u_int32_t divisor = 1, decimal_place_divisor = 1;
    if(in_data < 0)
    {
        out_data[out_len_cont++] = '-';
        in_data *= -1;
    }

    while((int)in_data / divisor > 0)
    {
        divisor*=10;
    }

    divisor/=10;

    int i = 0;
    while(i++ < decimal_place)
    {
        in_data*=10;
        divisor*=10;
        decimal_place_divisor *= 10;
    }
    
    while(divisor >= decimal_place_divisor)
    {
        out_data[out_len_cont++] = (int)in_data/divisor+'0';
        in_data = (int)in_data % divisor;
        divisor/=10;
    }
    out_data[out_len_cont++] = '.';
    while(decimal_place-- > 0)
    {
        out_data[out_len_cont++] = ((int)in_data)/divisor+'0';
        in_data = (int)in_data % divisor;
        divisor/=10;
    }
//Serial.printf("float_to_str end \n");
}
