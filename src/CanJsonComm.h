#pragma once

#include <Arduino.h>
#include <driver/twai.h>
#include <ArduinoJson.h>

class CanJsonComm {
public:
    typedef std::function<void(const JsonDocument&)> JsonCallback;

    CanJsonComm(uint32_t sendId, uint32_t recvId, gpio_num_t txPin, gpio_num_t rxPin);

    void begin();
    void sendJson(const JsonDocument& doc);
    void loop();  // Call in main loop to process incoming frames
    void onJsonReceived(JsonCallback cb);

private:
    uint32_t _sendId;
    uint32_t _recvId;
    gpio_num_t _txPin;
    gpio_num_t _rxPin;
    String _recvBuffer;
    uint8_t _expectedSeq = 0;
    JsonCallback _callback = nullptr;
};
