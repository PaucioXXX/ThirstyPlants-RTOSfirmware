#pragma once

#include <Arduino.h>

uint16_t medianFilter(uint16_t* moistureRawBuffer, const uint16_t sampleCount)
{
    uint16_t sortedValues[sampleCount];
    for (uint8_t i = 0 ; i < sampleCount ; i++)
        sortedValues[i] = moistureRawBuffer[i];

    for (uint8_t i = 0; i < sampleCount - 1 ; i++)
        for (uint8_t j = i + 1 ; j < sampleCount ; j++)
            {
                if (sortedValues[i] > sortedValues[j])
                {
                    uint16_t aux = sortedValues[i];
                    sortedValues[i] = sortedValues[j];
                    sortedValues[j] = aux;
                }
            }
    
    return sortedValues[sampleCount / 2];
}