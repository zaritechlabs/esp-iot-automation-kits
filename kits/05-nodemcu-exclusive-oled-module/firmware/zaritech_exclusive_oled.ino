/*
 * ====================================================================
 * Project: Zaritech Exclusive High-Performance IoT Module & Wireless OLED Node
 * Subsystem: Kit 05 - Zaritech Exclusive Series Prototyping Hub
 * Hardware: NodeMCU ESP8266, 0.96" I2C OLED Display (SSD1306), Custom Wiring Arrays
 * Maintainer: Zaritech Interactive (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution & Support: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Hardware Dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Network Protocols Configuration
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

WiFiServer exclusiveServer(80);
unsigned long interactionCount = 0;

void setup() {
  Serial.begin(115200);
  
  // Wire Bus Initialization (OLED Connections)
  Wire.begin(4, 5); // SDA = GPIO4 (D2), SCL = GPIO5 (D1)

  // Validate Display Allocation
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("[ZARITECH ERROR] OLED Screen allocation failed.");
    for(;;);
  }

  updateDisplayNode("EXCLUSIVE NODE", "BOOTING COMPLETED", "System Safe Status");
  delay(1200);

  // Initialize Wireless Signal Infrastructure
  WiFi.begin(ssid, password);
  updateDisplayNode("NET VERIFY", "CONNECTING TO:", ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }

  exclusiveServer.begin();
  
  // Successful Deployment Dashboard Trace
  updateDisplayNode("NODE ACTIVE", WiFi.localIP().toString().c_str(), "Exclusive Network");
}

void loop() {
  // Listen for Inbound Interaktive Remote Telemetry Queries
  WiFiClient client = exclusiveServer.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  // Track Web Interactions for Real-Time Local Updates
  if (request.indexOf("/DATA=REFRESH") != -1) {
    interactionCount++;
    updateDisplayNode("DATA REFRESHED", ("Pings: " + String(interactionCount)).c_str(), "Remote Triggered");
  }

  // Render Web-Based Interactive API Control Interface
  serveCorporateDashboard(client);
  delay(1);
}

// Low-Latency OLED Visual Layer Renderer
void updateDisplayNode(const char* title, const char* coreData, const char* baseTag) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  // Draw Zaritech Exclusive UI Branding Frame
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ZARITECH EXCLUSIVE");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  
  // Render Operational Dynamic Arrays
  display.setCursor(0, 18);
  display.print("MODE: ");
  display.println(title);
  
  display.setCursor(0, 36);
  display.setTextSize(1);
  display.println(coreData);
  
  display.setCursor(0, 54);
  display.println(baseTag);
  
  display.display();
}

// Production-Grade Technical Web Server Dashboard HTML Renderer
void serveCorporateDashboard(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  
  client.println("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<title>Zaritech Exclusive Module</title>");
  client.println("<style>body{font-family:'Courier New',monospace; text-align:center; background-color:#0f172a; color:#38bdf8;}");
  client.println(".panel{max-width:600px; margin:50px auto; padding:30px; background:#1e293b; border: 1px solid #0284c7; border-radius:4px;}");
  client.println(".metric{font-size:24px; color:#f1f5f9; margin:20px 0;}");
  client.println(".interactive-btn{display:inline-block; padding:12px 24px; font-weight:bold; color:#0f172a; background-color:#38bdf8; text-decoration:none; border-radius:2px;}");
  client.println(".interactive-btn:hover{background-color:#0ea5e9;}</style></head>");
  
  client.println("<body><div class=\"panel\">");
  client.println("<h1>Zaritech Interactive High-Performance Node</h1>");
  client.println("<p>Substrate Layer: <strong>Decoupled External Brain Topology</strong></p><hr>");
  client.println("<div class=\"metric\">Total Dynamic API Requests: " + String(interactionCount) + "</div>");
  client.println("<a href=\"/DATA=REFRESH\" class=\"interactive-btn\">TRIGGER LOCAL OLED PACKET</a>");
  client.println("<p style='font-size:12px; color:#64748b; margin-top:25px;'>Zaritech Dynamics Engineering Framework • Verified 2026</p>");
  client.println("</div></body></html>");
}
