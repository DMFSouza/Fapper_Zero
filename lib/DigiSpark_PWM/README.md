# DigiSpark_PWM

[![arduino-library-badge](https://www.ardu-badge.com/badge/DigiSpark_PWM.svg?)](https://www.ardu-badge.com/DigiSpark_PWM)
[![GitHub release](https://img.shields.io/github/release/soylentOrange/DigiSpark_PWM.svg)](https://github.com/soylentOrange/DigiSpark_PWM/releases)
[![GitHub](https://img.shields.io/github/license/soylentOrange/DigiSpark_PWM)](https://github.com/soylentOrange/DigiSpark_PWM/blob/master/LICENSE)
[![GitHub issues](https://img.shields.io/github/issues/soylentOrange/DigiSpark_PWM)](https://github.com/soylentOrange/DigiSpark_PWM/issues)

---

## Abstract
This hardware-based Pulse-Width-Modulation (PWM) library enables you to use the Hardware-PWM on **Arduino AVR ATtiny85-based boards (Digispark)** using [**ATTinyCore**](https://github.com/SpenceKonde/ATTinyCore). The **Timer/Counter1** is used in _asynchronous mode_ to give a PWM-output on **Pin PB1** or **Pin PB4** up to 500 kHz. The maximum resolution is **8-bit**, the effective resolution typically between **7-bit** and **8-bit**.

---

## Table of Contents

* [Why do we need this library](#why-do-we-need-this-library)
* [Example](#example)
* [Function Reference](#functions)
* [Installation](#installation)

---

## Why do we need this library

I needed a hardware PWM for a [**DigiSpark-board**](https://www.azdelivery.de/en/products/digispark-board) to drive a fan at 25kHz and didn't found a library supporting the [**ATTinyCore**](https://github.com/SpenceKonde/ATTinyCore).  
If you have one of the boards and need a flexible hardware-based PWM, this library might also fit for you. Otherwise, I'm still happy to use it ;).

> See the provided example on how to the library.

--

## Example
A bare minimum example is given below:
```c++
#include "DigiSpark_PWM.h"  // https://github.com/soylentOrange/DigiSpark_PWM

// Cretae instance of DigiSpark_PWM-class, connected to Pin-PB1
// (This Pin is connected to the onboard LED)
DigiSpark_PWM pwm = DigiSpark_PWM(PIN_PB1);

// For connecting to Pin PB4 simply use:
// DigiSpark_PWM pwm = DigiSpark_PWM();

// begin PWM-output
void setup() {
  // initialize and start PWM-output @1Hz with 50% duty-cycle
  pwm.begin(1, 50);
}

void loop() {
  // nothing to do here, the LED will blink driven by the hardware PWM
}
```

### Get example
The example can be obtained from within the Arduino IDE in File->Examples->DigiSpark_PWM->DigiSpark_PWM_example. 

---

## Functions
#### uint8_t begin(uint32_t frequency, uint8_t dutyCyclePercent) 
This function initializes the library. Call before use...  
Initial frequency (in Hz) and duty cycle (in percent) are given here.
The funcion will return an error if PWM is unavailable:
* ERROR_INVALID_PIN (0x02) - pwm is not supported on the pin,
* or 0 if everything went well.
#### uint8_t setDutyCycle(uint8_t dutyCyclePercent) 
Dynamically set the duty cycle (in percent).
The funcion will return an error if PWM is unavailable:
* ERROR_NOT_INITIALIZED  (0x01) - pwm is not initialized yet,
* ERROR_INVALID_PIN (0x02) - pwm is not supported on the pin,
* or 0 if everything went well.
#### uint8_t setFrequency(uint32_t frequency) 
Dynamically set the freuqncy of the pwm (in Hz). The duty cycle will match the prevous setting.
The funcion will return an error if PWM is unavailable:
* ERROR_NOT_INITIALIZED  (0x01) - pwm is not initialized yet,
* ERROR_INVALID_PIN (0x02) - pwm is not supported on the pin,
* or 0 if everything went well. 
#### uint8_t getPin()
Returns the pin given during instanciation.
---

## Installation

#### Library Manager
Open up the `Arduino Library Manager` in the Arduino IDE and search for [**DigiSpark_PWM**](https://github.com/soylentOrange/Forced-DigiSpark_PWM/). Select / install the the latest version. Click this badge [![arduino-library-badge](https://www.ardu-badge.com/badge/DigiSpark_PWM.svg?)](https://www.ardu-badge.com/DigiSpark_PWM) for more detailed instructions.

#### Download
Press the green _clone or download_ button in the upper-right and download as _.ZIP_. Then go to the Arduino IDE and go to _Sketch_>Use Library->Add .ZIP Library_ and select the just downloaded zip file.


