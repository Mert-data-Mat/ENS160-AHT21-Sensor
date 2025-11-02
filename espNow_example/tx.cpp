#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include "ScioSense_ENS160.h"  // ENS160 library
#include <ESP8266WiFi.h>
extern "C" { 
  #include <espnow.h> 
}

uint8_t RX_MAC[] = { 0x08, 0x3A, 0x8D, 0xE7, 0x55, 0x4A };  // example of my esp's mac address
  
typedef struct _attribute_((packed)) {
  uint8_t  aqi;      // Air Quality Index (1–5)
  uint16_t tvoc;     // Total Volatile Organic Compounds (ppb)
  uint16_t eco2;     // Equivalent CO2 (ppm)
  float    temperature;  // °C
  float    humidity;     // %RH
} AirData_t;

Adafruit_AHTX0 aht;

int tempC;        //To store the temperature in C
int humidity;     //To store the humidity

ScioSense_ENS160      ens160(ENS160_I2CADDR_1); //0x53..ENS160+AHT21

void setup() {

  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.disconnect(); // stay off any router (keeps channel free)
  if (esp_now_init() != 0) { Serial.println("ESP-NOW init failed"); while (1) {} }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  // Receiver AP runs on channel 1
  if (esp_now_add_peer(RX_MAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
    Serial.println("Add peer failed"); while (1) {}
  }

  Serial.print("ENS160...");
  ens160.begin();
  
  Serial.println(ens160.available() ? "done." : "failed!");
  
  if (ens160.available()) {
    // Print ENS160 versions
    Serial.print("\tRev: "); Serial.print(ens160.getMajorRev());
    Serial.print("."); Serial.print(ens160.getMinorRev());
    Serial.print("."); Serial.println(ens160.getBuild());
  
    Serial.print("\tStandard mode ");
    Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
  }

  // AHT20 start
  Serial.println("Adafruit AHT10/AHT20 demo!");

  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT20 found");
  //AHT20 end

}

void loop() {

    ///// AHT20 start
  sensors_event_t humidity1, temp; //had to change to  humidity1
  aht.getEvent(&humidity1, &temp);// populate temp and humidity objects with fresh data
  tempC = (temp.temperature) - 6; // Bc heat from the pcb is affecting the temp sensor
  humidity = (humidity1.relative_humidity);
  delay(3000);

    ///// AHT20 end

  if (ens160.available()) {
    // Give values to Air Quality Sensor.
    ens160.set_envdata(tempC - 6, humidity);
    ens160.measure(true);
    ens160.measureRaw(true);
  }
  delay(3000);
  AirData_t airData;

  airData.aqi = ens160.getAQI();
  airData.tvoc = ens160.getTVOC();
  airData.eco2 = ens160.geteCO2();
  airData.temperature = tempC;
  airData.humidity = humidity;

  // Send struct directly
  esp_now_send(RX_MAC, (uint8_t *)&airData, sizeof(airData));

   // Debug
  Serial.printf("TX | AQI=%u TVOC=%u eCO2=%u T=%.1f RH=%.1f\n",
                airData.aqi, airData.tvoc, airData.eco2, airData.temperature, airData.humidity);


}
