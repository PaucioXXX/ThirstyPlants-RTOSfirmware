#pragma once

#include <Arduino.h>
#include "SensorsManager.h"
#include "PumpManager.h"
#include "BoardPins.h"

extern SemaphoreHandle_t g_pumpWakeSemaphore;

enum class CriticalPumpErrors : uint8_t
{
    OK = 0,
    OVERDRY,
    OVERFLOW,
    TIMEOUT,
    SENSOR_FAILURE,
    SENSOR_UNAVAILABLE
};

constexpr uint8_t MIN_MOISTURE_PERCENTAGE_LIMIT = 20;
constexpr uint8_t MAX_MOISTURE_PERCENTAGE_LIMIT = 60; 
constexpr uint32_t SAFETY_PUMP_RUNTIME_MS = 30 * 1000;


class PumpManager
{
private:
    uint32_t _pumpLastTimeMS = 0;
    uint32_t _pumpStartTimeMS = 0;
    bool _isPumpRunning = false;

    SensorsManager * _sensorsManager = nullptr;

    static void s_pumpTask(void * arg);
  
public:

    void setSensorsManager(SensorsManager * sensorsManager);

    void turnOnPump();
    void turnOffPump();
    
    CriticalPumpErrors checkPumpCriticalStates();
    
    void handlePump();
    
    void beginTask();

};