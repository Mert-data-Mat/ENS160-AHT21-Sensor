#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

extern "C" { 
  #include <espnow.h>
}

// TFT display pins
#define TFT_CS    D8
#define TFT_RST   D4  
#define TFT_DC    D3

// Initialize ST7735
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

typedef struct __attribute__((packed)) {
  uint8_t  aqi;
  uint16_t tvoc;
  uint16_t eco2;
  float    temperature;
  float    humidity;
} AirData_t;

void drawStaticElements() {
  tft.fillScreen(ST7735_BLACK);
  
  // Header
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setCursor(15, 5);
  tft.print("AIR QUALITY");
  
  // Draw dividing line
  tft.drawFastHLine(0, 15, 128, ST7735_WHITE);
  
  // Labels
  tft.setTextColor(ST7735_CYAN);
  
  tft.setCursor(5, 25);
  tft.print("Temperature:");
  
  tft.setCursor(5, 40);
  tft.print("Humidity:");
  
  tft.setCursor(5, 55);
  tft.print("AQI:");
  
  tft.setCursor(5, 70);
  tft.print("CO2:");
  
  tft.setCursor(5, 85);
  tft.print("TVOC:");
  
  // Bottom divider
  tft.drawFastHLine(0, 100, 128, ST7735_WHITE);
}

void displayData(AirData_t data) {
  // Clear only the data value areas (not the whole screen)
  tft.fillRect(70, 25, 58, 60, ST7735_BLACK);
  
  // Temperature value
  tft.setCursor(70, 25);
  tft.setTextColor(ST7735_WHITE);
  tft.print(String(data.temperature, 1));
  tft.print("C");
  
  // Humidity value
  tft.setCursor(70, 40);
  tft.setTextColor(ST7735_WHITE);
  tft.print(String(data.humidity, 1));
  tft.print("%");
  
  // AQI value with color coding
  tft.setCursor(70, 55);
  if(data.aqi <= 50) {
    tft.setTextColor(ST7735_GREEN);
  } else if(data.aqi <= 100) {
    tft.setTextColor(ST7735_YELLOW);
  } else {
    tft.setTextColor(ST7735_RED);
  }
  tft.print(String(data.aqi));
  
  // CO2 value
  tft.setCursor(70, 70);
  tft.setTextColor(ST7735_WHITE);
  tft.print(String(data.eco2));
  tft.print("ppm");
  
  // TVOC value
  tft.setCursor(70, 85);
  tft.setTextColor(ST7735_WHITE);
  tft.print(String(data.tvoc));
  tft.print("ppb");
  
  // Status bar
  tft.fillRect(0, 105, 128, 20, ST7735_BLUE);
  tft.setCursor(35, 110);
  tft.setTextColor(ST7735_WHITE);
  tft.print("UPDATED");
}

void onDataRecv(uint8_t* mac, uint8_t* data, uint8_t len) {
  if (len != sizeof(AirData_t)) {
    Serial.printf("\nRX len=%u (expected %u)\n", len, (unsigned)sizeof(AirData_t));
    return;
  }
  AirData_t d; memcpy(&d, data, sizeof(d));
  Serial.printf("\nAQI=%u  TVOC=%u ppb  eCO2=%u ppm  T=%.1fC  RH=%.1f%%\n",
                d.aqi, d.tvoc, d.eco2, d.temperature, d.humidity);
  
  // Update TFT display
  displayData(d);
}

