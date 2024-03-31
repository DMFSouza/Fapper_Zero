#pragma once
#ifndef CONFIG_H
#define CONFIG_H
#include "app_typedef.h"
#include "lvgl.h"
#include "ui.h"
struct RGBcolor {
    int _red;
    int _green;
    int _blue;
};
extern RGBcolor rgb;
extern app_t config_ui;


#endif // CONFIG_H
