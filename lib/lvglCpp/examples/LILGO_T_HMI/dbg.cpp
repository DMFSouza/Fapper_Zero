#include "dbg.h"

#include <Arduino.h>
void DBG_print(String str) {
	Serial.print(str);
}

void DBG_print(char *str) {
		Serial.print(str);
}

void DBG_println(String str) {
		Serial.println(str);
}

void DBG_println(char *str) {
		Serial.println(str);
}

void DBG_printf(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args);
	va_end(args);
	char buffer[size + 1];
	va_start(args, fmt);
	vsnprintf(buffer, size + 1, fmt, args);
	va_end(args);
	Serial.print(buffer);
}
