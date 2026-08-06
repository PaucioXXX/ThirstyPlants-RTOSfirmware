#include <Arduino.h>
#include <BoardPins.h>
#include "DatabaseManager.h"
#include "SensorsManager.h"
#include "SensorsHandling.h"
#include "BleManager.h"
#include "ButtonManager.h"


BleManager fBLE;
ButtonManager fButton;
WiFiManager fWifi;

void setup() 
{
    Serial.begin(115200);
    fButton.begin(BLE_BUTTON_PIN, &fBLE); 
    fButton.beginTask(); 
    fBLE.begin(&fWifi);
}

void loop()
{  

}