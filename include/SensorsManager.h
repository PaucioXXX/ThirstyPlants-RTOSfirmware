#pragma once 

#include <Arduino.h>
#include "BoardPins.h"
#include "MoistureSensor.h"
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>


class SensorsManager
{
    private:
        DHT _temperatureSensor;
        BH1750 _lightingSensor;
        MoistureSensor _moistureSensor1;
        MoistureSensor _moistureSensor2;
        
        uint32_t _moistureReadingTimerMS = 5 * 60 * 1000; // debug // fixed 5 mins time between readings
        
        const uint16_t _moistureBufferTimerMS = 50; 

        uint8_t _latestMoisturePercent1;
        uint8_t _latestMoisturePercent2;

        float _latestLightReading = 0.0f;
        float _latestTemperatureReading = 0.0f;
        float _latestHumidityReading = 0.0f;
        
        


        static void s_sensorsManagerTask(void * arg);


    public:
        SensorsManager();

        void setupSensors();

        uint8_t getMoisturePercent(uint8_t whichSensor);

        uint8_t calculateValidMoisture();

        void beginTask();
};