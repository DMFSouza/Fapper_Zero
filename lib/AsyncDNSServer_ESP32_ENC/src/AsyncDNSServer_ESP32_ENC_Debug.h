/****************************************************************************************************************************
  AsyncDNSServer_ESP32_ENC_Debug.h

  For ESP32_ENC (ESP32 + ENC28J60)

  AsyncDNSServer_ESP32_ENC is a Async DNS Server library for the ESP32_ENC

  Based on and modified from ESPAsyncDNSServer Library (https://github.com/devyte/ESPAsyncDNSServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC
  Licensed under MIT license

  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      28/11/2022 Initial coding for ESP32 + LwIP ENC28J60. Sync with AsyncDNSServer_STM32
  *****************************************************************************************************************************/

#pragma once

#ifndef AsyncDNSServer_ESP32_ENC_Debug_H
#define AsyncDNSServer_ESP32_ENC_Debug_H

#ifdef ASYNC_DNS_ESP32_ENC_DEBUG_PORT
  #define DBG_PORT_ASYNC_DNS      ASYNC_DNS_ESP32_ENC_DEBUG_PORT
#else
  #define DBG_PORT_ASYNC_DNS      Serial
#endif

// Change _ASYNC_DNS_ESP32_ENC_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _ASYNC_DNS_ESP32_ENC_LOGLEVEL_
  #define _ASYNC_DNS_ESP32_ENC_LOGLEVEL_       0
#endif

/////////////////////////////////////////////////////////

const char ASYNC_DNS_MARK[] = "[DNS] ";

#define ASYNC_DNS_PRINT        DBG_PORT_ASYNC_DNS.print
#define ASYNC_DNS_PRINTLN      DBG_PORT_ASYNC_DNS.println

#define ASYNC_DNS_PRINT_MARK   ASYNC_DNS_PRINT(ASYNC_DNS_MARK)
#define ASYNC_DNS_PRINT_SP     DBG_PORT_ASYNC_DNS.print(" ")

///////////////////////////////////////

#define DNS_LOG(x)            { ASYNC_DNS_PRINTLN(x); }
#define DNS_LOG0(x)           { ASYNC_DNS_PRINT(x); }
#define DNS_LOG1(x,y)         { ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINTLN(y); }
#define DNS_LOG2(x,y,z)       { ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT(y); ASYNC_DNS_PRINTLN(z); }
#define DNS_LOG3(x,y,z,w)     { ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT(y); ASYNC_DNS_PRINT(z); ASYNC_DNS_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define DNS_LOGERROR(x)         if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>0) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINTLN(x); }
#define DNS_LOGERROR0(x)        if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>0) { ASYNC_DNS_PRINT(x); }
#define DNS_LOGERROR1(x,y)      if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>0) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(y); }
#define DNS_LOGERROR2(x,y,z)    if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>0) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(y); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(z); }
#define DNS_LOGERROR3(x,y,z,w)  if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>0) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(y); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(z); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define DNS_LOGWARN(x)          if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>1) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINTLN(x); }
#define DNS_LOGWARN0(x)         if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>1) { ASYNC_DNS_PRINT(x); }
#define DNS_LOGWARN1(x,y)       if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>1) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(y); }
#define DNS_LOGWARN2(x,y,z)     if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>1) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(y); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(z); }
#define DNS_LOGWARN3(x,y,z,w)   if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>1) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(y); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(z); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define DNS_LOGINFO(x)          if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>2) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINTLN(x); }
#define DNS_LOGINFO0(x)         if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>2) { ASYNC_DNS_PRINT(x); }
#define DNS_LOGINFO1(x,y)       if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>2) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(y); }
#define DNS_LOGINFO2(x,y,z)     if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>2) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(y); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(z); }
#define DNS_LOGINFO3(x,y,z,w)   if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>2) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(y); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(z); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define DNS_LOGDEBUG(x)         if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>3) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINTLN(x); }
#define DNS_LOGDEBUG0(x)        if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>3) { ASYNC_DNS_PRINT(x); }
#define DNS_LOGDEBUG1(x,y)      if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>3) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(y); }
#define DNS_LOGDEBUG2(x,y,z)    if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>3) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(y); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(z); }
#define DNS_LOGDEBUG3(x,y,z,w)  if(_ASYNC_DNS_ESP32_ENC_LOGLEVEL_>3) { ASYNC_DNS_PRINT_MARK; ASYNC_DNS_PRINT(x); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(y); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINT(z); ASYNC_DNS_PRINT_SP; ASYNC_DNS_PRINTLN(w); }


/////////////////////////////////////////////////////////

#endif    //AsyncDNSServer_ESP32_ENC_Debug_H
