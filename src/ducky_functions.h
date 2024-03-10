
#pragma once
#ifndef DUCKY_FUNCTIONS_H
#define DUCKY_FUNCTIONS_H
#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"
#include <hidcomposite.h>
#include "app_typedef.h"  // Inclua app_typedef.h aqui

extern HIDcomposite device;

void MouseDrawLine(String line);
void HIDKeySend(String str);
void runpayload(const char* path);
void runDuckyCode();
void initializeDevice();

extern app_t runDuckyCodeApp;
#endif // DUCKY_FUNCTIONS_H
