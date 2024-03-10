# FastLED NeoPixel Library
[![arduino-library-badge](https://www.ardu-badge.com/badge/FastLED%20NeoPixel.svg?)](https://www.ardu-badge.com/FastLED%20NeoPixel) [![Build Status](https://github.com/dmadison/FastLED_NeoPixel/workflows/build/badge.svg)](https://github.com/dmadison/FastLED_NeoPixel/actions/workflows/ci.yml) [![Documentation](https://img.shields.io/badge/Docs-Doxygen-blue.svg)](http://dmadison.github.io/FastLED_NeoPixel/docs/index.html)
[![MIT license](https://img.shields.io/badge/License-MIT-orange.svg)](https://github.com/dmadison/FastLED_NeoPixel/blob/master/LICENSE)


[FastLED NeoPixel](https://github.com/dmadison/FastLED_NeoPixel/) is an Arduino library that enables you to reuse most animations written for the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) with the [FastLED library](https://github.com/FastLED/FastLED) without having to rewrite them.

Some of the benefits of using the FastLED library over Adafruit NeoPixel include:

* Static memory allocation for LED data
* Non-destructive brightness control
* Temporal dithering
* High performance math and memory functions
* A wide variety of supported LED chipsets

If you are looking to use a NeoPixel LED strip with FastLED and have *not* written any code, *you do not need this library!* FastLED supports NeoPixel (WS2812B) LEDs natively. See the FastLED [examples](https://github.com/FastLED/FastLED/blob/master/examples/Blink/Blink.ino) and [documentation](http://fastled.io/docs/3.1/class_w_s2812_b.html) for more information.

## Quick Start

```cpp
#include <FastLED_NeoPixel.h>

// Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, COLOR_ORDER);  // <- The original Adafruit_NeoPixel object
FastLED_NeoPixel<LED_COUNT, LED_PIN, COLOR_ORDER> strip;      // <- The new FastLED_NeoPixel object
```

After installing the library, include the `FastLED_NeoPixel.h` header in your sketch. Replace your `strip` object definition with the `FastLED_NeoPixel` equivalent, using template arguments instead of function arguments. See the example above for the exact syntax required.

For most animations **no other changes are necessary!** The code will now use the FastLED library to handle the LED animations and data transfer to the LEDs. You can use additional library functions to get a pointer to the underlying `CRGB` array for use with other FastLED library functions and animations.

### Advanced Setup

```cpp
#include <FastLED_NeoPixel.h>

#define NUM_LEDS 60
#define DATA_PIN 6

CRGB leds[NUM_LEDS];
FastLED_NeoPixel_Variant strip(leds, NUM_LEDS);

void setup() {
	strip.begin(FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS));
}
```

If you already have an existing `CRGB` array, or if you'd like to use an LED strip type other than NeoPixel (WS2812B), you can use the `FastLED_NeoPixel_Variant` class. This is a non-templated class which uses a pointer to the LED data rather than storing it internally. You must also pass the `CLEDController` reference created by the `FastLED.addLeds<>()` call to the `begin()` function in order for the class `show()` calls to work. Past these initial setup changes, this functions identically to the all-encompassing `FastLED_NeoPixel` class.

## Limitations

Due to FastLED's need for compile-time constants, the functions to change the LED count, LED pin, and color order are not implemented. Additionally, because FastLED does not organize color data in memory based on the color channel order of the strip, animations that use the underlying LED data pointer (`uint8_t* getPixels()`) may not function correctly.

For more information on the functional differences between the two libraries, [see the documentation](http://dmadison.github.io/FastLED_NeoPixel/docs/modules.html).

## License

This library is licensed under the terms of the [MIT license](https://opensource.org/licenses/MIT). See the [LICENSE](https://github.com/dmadison/FastLED_NeoPixel/blob/master/LICENSE) file for more information.

In order to function this library links to both the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) and the [FastLED library](https://github.com/FastLED/FastLED). Please refer to their licensing documents as well.
