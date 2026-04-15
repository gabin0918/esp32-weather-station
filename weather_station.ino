/*
 * Project: Smart IoT Weather Station
 * Description: ESP32 station fetching outdoor weather from Open-Meteo API 
 *              and indoor data from BME280 sensor.
 * Hardware: ESP32 DevKit, SH1106 1.3" OLED, BME280 sensor.
 * Author: [gabi0918]
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// --- CONFIGURATION ---
const char* ssid     = "Your SSID";
const char* password = "Your Password";

// Open-Meteo API URL (Krakow coordinates)
String url = "http://api.open-meteo.com/v1/forecast?latitude=50.06&longitude=19.94&current=temperature_2m,relative_humidity_2m,precipitation,weather_code&timezone=auto";

// Display & Sensor Settings
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);
Adafruit_BME280 bme;

// Variables for Weather Data
float outTemp, outHum, precipitation;
int weatherCode;

// --- ICONS ---
void drawSun() {
  display.drawCircle(110, 18, 5, SH110X_WHITE); 
  for(int i=0; i<360; i+=45) {
    float angle = i * 3.14 / 180;
    display.drawLine(110+cos(angle)*7, 18+sin(angle)*7, 110+cos(angle)*10, 18+sin(angle)*10, SH110X_WHITE);
  }
}

void drawCloud() {
  display.fillCircle(105, 20, 4, SH110X_WHITE);
  display.fillCircle(110, 17, 5, SH110X_WHITE);
  display.fillCircle(116, 20, 4, SH110X_WHITE);
  display.fillRect(105, 20, 11, 4, SH110X_WHITE);
}

// --- API FETCHING ---
void getWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    if (http.GET() > 0) {
      JsonDocument doc;
      deserializeJson(doc, http.getString());
      outTemp = doc["current"]["temperature_2m"];
      outHum = doc["current"]["relative_humidity_2m"];
      precipitation = doc["current"]["precipitation"];
      weatherCode = doc["current"]["weather_code"];
    }
    http.end();
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  
  display.begin(0x3c, true);
  bme.begin(0x76);

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("System starting...");
  display.println("Connecting to WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  getWeatherData();
}

void loop() {
  float inTemp = bme.readTemperature();
  float inHum = bme.readHumidity();
  
  display.clearDisplay();
  
  // --- TOP SECTION: OUTDOOR (API) ---
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Weather in Krakow");
  
  if (WiFi.status() == WL_CONNECTED) {
    display.setCursor(0, 12);
    display.setTextSize(2);
    display.print(outTemp, 1); display.println(" C");
    
    display.setTextSize(1);
    display.print("Precip: "); display.print(precipitation, 1); display.println(" mm");
    
    // Draw icon next to temp
    if (weatherCode == 0) drawSun();
    else drawCloud();
  } else {
    display.setCursor(0, 15);
    display.println("WiFi Disconnected");
  }

  // --- DIVIDER LINE ---
  display.drawLine(0, 38, 128, 38, SH110X_WHITE);

  // --- BOTTOM SECTION: INDOOR (SENSOR) ---
  display.setCursor(0, 42);
  display.setTextSize(1);
  display.println("Your room");
  
  display.setCursor(0, 53);
  display.print("Temp: "); display.print(inTemp, 1); display.print(" C");
  display.print(" Hum: "); display.print(inHum, 0); display.println("%");

  display.display();
  
  // Update API every 10 minutes
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 600000) {
    getWeatherData();
    lastUpdate = millis();
  }
  
  delay(5000);
}