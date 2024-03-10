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

# This script generates a rainbow CSS animation, using multiple discrete steps
# for each keyframe.

def generate_rainbow_steps(name, num_steps):
	if num_steps > 360: num_steps = 360  # limit to 360 (hue degrees)
	elif num_steps < 2: num_steps = 2
 
	output = "@keyframes {} {{\n".format(name)

	percent_change = 100.0 / float(num_steps)  # percent for each keyframe
	degree_change = 360 / int(num_steps)  # hue degree for each keyframe

	for step in range(num_steps + 1):
		percent = (percent_change * step)
		degree = int((degree_change * step))
		output += "\t{:7.3f}% {{ fill: hsl({:3d},100%,50%); }}\n".format(percent, degree)

	output += "}"
	return output

if __name__ == "__main__":
	output = generate_rainbow_steps("rainbow", 32)
	print(output)
