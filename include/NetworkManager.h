#pragma once

#include <Arduino.h>


extern char WiFiSSID[64];
extern char WiFiPassword[64];

extern bool bleStarted;
extern bool newCredentialsReceived;

void startBLE();
bool connectToWiFi();