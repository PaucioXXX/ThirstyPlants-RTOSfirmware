#pragma once

#include <Arduino.h>
#include "SensorsManager.h"
#include "PumpManager.h"
#include "BoardPins.h"

enum class CriticalPumpErrors : uint8_t
{
    OK = 0,
    OVERDRY,
    OVERFLOW,
    TIMEOUT,
    SENSOR_FAILURE
};

constexpr uint8_t MIN_MOISTURE_PERCENTAGE_LIMIT = 20;
constexpr uint8_t MAX_MOISTURE_PERCENTAGE_LIMIT = 60;
constexpr uint32_t SAFETY_PUMP_RUNTIME_MS = 30 * 1000;


class PumpManager
{
private:
    uint32_t pumpLastTimeMS = 0;
    uint32_t pumpStartTimeMS = 0;
    bool isPumpRunning = false;

    SensorsManager * _sensorsManager = nullptr;

    static void s_pumpTask(void * arg);
  
public:

    void setSensorsManager(SensorsManager * sensorsManager);

    bool checkPumpTimer(uint32_t intervalToCheckMS);

    void turnOnPump();
    void turnOffPump();
    
    CriticalPumpErrors checkPumpCriticalStates();
    
    void handlePump();
    
    void beginTask();

};