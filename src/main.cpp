#include <Arduino.h>
#include <BoardPins.h>
#include "DatabaseManager.h"
#include "SensorsManager.h"
#include "SensorsHandling.h"
#include "BleManager.h"
#include "ButtonManager.h"


BleManager g_BLE;
ButtonManager g_Button;
WiFiManager g_WiFi;

void setup() 
{
    Serial.begin(115200);
    g_Button.begin(BLE_BUTTON_PIN, &g_BLE); 
    g_Button.beginTask(); 
    g_BLE.setWiFi(&g_WiFi);
}

void loop()
{  

}