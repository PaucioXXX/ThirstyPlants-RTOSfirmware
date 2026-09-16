#include "ButtonManager.h"

void ButtonManager::handleISR()
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

void ButtonManager::buttonTask()
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

void ButtonManager::s_handleISR(void *arg)
{
    ButtonManager *instance = static_cast<ButtonManager *>(arg);

    instance->handleISR();
}

void ButtonManager::s_buttonTask(void *arg)
{
    ButtonManager *instance = static_cast<ButtonManager *>(arg);

    instance->buttonTask();
}

void ButtonManager::setupButton(uint8_t pin, BleManager *ble)
{
    _pin = pin;
    _ble = ble;

    pressedButtonSemaphore = xSemaphoreCreateBinary();
    releasedButtonSemaphore = xSemaphoreCreateBinary();

    pinMode(_pin, INPUT_PULLUP);

    _lastIntrerruptTime = 0;

    attachInterruptArg(_pin, s_handleISR, this, CHANGE);
}

void ButtonManager::beginTask()
{
    xTaskCreate(
        s_buttonTask,
        "Button task",
        2048,
        this,
        2,
        nullptr);
}