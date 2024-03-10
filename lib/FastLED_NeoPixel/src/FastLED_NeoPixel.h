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

#ifndef FASTLED_NEOPIXEL_H
#define FASTLED_NEOPIXEL_H

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include <FastLED.h>


/**
* @file FastLED_NeoPixel.h
* @brief Header for the FastLED_NeoPixel library
*/


/**
* @brief FastLED implementation of the Adafruit_NeoPixel class. Base class
*        for all strips.
* 
* This provides a wrapper for the FastLED LED data that mimic the structure of
* the Adafruit_NeoPixel class. This lets you use an Adafruit_NeoPixel animation
* with FastLED without having to rewrite it.
* 
* This class can be used with any FastLED strip type. The LED data array must
* be defined externally and linked in the constructor along with the strip size
* (number of LEDs).
* 
* You must also call begin() before data will be sent to the strip. The begin()
* function takes a reference to the CLEDController returned by the
* FastLED.addLeds() function.
* 
* @see Adafruit_NeoPixel
* @see NewFuncs
* @see Unimplemented
*/
class FastLED_NeoPixel_Variant {
public:
	/**
	* Class constructor for all strip types.
	* 
	* @param ledPtr CRGB pointer to the LED data
	* @param nLeds number of LEDs in the CRGB array
	*/
	FastLED_NeoPixel_Variant(CRGB* ledPtr, uint16_t nLeds);

	/**
	* This function assigns a CLEDController instance, used for sending the LED
	* data to the strip, to the internal controller pointer. This must be
	* done before any LED data can be written.
	* 
	* To obtain a CLEDController object, call the FastLED.addLeds() function
	* using the relevant template parameters (strip type, data pin(s), and
	* RGB color order) and function parameters (CRGB array pointer and number
	* of LEDs in the strip). E.g.:
	* 
	*     FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
	* 
	* The addLeds function returns a reference to a static instance of a
	* CLEDController object. The addLeds call can be used directly as an
	* argument for the begin() function.
	* 
	* This is purposefully not done through the constructor to avoid a static
	* initialization order fiasco, as the "FastLED" instance (CFastLED class)
	* that contains the linked list of controllers exists in a separate
	* compilation unit from the main Arduino .ino file.
	* 
	* @param ctrl reference to assign to the internal controller pointer
	* @see Adafruit_NeoPixel::begin()
	*/
	void begin(CLEDController& ctrl);

	/** @ref Adafruit_NeoPixel::show() */
	void show();


	/** @ref Adafruit_NeoPixel::setPixelColor(uint16_t, uint8_t, uint8_t, uint8_t) */
	void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

	/** @ref Adafruit_NeoPixel::setPixelColor(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t) */
	void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

	/** @ref Adafruit_NeoPixel::setPixelColor(uint16_t, uint32_t) */
	void setPixelColor(uint16_t n, uint32_t c);

	/** @ref Adafruit_NeoPixel::fill(uint32_t, uint16_t, uint16_t) */
	void fill(uint32_t c = 0, uint16_t first = 0, uint16_t count = 0);


	/** @ref Adafruit_NeoPixel::setBrightness(uint8_t) */
	void setBrightness(uint8_t bright) { brightness = bright; }

	/** @ref Adafruit_NeoPixel::clear() */
	void clear();

	/**
	* Change the length of the strip object. The strip data is blanked.
	* The pin number and pixel format are unchanged.
	* 
	* @param n New length of the strip, in pixels.
	* @warning This function does not re-allocate the LED array 
	*          on the heap like Adafruit_NeoPixel. It will not set 
	*          a length longer than the length of the LED array, as
	*          passed in the constructor.
	* @see Adafruit_NeoPixel::updateLength(uint16_t)
	*/
	void updateLength(uint16_t n);

	/** @ref Adafruit_NeoPixel::canShow() */
	bool canShow();


	/** @ref Adafruit_NeoPixel::getPixels() const */
	uint8_t* getPixels() const { return (uint8_t*) leds; };

	/** @ref Adafruit_NeoPixel::getBrightness() const */
	uint8_t  getBrightness() const { return brightness; }

