#include <Wire.h>
#include "MFRC522_I2C.h"

// I2C GPIOs
#define IIC_CLK                         32          // internal
#define IIC_DATA                        33          // internal

// 2nd I2C GPIOs
#define RST_PIN                         12 
#define ext_IIC_CLK                     23          // 14-pin-header
#define ext_IIC_DATA                    18          // 14-pin-header

byte chipAddress = 0x28;

TwoWire i2cBusOne = TwoWire(0);
TwoWire i2cBusTwo = TwoWire(1);
MFRC522 mfrc522(chipAddress, RST_PIN, i2cBusTwo);

void setup() {
  byte version;
  byte value;
  Serial.begin(115200);           // Initialize serial communications with the PC
    Serial.println("TwoWire Bus 1 initiieren");
  i2cBusOne.begin(IIC_DATA, IIC_CLK, 40000);
    Serial.println("TwoWire Bus 2 initiieren");
  i2cBusTwo.begin(ext_IIC_DATA, ext_IIC_CLK, 40000);
    Serial.println("MFRC522 initiieren");
  mfrc522.PCD_Init();
    Serial.println("MFRC522 Selbsttest");
}

void loop() {
//  scanBus1();
  scanBus2();
  ShowReaderDetails();            // Show details of PCD - MFRC522 Card Reader details
 if (mfrc522.PCD_PerformSelfTest()) {
    Serial.println("Selftest   OK");
 } else Serial.println("Selftest not  OK");
  delay(5000);
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
/*  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
    pinMode(RST_PIN, OUTPUT);
    if (digitalRead(RST_PIN) == LOW) { //The MFRC522 chip is in power down mode.
      digitalWrite(RST_PIN, HIGH);   // Exit power down mode. This triggers a hard reset.
      // Section 8.8.2 in the datasheet says the oscillator start-up time is the start up time of the crystal + 37,74�s. Let us be generous: 50ms.
      delay(200);
    }
  else {
    Serial.println("done\n");
  } 
  } */
}

void ShowReaderDetails() {
 byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);

  switch (version) {
    case 0x88:  // Fudan Semiconductor FM17522 clone
      Serial.println("Fudan Semiconductor FM17522 clone");
      break;
    case 0x90:  // Version 0.0
      Serial.println("Version 0.0");
      break;
    case 0x91:  // Version 1.0
      Serial.println("Version 1.0");
      break;
    case 0x92:  // Version 2.0
      Serial.println("Version 2.0");
      break;
    default:  // Unknown version
      Serial.println("Unknown version");
  }
}