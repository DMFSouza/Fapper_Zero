#include "DigiKeyboardFr.h"

void setup() {
  // don't need to set anything up to use DigiKeyboard
}


void loop() {
  DigiKeyboardFr.sendKeyStroke(KEY_FR_R,MOD_GUI_LEFT); //Open the Run dialog box
  DigiKeyboardFr.delay(500);// wait for the dialog box appear
  DigiKeyboardFr.print("cmd");// type cmd wihout enter
  DigiKeyboardFr.sendKeyStroke(KEY_ENTER , MOD_CONTROL_LEFT | MOD_SHIFT_LEFT); // start as administrator Ctrl+Shift+Enter
  DigiKeyboardFr.delay(500);// wait for the admin dialog box appear
  DigiKeyboardFr.sendKeyStroke(KEY_ARROW_LEFT); //validate the oppening of the administrator cmd
  DigiKeyboardFr.sendKeyStroke(KEY_ENTER);
  DigiKeyboardFr.delay(10000);// wait
}
