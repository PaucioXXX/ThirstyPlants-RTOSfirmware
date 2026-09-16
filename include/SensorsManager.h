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

        static void s_sensorsManagerTask(void * arg)
        {
            SensorsManager * _manager = static_cast<SensorsManager *>(arg);
        }


    public:
        SensorsManager();

        void setupSensors();

        uint8_t getMoisturePercent(uint8_t whichSensor);

        uint8_t calculateValidMoisture();

        void beginTask();
};