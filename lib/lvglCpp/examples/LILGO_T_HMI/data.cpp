#include "data.h"

#include <ArduinoJson.h>

#include <nvs_flash.h>
#include <nvs.h>

#include <Arduino.h>
#include <stdlib.h>

void data_init(void) {
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

#define WARN_TAG "W [Data]: "
#define INFO_TAG "I [Data]: "

bool data_read(touch_calibration_t *data) {
    nvs_handle_t data_handle;
    size_t required_size = 0;
    esp_err_t err;

    err = nvs_open("touch", NVS_READWRITE, &data_handle);
    if (err != ESP_OK) {
        Serial.println(F(INFO_TAG"nvs open error"));
        goto OUT;
    }

    err = nvs_get_blob(data_handle, "data", NULL, &required_size);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        Serial.println(F(WARN_TAG"nvs open err"));
        goto OUT;
    }

    if (required_size == 0) {
        Serial.println(F(
		INFO_TAG"Nothing saved yet"));
        goto OUT;
    } else {
        uint8_t *str = (uint8_t *)malloc(required_size);
        if (!str) {
            Serial.println(F(WARN_TAG"malloc failed"));
            goto OUT;
        }
        err = nvs_get_blob(data_handle, "data", str, &required_size);
        if (err != ESP_OK) {
            Serial.println(F(
		WARN_TAG"Unable to get blob value"));
            free(str);
            goto OUT;
        }
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, str);
        if (error) {
            Serial.println(F(WARN_TAG"json parsing error"));
            goto OUT;
        }
        // Serial.printf("%s\n", str);
        data[0].rawX = doc["point_1"]["raw_x"].as<uint16_t>();
        data[0].rawY = doc["point_1"]["raw_Y"].as<uint16_t>();
        data[1].rawX = doc["point_2"]["raw_x"].as<uint16_t>();
        data[1].rawY = doc["point_2"]["raw_Y"].as<uint16_t>();
        data[2].rawX = doc["point_3"]["raw_x"].as<uint16_t>();
        data[2].rawY = doc["point_3"]["raw_Y"].as<uint16_t>();
        data[3].rawX = doc["point_4"]["raw_x"].as<uint16_t>();
        data[3].rawY = doc["point_4"]["raw_Y"].as<uint16_t>();
        Serial.println("point_1:");
        Serial.print("  raw_x: ");
        Serial.print(data[0].rawX);
        Serial.print("  raw_y: ");
        Serial.println(data[0].rawY);

        Serial.println("point_2:");
        Serial.print("  raw_x: ");
        Serial.print(data[1].rawX);
        Serial.print("  raw_y: ");
        Serial.println(data[1].rawY);

        Serial.println("point_3:");
        Serial.print("  raw_x: ");
        Serial.print(data[2].rawX);
        Serial.print("  raw_y: ");
        Serial.println(data[2].rawY);

        Serial.println("point_4:");
        Serial.print("  raw_x: ");
        Serial.print(data[3].rawX);
        Serial.print("  raw_y: ");
        Serial.println(data[3].rawY);
        free(str);
    }

    nvs_close(data_handle);
    return true;
OUT:
    nvs_close(data_handle);
    return false;
}


bool data_write(touch_calibration_t *data) {
    nvs_handle_t data_handle;
    size_t required_size = 0;
    esp_err_t err;
    StaticJsonDocument<1024> doc;
    char str[1024] = { 0 };

    err = nvs_open("touch", NVS_READWRITE, &data_handle);
    if (err != ESP_OK) {
        Serial.println(F(
		WARN_TAG"nvs open error")
);
return false;
}

 // doc["type"] = "Resistive touch screen";
doc["point_1"]["raw_x"] = data[0].rawX;
doc["point_1"]["raw_Y"] = data[0].rawY;
doc["point_2"]["raw_x"] = data[1].rawX;
doc["point_2"]["raw_Y"] = data[1].rawY;
doc["point_3"]["raw_x"] = data[2].rawX;
doc["point_3"]["raw_Y"] = data[2].rawY;
doc["point_4"]["raw_x"] = data[3].rawX;
doc["point_4"]["raw_Y"] = data[3].rawY;
serializeJson(doc, str, sizeof(str) - 1);

err = nvs_set_blob(data_handle, "data", str, strlen(str));
nvs_close(data_handle);
if (err != ESP_OK) {
return false;
}
return true;
}

