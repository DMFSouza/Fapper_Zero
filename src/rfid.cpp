#include <Arduino.h>
#include "RFID.h"
#include "Fapper_Zero.h"
#include "ui.h"
#include "global_flags.h"
#include <Wire.h>
#include "MFRC522_I2C.h"
#include "pin_config.h"
#include <SPIFFS.h>
#include <SPI.h>
#include <SD_MMC.h>
#include <iostream>
#include <cstdio>


LV_FONT_DECLARE(lv_font_montserrat_10);
MFRC522 mfrc522(0x28);
static void runRfidWindow(lv_obj_t *parent);
static void Rfid_ui(lv_obj_t *parent);
static void read_config(lv_event_t *e);
static void write_config(lv_event_t *e);
static void load_config(lv_event_t *e);
static void try_dump(lv_event_t *e);
static void read_function(lv_event_t *e);
static void save_function(lv_event_t *e);
static void create_container();
static void dump_result();
static void save_uid(lv_event_t *e);
static void save_dump(lv_event_t *e);


static lv_obj_t *container_obj = nullptr;
static lv_obj_t *parent_obj = nullptr;

static  lv_obj_t * label0  = nullptr;
static  lv_obj_t * label1  = nullptr;
static  lv_obj_t * btn4 =  nullptr;
static  lv_obj_t * btn5 =  nullptr;
static  lv_obj_t * btn6 =  nullptr;
static  lv_obj_t * btn7 =  nullptr;

static String sMessage;
static char path[22];

struct RFIDData {
    String cID;
    String uidString;
    String cSAK;
    String cATQA;
    String cData;
};

RFIDData data;

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

