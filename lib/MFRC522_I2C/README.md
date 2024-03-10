# MFRC522 i2c 
### Code is compatible to Arduino and ESP8266 (NodeMCU)

Tested on [https://electronics.semaf.at/MFRC522-i2c-Tiny-Breakout-Board](https://electronics.semaf.at/MFRC522-i2c-Tiny-Breakout-Board)
![MFRC522 i2c Breakout Board](https://cdn.semaf.at/media/image/product/1748/md/mfrc522-i2c-tiny-breakout-board.jpg "MFRC522 i2c Breakout Board")

__Set the Pin for the RST and i2c address!__
```c++
#define RST_PIN 6 // Arduino UNO
// #define RST_PIN 14 // D5 on NodeMCU

// 0x28 is i2c address of the NFC Reader. Check your address with i2cscanner if not match.
MFRC522_I2C mfrc522(0x28, RST_PIN);   // Create MFRC522 instance.
```

This Library is the extended Version from @arozcan [MFRC522 i2c Library](https://github.com/arozcan/MFRC522-I2C-Library)
Renamed to avoid Problems when using multiple Readers via SPI and I2C. Or when you have multiple Compile-Options in your Projetc like in @biologist79 [ESPuino](https://github.com/biologist79/ESPuino.git)

Added Features
 - Full TwoWire compatibility
