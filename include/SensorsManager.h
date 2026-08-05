#pragma once 

#include <Arduino.h>

void setupSensors();

float readTemp();
float readLighting();
float readHumidity();
int readMoisture(uint8_t moisturePin);