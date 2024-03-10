#include <Arduino.h>
#include "pin_config.h"
#include "config_check.h"
#include "ducky_functions.h"
#include <SPIFFS.h>
#include <SPI.h>
#include "global_flags.h"
#include "ui.h"
#include <NimBLEDevice.h>
#if CFG_TUD_HID

File file;
lv_obj_t *dd;
HIDcomposite device;
int DelayLength = 500;
int defaultdelay = DelayLength;
int settingsdefaultdelay = DelayLength;
int custom_delay;
int keyDelay = 10; // delay (ms) between keys when sending a string
// delay between commands
int keypressdone = 0;

static void ducky_init(lv_event_t *e);
static void event_handler(lv_event_t *e);
void MouseDrawLine( String line )
{
  String strline = line;
  int str_len = strline.length()+1;
  char keyarray[str_len];
  strline.toCharArray(keyarray, str_len);
  char delimiter[] = "+";
  char *strblock;
  strblock = strtok(keyarray, delimiter);
  int8_t move;
  while(strblock != NULL) {
    switch( strblock[0] ) {
      case 'X': // move X axis
        move = atoi( &strblock[1] );
        for( int i=0; i<abs(move);i++) {
          vTaskDelay(25);
          device.move( move>0?1:-1, 0 );
        }
      break;
      case 'Y': // move Y axis
        move = atoi( &strblock[1] );
        for( int i=0; i<abs(move);i++) {
          vTaskDelay(25);
          device.move( 0, move>0?1:-1 );
        }
      break;
      case 'C': // click
        vTaskDelay(15);
        device.pressLeft();
      break;
      default: // ignore
      break;
    }
    strblock = strtok(NULL, delimiter);
  }
}

void HIDKeySend( String str )
{
  device.sendString(str+"\n");
}



