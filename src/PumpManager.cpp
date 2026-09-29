#include <Arduino.h>
#include "SensorsManager.h"
#include "PumpManager.h"
#include "BoardPins.h"


void PumpManager::s_pumpTask(void * arg)
{
    PumpManager *_manager = static_cast<PumpManager *>(arg);

    while(true)
    {
        if (_manager->_isPumpRunning)
        {
            xSemaphoreTake(g_pumpWakeSemaphore, pdMS_TO_TICKS(500));
        }
        else
        {
            xSemaphoreTake(g_pumpWakeSemaphore, portMAX_DELAY);
        }

        _manager->handlePump();
    }
}

void PumpManager::setSensorsManager(SensorsManager * sensorsManager)
{
    _sensorsManager = sensorsManager;
}


void PumpManager::turnOnPump()
{
    if (!_isPumpRunning)
    {

    digitalWrite(PUMP_PIN, HIGH);

    _pumpStartTimeMS = millis();
    
    _isPumpRunning = true;
    
    }
}

void PumpManager::turnOffPump()
{
    digitalWrite(PUMP_PIN, LOW);

    _isPumpRunning = false;
}

CriticalPumpErrors PumpManager::checkPumpCriticalStates()
{

    if (_sensorsManager == nullptr)
    {
        return CriticalPumpErrors::SENSOR_UNAVAILABLE;
    }

    if (_isPumpRunning)
    {
        if ((millis() - _pumpStartTimeMS >= SAFETY_PUMP_RUNTIME_MS))
        {
            return CriticalPumpErrors::TIMEOUT;
        }

        if (_sensorsManager->getValidMoisture() >= MAX_MOISTURE_PERCENTAGE_LIMIT)
        {
            return CriticalPumpErrors::OVERFLOW;
        }
    }

    if (!_isPumpRunning)
    {
        if (_sensorsManager->getValidMoisture() <= MIN_MOISTURE_PERCENTAGE_LIMIT)
        {
            return CriticalPumpErrors::OVERDRY;
        }
    }

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
    pinMode(PUMP_PIN, OUTPUT);

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
