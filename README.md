# ENS160 Sensor

![Here is our little cheap hero ](images/FQR9R7ALR85VNX0.png)

**Figure 1.** Here is our little cheap hero.

I couldn’t find much clear information or working examples online for this specific ENS160 combo sensor, especially when using it with boards like the Wemos D1 mini. Most tutorials were either outdated or incomplete, so I decided to build and document a clean, fully tested setup myself — including wiring, libraries, and working code — to make it easier for others to get reliable readings right away.

Minimal and **fully working** setup for the combined air quality and temperature/humidity sensor board:

- **ENS160** → Air Quality Index (AQI), Total Volatile Organic Compounds (TVOC, ppb), Equivalent CO₂ (eCO₂, ppm)  
- **AHT20 / AHT21 (AHT2x)** → Temperature (°C) and Relative Humidity (%)

Tested with **Arduino UNO R4** — can easily be adapted to ESP32.

---

## Overview

This project shows how to:
- Read real-time **air quality**, **TVOC**, and **eCO₂** values  
- Use **AHT21** readings for temperature/humidity compensation  
- Display all data via the Arduino Serial Monitor
- Most importantly **wiring**.
---

## Wiring

| ENS160 + AHT2x Module | Arduino UNO R4 | Description |
|:----------------------|:------------------------|:-------------|
| **VIN** | **3V3** | Power (3.3V / 5V) |
| **GND** | **G** | Common ground |
| **SCL** | **SCL** | I²C Clock |
| **SDA** | **SDA** | I²C Data |

> Rest of the pins are for the special cases

![Wiring diagram](images/FJPJ494LR97Z1RS.png)

**Figure 2.** In this image wiring is shown with the Wemos D1 mini (ESP8266).



## Required Libraries

Install from **Arduino IDE → Library Manager**:

| Library | Purpose |
|:--------|:---------|
| **Adafruit AHTX0** | For AHT20/AHT21 temperature & humidity |
| *(optional)* **ScioSense ENS160** | For ENS160 high-level API (some boards use modified forks) |


[📘 **Source**](https://www.instructables.com/ENS160-AHT21-Sensor-for-Arduino/)
