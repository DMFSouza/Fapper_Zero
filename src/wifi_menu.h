#pragma once
#ifndef WIFI_MENU_H
#define WIFI_MENU_H
#include "app_typedef.h"
#include "lvgl.h"
#include "ui.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>


extern app_t wifi_ui;
// Configuração básica usando configurações de rede comuns (porta DNS usual, IP e porta do servidor web)
extern const byte DNS_PORT;
extern IPAddress apIP;
extern IPAddress netMsk;
extern DNSServer dnsServer;
extern WebServer server;

#endif // SD_WINDOW_H
