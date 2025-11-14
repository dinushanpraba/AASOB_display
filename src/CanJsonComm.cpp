#include "CanJsonComm.h"

CanJsonComm::CanJsonComm(uint32_t sendId, uint32_t recvId, gpio_num_t txPin, gpio_num_t rxPin)
    : _sendId(sendId), _recvId(recvId), _txPin(txPin), _rxPin(rxPin) {}

void CanJsonComm::begin()
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(_txPin, _rxPin, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    twai_driver_install(&g_config, &t_config, &f_config);
    twai_start();
}

void CanJsonComm::sendJson(const JsonDocument &doc)
{
    String jsonStr;
    serializeJson(doc, jsonStr);
    Serial.println("Sending JSON: ");
    serializeJson(doc, Serial);
    int len = jsonStr.length();
    uint8_t seq = 0;

    for (int i = 0; i < len; i += 7)
    {
        twai_message_t msg;
        msg.identifier = _sendId;
        msg.extd = 0;
        msg.rtr = 0;
        msg.data_length_code = 8;
        msg.data[0] = seq++;

        for (int j = 0; j < 7; j++)
        {
            msg.data[j + 1] = (i + j < len) ? jsonStr[i + j] : 0;
        }

        // twai_transmit(&msg, pdMS_TO_TICKS(100));
        if (twai_transmit(&msg, pdMS_TO_TICKS(1000)) == ESP_OK)
        {
            printf("Message queued for transmission\n");
        }
        else
        {
            printf("Failed to queue message for transmission\n");
        }
    }
}

void CanJsonComm::onJsonReceived(JsonCallback cb)
{
    _callback = cb;
}

void CanJsonComm::loop()
{
    twai_message_t rx_msg;
    if (twai_receive(&rx_msg, pdMS_TO_TICKS(10)) == ESP_OK &&
        rx_msg.identifier == _recvId && rx_msg.data_length_code == 8)
    {

        uint8_t seq = rx_msg.data[0];

        if (seq == 0)
        {
            _recvBuffer = "";
            _expectedSeq = 0;
        }

        if (seq == _expectedSeq)
        {
            for (int i = 1; i < 8; i++)
            {
                if (rx_msg.data[i] == 0)
                    break;
                _recvBuffer += (char)rx_msg.data[i];
            }
            _expectedSeq++;

            StaticJsonDocument<8192> doc;
            DeserializationError err = deserializeJson(doc, _recvBuffer);
            if (!err && _callback)
            {
                _callback(doc);
                _recvBuffer = "";
                _expectedSeq = 0;
            }
        }
    }
}
