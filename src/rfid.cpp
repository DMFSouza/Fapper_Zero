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
static  lv_obj_t * btn8 = nullptr;
static  lv_obj_t * btn9 = nullptr;
static  lv_obj_t * btn10 = nullptr;
static String sMessage;
static char path[22];
int fileCountOnSD = 0; // for counting files

static char fileDir[8][22];
static  String ids[8];
static  String uids[8];
struct RFIDData {
    String cID;
    String uidString;
    String cSAK;
    String cATQA;
    String cData;
};



byte buffer[18];
byte block;
byte waarde[64][16];

    
MFRC522::MIFARE_Key key;

#define NR_KNOWN_KEYS   8

byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 00 00 00 00 00 00
};

void dump_byte_array(byte *buffer, byte bufferSize);  


static void clone_card(lv_event_t *e);
static void change_uid(lv_event_t *e);
static void write_ui(lv_event_t *e);
static void wait_screen();
static void end_screen();
static void cuid_config(lv_event_t *e);
byte cardTypeSave;
lv_obj_t *dd;
static void delet_sd(lv_event_t *e);
RFIDData data;
File root;


void printDirectory(File dir, int numTabs);
static void delet_sd(lv_event_t *e,int index);
static void create_obj();

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

  lv_obj_t *btn2 = lv_list_add_btn(list1, LV_SYMBOL_COPY, "Clone");
  lv_obj_add_event_cb(btn2, write_config, LV_EVENT_CLICKED, NULL);


  lv_obj_t * btn3 = lv_list_add_btn(list1, LV_SYMBOL_SAVE, "Saved");
  lv_obj_add_event_cb(btn3, load_config, LV_EVENT_CLICKED, NULL);

  lv_obj_t * btn4 = lv_list_add_btn(list1, LV_SYMBOL_WARNING, "CUID");
  lv_obj_add_event_cb(btn4, cuid_config, LV_EVENT_CLICKED, NULL);

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
      lv_obj_t *mbox1 = lv_msgbox_create(NULL, "Success", "Data saved.", NULL, true);
      lv_obj_center(mbox1);
      sMessage = ""; 
      strcpy(path, "");
    }else {
        Serial.println("Write failed");
    }
    file.close();
    read_config(e);
}

static void write_config(lv_event_t *e){
    create_container();
    if (label0 == nullptr){
      label0 = lv_label_create(container_obj);
    }else{
      label0 =nullptr;
      label0 = lv_label_create(container_obj);
    }


  
  btn8 = lv_btn_create(container_obj);
  lv_obj_add_event_cb(btn8, write_ui, LV_EVENT_CLICKED, NULL);
    
  lv_obj_align(btn8,  LV_ALIGN_TOP_MID, 0, 70);
  lv_obj_t *label = lv_label_create(btn8);
  lv_label_set_text(label, "Copy");
  lv_obj_center(label);


  btn9 = lv_btn_create(container_obj);
  lv_obj_add_event_cb(btn9, clone_card, LV_EVENT_CLICKED, NULL);
  lv_obj_add_flag(btn9, LV_OBJ_FLAG_HIDDEN);  
  lv_obj_align(btn9,  LV_ALIGN_TOP_MID, 0, 70);
  lv_obj_t *label1 = lv_label_create(btn9);
  lv_label_set_text(label1, "Clone");
  lv_obj_center(label1);
  
  lv_obj_align(label0,LV_ALIGN_CENTER, 0, -30);
  char textBuffer[24]; 
  sprintf(textBuffer, "Place the original card");
  lv_label_set_text_fmt(label0,textBuffer);

}

