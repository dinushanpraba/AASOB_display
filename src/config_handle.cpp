#include "config_handle.h"

ConfigManager::ConfigManager(const char *filePath)
    : _filePath(filePath), _doc(10240) // Adjust size as needed
{
}

bool ConfigManager::begin()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("❌ Failed to mount SPIFFS");
        return false;
    }
    return load();
}

bool ConfigManager::load()
{

    Serial.print("Loading ......File Path....");
    Serial.println(_filePath);
    if (!SPIFFS.exists(_filePath))
    {
        Serial.println("⚡ Config file not found, saving defaults");
        setDefaults();
        return save();
    }

    File file = SPIFFS.open(_filePath, FILE_READ);
    if (!file)
    {
        Serial.println("❌ Failed to open config file");
        return false;
    }

    DeserializationError error = deserializeJson(_doc, file);
    file.close();

    if (error)
    {
        Serial.println("❌ Failed to parse config file");
        return false;
    }

    return true;
}

bool ConfigManager::save()
{
    serializeJsonPretty(_doc, Serial);
    Serial.print("File Path....");
    Serial.println(_filePath);
    File file = SPIFFS.open(_filePath, FILE_WRITE);
    if (!file)
    {
        Serial.println("❌ Failed to open config file for writing");
        return false;
    }

    if (serializeJson(_doc, file) == 0)
    {
        Serial.println("❌ Failed to write config to file");
        file.close();
        return false;
    }

    file.close();
    return true;
}

void ConfigManager::setDefaults()
{
    _doc.clear();
    // Set up a default structure, or leave empty for dynamic use
    // Example default:
    // _doc["STYLES"]["StyleName"] = "style01";
    // _doc["STYLES"]["BtnId"] = 1;
    // Add more defaults as needed
}

void ConfigManager::erase()
{
    SPIFFS.remove(_filePath);
}

DynamicJsonDocument &ConfigManager::getDoc()
{
    return _doc;
}
