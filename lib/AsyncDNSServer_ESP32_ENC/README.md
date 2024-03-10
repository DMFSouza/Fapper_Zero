# AsyncDNSServer_ESP32_ENC


[![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncDNSServer_ESP32_ENC.svg?)](https://www.ardu-badge.com/AsyncDNSServer_ESP32_ENC)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncDNSServer_ESP32_ENC.svg)](https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC/releases)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/AsyncDNSServer_ESP32_ENC.svg)](http://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC/issues)


<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>
<a href="https://profile-counter.glitch.me/khoih-prog/count.svg" title="Total khoih-prog Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog/count.svg" style="height: 30px;width: 200px;"></a>
<a href="https://profile-counter.glitch.me/khoih-prog-AsyncDNSServer_ESP32_ENC/count.svg" title="Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog-AsyncDNSServer_ESP32_ENC/count.svg" style="height: 30px;width: 200px;"></a>


---
---

## Table of Contents


* [Why do we need this AsyncDNSServer_ESP32_ENC library](#why-do-we-need-this-AsyncDNSServer_ESP32_ENC-library)
  * [Features](#features)
  * [Why Async is better](#why-async-is-better)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Libraries' Patches](#libraries-patches)
  * [1. For fixing ESP32 compile error](#1-for-fixing-esp32-compile-error)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error) 
* [HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)](#howto-use-analogread-with-esp32-running-wifi-andor-bluetooth-btble)
  * [1. ESP32 has 2 ADCs, named ADC1 and ADC2](#1--esp32-has-2-adcs-named-adc1-and-adc2)
  * [2. ESP32 ADCs functions](#2-esp32-adcs-functions)
  * [3. ESP32 WiFi uses ADC2 for WiFi functions](#3-esp32-wifi-uses-adc2-for-wifi-functions)
* [HOWTO Setting up the Async DNS Server](#howto-setting-up-the-async-dns-server)
* [Examples](#examples)
	* [ 1. AsyncCaptivePortalAdvanced_ESP32_ENC](examples/AsyncCaptivePortalAdvanced_ESP32_ENC)
  * [ 2. AsyncCaptivePortal_ESP32_ENC](examples/AsyncCaptivePortal_ESP32_ENC) 
  * [ 3. AsyncDNServerFull_ESP32_ENC](examples/AsyncDNServerFull_ESP32_ENC)
  * [ 4. AsyncDNSServer_ESP32_ENC](examples/AsyncDNSServer_ESP32_ENC)
* [Example AsyncDNSServer_ESP32_ENC](#example-AsyncDNSServer_ESP32_ENC)
  * [1. File AsyncDNSServer_ESP32_ENC.ino](#1-file-AsyncDNSServer_ESP32_ENCino)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [AsyncDNSServer_ESP32_ENC library](https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC)

#### Features

This [AsyncDNSServer_ESP32_ENC library](https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC) is a fully asynchronous DNSServer library, designed for a trouble-free, multi-connection network environment, for **ESP32 boards using LwIP ENC28J60 Ethernet**.

This library is based on, modified from:

1. [Develo's ESPAsyncDNSServer](https://github.com/devyte/ESPAsyncDNSServer)

to apply the better and faster **asynchronous** feature of the **powerful** [AsyncDNSServer_ESP32_ENC Library](https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC) into **ESP32 boards using LwIP ENC28J60 Ethernet**.


#### Why Async is better

- Using asynchronous network means that you can handle **more than one connection at the same time**
- You are called once the packet is ready
- After a DNS Client connected to this Async DNS server, you are **immediately ready** to handle other connections while the Server is taking care of receiving and responding to the UDP packets in the background.
- You are not required to check in a tight loop() the arrival of the DNS requesting packets to process them.
- **Speed is OMG**


#### Currently supported Boards

1. **ESP32_ENC boards** using ESP32 boards with `LwIP` ENC28J60 Ethernet

---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://www.arduino.cc/en/Main/Software)
 2. [`ESP32 Core 2.0.5+`](https://github.com/espressif/arduino-esp32) for ESP32-based boards. [![Latest release](https://img.shields.io/github/release/espressif/arduino-esp32.svg)](https://github.com/espressif/arduino-esp32/releases/latest/)
 3. [`WebServer_ESP32_ENC library 1.5.1+`](https://github.com/khoih-prog/WebServer_ESP32_ENC). To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/WebServer_ESP32_ENC.svg?)](https://www.ardu-badge.com/WebServer_ESP32_ENC)
 4. [`AsyncUdp_ESP32_ENC library 2.0.0+`](https://github.com/khoih-prog/AsyncUdp_ESP32_ENC). To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncUdp_ESP32_ENC.svg?)](https://www.ardu-badge.com/AsyncUdp_ESP32_ENC)
 
---
---

### Installation

The suggested way to install is to:

#### Use Arduino Library Manager

The best way is to use `Arduino Library Manager`. Search for `AsyncDNSServer_ESP32_ENC`, then select / install the latest version. You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncDNSServer_ESP32_ENC.svg?)](https://www.ardu-badge.com/AsyncDNSServer_ESP32_ENC) for more detailed instructions.

### Manual Install

1. Navigate to [AsyncDNSServer_ESP32_ENC](https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC) page.
2. Download the latest release `AsyncDNSServer_ESP32_ENC-main.zip`.
3. Extract the zip file to `AsyncDNSServer_ESP32_ENC-main` directory 
4. Copy the whole `AsyncDNSServer_ESP32_ENC-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**AsyncDNSServer_ESP32_ENC** library](https://registry.platformio.org/libraries/AsyncDNSServer_ESP32_ENC) by using [Library Manager](https://registry.platformio.org/libraries/AsyncDNSServer_ESP32_ENC/installation). Search for AsyncDNSServer_ESP32_ENC in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### Libraries' Patches

#### 1. For fixing ESP32 compile error

To fix [`ESP32 compile error`](https://github.com/espressif/arduino-esp32), just copy the following file into the [`ESP32`](https://github.com/espressif/arduino-esp32) cores/esp32 directory (e.g. ./arduino-1.8.19/hardware/espressif/cores/esp32) to overwrite the old file:
- [Server.h](LibraryPatches/esp32/cores/esp32/Server.h)

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include this `.hpp` file

```cpp
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "AsyncDNSServer_ESP32_ENC.hpp"     //https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC
```

in many files. But be sure to use the following `.h` file **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```cpp
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "AsyncDNSServer_ESP32_ENC.h"       //https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.

---
---

### HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)

Please have a look at [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to have more detailed description and solution of the issue.

#### 1.  ESP32 has 2 ADCs, named ADC1 and ADC2

#### 2. ESP32 ADCs functions

- `ADC1` controls ADC function for pins **GPIO32-GPIO39**
- `ADC2` controls ADC function for pins **GPIO0, 2, 4, 12-15, 25-27**

#### 3.. ESP32 WiFi uses ADC2 for WiFi functions

Look in file [**adc_common.c**](https://github.com/espressif/esp-idf/blob/master/components/driver/adc_common.c#L61)

> In `ADC2`, there're two locks used for different cases:
> 1. lock shared with app and Wi-Fi:
>    ESP32:
>         When Wi-Fi using the `ADC2`, we assume it will never stop, so app checks the lock and returns immediately if failed.
>    ESP32S2:
>         The controller's control over the ADC is determined by the arbiter. There is no need to control by lock.
> 
> 2. lock shared between tasks:
>    when several tasks sharing the `ADC2`, we want to guarantee
>    all the requests will be handled.
>    Since conversions are short (about 31us), app returns the lock very soon,
>    we use a spinlock to stand there waiting to do conversions one by one.
> 
> adc2_spinlock should be acquired first, then adc2_wifi_lock or rtc_spinlock.


- In order to use `ADC2` for other functions, we have to **acquire complicated firmware locks and very difficult to do**
- So, it's not advisable to use `ADC2` with WiFi/BlueTooth (BT/BLE).
- Use `ADC1`, and pins GPIO32-GPIO39
- If somehow it's a must to use those pins serviced by `ADC2` (**GPIO0, 2, 4, 12, 13, 14, 15, 25, 26 and 27**), use the **fix mentioned at the end** of [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to work with ESP32 WiFi/BlueTooth (BT/BLE)

---
---

## HOWTO Setting up the Async DNS Server

```cpp
#include <AsyncDNSServer_ESP32_ENC.h>
#include <ESPAsyncWebServer.h>

const byte DNS_PORT = 53;

IPAddress apIP;

AsyncDNSServer dnsServer;

AsyncWebServer server(80);

void setup()
{
  ...
  
  ///////////////////////////////////

  // To be called before ETH.begin()
  ESP32_ENC_onEvent();

  //bool begin(int MISO_GPIO, int MOSI_GPIO, int SCLK_GPIO, int CS_GPIO, int INT_GPIO, int SPI_CLOCK_MHZ,
  //           int SPI_HOST, uint8_t *ENC28J60_Mac = ENC28J60_Default_Mac);
  ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, SPI_HOST );

  // Static IP, leave without this line to get IP via DHCP
  //bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  //ETH.config(myIP, myGW, mySN, myDNS);

  ESP32_ENC_waitForConnect();

  apIP = ETH.localIP();

  ///////////////////////////////////

	// modify TTL associated  with the domain name (in seconds)
	// default is 60 seconds
	dnsServer.setTTL(300);
	// set which return code will be used for all other domains (e.g. sending
	// ServerFailure instead of NonExistentDomain will reduce number of queries
	// sent by clients)
	// default is AsyncDNSReplyCode::NonExistentDomain
	dnsServer.setErrorReplyCode(AsyncDNSReplyCode::ServerFailure);

	// start DNS server for a specific domain name
	dnsServer.start(DNS_PORT, "*", apIP);

  ...
}

void loop() 
{
}

```

---
---

### Examples

 1. [AsyncCaptivePortalAdvanced_ESP32_ENC](examples/AsyncCaptivePortalAdvanced_ESP32_ENC)
 2. [AsyncCaptivePortal_ESP32_ENC](examples/AsyncCaptivePortal_ESP32_ENC)
 3. [AsyncDNServerFull_ESP32_ENC](examples/AsyncDNServerFull_ESP32_ENC) 
 4. [AsyncDNSServer_ESP32_ENC](examples/AsyncDNSServer_ESP32_ENC)
 
--- 
---

### Example [AsyncDNSServer_ESP32_ENC](examples/AsyncDNSServer_ESP32_ENC)

#### 1. File [AsyncDNSServer_ESP32_ENC.ino](examples/AsyncDNSServer_ESP32_ENC/AsyncDNSServer_ESP32_ENC.ino)

https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC/blob/dc4ae2473b2e323cb6bc57c8c13710554ed68948/examples/AsyncDNSServer_ESP32_ENC/AsyncDNSServer_ESP32_ENC.ino#L11-L164

---
---

### Debug

Debug is enabled by default on Serial. To disable, use level 0

```cpp
#define ASYNC_DNS_ESP32_ENC_DEBUG_PORT      Serial

// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define _ASYNC_DNS_ESP32_ENC_LOGLEVEL_      0
```

You can also change the debugging level from 0 to 4

```cpp
#define ASYNC_DNS_ESP32_ENC_DEBUG_PORT      Serial


// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define _ASYNC_DNS_ESP32_ENC_LOGLEVEL_    4
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of Arduino IDE, the Arduino `ESP32` core or depending libraries.

Sometimes, the library will only work if you update the `ESP32` core to the latest version because I am always using the latest cores /libraries.


---
---


### Issues

Submit issues to: [AsyncDNSServer_ESP32_ENC issues](https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC/issues)

---


## TO DO

 1. Fix bug. Add enhancement


## DONE

 1. Add support to **ESP32 boards using LwIP ENC28J60 Ethernet**
 2. Add more examples.
 3. Add debugging features
 4. Add astyle using `allman` style. Restyle the library
 
---
---

### Contributions and Thanks

1. Based on and modified from [Develo's ESPAsyncDNSServer Library](https://github.com/devyte/ESPAsyncDNSServer).


<table>
  <tr>
    <td align="center"><a href="https://github.com/devyte"><img src="https://github.com/devyte.png" width="100px;" alt="devyte"/><br/><sub><b>⭐️ Develo</b></sub></a><br/></td>
  </tr> 
</table>

---

### Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---
---

### License

- The library is licensed under [GPLv3](https://github.com/khoih-prog/AsyncDNSServer_ESP32_ENC/blob/main/LICENSE)

---

## Copyright

- Copyright (c) 2016- Develo
- Copyright (c) 2022- Khoi Hoang


