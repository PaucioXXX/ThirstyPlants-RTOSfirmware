#pragma once

#include <Arduino.h>
#include "BleManager.h"

constexpr uint16_t BUTTON_TIMER = 5000;

class ButtonManager
{
private:
    uint8_t _pin;

    BleObject* _ble;

    SemaphoreHandle_t pressedButtonSemaphore;
    SemaphoreHandle_t releasedButtonSemaphore;

    volatile uint32_t _lastIntrerruptTime;

    void IRAM_ATTR handleISR()
    {
        if(digitalRead(_pin) == LOW && millis() - _lastIntrerruptTime >= 50)
        {
            _lastIntrerruptTime = millis();
            xSemaphoreGiveFromISR(pressedButtonSemaphore, NULL);       
        }
        else if (millis() - _lastIntrerruptTime >= 50)
        {
            xSemaphoreGiveFromISR(releasedButtonSemaphore, NULL); 
        }

    }

    static void IRAM_ATTR s_handleISR(void* arg)
    {
        ButtonManager* instance = static_cast<ButtonManager*>(arg);

        instance->handleISR();
    }

public:
    void begin(uint8_t pin, BleObject* ble)
    {
        _pin = pin;
        _ble = ble;

        pressedButtonSemaphore = xSemaphoreCreateBinary();
        releasedButtonSemaphore = xSemaphoreCreateBinary();

        pinMode(_pin, INPUT_PULLUP);

        _lastIntrerruptTime = 0;

        attachInterruptArg(_pin, s_handleISR, this, CHANGE);
    }

    void beginTask()
    {
        
    }
};
