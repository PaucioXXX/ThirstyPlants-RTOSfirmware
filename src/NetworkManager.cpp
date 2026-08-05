#include "NetworkManager.h"

#include <WiFi.h>


char WiFiSSID[64] = {0};
char WiFiPassword[64] = {0};

bool connectToWiFi()
{
    WiFi.begin(WiFiSSID, WiFiPassword);

    int attempts = 0;
    while(WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(500);
        Serial.print('.');
        attempts++;
    }
}

bool isConnectedToWiFi()
{
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nSUCCESS! Connected to Wi-Fi.");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    Serial.println("Shutting down BLE to save memory...");
        // BLEDevice::deinit(true); 
        bleStarted = false;
        return true;
    } else {
        Serial.println("\nFailed to connect. Wrong password or network out of range.");
        return false;
    }
}