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

// this array will hold a pixel number and the rgb values for the
// randomly generated pixel values
uint8_t random_pixel_setting[4];

// select a random pixel number in the string
uint8_t get_pixel_number(){
    return((uint16_t)random(0,MAXIMUM_NUM_NEOPIXELS - 1));
}

// select a random intensity
uint8_t get_pixel_intensity(){
    return((uint8_t)random(0,255));
}

void get_random_pixel_and_color(){
    random_pixel_setting[0] = get_pixel_number();
    random_pixel_setting[1] = get_pixel_intensity();
    random_pixel_setting[2] = get_pixel_intensity();
    random_pixel_setting[3] = get_pixel_intensity();
}

void setup(){
    Serial.begin(115200);
    delay(2000);
    Serial.println("In setup");
}

void loop(){

    // get a pixel number
    get_random_pixel_and_color();

    // display the randomly assigned pixel and color
    p.neoPixelSetValue(random_pixel_setting[0], random_pixel_setting[1],
                       random_pixel_setting[2],
                       random_pixel_setting[3], true);
    delay(100);
    // clear all pixels
    p.neoPixelClear();
    delay(100);
}


