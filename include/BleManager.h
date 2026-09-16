#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "Secrets.h"
#include "BoardPins.h"
#include "WiFiManager.h"

extern char g_deviceName[64];

class BleManager
{
private:
    volatile bool _bleStarted = false;
    volatile bool _newCredentialsReceived = false;
    char _receivedCredentials[128] = {0};

    WiFiManager *_wifiManager = nullptr;
    BLEServer *_pServer = nullptr;
    BLECharacteristic *_pCharacteristic = nullptr;
    BLECharacteristicCallbacks *_callbacks = nullptr;

    static void startBleTask(void *arg);

    class credentialsCallbacks : public BLECharacteristicCallbacks
    {
    private:
        BleManager *_manager;

    public:
        credentialsCallbacks(BleManager *manager);

        void onWrite(BLECharacteristic *pCharacteristic);

        void parseCredentials();
    };

    void startBLE();
    void stopBLE();
    

public:
    
    void setWiFi(WiFiManager * wifiManager);

    void beginTask();

    bool isStarted();


};
