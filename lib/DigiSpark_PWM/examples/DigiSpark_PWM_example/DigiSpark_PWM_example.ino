/* DigiSpark_PWM Library
   soylentOrange - https://github.com/soylentOrange/DigiSpark_PWM
*/

#include "DigiSpark_PWM.h"  // https://github.com/soylentOrange/DigiSpark_PWM

uint8_t g_dutyCycle;   // DC in %
uint32_t g_frequency;  // frequency in Hz

// Cretae instance of DigiSpark_PWM-class, connected to Pin-PB1
// (This Pin is connected to the onboard LED)
DigiSpark_PWM pwm = DigiSpark_PWM(PIN_PB1);

// For connecting to Pin PB4 simply use:
// DigiSpark_PWM pwm = DigiSpark_PWM();

// begin PWM-output
void setup() {
  // initialize and start PWM-output @1Hz with 50% duty-cycle
  g_frequency = 1;   // 1Hz
  g_dutyCycle = 50;  // 50%
  pwm.begin(g_frequency, g_dutyCycle);
}

void loop() {
  // nothing to do here, the LED will blink driven by the hardware PWM
}
