#include "PumpManager.h"
#include <BoardPins.h>
#include <SensorsHandling.h>

#include <Arduino.h>

enum class CriticalPumpErrors : uint8_t 
{
    OK = 0,
    OVERDRY,
    OVERFLOW,
    TIMEOUT
};

constexpr uint8_t MIN_MOISTURE_PERCENTAGE_LIMIT = 20;
constexpr uint8_t MAX_MOISTURE_PERCENTAGE_LIMIT = 60;
constexpr uint32_t SAFETY_PUMP_RUNTIME_MS = 30 * 1000;

uint32_t pumpLastTimeMs = 0;
uint32_t pumpStartTimeMs = 0;
bool isPumpRunning = false;

static bool checkPumpTimer(uint32_t intervalToCheckMs)
{
    if  (millis() - pumpLastTimeMs >= intervalToCheckMs)
    {
        pumpLastTimeMs = millis();
        return true;
    }
    return false;
}

static void turnOnPump()
{
    digitalWrite(PUMP_PIN, HIGH);

    if (!isPumpRunning)
    {
        pumpStartTimeMs = millis();
        isPumpRunning = true;
    }
    
}

static void turnOffPump()
{
    digitalWrite(PUMP_PIN, LOW);

    isPumpRunning = false;
}

static CriticalPumpErrors checkPumpCriticalStates()
{
    if (isPumpRunning && (millis() - pumpStartTimeMs >= SAFETY_PUMP_RUNTIME_MS))
    {
        return CriticalPumpErrors::TIMEOUT;
    }

    if (moisturePercent <= MIN_MOISTURE_PERCENTAGE_LIMIT)
        return CriticalPumpErrors::OVERDRY;

    if (moisturePercent >= MAX_MOISTURE_PERCENTAGE_LIMIT) 
        return CriticalPumpErrors::OVERFLOW;

    return CriticalPumpErrors::OK;
}

void handlePump()
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
