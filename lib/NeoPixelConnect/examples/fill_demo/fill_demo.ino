/*
 Copyright (c) 2020-2021 Alan Yorinks All rights reserved.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE
 Version 3 as published by the Free Software Foundation; either
 or (at your option) any later version.
 This library is distributed in the hope that it will be useful,f
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
 along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <NeoPixelConnect.h>
#define MAXIMUM_NUM_NEOPIXELS 8

// Create an instance of NeoPixelConnect and initialize it
// to use GPIO pin 4 (D12) as the control pin, for a string
// of 8 neopixels. Name the instance p

NeoPixelConnect p(4, MAXIMUM_NUM_NEOPIXELS, pio0, 0);


void setup(){
    Serial.begin(115200);
    delay(2000);
    Serial.println("In setup");
}


void loop(){

    // set all LEDs to red, then green, and then blue
    p.neoPixelFill(255, 0, 0, true);
    delay(1000);
    p.neoPixelClear(true);
    delay(1000);

    p.neoPixelFill(0, 255, 0, true);
    delay(1000);
    p.neoPixelClear(true);
    delay(1000);

    p.neoPixelFill(0, 0, 255, true);
    delay(1000);
    p.neoPixelClear(true);
    delay(1000);
}


