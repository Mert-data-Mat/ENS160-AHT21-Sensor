/*
  Wiring (UNO R4):
    VCC -> 3.3V
    GND -> GND
    SDA -> SDA
    SCL -> SCL

  Notes:
  - Give ENS160 ~30–60 s after power for stable readings.
  - If AQI/TVOC/eCO2 stay at 0, re-check CS/ADD wiring and warm-up time.
*/

#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include "ScioSense_ENS160.h"

// ---------------- Configuration ----------------
static const long   SERIAL_BAUD     = 115200;
static const bool   WAIT_FOR_SERIAL = false;   // set true if you want to wait for USB Serial
static const uint16_t LOOP_INTERVAL_MS = 1000; // print period

// Pick the ENS160 I2C address your board uses:
//   ENS160_I2CADDR_0 -> 0x52 (ADD = 3.3V)
//   ENS160_I2CADDR_1 -> 0x53 (ADD = GND)
ScioSense_ENS160 ens160(ENS160_I2CADDR_1);     // 0x53 is typical for combo boards
Adafruit_AHTX0   aht;

// ---------------- Helpers ----------------
void printDivider() {
  Serial.println(F("------------------------------------------------------------"));
}

void printEns160Info() {
  Serial.print(F("\tRev: "));
  Serial.print(ens160.getMajorRev());
  Serial.print('.');
  Serial.print(ens160.getMinorRev());
  Serial.print('.');
  Serial.println(ens160.getBuild());
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(SERIAL_BAUD);
  if (WAIT_FOR_SERIAL) { while (!Serial) { /* wait */ } }
  delay(50);

  printDivider();
  Serial.println(F("ENS160 - Digital air quality sensor (ScioSense)"));
  Serial.println(F("AHT21 (Adafruit) for temperature & humidity"));
  printDivider();

  // I2C init (UNO R4 uses dedicated SDA/SCL pins)
  Wire.begin();
  // Optional: faster I2C
  // Wire.setClock(400000);

  // ---- ENS160 init ----
  Serial.print(F("ENS160 init @0x53... "));
  ens160.begin();                                // ScioSense begin() has no return on some forks
  if (!ens160.available()) {
    Serial.println(F("FAILED (not available). Check wiring."));
  } else {
    Serial.println(F("OK"));
    printEns160Info();

    Serial.print(F("\tSet STANDARD mode... "));
    bool modeOK = ens160.setMode(ENS160_OPMODE_STD); // I dont think we need the set mode for normal use but anyway.
    Serial.println(modeOK ? F("OK") : F("FAILED"));

    Serial.println(F("Warm-up ~2 seconds..."));
    delay(2000);
  }

  // ---- AHT21 init ----
  Serial.print(F("AHT21 init... "));
  if (!aht.begin(&Wire)) {
    Serial.println(F("FAILED (check wiring)"));
    while (true) { delay(1000); }
  }
  Serial.println(F("OK"));
  printDivider();
}

// ---------------- Loop ----------------
void loop() {
  static uint32_t last = 0;
  uint32_t now = millis();
  if (now - last < LOOP_INTERVAL_MS) return;
  last = now;

  // --- Read AHT21 ---
  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);
  float tC = temp.temperature;
  float rH = hum.relative_humidity;

  Serial.print(F("T "));
  Serial.print(tC - 4.0, 1);
  Serial.print(F(" °C  H "));
  Serial.print(rH, 1);
  Serial.print(F(" %  |  "));

  // --- ENS160 path ---
  if (ens160.available()) {
    // Provide environmental data to ENS160 (method name per ScioSense fork)
    ens160.set_envdata(tC, rH);

    // Trigger a measurement (some forks need this; harmless otherwise)
    ens160.measure(true);
    ens160.measureRaw(true);

    // Read values
    uint8_t  aqi  = ens160.getAQI();     // Air quality score 1 to 5 (0 means not ready)
    uint16_t tvoc = ens160.getTVOC();    // ppb , organic molecules in the air
    uint16_t eco2 = ens160.geteCO2();    // ppm

    Serial.print(F("AQI "));
    Serial.print(aqi);
    Serial.print(F("  TVOC "));
    Serial.print(tvoc);
    Serial.print(F(" ppb  eCO2 "));
    Serial.print(eco2);
    Serial.println(F(" ppm"));

    // Optional: raw heater resistances (diagnostics)
    // Serial.print(F("R HP0: ")); Serial.print(ens160.getHP0()); Serial.print(F(" Ω  "));
    // Serial.print(F("R HP1: ")); Serial.print(ens160.getHP1()); Serial.print(F(" Ω  "));
    // Serial.print(F("R HP2: ")); Serial.print(ens160.getHP2()); Serial.print(F(" Ω  "));
    // Serial.print(F("R HP3: ")); Serial.print(ens160.getHP3()); Serial.println(F(" Ω"));
  } else {
    Serial.println(F("ENS160 not available"));
  }
}
