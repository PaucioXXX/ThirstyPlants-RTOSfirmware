# Thirsty Plants - ESP32 Firmware

This repository contains the C++ firmware for an automated plant-watering system running on an ESP32.

The firmware uses FreeRTOS tasks to read two capacitive soil-moisture sensors, a DHT22 temperature/humidity sensor, and a BH1750 light sensor. Sensor readings are processed using a median filter, and the readings from the two moisture sensors are averaged to improve accuracy and provide additional protection in case one sensor fails.

The ESP32 handles critical watering autonomously. If the soil moisture drops below a configured threshold, it triggers the pump locally, even if there is no network connection.

The ESP32 also acts as a telemetry node, periodically pushing sensor data to Firebase so a separate Flutter app can log historical data and, eventually, use it to predict future watering cycles.

The physical wiring is defined in `src/BoardPins.h`.

[ESP32-WROOM-32D Pinout](assets/pinout.jpg)
