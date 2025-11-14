#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <ArduinoJson.h>
#include "CanJsonComm.h"
#include <config_handle.h>

#include "ui/ui.h"

extern String HandShake;
extern CanJsonComm canNode;
extern "C" void action_stop_btn_fn(lv_event_t *e);
extern ConfigManager countSave;

extern int TotalCount;
extern int DailyCount;

// Declare your handler
void handleJsonReceived(const JsonDocument &doc);

#endif