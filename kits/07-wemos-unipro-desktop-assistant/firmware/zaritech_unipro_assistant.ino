/*
 * ====================================================================
 * Project: Zaritech UniPro Wireless Desktop Assistant & Live Telemetry Node
 * Subsystem: Kit 07 - Zaritech Labs Ar-Ge Series Assistant Module
 * Hardware: Wemos D1 R2 (ESP8266), 0.96" I2C OLED (SSD1306), Rotary Encoder
 * Maintainer: Zaritech Labs (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution & Support: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <time.h>

// OLED Geometry Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Enterprise Network Credentials
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

// Time Protocol Settings (NTP Configuration)
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 10800; // UTC +3 for Istanbul/Turkey Standard Time
const int   daylightOffset_sec = 0;

void setup() {
  Serial.begin(115200);

  // Initialize Local I2C Visual Substrate on Wemos Pins
  Wire.begin(4, 5); // SDA = D2 (GPIO4), SCL = D1 (GPIO5)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("[ZARITECH ERROR] OLED Screen allocation failed.");
    for(;;);
  }

  display.clearDisplay();
  renderStatusBar("BOOTING...", "Zaritech UniPro");
  delay(1000);

  // Initialize Wireless Network Protocol Connection
  WiFi.begin(ssid, password);
  renderStatusBar("WIFI_CONNECT", "Syncing Network...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Sync Internal Clock with Network Time Protocol (NTP) Server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  renderStatusBar("NTP_SYNC", "Fetching UTC Time...");
  delay(1000);
}

void loop() {
  // Update Real-Time Network Chronometer Metrics
  time_t now = time(nullptr);
  struct tm* timeInfo = localtime(&now);

  if (timeInfo->tm_year > 70) { // Verify absolute valid time stamp
    char timeBuffer[10];
    char dateBuffer[12];
    
    sprintf(timeBuffer, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    sprintf(dateBuffer, "%02d/%02d/%04d", timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900);
    
    renderAssistantScreen(timeBuffer, dateBuffer);
  } else {
    renderStatusBar("TIME_ERR", "Re-syncing NTP...");
  }

  delay(1000); // Refresh visual layer every 1-second interval
}

// Low-Latency Status Layer Renderer
void renderStatusBar(const char* status, const char* message) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ZARITECH UNIPRO OS");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  
  display.setCursor(0, 24);
  display.print("NET: ");
  display.println(status);
  display.setCursor(0, 42);
  display.println(message);
  display.display();
}

// Production Assistant Dashboard GUI Renderer
void renderAssistantScreen(const char* timeStr, const char* dateStr) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  // Header Component
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ZARITECH DESK NODE");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  
  // Chrono Data String Render (Centered Large Font)
  display.setTextSize(2);
  display.setCursor(16, 22);
  display.println(timeStr);
  
  // Calendar Metric Data String Render
  display.setTextSize(1);
  display.setCursor(32, 48);
  display.print(dateStr);
  
  display.display();
}
