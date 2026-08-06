#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <Secrets.h>
#include <BoardPins.h>
#include <WiFiManager.h>

#define CREDENTIALS_SEPARATOR ";;"


class BleManager
{
private:
    volatile bool _bleStarted = false;
    volatile bool _newCredentialsReceived = false;
    char _recievedCredentials[128] = {0};

    WiFiManager * _wifiManager;

    static void startBleTask(void *pvParameters)
    {
        BleManager *BLE = static_cast<BleManager *>(pvParameters);

        BLE->startBLE();

        while (true)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    class credentialsCallbacks : public BLECharacteristicCallbacks
    {
    private:
        BleManager *_manager;

    public:
        credentialsCallbacks(BleManager *manager)
        {
            _manager = manager;
        }
        void onWrite(BLECharacteristic *pCharacteristic)
        {
            char credentialsAsString[128];

            snprintf(credentialsAsString, sizeof(credentialsAsString), "%s", pCharacteristic->getValue().c_str());

            Serial.printf("WiFI and Pass: %s \n", credentialsAsString);

            strlcpy(_manager->_recievedCredentials, credentialsAsString, sizeof(_manager->_recievedCredentials));
            _manager->_newCredentialsReceived = true;

            parsingCredentials();
        }

        void parsingCredentials()
        {   
            char * separator = strstr(_manager->_recievedCredentials, CREDENTIALS_SEPARATOR);

            if(separator != NULL)
            {
                separator[0] = '\0';

                _manager->_wifiManager->setCredentials(_manager->_recievedCredentials, separator + 2);
            }
            
        
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

    void begin(WiFiManager * wifiManager)
    {
        _wifiManager = wifiManager;
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
