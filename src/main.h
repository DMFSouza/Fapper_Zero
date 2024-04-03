
#include <pgmspace.h>

// Two regions!
#define NA 1 //set by a HIGH on REGIONSWITCH pin
#define EU 0 //set by a LOW on REGIONSWITCH pin


#define IRLED 17         //the IR sender LED 
#define NUM_ELEM(x) (sizeof (x) / sizeof (*(x)));

// set define to 0 to turn off debug output
#define DEBUG 0
#define DEBUGP(x) if (DEBUG == 1) { x ; }

// Shortcut to insert single, non-optimized-out nop
#define NOP __asm__ __volatile__ ("nop")

// Not used any more on esp8266, so don't bother
// Tweak this if neccessary to change timing
// -for 8MHz Arduinos, a good starting value is 11
// -for 16MHz Arduinos, a good starting value is 25
#define DELAY_CNT 25

// Makes the codes more readable. the OCRA is actually
// programmed in terms of 'periods' not 'freqs' - that
// is, the inverse!
// #define freq_to_timerval(x) (F_CPU / 8 / x - 1)
#define freq_to_timerval(x) (x / 1000)

// The structure of compressed code entries
struct IrCode {
  uint8_t timer_val;
  uint8_t numpairs;
  uint8_t bitcompression;
  uint16_t const *times;
  uint8_t const *codes;
};
