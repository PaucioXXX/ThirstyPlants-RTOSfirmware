#include <Arduino.h>
#include <BoardPins.h>
#include "NetworkManager.h"
#include "DatabaseManager.h"
#include "SensorsManager.h"
#include "SensorsHandling.h"
#include <BleManager.h>

BleObject fBLE;

void setup() 
{
    Serial.begin(115200);
    fBLE.beginTask();
}

void loop()
{  
    if (fBLE.isStarted())
    {
        fBLE.startBLE();
    }
}