static void write_ui(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    
    Wire.begin(RFID_SDA,RFID_SCL);
    mfrc522.PCD_Init();  
    
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
    if ( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()){

      }
   // Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    //Serial.println();
    //Serial.print(F("PICC type: "));
    cardTypeSave =  mfrc522.PICC_GetType(mfrc522.uid.sak);
    //Serial.println(mfrc522.PICC_GetTypeName( cardTypeSave));

    
   lv_label_set_text_fmt(label0,"");
   lv_obj_clean(label0);
   wait_screen();
   lv_obj_add_flag(btn8, LV_OBJ_FLAG_HIDDEN);
   lv_obj_clear_flag(btn9, LV_OBJ_FLAG_HIDDEN); 
  
 
  
} 
static void wait_screen(){
   
   label1 = lv_label_create(container_obj);
   lv_obj_align(label1,LV_ALIGN_CENTER, 0, -30);
   char textBuffer[26];
   sprintf(textBuffer, "Insert destination card");
   lv_label_set_text(label1,textBuffer);
   
}


void dump_byte_array(byte *buffer, byte bufferSize) {
     for (byte i = 0; i < bufferSize; i++) {
        //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        //Serial.print(buffer[i], HEX);
    }
}


static void clone_card(lv_event_t *e){ 

    lv_obj_add_flag(btn9, LV_OBJ_FLAG_HIDDEN);      
    lv_label_set_text_fmt(label1,"");
    lv_obj_clean(label1);
    //Serial.println("Insert new card...");
  
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;


    //Serial.println("Insert destination card...");
    
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

  for(int i = 4; i <= 62; i++){
    if(i == 7 || i == 11 || i == 15 || i == 19 || i == 23 || i == 27 || i == 31 || i == 35 || i == 39 || i == 43 || i == 47 || i == 51 || i == 55 || i == 59){
      i++;
    }
    block = i;
    
    //Serial.println(F("Authenticating using key A..."));
    byte status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        //Serial.print(F("PCD_Authenticate() failed: "));
        //Serial.println(mfrc522.GetStatusCodeName(status));
        return;
        
    }
    
    //Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        //Serial.print(F("PCD_Authenticate() failed: "));
        //Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    //Serial.print(F("Writing data into block ")); 
    //Serial.print(block);
    //Serial.println("\n");
          
    dump_byte_array(waarde[block], 16); 
    
          
     status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(block, waarde[block], 16);
      if (status != MFRC522::STATUS_OK) {
        //Serial.print(F("MIFARE_Write() failed: "));
        //Serial.println(mfrc522.GetStatusCodeName(status));
      }

  }
  mfrc522.PICC_HaltA();       
  mfrc522.PCD_StopCrypto1(); 

  mfrc522.PCD_Reset();  
  Wire.end();
  end_screen();
  
}
static void end_screen(){
  lv_label_set_text_fmt(label0,"");
  lv_obj_clean(label0);
  label0 = lv_label_create(container_obj);
  lv_obj_align(label0,LV_ALIGN_CENTER, 0, 0);
  lv_label_set_recolor(label0, true);
  char textBuffer1[14]; 
  sprintf(textBuffer1, "#03fc07 Done#");
  lv_label_set_text_fmt(label0,textBuffer1 );
}


static void cuid_config(lv_event_t *e){
  create_container();

  lv_obj_t *mbox1 = lv_msgbox_create(NULL, "Attention!", "Only use this routine with Mifare Magic cards!", NULL, true);
  lv_obj_center(mbox1);
 
  pinMode(PIN_SD_CS, OUTPUT);
 
  digitalWrite(PIN_SD_CS, 1);
  SD_MMC.setPins(PIN_SD_SCK, PIN_SD_MOSI, PIN_SD_MISO);
  
  if (!SD_MMC.begin("/sdcard", true)) {
      Serial.println("Failed to mount SD card");
      return;
   }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
       Serial.println("No SD card attached");
       return;
  }
  
  if (cardType == CARD_MMC) {
  } else if (cardType == CARD_SD) { 
  } else if (cardType == CARD_SDHC) {
      
  }
  
  
  root = SD_MMC.open("/rfid");

  printDirectory(root, 0);

  char path[30];
  char buffer[1024];
  String cuid ="";
