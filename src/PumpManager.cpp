#include <Arduino.h>
#include "SensorsManager.h"
#include "PumpManager.h"
#include "BoardPins.h"


void PumpManager::s_pumpTask(void * arg)
{
    PumpManager *_manager = static_cast<PumpManager *>(arg);

    while(true)
    {
        _manager -> handlePump();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void PumpManager::setSensorsManager(SensorsManager * sensorsManager)
{
    _sensorsManager = sensorsManager;
}

bool PumpManager::checkPumpTimer(uint32_t intervalToCheckMS)
{
    if  (millis() - pumpLastTimeMS >= intervalToCheckMS)
    {
        pumpLastTimeMS = millis();
        return true;
    }
    return false;
}

void PumpManager::turnOnPump()
{
    digitalWrite(PUMP_PIN, HIGH);

    if (!isPumpRunning)
    {
        pumpStartTimeMS = millis();
        isPumpRunning = true;
    }  
}

void PumpManager::turnOffPump()
{
    digitalWrite(PUMP_PIN, LOW);

    isPumpRunning = false;
}

CriticalPumpErrors PumpManager::checkPumpCriticalStates()
{
    if (isPumpRunning && (millis() - pumpStartTimeMS >= SAFETY_PUMP_RUNTIME_MS))
    {
        return CriticalPumpErrors::TIMEOUT;
    }

    if (_sensorsManager -> getMoisturePercent(1) <= MIN_MOISTURE_PERCENTAGE_LIMIT)
        return CriticalPumpErrors::OVERDRY;

    if (_sensorsManager -> getMoisturePercent(1) >= MAX_MOISTURE_PERCENTAGE_LIMIT) 
        return CriticalPumpErrors::OVERFLOW;

    return CriticalPumpErrors::OK;
}

void PumpManager::handlePump()
{
    CriticalPumpErrors pumpState = checkPumpCriticalStates();

    if (pumpState == CriticalPumpErrors::TIMEOUT)
    {
        turnOffPump();
    }
    else if (pumpState == CriticalPumpErrors::OVERFLOW)
    {
        turnOffPump();
    }
    else if (pumpState == CriticalPumpErrors::OVERDRY)
    {
        turnOnPump();
    }
    else if (pumpState == CriticalPumpErrors::OK)
    {
        
    }

}

void PumpManager::beginTask()
{
    xTaskCreatePinnedToCore(
        s_pumpTask,
        "Pump Task",
        2048,
        this,
        2,
        nullptr,
        1
    );
}
