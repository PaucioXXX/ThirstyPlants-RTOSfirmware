#include "SensorsManager.h"
#include "BoardPins.h"

#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>

SensorsManager::SensorsManager()
    : _temperatureSensor(DHT_PIN, DHT22),
      _moistureSensor1(MOISTURE_PIN_1),
      _moistureSensor2(MOISTURE_PIN_2)
{
}

void SensorsManager::setupSensors()
{
    _temperatureSensor.begin();

    Wire.begin();
    _lightingSensor.begin();
}

uint8_t SensorsManager::getMoisturePercent(uint8_t whichSensor)
{
    if  (whichSensor == 1)
    {
        return _moistureSensor1.getMoisturePercent();
    }
    else if (whichSensor == 2)
    {   
        return _moistureSensor2.getMoisturePercent();
    }

    return 0;
}

uint8_t SensorsManager::calculateValidMoisture()
{
    bool isMoistureSensor1Healthy = _moistureSensor1.checkMoistureValue();
    bool isMoistureSensor2Healthy = _moistureSensor2.checkMoistureValue(); 

    uint8_t healthySensorCount = 0;
    uint16_t moisturePercentSum = 0;

    if (isMoistureSensor1Healthy)
    {
        moisturePercentSum += _moistureSensor1.getMoisturePercent();
        healthySensorCount ++;
    }

    if (isMoistureSensor2Healthy)
    {
        moisturePercentSum += _moistureSensor2.getMoisturePercent();
        healthySensorCount ++;
    }

    if (healthySensorCount == 0)
        return 0;

    return static_cast<uint8_t>(moisturePercentSum / healthySensorCount);
}

void SensorsManager::beginTask()
{
    xTaskCreate(
        s_sensorsManagerTask,
        "Sensors Task",
        2048,
        this,
        2,
        nullptr);
}
