# ENS160 Sensor

![Here is our little cheap hero ](images/FQR9R7ALR85VNX0.png)

**Figure 1.** Here is our little cheap hero.

[🛒 **AliExpress Link of the Sensor**](https://de.aliexpress.com/item/1005009070352900.html?spm=a2g0o.order_list.order_list_main.11.7e121802fccWTp&gatewayAdapt=glo2deu)


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

## Wireless ESP-NOW Extension (ESP8266)

After getting stable ENS160 + AHT21 readings over I²C, I extended the setup to work completely wirelessly using ESP-NOW on two Wemos D1 mini boards.
This lets one board act as a sensor node, sending air-quality data to another board that simply receives and prints it — no Wi-Fi router or internet required.

typedef struct __attribute__((packed)) {
  uint8_t  aqi;          // Air Quality Index (1–5)
  uint16_t tvoc;         // Total VOC (ppb)
  uint16_t eco2;         // Equivalent CO₂ (ppm)
  float    temperature;  // °C
  float    humidity;     // %RH
} AirData_t;

Compact 12-byte packet for reliable ESP-NOW transmission
Binary-safe and efficient — avoids String or text parsing
Matches exactly on both sender and receiver sides

🧩 How It Works
	1.	The sender reads ENS160 + AHT21 data and sends an AirData_t packet via ESP-NOW every 0.5 s.
	2.	The receiver runs a simple listener sketch that prints all incoming values with AQI labels.
	3.	Both devices use channel 1, and the sender is paired to the receiver’s AP MAC address.

This small addition turns the ENS160 module into a wireless air-quality telemetry node that can feed data to dashboards, displays, or dataloggers without any cables.


[📘 **Source**](https://www.instructables.com/ENS160-AHT21-Sensor-for-Arduino/)
