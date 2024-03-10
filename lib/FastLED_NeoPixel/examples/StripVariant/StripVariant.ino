/*
 *  Project     FastLED NeoPixel Library
 *  @author     David Madison
 *  @link       github.com/dmadison/FastLED_NeoPixel
 *  @license    MIT - Copyright (c) 2021 David Madison
 * 
 * This file is part of the FastLED NeoPixel Library.
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
 * @file    StripVariant.ino
 * @brief   Demonstrates how to use a different strip type.
 * @example StripVariant.ino
 * @brief   Demonstrates how to use a different strip type.
 */

#include <FastLED_NeoPixel.h>

 // Which pin on the Arduino is connected to the LEDs?
#define DATA_PIN 6

// How many LEDs are attached to the Arduino?
#define NUM_LEDS 60

// LED brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50

/*
* Instead of using the typical FastLED_NeoPixel class for WS2812B LEDs,
* we're going to use the FastLED_NeoPixel_Variant class. This provides more
* flexibility with how the data is organized, but requires a bit more setup.
* 
* The two main differences are that:
*     * LED data is stored externally and passed in the constructor
*     * Controller creation must be done "by hand" in setup()
* 
* In the global namespace we are going to define two things: the CRGB array
* that contains the LED data, and the strip object itself (passing the LEDs
* to the constructor).
*/
CRGB leds[NUM_LEDS];
FastLED_NeoPixel_Variant strip(leds, NUM_LEDS);


void setup() {
	/*
	* Instead of using the plain 'strip.begin()', we need to pass the
	* CLEDController reference returned by the 'FastLED.addLeds()' call. This
	* lets the strip use 'show()' and brightness independent of the rest of
	* program and any other present LED strips.
	* 
	* Although this is using the typical WS2812B strip setup, you can customize
	* it for any strip configuration, including strips that use SPI (data + clock).
	*/
	strip.begin(FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS));
	strip.setBrightness(BRIGHTNESS);
}

void loop() {
	rainbow(10);  // run a rainbow animation, 10 ms between each of the 256 colors
}

void rainbow(unsigned long wait) {
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
