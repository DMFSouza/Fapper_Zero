/* DigiSpark_PWM Library
   soylentOrange - https://github.com/soylentOrange/DigiSpark_PWM
*/

#ifndef __DIGISPARK_PWM_HPP
#define __DIGISPARK_PWM_HPP

#include "Arduino.h"

#if !defined(ATTINY_CORE) || !defined(__AVR_ATtinyX5__)
#error This library is designed only for AVR_ATtinyX5 boards using ATTinyCore
#endif

#define ERROR_OK 0
#define ERROR_NOT_INITIALIZED 1
#define ERROR_INVALID_PIN 2

class DigiSpark_PWM {
protected:
  uint32_t _frequency;
  uint8_t _TOP_value;
  uint8_t _dutyCyclePercent;
  bool _PWM_initialized;
  uint8_t _pin;

public:
  DigiSpark_PWM();
  DigiSpark_PWM(uint8_t pin);

  uint8_t begin(uint32_t frequency = 25000, uint8_t dutyCyclePercent = 0);
  uint8_t setDutyCycle(uint8_t dutyCyclePercent = 0);
  uint8_t setFrequency(uint32_t frequency = 25000);
  uint8_t getPin();
};

#endif  //__DIGISPARK_PWM_HPP
