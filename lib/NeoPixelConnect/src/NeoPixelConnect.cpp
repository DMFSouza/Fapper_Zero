/*
 Copyright (c) 2020-2022 Alan Yorinks All rights reserved.

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


/// @brief Constructor - pio will be set to pio0 and sm to 0
/// @param pinNumber: GPIO pin that controls the NeoPixel string.
/// @param numberOfPixels: Number of pixels in the string

NeoPixelConnect::NeoPixelConnect(byte pinNumber, uint16_t numberOfPixels) {
    this->pixelSm = 0;
    this->pixelPio = pio0;
    this->neoPixelInit(pinNumber, numberOfPixels);
}

/// @brief Constructor
/// @param pinNumber: GPIO pin that controls the NeoPixel string.
/// @param numberOfPixels: Number of pixels in the string
/// @param pio: pio selected - default = pio0. pio1 may be specified
/// @param sm: state machine selected. Default = 0
NeoPixelConnect::NeoPixelConnect(byte pinNumber, uint16_t numberOfPixels, PIO pio, uint sm) {
    this->pixelSm = sm;
    this->pixelPio = pio;
    this->neoPixelInit(pinNumber, numberOfPixels);
}

/// @brief Continuation of Constructor
/// @param pinNumber: GPIO pin that controls the NeoPixel string.
/// @param numberOfPixels: Number of pixels in the string
void NeoPixelConnect::neoPixelInit(byte pinNumber, uint16_t numberOfPixels) {
    uint offset = pio_add_program(this->pixelPio, &ws2812_program);
    ws2812_program_init(this->pixelPio, this->pixelSm, offset, pinNumber, 800000, false);

    // save the number of pixels in use
    this->actual_number_of_pixels = numberOfPixels;

    // set the pixels to the fill color
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++) {
        this->pixelBuffer[i][RED] = 0;
        this->pixelBuffer[i][GREEN] = 0;
        this->pixelBuffer[i][BLUE] = 0;
    }

    // show the pixels
    this->neoPixelShow();
    delay(1);
}

/// @brief Set a NeoPixel to a given color. By setting autoShow to true, change is
/// displayed immediately.
/// @param pixelNumber: set a color for a specific neopixel in the string
/// @param r: red value (0-255)
/// @param g: green value(0-255)
/// @param b: blue value (0-255)
/// @param autoShow: If true, show the change immediately.
void NeoPixelConnect::neoPixelSetValue(uint16_t pixelNumber, uint8_t r, uint8_t g, uint8_t b, bool autoShow) {
    this->pixelBuffer[pixelNumber][RED] = r;
    this->pixelBuffer[pixelNumber][GREEN] = g;
    this->pixelBuffer[pixelNumber][BLUE] = b;
    if (autoShow) {
        this->neoPixelShow();
    }
}

/// @brief Set all the pixels to "off".
/// @param autoShow: If true, show the change immediately
void NeoPixelConnect::neoPixelClear(bool autoShow) {
    // set all the neopixels in the buffer to all zeroes
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++) {
        this->pixelBuffer[i][RED] = 0;
        this->pixelBuffer[i][GREEN] = 0;
        this->pixelBuffer[i][BLUE] = 0;
    }
    if (autoShow) {
        this->neoPixelShow();
    }
}

/// @brief Fill all the pixels with same value
/// @param r: red value (0-255)
/// @param g: green value(0-255)
/// @param b: blue value (0-255)
/// @param autoShow: If true, show the change immediately.
void NeoPixelConnect::neoPixelFill(uint8_t r, uint8_t g, uint8_t b, bool autoShow) {
    // fill all the neopixels in the buffer with the
    // specified rgb values.
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++) {
        this->pixelBuffer[i][RED] = r;
        this->pixelBuffer[i][GREEN] = g;
        this->pixelBuffer[i][BLUE] = b;
    }
    if (autoShow) {
        this->neoPixelShow();
    }
}

/// @brief Display all the pixels in the buffer
void NeoPixelConnect::neoPixelShow(void) {
    // show the neopixels in the buffer
    for (uint16_t i = 0; i < this->actual_number_of_pixels; i++) {
        this->putPixel(
                urgb_u32(
                        pixelBuffer[i][RED],
                        pixelBuffer[i][GREEN],
                        pixelBuffer[i][BLUE]
                )
        );
    }
}

/// @brief convert rgb values into a single uint32_t
/// @param r: red value (0-255)
/// @param g: green value(0-255)
/// @param b: blue value (0-255)
uint32_t NeoPixelConnect::urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t)(r) << 8) |
            ((uint32_t)(g) << 16) |
            (uint32_t)(b);
}

/// @brief set a pixel's value to reflect pixel_grb
/// @param pixel_grb: rgb represented as a 32 bit value
void NeoPixelConnect::putPixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(this->pixelPio, this->pixelSm,
                        pixel_grb << 8u);
}