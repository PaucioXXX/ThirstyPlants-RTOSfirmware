#pragma once

#include <Arduino.h>

uint16_t medianFilter(uint16_t* moistureRawBuffer, const uint16_t sampleCount);