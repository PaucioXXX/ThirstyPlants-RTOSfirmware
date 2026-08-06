#pragma once

#include <Arduino.h>
#include "BleManager.h"

constexpr uint16_t BUTTON_TIMER = 5000;

class ButtonManager
{
private:
    uint8_t _pin;

    BleManager *_ble;

    SemaphoreHandle_t pressedButtonSemaphore;
    SemaphoreHandle_t releasedButtonSemaphore;

    volatile uint32_t _lastIntrerruptTime;

    void handleISR()
    {
        if (digitalRead(_pin) == LOW && millis() - _lastIntrerruptTime >= 50)
        {
            _lastIntrerruptTime = millis();
            xSemaphoreGiveFromISR(pressedButtonSemaphore, NULL);
        }
        else if (millis() - _lastIntrerruptTime >= 50)
        {
            _lastIntrerruptTime = millis();
            xSemaphoreGiveFromISR(releasedButtonSemaphore, NULL);
        }
    }

    void buttonTask()
    {
        while (true)
        {
            if (xSemaphoreTake(pressedButtonSemaphore, portMAX_DELAY) == pdTRUE)
            {
                if (xSemaphoreTake(releasedButtonSemaphore, pdMS_TO_TICKS(BUTTON_TIMER)) == pdTRUE)
                {
                }
                else
                {
                    if (!_ble->isStarted())
                    {
                        _ble->beginTask();
                        
                    }
                    xSemaphoreTake(releasedButtonSemaphore, portMAX_DELAY);
                }
            }
        }
    }

    static void s_handleISR(void *arg)
    {
        ButtonManager *instance = static_cast<ButtonManager *>(arg);

        instance->handleISR();
    }

    static void s_buttonTask(void *arg)
    {
        ButtonManager *instance = static_cast<ButtonManager *>(arg);

        instance->buttonTask();
    }

public:
    void begin(uint8_t pin, BleManager *ble)
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
        xTaskCreate(
            s_buttonTask,
            "Button task",
            2048,
            this,
            1,
            NULL);
    }
};
