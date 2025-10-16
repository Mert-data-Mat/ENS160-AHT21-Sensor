#ENS160
Minimal and **fully working** setup for the combined air quality and temperature/humidity sensor board:

- **ENS160** → Air Quality Index (AQI), Total Volatile Organic Compounds (TVOC, ppb), Equivalent CO₂ (eCO₂, ppm)  
- **AHT20 / AHT21 (AHT2x)** → Temperature (°C) and Relative Humidity (%)

Tested with **Arduino UNO R4** — can easily be adapted to ESP32.

---

## 🧠 Overview

This project shows how to:
- Read real-time **air quality**, **TVOC**, and **eCO₂** values  
- Use **AHT21** readings for temperature/humidity compensation  
- Display all data via the Arduino Serial Monitor
- Most importantly **wiring**.
---

## 🪛 Wiring

| ENS160 + AHT2x Module | Arduino UNO R4 | Description |
|:----------------------|:------------------------|:-------------|
| **VIN** | **3V3** | Power (3.3V / 5V) |
| **GND** | **G** | Common ground |
| **SCL** | **SCL** | I²C Clock |
| **SDA** | **SDA** | I²C Data |

> Rest of the pins are for the special cases


## Required Libraries

Install from **Arduino IDE → Library Manager**:

| Library | Purpose |
|:--------|:---------|
| **Adafruit AHTX0** | For AHT20/AHT21 temperature & humidity |
| *(optional)* **ScioSense ENS160** | For ENS160 high-level API (some boards use modified forks) |


[📘 **Source**](https://www.instructables.com/ENS160-AHT21-Sensor-for-Arduino/)