	/** @ref Adafruit_NeoPixel::numPixels() const */
	uint16_t numPixels() const { return numLEDs; }

	/** @ref Adafruit_NeoPixel::getPixelColor(uint16_t) const */
	uint32_t getPixelColor(uint16_t n) const;


	/** @ref Adafruit_NeoPixel::sine8(uint8_t) */
	static uint8_t sine8(uint8_t x)  { return Adafruit_NeoPixel::sine8(x); }

	/** @ref Adafruit_NeoPixel::gamma8(uint8_t) */
	static uint8_t gamma8(uint8_t x) { return Adafruit_NeoPixel::gamma8(x); }


	/** @ref Adafruit_NeoPixel::Color(uint8_t, uint8_t, uint8_t) */
	static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) { return Adafruit_NeoPixel::Color(r, g, b); }

	/** @ref Adafruit_NeoPixel::Color(uint8_t, uint8_t, uint8_t, uint8_t) */
	static uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) { return Adafruit_NeoPixel::Color(r, g, b, w); }

	/** @ref Adafruit_NeoPixel::ColorHSV(uint16_t, uint8_t, uint8_t) */
	static uint32_t ColorHSV(uint16_t hue, uint8_t sat = 255, uint8_t val = 255) { return Adafruit_NeoPixel::ColorHSV(hue, sat, val); }

	/** @ref Adafruit_NeoPixel::gamma32(uint32_t) */
	static uint32_t gamma32(uint32_t x) { return Adafruit_NeoPixel::gamma32(x); }

	
	/**
	* @defgroup NewFuncs New Functions
	* 
	* @brief These are new functions available in FastLED_NeoPixel that are not
	*         present in the original Adafruit_NeoPixel class.
	* 
	* @{
	*/

	/**
	* Returns a pointer to the internal CRGB array containing the LED
	* color data.
	* 
	* This can be used for native FastLED animations and functions.
	* 
	* @return CRGB pointer to color data
	*/
	CRGB* getLeds() const { return leds; };

	/**
	* Returns a pointer to the internal CLEDController object used for sending
	* the LED data to the strip.
	*
	* This can be used for setting color correction or sending other LED data
	* out on the LED pin used by the class.
	* 
	* @warning This will return nullptr until the `begin()` function is
	* called with the controller instance.
	*
	* @return CLEDController pointer to the controller instance
	*/
	CLEDController* getController() const { return controller; }

	/**
	* Overloaded version of the Color(uint8_t, uint8_t, uint8_t) function that
	* takes a CRGB struct as an argument and returns a packed uint32_t containing
	* the color data.
	* 
	* @param c the CRGB color to pack
	* @return uint32_t packed 24-bit version of the color data in RGB order, with blue as LSB
	* @see Adafruit_NeoPixel::Color(uint8_t, uint8_t, uint8_t)
	*/
	static uint32_t Color(const CRGB& c) { return Adafruit_NeoPixel::Color(c.r, c.g, c.b); }

	/**
	* Sets the 'blend white' feature of the class. If this is enabled, white
	* channel (RGB<b>W</b>) values passed to the setPixelColor function will
	* be blended into the RGB channels.
	*
	* This allows for simple animations written for RGBW strips to run on RGB
	* strips without modification. Because this blends the white channel data
	* into the RGB channels, more complicated animations that read the data
	* back from the strip will not work properly.
	* 
	* This feature is disabled by default.
	* 
	* @param blend boolean option for the state of the blend feature. Set to 'true'
	*        to enable white channel blending.
	*/
	void setBlendWhite(bool blend) { blendWhite = blend; }

	/** @} */


	/**
	* @defgroup Unimplemented Unimplemented Functions
	*
	* @brief These are the functions that are present in Adafruit_NeoPixel but
	*        for technical reasons are missing in this implementation.
	* 
	* All of these functions are declared but not defined. Attempting to use
	* them will create a compile-time error.
	* 
	* Unlike the Adafruit_NeoPixel library the FastLED library uses
	* compile-time constants for optimization purposes. Because of this it's
	* not easy to change LED settings such as the strip type, color order,
	* or data pin on the fly.
	* 
	* Because these arguments must be known at compile-time it's not possible
	* to use a simple member function to set them. The functions would need to
	* use a template parameter instead of an argument, which isn't the end of
	* the world but it breaks the "no modifications to your sketch" purpose
	* of the library.
	* 
	* The more significant complications is that the CLEDController objects
	* which send data to the strips are defined within a linked list using the
	* FastLED.addLeds() function. These objects have static storage duration,
	* so defining multiple controllers for the same strip will create a memory
	* leak. It may also cause a slowdown of the code, as calling FastLED.show()
	* will send the data to all CLEDController objects regardless of whether
	* they're currently in use or not.
	* 
	* It is possible to work around this by declaring a CLEDController object
	* *without* calling the FastLED.addLeds function. This will avoid static
	* storage duration and will avoid the memory leak / slowdown issues. The
	* downside is that the strip will not work with FastLED.show() and the
	* other linked list functions. The function to build the CLEDController
	* would also have to be templated, which means all of the values would
	* need to be passed together in a single function. At that point it's
	* simpler for the user to create the controller themselves and pass
	* it to the existing FastLED_NeoPixel_Variant::begin(CLEDController&) function.
	* 
	* @{
	*/

	/**
	* UNIMPLEMENTED: Changes the output pin
	* 
	* FastLED uses compile-time pin definitions so that the optimized data
	* latching code is only included for the pins used. This is a run-time
	* function and the arguments are not known at compile-time.
	* 
	* @see Adafruit_NeoPixel::setPin(uint16_t)
	*/
	void setPin(uint16_t p);

	/**
	* UNIMPLEMENTED: Changes the pixel format of the strip
	* 
	* The strip's color order is required at compile-time. Changing it during
	* the program would require rebuilding the CLEDController object with
	* all necessary compile-time constants.
	*/
	void updateType(neoPixelType T);

	/** @} */

