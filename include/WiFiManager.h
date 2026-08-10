#pragma once 

#include <Arduino.h>
#include <WiFi.h>

class WiFiManager
{
private:
    char _WiFiPass[64] = {0};
    char _WiFiSSID[64] = {0};
    char _macAdress[64] = {0};
    char _networkDisplayId[64] = {0};

    static void s_wifiTask(void *arg)
    {
        WiFiManager * _manager = static_cast<WiFiManager*>(arg);

        strcpy(_manager->_networkDisplayId, "Thirsty Plants");
        strcpy(_manager->_macAdress, WiFi.macAddress().c_str());
        strcat(_manager->_networkDisplayId, _manager->_macAdress);
        
        WiFi.setHostname(_manager->_networkDisplayId);
        
        _manager -> connectToWiFi();
        
        vTaskDelete(nullptr);
    }

public:
    void connectToWiFi()
    {
        WiFi.disconnect(true);
        WiFi.begin(_WiFiSSID, _WiFiPass);

        while(WiFi.status() != WL_CONNECTED)
        {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

    void setCredentials(char * ssid, char * password)
    {
        strlcpy(_WiFiSSID, ssid, sizeof(_WiFiSSID));
        strlcpy(_WiFiPass, password, sizeof(_WiFiPass));

        Serial.println(_WiFiPass);
        Serial.println(_WiFiSSID);
    }

    void beginTask()
    {
        xTaskCreate(
            s_wifiTask,
            "WiFi Task",
            4096,
            this,
            1,
            NULL
        );
    }

    bool isConnected()
    {
        return (WiFi.status() == WL_CONNECTED);
    }
};