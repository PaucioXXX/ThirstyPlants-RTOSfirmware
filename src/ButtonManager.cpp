#include "ButtonManager.h"

void ButtonManager::handleISR()
{
    if (millis() - _lastInterruptTimeMS >= 50)
    {
        _lastInterruptTimeMS = millis();

        xSemaphoreGiveFromISR(pressedButtonSemaphore, nullptr);
    }
}

void ButtonManager::buttonTask()
{
    while (true)
    {

        if (xSemaphoreTake(pressedButtonSemaphore, portMAX_DELAY) == pdTRUE)
        {
            if (digitalRead(_pin) == LOW)
            {
                if (xSemaphoreTake(pressedButtonSemaphore, pdMS_TO_TICKS(BUTTON_TIMER)) == pdFALSE)
                {
                    if (!_bleManager->isStarted())
                    {
                        _bleManager->beginTask();
                    }

                    xSemaphoreTake(pressedButtonSemaphore, portMAX_DELAY);
                }
            }
        }
    }
}

void ButtonManager::s_handleISR(void *arg)
{
    ButtonManager *_manager = static_cast<ButtonManager *>(arg);

    _manager->handleISR();
}

void ButtonManager::s_buttonTask(void *arg)
{
    ButtonManager *_manager = static_cast<ButtonManager *>(arg);

    _manager->buttonTask();
}

void ButtonManager::setupButton(uint8_t pin, BleManager *bleManager)
{
    _pin = pin;
    _bleManager = bleManager;

    pressedButtonSemaphore = xSemaphoreCreateBinary();

    pinMode(_pin, INPUT_PULLUP);

    _lastInterruptTimeMS = 0;

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