private:
	/**
	* Converts an RGB or RGBW packed color (uint32_t) into a CRGB struct.
	* 
	* This accounts for white channel blending, which is why this cannot
	* be static.
	*/
	CRGB packedToColor(uint32_t c) const;

	CRGB* const leds;          ///< Pointer to LED data, in CRGB array
	const uint16_t maxLEDs;    ///< Number of RGB LEDs in the CRGB array
	uint16_t numLEDs;          ///< Number of RGB LEDs in the current strip
	CLEDController* controller = nullptr;  ///< Pointer to controller object for latching LED data

	uint8_t brightness = 255;  ///< Strip brightness, 0-255
	uint32_t endTime;          ///< Time of previous latch, in microseconds
	bool blendWhite = false;   ///< White blending feature state, on or off
};


/**
* constexpr function that converts an Adafruit_NeoPixel color order
* preprocessor define into a FastLED color order enum (EOrder) for
* use with the FastLED library.
* 
* For C++11 compatibility this uses chained ternaries instead of a more
* elegant switch statement.
* 
* This accounts for all RGB Adafruit_NeoPixel color orders, including both
* 800 KHz and 400 KHz speeds. It also handles FastLED color orders (EOrder).
* 
* Although the speed values are evaluated because they modify the underlying
* number, passing a speed value to this function has no effect on the output
* speed of the FastLED library. This function purely handles color order.
* 
* FastLED colors enumeration values (EOrder) are evaluated first, then high
* speed (default) NeoPixel color orders, then finally low speed NeoPixel
* color orders. If no match is found then 'GRB' color order is returned.
* This is the default color order used by most NeoPixel (WS2812B) strips.
* 
* RGBW color order is purposefully *not* evaluated. The FastLED library does
* not support RGBW and any attempts to use an RGBW strip with it are going
* to end badly ([PartsNotIncluded.com hackery aside]
* (https://www.partsnotincluded.com/fastled-rgbw-neopixels-sk6812/)). If they
* are passed to the function, RGBW color order defines will use the default
* GRB order.
* 
* This function is auto-generated from a Python script. See the
* neo_order_generator.py script in the `extras` folder.
*/
inline constexpr EOrder NeoToEOrder(uint32_t neoOrder) {
	return
		// FastLED Color Orders
		neoOrder == ((uint32_t) RGB) ? RGB :
		neoOrder == ((uint32_t) RBG) ? RBG :
		neoOrder == ((uint32_t) GRB) ? GRB :
		neoOrder == ((uint32_t) GBR) ? GBR :
		neoOrder == ((uint32_t) BRG) ? BRG :
		neoOrder == ((uint32_t) BGR) ? BGR :

		// NeoPixel Color Orders at Speed 'NEO_KHZ800'
		neoOrder == (NEO_RGB + NEO_KHZ800) ? RGB :
		neoOrder == (NEO_RBG + NEO_KHZ800) ? RBG :
		neoOrder == (NEO_GRB + NEO_KHZ800) ? GRB :
		neoOrder == (NEO_GBR + NEO_KHZ800) ? GBR :
		neoOrder == (NEO_BRG + NEO_KHZ800) ? BRG :
		neoOrder == (NEO_BGR + NEO_KHZ800) ? BGR :

		// NeoPixel Color Orders at Speed 'NEO_KHZ400'
		neoOrder == (NEO_RGB + NEO_KHZ400) ? RGB :
		neoOrder == (NEO_RBG + NEO_KHZ400) ? RBG :
		neoOrder == (NEO_GRB + NEO_KHZ400) ? GRB :
		neoOrder == (NEO_GBR + NEO_KHZ400) ? GBR :
		neoOrder == (NEO_BRG + NEO_KHZ400) ? BRG :
		neoOrder == (NEO_BGR + NEO_KHZ400) ? BGR :

		GRB;  // 'neoOrder' is not a valid color selection, using GRB as default
}


