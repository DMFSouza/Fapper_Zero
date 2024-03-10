#
#  Project      FastLED NeoPixel Library
#  @author      David Madison
#  @link        github.com/dmadison/FastLED_NeoPixel
#  @license     MIT - Copyright (c) 2021 David Madison
#
#  This file is part of the FastLED_NeoPixel library.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

import re

# This script generates the function to translate NeoPixel channel color order
# definitions into FastLED color order definitions ("NeoToEOrder")

neopixel_color_orders = """
// RGB NeoPixel permutations; white and red offsets are always same
// Offset:         W        R        G        B
#define NEO_RGB  ((0<<6) | (0<<4) | (1<<2) | (2)) ///< Transmit as R,G,B
#define NEO_RBG  ((0<<6) | (0<<4) | (2<<2) | (1)) ///< Transmit as R,B,G
#define NEO_GRB  ((1<<6) | (1<<4) | (0<<2) | (2)) ///< Transmit as G,R,B
#define NEO_GBR  ((2<<6) | (2<<4) | (0<<2) | (1)) ///< Transmit as G,B,R
#define NEO_BRG  ((1<<6) | (1<<4) | (2<<2) | (0)) ///< Transmit as B,R,G
#define NEO_BGR  ((2<<6) | (2<<4) | (1<<2) | (0)) ///< Transmit as B,G,R
"""

fastled_color_orders = """
// enum EOrder {
	RGB=0012,
	RBG=0021,
	GRB=0102,
	GBR=0120,
	BRG=0201,
	BGR=0210
// }
"""

argument_name = "neoOrder"

function_start = """
inline constexpr EOrder NeoToEOrder(uint32_t neoOrder) {
	return
"""

function_end = "\t\tGRB;  // '{}' is not a valid color selection, using GRB as default\n}}".format(argument_name)


def generate_color_orders():
	output = function_start

	# FastLED color orders
	output += "\t\t// FastLED Color Orders\n"
	for line in fastled_color_orders.splitlines():
		if line == "" or line.startswith("//"): continue

		match = re.search("[RGBW]{3,}", line)
		if match is not None:
			fastled_order = match.group(0)  # get match text from output
			output += "\t\t{} == ((uint32_t) {}) ? {} :\n".format(argument_name, fastled_order, fastled_order)
		else:
			raise RuntimeError("Could not find color order in string \"{}\"".format(line))
	output += '\n'

	# NeoPixel color orders
	for speed in ["NEO_KHZ800", "NEO_KHZ400"]:
		output += "\t\t// NeoPixel Color Orders at Speed '{}'\n".format(speed)

		for line in neopixel_color_orders.splitlines():
			if line == "" or line.startswith("//"): continue

			match = re.search("NEO_[RGBW]{3,}", line)
			if match is not None:
				neopixel_order = match.group(0)  # get match text from output
				fastled_order = neopixel_order.replace("NEO_", "").replace("W", "")  # get plain "RGB" order
				neopixel_combined = "({:8s} + {})".format(neopixel_order, speed)
				output += "\t\tneoOrder == {} ? {} :\n".format(neopixel_combined, fastled_order)
			else:
				raise RuntimeError("Could not find color order in string \"{}\"".format(line))
		output += '\n'
	
	output += function_end
	return output


if __name__ == "__main__":
	output = generate_color_orders()
	print(output)
