#include "SensorsManager.h"
#include "BoardPins.h"

#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>

DHT tempSensor(DHT_PIN, DHT22);
BH1750 lightingSensor;

void setupSensors()
{
    tempSensor.begin();

    Wire.begin(SDA_PIN, SCL_PIN);
    if (!lightingSensor.begin())
    {
        Serial.println("BH1750 failed to start"); // debug
    }
}

float readTemp()
{
    return tempSensor.readTemperature();
}

float readLighting()
{
    return lightingSensor.readLightLevel();
}

float readHumidity()
{
    return tempSensor.readHumidity();
}

int readMoisture(uint8_t moisturePin)
{
    return analogRead(moisturePin);

}

