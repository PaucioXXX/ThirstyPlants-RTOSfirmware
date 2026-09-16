#include "WiFiManager.h"
#include "Secrets.h"

void WiFiManager::s_wifiTask(void *arg)
{
    WiFiManager *_manager = static_cast<WiFiManager *>(arg);

    snprintf(_manager->_networkDisplayId, sizeof(_manager->_networkDisplayId), "ThirstyPlants:%s", g_macAddress);

    WiFi.mode(WIFI_STA);

    WiFi.setHostname(_manager->_networkDisplayId);

    _manager->connectToWiFi();

    if (_manager->isConnected())
    {
        if (_manager->_databaseManager != nullptr)
        {
            _manager->_databaseManager->beginTask();
        }
    }

    vTaskDelete(nullptr);
}

void WiFiManager::connectToWiFi()
{
    WiFi.disconnect(true);
    WiFi.begin(_WiFiSSID, _WiFiPass);

    uint8_t attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 40)
    {
        attempts++;
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        if (_isSavedWiFi == false)
        {
            saveCredentials();
            _isSavedWiFi = true;
        }
    }
    else
    {
        // debug
    }
}
void WiFiManager::setDatabase(DatabaseManager *databaseManager)
{
    _databaseManager = databaseManager;
}

void WiFiManager::setCredentials(const char *ssid, const char *password)
{
    strlcpy(_WiFiSSID, ssid, sizeof(_WiFiSSID));
    strlcpy(_WiFiPass, password, sizeof(_WiFiPass));

    Serial.println(_WiFiPass); // debug
    Serial.println(_WiFiSSID); // debug
}

void WiFiManager::beginTask()
{
    xTaskCreate(
        s_wifiTask,
        "WiFi Task",
        4096,
        this,
        1,
        NULL);
}

void WiFiManager::saveCredentials()
{
    preferences.begin("wifi", false);
    preferences.putString("ssid", _WiFiSSID);
    preferences.putString("pass", _WiFiPass);
    preferences.end();
}

bool WiFiManager::loadCredentials()
{
    preferences.begin("wifi", true);

    size_t ssidLen = preferences.getString("ssid", _WiFiSSID, sizeof(_WiFiSSID));
    size_t passLen = preferences.getString("pass", _WiFiPass, sizeof(_WiFiPass));

    preferences.end();

    if (ssidLen > 0)
    {
        _isSavedWiFi = true;
        return true;
    }

    _isSavedWiFi = false;
    return false;
}

void WiFiManager::clearCredentials()
{
    preferences.begin("wifi", false);
    preferences.clear();
    preferences.end();
}

bool isConnected()
{
    return (WiFi.status() == WL_CONNECTED);
}