for(int x = 0; x < fileCountOnSD; x++) {
  sprintf(path, "/rfid/%s", fileDir[x]);
  File file = SD_MMC.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  
  // Ler apenas a primeira linha do arquivo
  String firstLine = file.readStringUntil('\n');
  file.close();

  int uidIndex = firstLine.indexOf("\"uid\":");

  if (uidIndex != -1) {
 
    String uidValue = firstLine.substring(uidIndex + 8, uidIndex + 19);
    cuid+= uidValue+"\n";

  } else {
    Serial.println("Could not find 'id' and/or 'uid' in the first line of the file.");
  }
}


    dd = lv_dropdown_create(container_obj);
    lv_dropdown_set_options(dd,cuid.c_str() );

    lv_obj_align(dd, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_event_cb(dd, event_handler, LV_EVENT_ALL, NULL);
    
    btn10 = lv_btn_create(container_obj);
    lv_obj_add_event_cb(btn10, change_uid,LV_EVENT_CLICKED, NULL);
    lv_obj_align(btn10,  LV_ALIGN_TOP_MID, 0, 60);

    lv_obj_t *label = lv_label_create(btn10);
    lv_label_set_text(label, "Change UID");
    lv_obj_center(label);

}

static void change_uid(lv_event_t *e){ //Read card
   
  Wire.begin(RFID_SDA,RFID_SCL);
  mfrc522.PCD_Init();  
  char buffer[32]; 
  lv_dropdown_get_selected_str(dd, buffer, sizeof(buffer));
 // Serial.println(String(buffer));
  
  if (String(buffer).isEmpty()) {
    return;
  } 
    int index = 0;
    byte UID[4]; 
    
    char *token = strtok(buffer, " ");
    while (token != NULL && index < 4) {
        // Converte o token em um valor hexadecimal literal
        sscanf(token, "%hhx", &UID[index]);
        index++;

        // Obtenha o próximo token
        token = strtok(NULL, " ");
    }


 
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    
  }


  for (byte i = 0; i < mfrc522.uid.size; i++) {

  } 
  byte newUid[sizeof(UID)];
  memcpy(newUid, UID, sizeof(UID));
  
  if ( mfrc522.MIFARE_SetUid(newUid, (byte)4, true) ) {
    // Serial.println(F("Wrote new UID to card."));
  }

  mfrc522.PICC_HaltA();
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    
  }
  //Serial.println(F("New UID and contents:"));
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  delay(2000); 
  lv_obj_add_flag(btn10, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(dd, LV_OBJ_FLAG_HIDDEN);
  label0 = lv_label_create(container_obj);
  lv_obj_align(label0,LV_ALIGN_CENTER, 0, 0);
  lv_label_set_recolor(label0, true);
  char textBuffer[14]; 
  sprintf(textBuffer, "#03fc07 Done#");
  lv_label_set_text_fmt(label0,textBuffer );
  mfrc522.PCD_Reset();  
  Wire.end();
}



static void save_function(lv_event_t *e){
    create_container();
}

static void load_config(lv_event_t *e)
{
  create_container();
  pinMode(PIN_SD_CS, OUTPUT);
 
  digitalWrite(PIN_SD_CS, 1);
  SD_MMC.setPins(PIN_SD_SCK, PIN_SD_MOSI, PIN_SD_MISO);
  
  if (!SD_MMC.begin("/sdcard", true)) {
      Serial.println("Failed to mount SD card");
      return;
   }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
       Serial.println("No SD card attached");
       return;
  }
  
  if (cardType == CARD_MMC) {
  } else if (cardType == CARD_SD) { 
  } else if (cardType == CARD_SDHC) {
      
  }
  
  
  root = SD_MMC.open("/rfid");

  printDirectory(root, 0);
  int nPos = -30;
  lv_obj_t* objArray[fileCountOnSD]; // Array para armazenar os ponteiros dos objetos
  lv_obj_t* btnArray[fileCountOnSD];
  lv_obj_t *labelArray[fileCountOnSD];
  lv_obj_t *labelTxt[fileCountOnSD];
