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
 * @file    Strandtest.ino
 * @brief   A basic strip test program.
 * @example Strandtest.ino
 * @brief   A basic strip test program.
 * 
 * @details This partially mirrors the behavior of the Adafruit NeoPixel
 * "strandtest" example. Due to licensing issues that example cannot be
 * included in this library, but I recommend trying to modify it yourself with
 * the FastLED NeoPixel alterations.
 *
 * You can find the original example here:
 * https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/strandtest/strandtest.ino
 */

#include <FastLED_NeoPixel.h>

 // Which pin on the Arduino is connected to the LEDs?
#define DATA_PIN 6

// How many LEDs are attached to the Arduino?
#define NUM_LEDS 60

// LED brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50

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
	colorWipe(strip.Color(255, 0, 0), 25);  // red
	colorWipe(strip.Color(0, 255, 0), 25);  // green
	colorWipe(strip.Color(0, 0, 255), 25);  // blue

	colorWipe(strip.Color(255, 255, 255), 25);  // white

	theaterChase(strip.Color(0, 255, 255), 100, 3, 5);  // cyan
	theaterChase(strip.Color(255, 0, 255), 100, 3, 5);  // magenta
	theaterChase(strip.Color(255, 255, 0), 100, 3, 5);  // yellow

	rainbow(10, 3);
	
	blank(1000);
}

/*
* Fills a strip with a specific color, starting at 0 and continuing
* until the entire strip is filled. Takes two arguments:
* 
*     1. the color to use in the fill
*     2. the amount of time to wait after writing each LED
*/
void colorWipe(uint32_t color, unsigned long wait) {
	for (unsigned int i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, color);
		strip.show();
		delay(wait);
	}
}

/*
* Runs a marquee style "chase" sequence. Takes three arguments:
*
*     1. the color to use in the chase
*     2. the amount of time to wait between frames
*     3. the number of LEDs in each 'chase' group
*     3. the number of chases sequences to perform
*/
void theaterChase(uint32_t color, unsigned long wait, unsigned int groupSize, unsigned int numChases) {
	for (unsigned int chase = 0; chase < numChases; chase++) {
		for (unsigned int pos = 0; pos < groupSize; pos++) {
			strip.clear();  // turn off all LEDs
			for (unsigned int i = pos; i < strip.numPixels(); i += groupSize) {
				strip.setPixelColor(i, color);  // turn on the current group
			}
			strip.show();
			delay(wait);
		}
	}
}

/*
* Simple rainbow animation, iterating through all 8-bit hues. LED color changes
* based on position in the strip. Takes two arguments:
* 
*     1. the amount of time to wait between frames
*     2. the number of rainbows to loop through
*/
void rainbow(unsigned long wait, unsigned int numLoops) {
	for (unsigned int count = 0; count < numLoops; count++) {
		// iterate through all 8-bit hues, using 16-bit values for granularity
		for (unsigned long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 256) {
			for (unsigned int i = 0; i < strip.numPixels(); i++) {
				unsigned long pixelHue = firstPixelHue + (i * 65536UL / strip.numPixels()); // vary LED hue based on position
				strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));  // assign color, using gamma curve for a more natural look
			}
			strip.show();
			delay(wait);
		}
	}
}

/*
* Blanks the LEDs and waits for a short time.
*/
void blank(unsigned long wait) {
	strip.clear();
	strip.show();
	delay(wait);
}
