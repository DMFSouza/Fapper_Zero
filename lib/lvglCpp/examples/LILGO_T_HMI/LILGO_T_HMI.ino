#include "Arduino.h"

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_rgb.h>
#include <esp_lcd_panel_vendor.h>

#include <lvgl.h>
#include <lvglCpp.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <OneButton.h>
#include <SPI.h>


#include "pins.h"
#include "xpt2046.h" // https://github.com/liangyingy/arduino_xpt2046_library.git

#include "data.h"
#include "logo.h"
#include "dbg.h"


/**
 * Please update the following configuration according to your LCD spec
 */
#define LCD_PIXEL_CLOCK_HZ     (10 * 1000 * 1000)
#define LCD_BK_LIGHT_ON_LEVEL  (1)
#define LCD_BK_LIGHT_OFF_LEVEL (!LCD_BK_LIGHT_ON_LEVEL)

/**
 * The pixel number in horizontal and vertical
 */
#define LCD_H_RES              (240)
#define LCD_V_RES              (320)

/**
 * Bit number used to represent command and parameter
 */
#define LCD_CMD_BITS           (8)
#define LCD_PARAM_BITS         (8)

XPT2046 touch = XPT2046(SPI, TOUCHSCREEN_CS_PIN, TOUCHSCREEN_IRQ_PIN);

OneButton buttonOK(21, false, false);
OneButton buttonNext(BUTTON1_PIN, true, false);

uint8_t currentScreen = 0;

portMUX_TYPE flushLock = (portMUX_TYPE) portMUX_INITIALIZER_UNLOCKED;

static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
static lv_disp_drv_t disp_drv;      // contains callback functions
static lv_indev_drv_t indev_drv;

/*********************************************************/
static void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map) {
	esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;
	//taskENTER_CRITICAL(&flushLock);
	esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_map);
	//taskEXIT_CRITICAL(&flushLock);
	lv_disp_flush_ready(drv);
}
/*********************************************************/
static void lv_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
	XPT2046 *touch = (XPT2046*) indev_driver->user_data;
	if (touch->pressed()) {
		data->state = LV_INDEV_STATE_PR;
		data->point.x = touch->X();
		data->point.y = touch->Y();
		// DBG_print("X: ");
		// DBG_print(data->point.x);
		// DBG_print(", Y: ");
		// DBG_println(data->point.y);
	} else {
		data->state = LV_INDEV_STATE_REL;
	}
}
/*********************************************************/
static void power_off() {
	DBG_println("shutting down...");
	delay(100);
	digitalWrite(PWR_ON_PIN, LOW);
}
/*********************************************************/
static void lvglTask(void *param) {
	while (1) {
		buttonOK.tick();
		buttonNext.tick();
		delay(50);
	}
	vTaskDelete(NULL);
}

extern void uiInit();