char path[30];
char buffer[1024];


// Zerando o array de ids
for (int i = 0; i < 8; ++i) {
    ids[i] = "";
}

// Zerando o array de uids
for (int i = 0; i < 8; ++i) {
    uids[i] = "";
}
for(int x = 0; x < fileCountOnSD; x++) {
  sprintf(path, "/rfid/%s", fileDir[x]);
  File file = SD_MMC.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  
  // Ler apenas a primeira linha do arquivo
  String firstLine = file.readStringUntil('\n');
  file.close();

  // Encontrar a posição de "id": e "uid": na primeira linha
  int idIndex = firstLine.indexOf("\"id\":");
  int uidIndex = firstLine.indexOf("\"uid\":");

  if (idIndex != -1 && uidIndex != -1) {
    // Extrair o valor do ID entre "id": e "uid":
    String idValue = firstLine.substring(idIndex + 6, uidIndex - 2);
    ids[x] = idValue;

    // Extrair os 11 caracteres do UID após "uid":
    String uidValue = firstLine.substring(uidIndex + 8, uidIndex + 19); // Ajuste para 11 caracteres
    uids[x] = uidValue;

    // Imprimir o ID e o UID para verificar

  } else {
    Serial.println("Could not find 'id' and/or 'uid' in the first line of the file.");
  }
}
 
char cTxt[50];
for (int x = 0; x < fileCountOnSD; x++) {
   int *btn_no = new int;
   *btn_no = x;
    objArray[x] = lv_obj_create(container_obj);
    lv_obj_set_size(objArray[x], 150, 50);
    lv_obj_align(objArray[x], LV_ALIGN_LEFT_MID, 0, nPos);

    labelTxt[x] = lv_label_create(objArray[x]);
    lv_label_set_recolor(labelTxt[x], true); 
    lv_obj_set_style_text_font(labelTxt[x], &lv_font_montserrat_10, 0);
    // Use sprintf para formatar a string e armazená-la em cTxt
    sprintf(cTxt, "ID: %s\nUID: #03fc07 %s#", ids[x].c_str(), uids[x].c_str());
    lv_label_set_text_fmt(labelTxt[x], cTxt);
    
    btnArray[x] = lv_btn_create(objArray[x]);
    lv_obj_set_size(btnArray[x], 20, 20);
    lv_obj_align(btnArray[x], LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_add_event_cb(btnArray[x], delet_sd,LV_EVENT_CLICKED,btn_no);
    
    labelArray[x] = lv_label_create(btnArray[x]);
    lv_label_set_text(labelArray[x], LV_SYMBOL_TRASH);
    lv_obj_center(labelArray[x]);
    nPos += 50;
}

  
}
static void delet_sd(lv_event_t *e){
  int * btn_no;
  btn_no = (int*)lv_event_get_user_data(e); 

  sprintf(path, "/rfid/%s", fileDir[*btn_no]);
  delete btn_no;

  Serial.printf("Deleting file: %s\n", path);
  if(SD_MMC.remove(path)){
      Serial.println("File deleted");
  } else {
      Serial.println("Delete failed");
  }
  load_config(e);
}

void printDirectory(File dir, int numTabs) {
  fileCountOnSD = (fileCountOnSD>0) ? 0:fileCountOnSD;
  while (true) {
    
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
       
    }
    if (fileCountOnSD<8){
       strncpy(fileDir[fileCountOnSD], entry.name(), 22);
      
    }
    
    
    fileCountOnSD++;
  
    if (entry.isDirectory()) {
   
      printDirectory(entry, numTabs + 1);
    } else {
        
    }
    entry.close();
  }
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
