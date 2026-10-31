/*
 * ====================================================================
 * Project: Zaritech Industrial HMI Rotary Menu System & OLED Interface
 * Subsystem: Kit 06 - Zaritech Labs Ar-Ge Series Control Module
 * Hardware: NodeMCU ESP8266, Rotary Encoder, 0.96" I2C OLED (SSD1306)
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

// OLED Geometry Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Hardware Pin Topologies (Rotary Encoder)
const int encoderCLK = 14; // D5 - Clock
const int encoderDT  = 12; // D6 - Data
const int encoderSW  = 13; // D7 - Switch

// Menu Logic Variables
int currentMenuIndex = 0;
const int maxMenuItems = 3;
const char* menuItems[maxMenuItems] = {"1. Wi-Fi Status", "2. Relay Control", "3. System Metrics"};

int lastClkState;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 200;

WiFiServer hmiServer(80);

void setup() {
  Serial.begin(115200);

  // Initialize Input Interfaces with Internal Pull-Ups
  pinMode(encoderCLK, INPUT_PULLUP);
  pinMode(encoderDT, INPUT_PULLUP);
  pinMode(encoderSW, INPUT_PULLUP);

  // Read Initial Clock State
  lastClkState = digitalRead(encoderCLK);

  // Initialize Local I2C Visual Substrate
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("[ZARITECH ERROR] OLED Screen allocation failed.");
    for(;;);
  }

  display.clearDisplay();
  renderMenuFrame();

  // Passive Network Initialization (Optional local connection placeholder)
  WiFi.mode(WIFI_OFF); 
}

void loop() {
  // Read Quadrature Pulse String (Rotary Encoder Movement)
  int currentClkState = digitalRead(encoderCLK);
  
  if (currentClkState != lastClkState && currentClkState == LOW) {
    // Determine Rotation Direction based on Detent States
    if (digitalRead(encoderDT) != currentClkState) {
      currentMenuIndex++;
      if (currentMenuIndex >= maxMenuItems) currentMenuIndex = 0;
    } else {
      currentMenuIndex--;
      if (currentMenuIndex < 0) currentMenuIndex = maxMenuItems - 1;
    }
    
    // Update Local Visual Layer immediately
    renderMenuFrame();
  }
  lastClkState = currentClkState;

  // Track Select Button Activation (Debounced)
  if (digitalRead(encoderSW) == LOW) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      executeMenuSelection();
      lastDebounceTime = millis();
    }
  }
  
  delay(1); // Yield control to background processing cores
}

// Low-Latency Graphical Interface Renderer
void renderMenuFrame() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  // Draw Zaritech UI Structural Header Bar
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ZARITECH HMI OS v1.2");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  
  // Loop and Display Dynamic Nested Menu Array
  for (int i = 0; i < maxMenuItems; i++) {
    display.setCursor(10, 18 + (i * 14));
    if (i == currentMenuIndex) {
      display.print("> "); // Render Selection Cursor Arrow
      display.println(menuItems[i]);
    } else {
      display.print("  ");
      display.println(menuItems[i]);
    }
  }
  
  display.display();
}

// Action Trigger System on Selection Click
void executeMenuSelection() {
  display.clearDisplay();
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  display.setCursor(0, 18);
  display.println("EXECUTING NODE:");
  display.setCursor(0, 36);
  display.setTextSize(1);
  display.println(menuItems[currentMenuIndex]);
  display.display();
  
  Serial.print("[ZARITECH HMI] Selected Endpoint Loop: ");
  Serial.println(menuItems[currentMenuIndex]);
  
  delay(1500); // Display feedback frame before returning to main loop
  renderMenuFrame();
}
