#include <ESP8266WiFi.h>
extern "C" { 
  #include <espnow.h>
}

typedef struct __attribute__((packed)) {
  uint8_t  aqi;
  uint16_t tvoc;
  uint16_t eco2;
  float    temperature;
  float    humidity;
} AirData_t;

void onDataRecv(uint8_t* mac, uint8_t* data, uint8_t len) {
  if (len != sizeof(AirData_t)) {
    Serial.printf("\nRX len=%u (expected %u)\n", len, (unsigned)sizeof(AirData_t));
    return;
  }
  AirData_t d; memcpy(&d, data, sizeof(d));
  Serial.printf("\nAQI=%u  TVOC=%u ppb  eCO2=%u ppm  T=%.1f°C  RH=%.1f%%\n",
                d.aqi, d.tvoc, d.eco2, d.temperature, d.humidity);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.mode(WIFI_AP_STA);
  WiFi.setSleep(false);

  // Bring up AP on channel 1 (sender must add peer with ch=1)
  bool ap = WiFi.softAP("ESPNOW_RX", "", 1 /*channel*/, false);
  Serial.printf("AP %s, channel=%d\n", ap ? "up" : "fail", WiFi.channel());

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init FAIL");
    while (true) { delay(1000); }
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("Receiver ready. Waiting for ESP-NOW packets…");
}

void loop() {}
