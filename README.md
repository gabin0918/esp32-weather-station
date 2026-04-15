# Simple ESP32 Weather Station 🌦️

A straightforward DIY IoT project that displays indoor climate data alongside real-time outdoor weather from the internet.

## Features
- **Indoor Climate:** Real-time temperature and humidity readings from a **BME280** sensor.
- **Outdoor Weather:** Fetches current data for **Krakow** via the **Open-Meteo API** (No API key required).
- **Visuals:** Displays results on a 1.3" OLED screen with dynamic icons (Sun/Cloud) based on weather codes.
- **Connectivity:** Optimized for **2.4GHz IoT networks**.

## Hardware
- **ESP32** DevKit V1
- **BME280** Sensor (Temp/Hum/Pres)
- **SH1106** 1.3" OLED Display (I2C)
- Breadboard and jumper wires

## Wiring (I2C)
Both devices share the same I2C bus on the ESP32:

| Component | Pin | ESP32 Pin |
|-----------|-----|-----------|
| **OLED/BME280** | SDA | **GPIO 21** |
| **OLED/BME280** | SCL/SCK | **GPIO 22** |
| **OLED/BME280** | VCC | **3.3V** |
| **OLED/BME280** | GND | **GND** |

![IMG_20260411_173753](https://github.com/user-attachments/assets/95074bf1-a311-4db1-b5ef-2433b77680b0)

## Requirements
To compile this project, the following libraries are required:
- `Adafruit SH110X`
- `Adafruit BME280 Library`
- `Adafruit GFX Library`
- `ArduinoJson`

## Setup
1. Open the `.ino` file in Arduino IDE.
2. Enter your WiFi credentials in the `ssid` and `password` variables.
3. Upload to your ESP32.

---
Created by **gabin0918**
