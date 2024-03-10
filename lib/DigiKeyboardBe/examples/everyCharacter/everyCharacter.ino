
#include "DigiKeyboardFr.h"

void setup() {
  // don't need to set anything up to use DigiKeyboard
}


void loop() {
  // every charater that thislibrary can type
  DigiKeyboardFr.println("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~");
  
  // It's better to use DigiKeyboard.delay() over the regular Arduino delay()
  // if doing keyboard stuff because it keeps talking to the computer to make
  // sure the computer knows the keyboard is alive and connected
  DigiKeyboardFr.delay(500);
}
