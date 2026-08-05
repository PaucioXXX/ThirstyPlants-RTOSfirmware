#pragma once

#include <Arduino.h>
#include <SensorsManager.h>
#include <Filters.h>

constexpr uint16_t moistureTimer = 5000; // debug
constexpr uint16_t MAX_MOISTURE_RAW_VALUE = 4050;
constexpr uint16_t MIN_MOISTURE_RAW_VALUE = 50;
constexpr uint16_t REAL_MAX_MOISTURE_RAW = 3200;
constexpr uint16_t REAL_MIN_MOISTURE_RAW = 1100;
constexpr uint16_t MOISTURE_SAMPLE_COUNT = 5;

class MoistureSensor
{
private:
    uint8_t _sensorPin;
    uint16_t _rawMoistureBuffer[MOISTURE_SAMPLE_COUNT];
    uint8_t _bufferIndex;
    uint16_t _rawMoisutreFiltered;
    uint8_t _moisturePercent;
    uint32_t _sensorLastTimeMs;
    uint32_t _bufferLastTimeMs;
    uint32_t _bufferTimerMs;

    static void processMoisture(void * parameters)
    {
        MoistureSensor *sensor = static_cast<MoistureSensor *>(parameters);
        
        for(;;)
        {
            
        }
    }
public:
    MoistureSensor(uint8_t sensorPin)
        : _sensorPin(sensorPin),
          _rawMoistureBuffer{0},
          _bufferIndex(0),
          _rawMoisutreFiltered(0),
          _moisturePercent(0),
          _sensorLastTimeMs(0),
          _bufferLastTimeMs(0),
          _bufferTimerMs(0)
    {
    }

    bool checkMoistureValue(uint16_t moistureValueRaw)
    {
        if (moistureValueRaw > MAX_MOISTURE_RAW_VALUE || moistureValueRaw < MIN_MOISTURE_RAW_VALUE)
            return false;

        return true;
    }
    static uint8_t convertMoistureRawToPercent(uint16_t moistureValueRaw)
    {
        int moisturePercent = map(
            moistureValueRaw,
            REAL_MAX_MOISTURE_RAW,
            REAL_MIN_MOISTURE_RAW,
            0,
            100);
        moisturePercent = constrain(moisturePercent, 0, 100);
        return moisturePercent;
    }
    

    

};