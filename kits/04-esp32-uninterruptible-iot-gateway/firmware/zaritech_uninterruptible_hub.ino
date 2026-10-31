/*
 * ====================================================================
 * Project: Zaritech ESP32 Uninterruptible IoT Gateway & OLED Automation Node
 * Subsystem: Kit 04 - Zaritech Uninterruptible IoT Gateway Platform
 * Hardware: ESP32 NodeMCU, 0.96" I2C OLED (SSD1306), 5V Relay, TP4056, Power Boost
 * Maintainer: Zaritech Labs (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution & Support: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen Geometry Definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Enterprise Network Credentials
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

// Actuator Pin Definition
const int relayPin = 14; // GPIO14

WiFiServer secureServer(80);
String currentSystemStatus = "GRID_STANDBY";

void setup() {
  Serial.begin(115200);
  
  // Initialize Actuation Control Output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Disengage active-low relay matrix

  // Initialize Localized I2C Visual Telemetry Substrate
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("[ZARITECH ERROR] SSD1306 OLED Allocation Failed.");
    for(;;);
  }
  
  renderOledMessage("BOOTING...", "Zaritech Labs", "Initializing Subsystems");
  delay(1500);

  // Establish Secure Wireless Network Link
  WiFi.begin(ssid, password);
  renderOledMessage("WIFI_CONNECT", "Targeting SSID:", ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  secureServer.begin();
  
  // Successful Boot Visualization
  currentSystemStatus = "BATTERY_BACKED_ACTIVE";
  renderOledMessage("ONLINE", WiFi.localIP().toString().c_str(), "UPS Power Active");
}

void loop() {
  // Check for Inbound Enterprise Command Arrays
  WiFiClient client = secureServer.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  // Route Infrastructure Actuator States
  if (request.indexOf("/GATEWAY=ENGAGE") != -1) {
    digitalWrite(relayPin, LOW); // Engage Relay
    renderOledMessage("RELAY ACTIVE", WiFi.localIP().toString().c_str(), "Load: ENGAGED");
  }
  if (request.indexOf("/GATEWAY=DISENGAGE") != -1) {
    digitalWrite(relayPin, HIGH); // Disengage Relay
    renderOledMessage("ONLINE", WiFi.localIP().toString().c_str(), "Load: DISENGAGED");
  }

  // Render and Serve Corporate Web Management Dashboard
  serveHtmlDashboard(client);
  delay(1);
}

// Localized Visual Telemetry Renderer
void renderOledMessage(const char* header, const char* middle, const char* footer) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  // Draw Zaritech Decorative Header Bar
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ZARITECH GATEWAY v1.0");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  
  // Render Operational Context
  display.setCursor(0, 18);
  display.setTextSize(1);
  display.print("STATUS: ");
  display.println(header);
  
  display.setCursor(0, 34);
  display.println(middle);
  
  display.setCursor(0, 52);
  display.setTextSize(1);
  display.println(footer);
  
  display.display();
}

// Production-Grade Responsive Localized Web Server HTML Renderer
void serveHtmlDashboard(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  
  client.println("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<title>Zaritech IoT Uninterruptible Gateway</title>");
  client.println("<style>body{font-family:Arial,sans-serif; text-align:center; background-color:#2c3e50; color:#ecf0f1;}");
  client.println(".panel{max-width:550px; margin:60px auto; padding:25px; background:#34495e; border-radius:12px; box-shadow:0 6px 12px rgba(0,0,0,0.4);}");
  client.println(".status-badge{background-color:#27ae60; padding:10px; border-radius:5px; display:inline-block; font-weight:bold; margin-bottom:20px;}");
  client.println(".control-btn{display:inline-block; padding:15px 30px; margin:10px; font-size:16px; border:none; border-radius:6px; cursor:pointer; text-decoration:none; color:#fff;}");
  client.println(".btn-on{background-color:#16a085;} .btn-off{background-color:#c0392b;}</style></head>");
  
  client.println("<body><div class=\"panel\">");
  client.println("<h1>Zaritech Uninterruptible IoT Hub</h1>");
  client.println("<div class=\"status-badge\">SYSTEM STATUS: BATTERY_BACKED_ONLINE</div>");
  client.println("<p>Central Infrastructure Loop managed by <strong>Zaritech Dynamics</strong></p><hr>");
  client.println("<p>Remote Grid Power Relay Manipulation Array:</p>");
  client.println("<a href=\"/GATEWAY=ENGAGE\" class=\"control-btn btn-on\">ENGAGE NODE</a>");
  client.println("<a href=\"/GATEWAY=DISENGAGE\" class=\"control-btn btn-off\">DISENGAGE NODE</a>");
  client.println("</div></body></html>");
}
