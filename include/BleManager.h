#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include "Secrets.h"


class BleObject
{
private:
    volatile bool _bleStarted = false;
    volatile bool _newCredentialsReceived = false;

    static void startBleTask(void *pvParameters)
    {
        BleObject *BLE = static_cast<BleObject *>(pvParameters);

        BLE->startBLE();

        while (true)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    class credentialsCallbacks : public BLECharacteristicCallbacks
    {
    private:
        BleObject *_manager;

    public:
        credentialsCallbacks(BleObject *manager)
        {
            _manager = manager;
        }
        void onWrite(BLECharacteristic *pCharacteristic)
        {
            char credentialsAsString[128];

            snprintf(credentialsAsString, sizeof(credentialsAsString), "%s", pCharacteristic->getValue().c_str());

            Serial.printf("WiFI and Pass: %s \n", credentialsAsString);

            _manager->_newCredentialsReceived = true;

        }
    };

public:
    void startBLE()
    {
        BLEDevice::init("My ESP32");
        BLEServer *pServer = BLEDevice::createServer();
        BLEService *pService = pServer->createService(SERVICE_UUID);
        BLECharacteristic *pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_WRITE);

        pCharacteristic->setCallbacks(new credentialsCallbacks(this));
        pService->start();
        pServer->getAdvertising()->start();

        Serial.println("Waiting for a client connection to notify...");

        _bleStarted = true;
    }

    void beginTask()
    {
        xTaskCreate(
            startBleTask,
            "BLE Task",
            4096,
            this,
            1,
            NULL
        );
    }

    bool isStarted()
    {
        return _bleStarted;
    }

    


};
