#include "MoistureSensor.h"

#include <Arduino.h>
#include "Filters.h"

MoistureSensor::MoistureSensor(uint8_t sensorPin)
    : _sensorPin(sensorPin),
      _rawMoistureBuffer{0},
      _bufferIndex(0),
      _rawMoistureFiltered(0),
      _moisturePercent(0),
      _sensorLastTimeMS(0),
      _bufferLastTimeMS(0),
      _bufferTimerMS(0)
{
}

bool MoistureSensor::checkTimer(uint32_t intervalToCheckMS, uint32_t &lastCheckMS)
{
    if (millis() - lastCheckMS >= intervalToCheckMS)
    {
        lastCheckMS = millis();

        return true;
    }

    return false;
}

void MoistureSensor::readAndProcess()
{
    if (!checkTimer(_bufferTimerMS, _bufferLastTimeMS))
    {
        return;
    }

    _rawMoistureBuffer[_bufferIndex] = analogRead(_sensorPin);

    _bufferIndex++;
    

    if (_bufferIndex >= BUFFER_SAMPLE_COUNT)
    {
        _rawMoistureFiltered = medianFilter(_rawMoistureBuffer, BUFFER_SAMPLE_COUNT);
        _moisturePercent = convertMoistureRawToPercent(_rawMoistureFiltered);
    
        _bufferIndex = 0;
    }

    
}

bool MoistureSensor::checkMoistureValue()
{
    if (_rawMoistureFiltered > MAX_MOISTURE_RAW_VALUE || _rawMoistureFiltered < MIN_MOISTURE_RAW_VALUE)
        return false;

    return true;
}

uint8_t MoistureSensor::convertMoistureRawToPercent(uint16_t moistureValueRaw)
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

uint8_t MoistureSensor::getMoisturePercent()
{
    return _moisturePercent;
}