void setup() {
	Serial.begin(115200);

	pinMode(PWR_ON_PIN, OUTPUT);
	digitalWrite(PWR_ON_PIN, HIGH);

	DBG_println("Power on peripherals, such as the LCD backlight");
	pinMode(PWR_EN_PIN, OUTPUT);
	digitalWrite(PWR_EN_PIN, HIGH);

	DBG_println("Initialize Intel 8080 bus");
	esp_lcd_i80_bus_handle_t i80_bus = NULL;
	esp_lcd_i80_bus_config_t bus_config = {
	    .dc_gpio_num = DC_PIN,
	    .wr_gpio_num = PCLK_PIN,
	    .clk_src = LCD_CLK_SRC_PLL160M,
	    .data_gpio_nums = {
	        LCD_DATA0_PIN,
	        LCD_DATA1_PIN,
	        LCD_DATA2_PIN,
	        LCD_DATA3_PIN,
	        LCD_DATA4_PIN,
	        LCD_DATA5_PIN,
	        LCD_DATA6_PIN,
	        LCD_DATA7_PIN,
	    },
	    .bus_width = 8,
	    .max_transfer_bytes = LCD_H_RES * LCD_V_RES * sizeof(uint16_t)
	};
	ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));

	esp_lcd_panel_io_handle_t io_handle = NULL;
	esp_lcd_panel_io_i80_config_t io_config = {
	    .cs_gpio_num = CS_PIN,
	    .pclk_hz = LCD_PIXEL_CLOCK_HZ,
	    .trans_queue_depth = 10,
	    // .on_color_trans_done = notify_lvgl_flush_ready,
	    .user_ctx = &disp_drv,
	    .lcd_cmd_bits = LCD_CMD_BITS,
	    .lcd_param_bits = LCD_PARAM_BITS,
	    .dc_levels = {
	        .dc_idle_level = 0,
	        .dc_cmd_level = 0,
	        .dc_dummy_level = 0,
	        .dc_data_level = 1,
	    },
	    .flags = {
	        .swap_color_bytes = 1,
	    }
	};
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));

	DBG_println("Install LCD driver of st7789");
	esp_lcd_panel_handle_t panel_handle = NULL;
	esp_lcd_panel_dev_config_t panel_config = {
	    .reset_gpio_num = RST_PIN,
	    .color_space = ESP_LCD_COLOR_SPACE_RGB,
	    .bits_per_pixel = 16,
	};
	ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

	DBG_println("Initialize LCD driver of st7789");
	esp_lcd_panel_reset(panel_handle);
	esp_lcd_panel_init(panel_handle);
	esp_lcd_panel_mirror(panel_handle, false, true);
	esp_lcd_panel_swap_xy(panel_handle, true);

	DBG_println("Turn on the LCD backlight");
	pinMode(BK_LIGHT_PIN, OUTPUT);
	digitalWrite(BK_LIGHT_PIN, LCD_BK_LIGHT_ON_LEVEL);

	esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 320, 240, gImage_logo);
	delay(500);

	DBG_println("Initialize touch screen calibration data");
	data_init();
	touch_calibration_t calibrationData[4];
	bool calibrated = data_read(calibrationData);

	DBG_println("Initialize the touch screen");
	SPI.begin(TOUCHSCREEN_SCLK_PIN, TOUCHSCREEN_MISO_PIN, TOUCHSCREEN_MOSI_PIN);
	touch.begin(240, 320);
	if (calibrated) {
		touch.setCal(calibrationData[0].rawX, calibrationData[2].rawX, calibrationData[0].rawY, calibrationData[2].rawY, 240, 320);
	} else {
		touch.setCal(285, 1788, 311, 1877, 240, 320);
	}
	touch.setRotation(3);

	DBG_println("Initialize lvgl");
	lv_init();
	// alloc draw buffers used by LVGL from PSRAM
	lv_color_t *buf1 = (lv_color_t*) heap_caps_malloc(
	LCD_H_RES * LCD_V_RES * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
	if (!buf1) {
		DBG_println("lvgl buffer 1 application failed");
		DBG_println("system restarting...");
		delay(100);
		esp_restart();
	}
	lv_color_t *buf2 = (lv_color_t*) heap_caps_malloc(
	LCD_H_RES * LCD_V_RES * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
	if (buf2) {
		DBG_println("lvgl uses double buffer");
		lv_disp_draw_buf_init(&disp_buf, buf1, buf2,
		//NULL,
		        LCD_H_RES * LCD_V_RES);
	} else {
		DBG_println("lvgl buffer 2 application failed");
		DBG_println("use single buffer");
		lv_disp_draw_buf_init(&disp_buf, buf1,
		NULL,
		LCD_H_RES * LCD_V_RES);
	}

	DBG_println("Register display driver to lvgl");
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res = LCD_V_RES;
	disp_drv.ver_res = LCD_H_RES;
	disp_drv.flush_cb = lvgl_flush_cb;
	disp_drv.draw_buf = &disp_buf;
	disp_drv.user_data = panel_handle;
	disp_drv.full_refresh = 1;
	lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

	if (calibrated) {
		DBG_println("Register input driver to lvgl");
		lv_indev_drv_init(&indev_drv);
		indev_drv.type = LV_INDEV_TYPE_POINTER;
		indev_drv.read_cb = lv_touchpad_read;
		indev_drv.user_data = &touch;
		lv_indev_t *indev = lv_indev_drv_register(&indev_drv);
	}

	DBG_println("run lvgl task");
/*******************************************************************************************/
	uiInit();

	xTaskCreatePinnedToCore(lvglTask, "lvglTask", 4096, NULL, 6, NULL, 1);
	buttonOK.attachLongPressStop(power_off);
}
int angle = 0;
// The loop function is called in an endless loop
void loop() {
	lv_timer_handler();
	delay(10);
}
