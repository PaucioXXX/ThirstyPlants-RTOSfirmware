#pragma once

#include <Arduino.h>
#include "BleManager.h"

constexpr uint16_t BUTTON_TIMER = 5000;

class ButtonManager
{
private:
    uint8_t _pin;

    BleManager *_bleManager;

    SemaphoreHandle_t pressedButtonSemaphore;
    SemaphoreHandle_t releasedButtonSemaphore;

    uint64_t _lastInterruptTimeMS; 

    void handleISR();

    void buttonTask();

    static void s_handleISR(void *arg);

    static void s_buttonTask(void *arg);

public:
    void setupButton(uint8_t pin, BleManager *ble);

    void beginTask();
};