/**
* @brief FastLED implementation of the Adafruit_NeoPixel class for WS2812B strips, with data
*
* This is templated for a few reasons:
* * It allows the user to create a FastLED_NeoPixel class with (almost) the
*   same syntax as Adafruit_NeoPixel (using template args instead of function arguments):
*       
*       Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
*       FastLED_NeoPixel<LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800> strip;
* 
*   For ease of use this is preferable to splitting the arguments (`LED_COUNT`) 
*   between template and function.
* 
* * It allows allocating a static CRGB array for the LED data, so we don't have
*   to use heap allocation with `new()`
* * It allows us to create a begin() function that calls FastLED.addLeds
*   "automatically" without any other input or arguments. Just like
*   Adafruit_NeoPixel!
* 
* The one downside is that this splits the FastLED_NeoPixel and
* FastLED_NeoPixel_Variant classes, so static calls to FastLED_NeoPixel will
* fail for lack of template arguments. But that's how the cookie crumbles.
* 
* @tparam NumLeds the number of LEDs in the strip
* @tparam DataPin the number for the data pin connected to the strip
* @tparam RgbOrder the RGB channel order for the strips, either as a NeoPixel
*         define (e.g. NEO_GRB) or a FastLED EOrder enum value (e.g. GRB). By
*         default this is GRB, which is the typical color order for most
*         NeoPixel (WS2812B) strips.
* 
* @see Adafruit_NeoPixel
* @see NewFuncs
* @see Unimplemented
*/
template<uint16_t NumLeds, uint8_t DataPin, uint32_t RgbOrder = GRB>
class FastLED_NeoPixel : public FastLED_NeoPixel_Variant {
public:
	FastLED_NeoPixel() : FastLED_NeoPixel_Variant(ledData, NumLeds) 
	{
		memset(ledData, 0, sizeof(ledData));  // set all LEDs to black
	}

	/**
	* Creates and saves the CLEDController object for the strip, using WS2812B
	* as the strip type.
	*/
	void begin() {
		FastLED_NeoPixel_Variant::begin(FastLED.addLeds<WS2812B, DataPin, NeoToEOrder(RgbOrder)>(ledData, NumLeds));
	}

	/** @ref Adafruit_NeoPixel::getPin() */
	uint8_t getPin() const { return DataPin; };

private:
	CRGB ledData[NumLeds];
};


#endif // FASTLED_NEOPIXEL_H
