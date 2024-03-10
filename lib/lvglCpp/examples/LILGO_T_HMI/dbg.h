#ifndef __DBG_H__
#define __DBG_H__

#include <Arduino.h>

void DBG_print(String str);
void DBG_print(char *str);
void DBG_println(String str);
void DBG_println(char *str);
void DBG_printf(char *fmt, ...);

#endif // __DBG_H__
