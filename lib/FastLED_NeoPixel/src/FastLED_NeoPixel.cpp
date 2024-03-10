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

// suppress FastLED pragma version message
#define FASTLED_INTERNAL

#include "FastLED_NeoPixel.h"

/**
* @file FastLED_NeoPixel.cpp
* @brief Source for the FastLED_NeoPixel library
*/

FastLED_NeoPixel_Variant::FastLED_NeoPixel_Variant(CRGB* ledPtr, uint16_t nLeds)
	:
	leds(ledPtr),
	maxLEDs(nLeds),
	numLEDs(nLeds)
{
	endTime = micros();
}

void FastLED_NeoPixel_Variant::begin(CLEDController& ctrl) {
	controller = &ctrl;
}

void FastLED_NeoPixel_Variant::show() {
	if (controller != nullptr) {
		controller->show(leds, numLEDs, brightness);
		endTime = micros();
	}
}

void FastLED_NeoPixel_Variant::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
	if (n >= numLEDs) return;
	leds[n] = CRGB(r, g, b);
}

void FastLED_NeoPixel_Variant::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	setPixelColor(n, Color(r, g, b, w));
}

void FastLED_NeoPixel_Variant::setPixelColor(uint16_t n, uint32_t c) {
	if (n >= numLEDs) return;
	leds[n] = packedToColor(c);
}

void FastLED_NeoPixel_Variant::fill(uint32_t c, uint16_t first, uint16_t count) {
	if (first >= numLEDs) return;

	if (first == 0 && count == 0) count = numLEDs;  // if auto, fill from start to end
	else if (first != 0 && count == 0) count = numLEDs - first;  // if only first is set, fill from first to end
	else count = min(count, static_cast<uint16_t>(numLEDs - first));  // if both are set, fill from first to end without overrunning buffer

	fill_solid(leds + first, count, packedToColor(c));
}

void FastLED_NeoPixel_Variant::clear() {
	fill_solid(leds, numLEDs, CRGB::Black);
}

void FastLED_NeoPixel_Variant::updateLength(uint16_t n) {
	if (n > maxLEDs) return;  // out of range
	numLEDs = n;
	clear();
}

bool FastLED_NeoPixel_Variant::canShow() {
	if (endTime > micros()) {
		endTime = micros();
	}
	return (micros() - endTime) >= 300UL;
}

uint32_t FastLED_NeoPixel_Variant::getPixelColor(uint16_t n) const {
	if (n >= numLEDs) return Color(CRGB::Black);
	return Color(leds[n]);
}

CRGB FastLED_NeoPixel_Variant::packedToColor(uint32_t c) const {
	CRGB color(c);
	if (blendWhite == true) color.addToRGB(c >> 24);
	return color;
}