void runpayload(const char *path)
{

  file = SD_MMC.open(path);
  if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

  while (file.available())
  {
    String line = file.readStringUntil('\n');
    line.replace("&lt;", "<");

    String fullkeys = line;
    int str_len = fullkeys.length() + 1;
    char keyarray[str_len];
    fullkeys.toCharArray(keyarray, str_len);

    char *i;
    String cmd;
    String cmdinput;
    cmd = String(strtok_r(keyarray, ":", &i));
    cmdinput = String(strtok_r(NULL, ":", &i));

    if (cmd == "Rem")
    {
      DelayLength = 0;
    }
    else if (cmd == "DefaultDelay")
    {
      DelayLength = 1500;
      defaultdelay = cmdinput.toInt();
    }
    else if (cmd == "CustomDelay")
    {
      custom_delay = cmdinput.toInt();
      DelayLength = custom_delay;
    }
    else if (cmd == "KeyDelay")
    {
      DelayLength = 0;
      keyDelay = atoi(cmdinput.c_str());
    }
    else if (cmd == "Key")
    { //If command equals "Key:X"
      int key = atoi(cmdinput.c_str());
      vTaskDelay(25);
      device.sendPress(key);
      vTaskDelay(25);
      device.sendRelease();
    }
    else if (cmd == "Press")
    { //If command equals "Press:X" or "Press:X+Y+ETC"
      keypressdone = 1;
      String fullkeys = cmdinput;
      int str_len = fullkeys.length() + 1;
      char keyarray[str_len];
      fullkeys.toCharArray(keyarray, str_len);
      char delimiter[] = "+";
      char *keypart;
      keypart = strtok(keyarray, delimiter);
      while (keypart != NULL)
      {
        int key = atoi(keypart);
        vTaskDelay(25);
        device.sendPress(key);
        keypart = strtok(NULL, delimiter);
      }
      if (keypressdone == 1)
      {
        vTaskDelay(25);
        device.sendRelease();
        keypressdone = 0;
      }
    }
    else if (cmd == "Print")
    { //If command equals "Print:X"
      vTaskDelay(25);
      size_t len = cmdinput.length();
      uint8_t keycode;
      for (size_t i = 0; i < len; i++)
      {
        device.sendChar(cmdinput[i]);
        vTaskDelay(3);
      }
    }
    else if (cmd == "PrintLine")
    { //If command equals "PrintLine:X"
      vTaskDelay(25);
      size_t len = cmdinput.length();
      uint8_t keycode;
      for (size_t i = 0; i < len; i++)
      {
        device.sendChar(cmdinput[i]);
        vTaskDelay(keyDelay);
      }
      vTaskDelay(250);
      device.sendPress(HID_KEY_ENTER);
      vTaskDelay(25);
      device.sendRelease();
    }
    else if (cmd == "MouseMoveUp")
    { //If command equals "MouseMoveUp:X"
      int mousemoveamt = cmdinput.toInt();
      vTaskDelay(25);
      device.move(0, 0);
      device.move(0, mousemoveamt - 1);
    }
    else if (cmd == "MouseMoveDown")
    { //If command equals "MouseMoveDown:X"
      int mousemoveamt = cmdinput.toInt();
      vTaskDelay(25);
      device.move(0, 0);
      device.move(0, mousemoveamt);
    }
    else if (cmd == "MouseMoveLeft")
    { //If command equals "MouseMoveLeft:X"
      int mousemoveamt = cmdinput.toInt();
      vTaskDelay(25);
      device.move(0, 0);
      device.move(mousemoveamt * -1, 0);
    }
    else if (cmd == "MouseMoveRight")
    { //If command equals "MouseMoveRight:X"
      int mousemoveamt = cmdinput.toInt();
      vTaskDelay(25);
      device.move(0, 0);
      device.move(mousemoveamt, 0);
    }
    else if (cmd == "MouseClickRIGHT")
    { //mouse click command EX: MouseClickLEFT: MouseClickRIGHT: MouseClickMIDDLE:
      vTaskDelay(25);
      device.pressRight();
    }
    else if (cmd == "MouseClickLEFT")
    {
      vTaskDelay(25);
      device.pressLeft();
    }
    else if (cmd == "MouseClickMIDDLE")
    {
      vTaskDelay(25);
      device.pressMiddle();
    }
    else if (cmd == "MouseDoubleClickLEFT")
    {
      vTaskDelay(25);
      device.doublePressLeft();
    }
    else if (cmd == "MouseDrawLine")
    {
      vTaskDelay(25);
      MouseDrawLine(cmdinput);
      //device.doublePressLeft();
    }
    else
    {
      // wut ?
    }
    vTaskDelay(DelayLength); //delay between lines in payload, I found running it slower works best
    DelayLength = defaultdelay;
  }
  file.close();
  // restore settings
  DelayLength = settingsdefaultdelay;
}

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
void runDuckyCodeSetup(lv_obj_t *parent) {
  device.begin();
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
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container,260,140);
    lv_obj_align(container,  LV_ALIGN_TOP_MID, 0, 15);

  File root = SD_MMC.open("/duckyscripts");
  if (!root) {
    //return;
  }
String  duckyFiles = "";
String fileName ="";
  File file = root.openNextFile();
  while (file) {
  fileName = String(file.name()) ;
   if (fileName.endsWith(".txt")) {
     duckyFiles+= fileName.substring(0, fileName.length() - 4)+ "\n";
  }
    
   // String(file.name()) ;
    file = root.openNextFile();
  }


    dd = lv_dropdown_create(container);
    lv_dropdown_set_options(dd,duckyFiles.c_str() );

    lv_obj_align(dd, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_event_cb(dd, event_handler, LV_EVENT_ALL, NULL);
    
    lv_obj_t * btn1 = lv_btn_create(container);
    lv_obj_add_event_cb(btn1, ducky_init,LV_EVENT_CLICKED, NULL);
    lv_obj_align(btn1,  LV_ALIGN_TOP_MID, 0, 60);

    lv_obj_t *label = lv_label_create(btn1);
    lv_label_set_text(label, "Start");
    lv_obj_center(label);

}
static void ducky_init(lv_event_t *e)
{

    char buffer[32]; 
    lv_dropdown_get_selected_str(dd, buffer, sizeof(buffer));
    String duckyFile = "/duckyscripts/"+String(buffer)+".txt";

    runpayload(duckyFile.c_str());

}

void runDuckyCodeExit(lv_obj_t *parent) {
  // Lógica de limpeza específica para a aplicação Rubber Ducky
}
app_t runDuckyCodeApp = {
  .setup_func_cb = runDuckyCodeSetup,
  .exit_func_cb = runDuckyCodeExit,
  .user_data = nullptr
};
#endif