void Rfid_ui(lv_obj_t *parent)
{
  parent_obj = parent;

  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_size(container, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW_WRAP);
  lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
  lv_obj_t *list1 = lv_list_create(container);
  lv_obj_set_size(list1, 115, 140);
  lv_obj_set_align(list1, LV_ALIGN_TOP_LEFT);

  lv_obj_t *btn = lv_list_add_btn(list1, LV_SYMBOL_WIFI, "Read");
  lv_obj_add_event_cb(btn, read_config, LV_EVENT_CLICKED, NULL);

  lv_obj_t *btn2 = lv_list_add_btn(list1, LV_SYMBOL_COPY, "Write");
  lv_obj_add_event_cb(btn2, write_config, LV_EVENT_CLICKED, NULL);


  lv_obj_t * btn3 = lv_list_add_btn(list1, LV_SYMBOL_SAVE, "Saved");
  lv_obj_add_event_cb(btn3, load_config, LV_EVENT_CLICKED, NULL);


}
static void create_container(){
  container_obj=nullptr;
  container_obj = lv_obj_create(parent_obj);
  lv_obj_set_size(container_obj, 185, 140);
  lv_obj_align(container_obj,  LV_ALIGN_TOP_MID, 65, 15);
}
static void read_config(lv_event_t *e)
{
  create_container();
  
  if (label0 == nullptr){
     label0 = lv_label_create(container_obj);
  }else{
    label0 =nullptr;
    label0 = lv_label_create(container_obj);
  }
  
 if (btn4 == nullptr && btn5 == nullptr && btn6 == nullptr ){
      btn4 = lv_btn_create(container_obj);
      btn5 = lv_btn_create(container_obj);
      btn6 = lv_btn_create(container_obj);
      btn7 = lv_btn_create(container_obj);
  }else{
      btn4 = nullptr;
      btn5 = nullptr;
      btn6 = nullptr;
      btn7 = nullptr;
      btn4 = lv_btn_create(container_obj);
      btn5 = lv_btn_create(container_obj);
      btn6 = lv_btn_create(container_obj);
      btn7 = lv_btn_create(container_obj);
  }
  lv_label_set_text_fmt(label0,"%s\n%s\n%s\n%s","UID:","SAK:","ATQA:","Tech:");
 
 
  lv_obj_add_event_cb(btn4, read_function, LV_EVENT_CLICKED, NULL);
  lv_obj_align(btn4,  LV_ALIGN_TOP_MID, 0, 70);
  lv_obj_t *label = lv_label_create(btn4);
  lv_label_set_text(label, "Read");
  lv_obj_center(label);


 
  lv_obj_set_size(btn5,70,30);
  lv_obj_add_flag(btn5, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_event_cb(btn5, save_uid, LV_EVENT_CLICKED, NULL);
  lv_obj_align(btn5,  LV_ALIGN_TOP_MID, -40, 70);
  lv_obj_t *label2 = lv_label_create(btn5);
  lv_obj_set_style_text_font(label2, &lv_font_montserrat_10, 0);
  lv_label_set_text(label2, "Save UID");
  lv_obj_center(label2);


  
  lv_obj_set_size(btn6,70,30);
  lv_obj_add_flag(btn6, LV_OBJ_FLAG_HIDDEN);
  
  lv_obj_add_event_cb(btn6, try_dump, LV_EVENT_CLICKED, NULL);
  lv_obj_align(btn6,  LV_ALIGN_TOP_MID, +40, 70);
  lv_obj_t *label3 = lv_label_create(btn6);
  lv_obj_set_style_text_font(label3, &lv_font_montserrat_10, 0);
  lv_label_set_text(label3, "Default Keys");
  lv_obj_center(label3);

  lv_obj_set_size(btn7,120,30);
  lv_obj_add_flag(btn7, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_event_cb(btn7, save_uid, LV_EVENT_CLICKED, NULL);
  lv_obj_align(btn7,  LV_ALIGN_TOP_MID, 0, 75);
  lv_obj_t *label4 = lv_label_create(btn7);
  lv_label_set_text(label4, "save found keys");
  lv_obj_center(label4);
  
}


static void read_function(lv_event_t *e)
{
  byte sak; 
  String cSAK  ="";
  String cATQA ="";
  String cType ="";
  String uidString = ""; 
  String cID;
  bool lPesq = false; 

  lv_obj_add_flag(btn4, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(btn5, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(btn6, LV_OBJ_FLAG_HIDDEN);

  Wire.begin(RFID_SDA,RFID_SCL);
  
  mfrc522.PCD_Init();  
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
 
  }
 
  byte atqa[2]; // Buffer para armazenar o ATQA
  byte bufferSize = sizeof(atqa); // Tamanho do buffer
  
  // Solicita o ATQA
  byte status = mfrc522.PICC_REQA_or_WUPA(MFRC522::PICC_CMD_REQA, atqa, &bufferSize);
  int y= 0;
 
  if (label1 == nullptr){
    label1 = lv_label_create(container_obj);
  } else{
    label1 =nullptr;
    label1 = lv_label_create(container_obj);
  }
  
 lv_label_set_recolor(label1, true); 
 lv_obj_align(label1, LV_ALIGN_CENTER, 15, -23);
 lv_label_set_text_fmt(label1,"%s\n %s\n %s\n %s","","","","");
  
 for (y=0;y<4;y++){
    if (status == MFRC522::STATUS_OK) {

      cATQA.concat(atqa[1] < 0x10 ? "0" : ""); 
      cATQA.concat(String(atqa[1], HEX)); 
      cATQA.concat(" ");
      cATQA.concat(atqa[0] < 0x10 ? "0" : ""); 
      cATQA.concat(String(atqa[0], HEX)); 
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          cID+=mfrc522.uid.uidByte[i];
          uidString += (mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          String hexByte = String(mfrc522.uid.uidByte[i], HEX); // Converte o byte hexadecimal em String
          hexByte.toUpperCase();
          uidString += hexByte;
          cSAK = String(mfrc522.uid.sak, HEX);
      
          byte cardType = mfrc522.PICC_GetType(mfrc522.uid.sak);
          cType = mfrc522.PICC_GetTypeName(cardType);

          mfrc522.PCD_StopCrypto1();
          lv_label_set_text_fmt(label1,"#03fc07 %s#\n %s\n %s\n %s",uidString,cSAK,cATQA,cType);
          lPesq = true;
          data.cID = cID;
          data.uidString = uidString;
          data.cSAK = cSAK;
          data.cATQA = cATQA;
        int messageSize = snprintf(nullptr, 0, "{\"id\":\"%s\",\"uid\":\"%s\",\"sak\":%s,\"atqa\":%s,\"data\":[]}", cID, uidString, cSAK, cATQA);
        if (messageSize < 0) {
          std::cerr << "Erro ao calcular o tamanho necessário para message!" << std::endl;
        }

      // Alocando dinamicamente memória para message
      char* message = new char[messageSize + 1]; // +1 para o caractere nulo de terminação
      if (message == nullptr) {
        std::cerr << "Erro ao alocar memória para message!" << std::endl;
      }

      // Formatando message
      sprintf(message, "{\"id\":\"%s\",\"uid\":\"%s\",\"sak\":%s,\"atqa\":%s,\"data\":[]}", cID, uidString, cSAK, cATQA);

      sprintf(path,"/rfid/%s.json",cID);
      sMessage= String(message);
      delete[] message;
      }
       break;

    }
  delay(1000);
  status = mfrc522.PICC_REQA_or_WUPA(MFRC522::PICC_CMD_REQA, atqa, &bufferSize);
  }
  mfrc522.PCD_Reset();  
  Wire.end();
  
}

static void try_dump(lv_event_t *e){
  
  lv_obj_add_flag(btn5, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(btn6, LV_OBJ_FLAG_HIDDEN);
  lv_label_set_text_fmt(label1,"%s\n %s\n %s\n %s","","","","");
  lv_label_set_recolor(label0, true);


  dump_result();
}
static void dump_result(){
    const int MAX_SECTORS = 16;
    const int BLOCKS_PER_SECTOR = 4;
    const char *status[MAX_SECTORS][BLOCKS_PER_SECTOR];
    const char *colors[2] = { "#03fc07", "#ff0000" };



    Wire.begin(RFID_SDA,RFID_SCL);
    mfrc522.PCD_Init();  

    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        // Tratamento de erro, se necessário
    }
 
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

    for (int setor = 0; setor < MAX_SECTORS; setor++) {
        for (int bloco = 0; bloco < BLOCKS_PER_SECTOR; bloco++) {
            int authIndex = mfrc522.sectorStatus[setor][bloco].authenticated ? 0 : 1;
            const char *color = colors[authIndex];
            char statusChar = mfrc522.sectorStatus[setor][bloco].authenticated ? 'V' : 'X'; // Definindo o caractere de status com base na autenticação

            // Concatenar a cor e o status no formato "#rrggbb X#"
            char statusString[11]; // Máximo de 7 caracteres para a cor + 1 para o caractere de status + 1 para '#'
            sprintf(statusString, "%s %c#", color, statusChar);
            status[setor][bloco] = strdup(statusString);

        }
    }

    // Substituir as informações no loop
    char textBuffer[500];
    sprintf(textBuffer,
        "keys\n"
        "     0   1   2   3  4  5  6  7\n"
        "A  %s  %s  %s  %s  %s  %s  %s  %s\n"
        "B  %s  %s  %s  %s  %s  %s  %s  %s\n"
        "     8   9  10  11 12 13 14 15\n"
        "A   %s  %s  %s  %s  %s  %s  %s  %s\n"
        "B   %s  %s  %s  %s  %s  %s  %s  %s",
        status[0][0], status[0][1], status[0][2], status[0][3],
        status[1][0], status[1][1], status[1][2], status[1][3],
        status[2][0], status[2][1], status[2][2], status[2][3],
        status[3][0], status[3][1], status[3][2], status[3][3],
        status[4][0], status[4][1], status[4][2], status[4][3],
        status[5][0], status[5][1], status[5][2], status[5][3],
        status[6][0], status[6][1], status[6][2], status[6][3],
        status[7][0], status[7][1], status[7][2], status[7][3],
        status[8][0], status[8][1], status[8][2], status[8][3],
        status[9][0], status[9][1], status[9][2], status[9][3],
        status[10][0], status[10][1], status[10][2], status[10][3],
        status[11][0], status[11][1], status[11][2], status[11][3],
        status[12][0], status[12][1], status[12][2], status[12][3],
        status[13][0], status[13][1], status[13][2], status[13][3],
        status[14][0], status[14][1], status[14][2], status[14][3],
        status[15][0], status[15][1], status[15][2], status[15][3]
    );

    lv_obj_align(label0,LV_ALIGN_TOP_MID, 0, -10);
    lv_obj_set_style_text_font(label0, &lv_font_montserrat_10, 0);
   
    lv_label_set_text_fmt(label0, textBuffer);

    lv_obj_clear_flag(btn7, LV_OBJ_FLAG_HIDDEN); 
    int messageSize = snprintf(nullptr, 0, "{\"id\":\"%s\",\"uid\":\"%s\",\"sak\":%s,\"atqa\":%s,\"data\":[\n%s]}",data.cID.c_str(),data.uidString.c_str(),data.cSAK.c_str(),data.cATQA.c_str(),mfrc522.info.infoDumpStr.c_str());
    
        if (messageSize < 0) {
          std::cerr << "Erro ao calcular o tamanho necessário para message!" << std::endl;
        }

      // Alocando dinamicamente memória para message
      char* message = new char[messageSize + 1]; // +1 para o caractere nulo de terminação
      if (message == nullptr) {
        std::cerr << "Erro ao alocar memória para message!" << std::endl;
      }
    sprintf(message,"{\"id\":\"%s\",\"uid\":\"%s\",\"sak\":%s,\"atqa\":%s,\"data\":[\n%s]}",data.cID.c_str(),data.uidString.c_str(),data.cSAK.c_str(),data.cATQA.c_str(),mfrc522.info.infoDumpStr.c_str());
    sMessage= String(message);
    delete[] message;
}

static void save_uid(lv_event_t *e){
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
  

    File file = SD_MMC.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(sMessage.c_str())) {
      Serial.println("File written");
      lv_obj_t *mbox1 = lv_msgbox_create(NULL, "Success", "Data saved.", NULL, true);
      lv_obj_center(mbox1);
      sMessage = ""; 
      strcpy(path, "");
    }else {
        Serial.println("Write failed");
    }
    file.close();
}

static void save_dump(lv_event_t *e){
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
}
static void write_config(lv_event_t *e){
    create_container();
}

static void save_function(lv_event_t *e){
    create_container();
}

static void load_config(lv_event_t *e)
{
  create_container();
}

void runRfidWindow(lv_obj_t *parent)
{
  Rfid_ui(parent);

}

app_t rfid_ui = {
  .setup_func_cb = runRfidWindow,
  .exit_func_cb = nullptr,
  .user_data = nullptr
}; 
