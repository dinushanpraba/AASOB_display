// #ifndef CONFIG_HANDLE_H
// #define CONFIG_HANDLE_H

// void loadConfig();
// // void motor_run();

// #endif

#pragma once

#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

class ConfigManager
{
public:
    ConfigManager(const char *filePath = "/test.json");

    bool begin();
    bool load();
    bool save();
    void setDefaults();
    void erase();

    DynamicJsonDocument &getDoc();

private:
    const char *_filePath;
    DynamicJsonDocument _doc;
};
