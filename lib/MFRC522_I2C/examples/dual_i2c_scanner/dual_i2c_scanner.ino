#include <Wire.h>
#include "MFRC522_I2C.h"

// I2C GPIOs
#define IIC_CLK                         32          // internal
#define IIC_DATA                        33          // internal

// 2nd I2C GPIOs
#define RST_PIN 12 
#define ext_IIC_CLK                     23          // 14-pin-header
#define ext_IIC_DATA                    18          // 14-pin-header

TwoWire i2cBusOne = TwoWire(0);
TwoWire i2cBusTwo = TwoWire(1);
MFRC522 mfrc522(RST_PIN , 0x28, i2cBusTwo);

void setup() {
  Serial.begin(115200);           // Initialize serial communications with the PC
  i2cBusOne.begin(IIC_DATA, IIC_CLK, 40000);
  i2cBusTwo.begin(ext_IIC_DATA, ext_IIC_CLK, 40000);
}

void loop() {
  scanBus1();
  delay(3000);
  scanBus2();
  mfrc522.PCD_Init();             // Init MFRC522
  ShowReaderDetails();            // Show details of PCD - MFRC522 Card Reader details

}

void scanBus1 () {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    i2cBusOne.beginTransmission(address);
    error = i2cBusOne.endTransmission();
    if (error == 0) {
      Serial.print("Bus1: I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
}

void scanBus2 () {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    i2cBusTwo.beginTransmission(address);
    error = i2cBusTwo.endTransmission();
    if (error == 0) {
      Serial.print("Bus2: I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
}

void ShowReaderDetails() {
  // Get the MFRC522 software version
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Software Version: 0x"));
  Serial.print(v, HEX);
  if (v == 0x91)
    Serial.print(F(" = v1.0"));
  else if (v == 0x92)
    Serial.print(F(" = v2.0"));
  else
    Serial.print(F(" (unknown)"));
  Serial.println("");
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
  }
}

