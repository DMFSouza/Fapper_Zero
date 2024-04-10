#pragma once
#include "Arduino.h"
#include "lvgl.h"
#include "ducky_functions.h"
#include "sd_window.h"
#include "ble_spam.h"
#include "wifi_menu.h"
//#include "rf_menu.h"
#include "rfid.h"
#include "config.h"
#include "ir_control.h"

#define LV_DELAY(x)                                                                                                                                  \
  do {                                                                                                                                               \
    uint32_t t = x;                                                                                                                                  \
    while (t--) {                                                                                                                                    \
      lv_timer_handler();                                                                                                                            \
      delay(1);                                                                                                                                      \
    }                                                                                                                                                \
  } while (0);

void ui_init();
void ui_boot_anim();

/**
 * @brief A brief
 *  string of information appears at the top of the page.
 * 
 * @param str String to display
 * @param delay Display time
 */
void ui_send_msg(char *str, uint32_t delay);
