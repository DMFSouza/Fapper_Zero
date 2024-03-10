#include "DigiKeyboardFr.h"

void setup() {
	// don't need to set anything up to use DigiKeyboard
}

void loop() {
	DigiKeyboardFr.sendKeyStroke(KEY_CAPSLOCK);
	DigiKeyboardFr.sendKeyStroke(KEY_NUMLOCK);
	DigiKeyboardFr.delay(1000);
	DigiKeyboardFr.sendKeyStroke(KEY_CAPSLOCK);
	DigiKeyboardFr.sendKeyStroke(KEY_NUMLOCK);
	DigiKeyboardFr.delay(1000);
}