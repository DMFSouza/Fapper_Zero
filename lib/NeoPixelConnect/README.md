# WS2812 NeoPixel Library For the Arduino Nano RP2040 Connect
This library implements NeoPixel control and is based on the [pico examples for the WS2812.](https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812)

This library uses 4 PIO instructions and defaults to pio0 and sm 0.
You may select a different pio or sm in the constructor.

A "Fireworks" example:

```asm
#include <NeoPixelConnect.h>

// Create an instance of NeoPixelConnect and initialize it
// to use GPIO pin 4 (D12) as the control pin, for a string
// of 8 neopixels. Name the instance p
NeoPixelConnect p(4, 8);

// this array will hold a pixel number and the rgb values for the
// randomly generated pixel values
uint8_t random_pixel_setting[4];

// select a random pixel number in the string
uint8_t get_pixel_number(){
    return((uint8_t)random(0,7));
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

```
