/*
 *  Project     FastLED NeoPixel Library
 *  @author     David Madison
 *  @link       github.com/dmadison/FastLED_NeoPixel
 *  @license    MIT - Copyright (c) 2021 David Madison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

 /**
 * @file    Blink.ino
 * @brief   Runs a simple blink animation.
 * @example Blink.ino
 * @brief   Runs a simple blink animation.
 */

#include <FastLED_NeoPixel.h>

// Which pin on the Arduino is connected to the LEDs?
#define DATA_PIN 6

// How many LEDs are attached to the Arduino?
#define NUM_LEDS 60

// LED brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50

// Amount of time for each half-blink, in milliseconds
#define BLINK_TIME 1000

/* Declare the NeoPixel strip object:
*     * Argument 1 = Number of LEDs in the LED strip
*     * Argument 2 = Arduino pin number
*     * Argument 3 = LED strip color order
* 
* The FastLED_NeoPixel version uses template arguments instead of function
* arguments. Note the use of '<>' brackets!
* 
* You can switch between libraries by commenting out one of these two objects.
* In this example they should behave identically.
*/
// Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_GRB);  // <- Adafruit NeoPixel version
FastLED_NeoPixel<NUM_LEDS, DATA_PIN, NEO_GRB> strip;      // <- FastLED NeoPixel version


void setup() {
	strip.begin();  // initialize strip (required!)
	strip.setBrightness(BRIGHTNESS);
}

void loop() {
	strip.setPixelColor(0, strip.Color(0, 0, 255));  // set pixel 0 to blue
	strip.show();
	delay(BLINK_TIME);

	strip.setPixelColor(0, strip.Color(0, 0, 0));  // set pixel 0 to black
	strip.show();
	delay(BLINK_TIME);
}
