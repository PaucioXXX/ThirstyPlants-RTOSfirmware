#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <DatabaseManager.h>

extern char g_macAddress[13];

class WiFiManager
{
private:
    char _WiFiPass[64] = {0};
    char _WiFiSSID[64] = {0};
    char _networkDisplayId[64] = {0};
    bool _isSavedWiFi = false;

    Preferences preferences;

    DatabaseManager *_databaseManager = nullptr;

    static void s_wifiTask(void *arg);

public:
    void connectToWiFi();

    void setDatabase(DatabaseManager *databaseManager);

    void setCredentials(const char *ssid, const char *password);

    void beginTask();

    void saveCredentials();

    bool loadCredentials();

    void clearCredentials();

    bool isConnected();
};