#include <Arduino.h>
#include <WiFi.h>
#include <BoardPins.h>
#include "DatabaseManager.h"
#include "SensorsManager.h"
#include "BleManager.h"
#include "ButtonManager.h"
#include "Secrets.h"
#include "PumpManager.h"

BleManager g_BLE;
ButtonManager g_Button;
WiFiManager g_WiFi;
DatabaseManager g_Database;
PumpManager g_Pump;
SensorsManager g_Sensors;

bool fbi; // debug

uint32_t lastTime = 0; // debug

char g_macAddress[13] = {0};
char g_deviceName[64] = {0};

void setDeviceName()
{
    snprintf(g_deviceName, sizeof(g_deviceName), "%s", "DefaultDeviceName");
}

void getMacAdress()
{
    uint8_t baseMacAdress[6];

    esp_read_mac(baseMacAdress, ESP_MAC_WIFI_STA);

    snprintf(g_macAddress, sizeof(g_macAddress), "%02X%02X%02X%02X%02X%02X",
    baseMacAdress[0], baseMacAdress[1], baseMacAdress[2], baseMacAdress[3], baseMacAdress[4], baseMacAdress[5]);

    Serial.println(g_macAddress); // debug
}

void setup()
{
    Serial.begin(115200);

    getMacAdress();
    setDeviceName();

    g_Button.setupButton(BLE_BUTTON_PIN, &g_BLE);
    g_Button.beginTask();
    g_Pump.setSensorsManager(&g_Sensors);
    g_Sensors.beginTask();
    g_WiFi.setDatabase(&g_Database);

    g_BLE.setWiFi(&g_WiFi);

    if (g_WiFi.loadCredentials())
    {
        g_WiFi.beginTask();
    }
    else
    {
        g_BLE.beginTask();
    }

}

void loop()
{
    vTaskDelay(1000);
}