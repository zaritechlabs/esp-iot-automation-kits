/*
 * ====================================================================
 * Project: Zaritech Multi-Architecture IoT Integration & Prototyping Lab
 * Hardware: NodeMCU V3 / Wemos D1 Mini, TTP229 Touch Keypad, LDR Array, 5V Relay
 * Maintainer: Zaritech Interactive (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <ESP8266WiFi.h>

// Enterprise Network Credentials
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

// Hardware Pin Configurations
const int touchSCL = 5;     // D1 - Clock Line for Touch Keypad
const int touchSDO = 4;     // D2 - Data Line for Touch Keypad
const int relayPin = 14;    // D5 - Actuation Relay Control
const int buzzerPin = 12;   // D6 - Acoustic Diagnostics Alarm
const int ldrAnalogPin = A0;// Analog Telemetry Input

WiFiServer secureServer(80);

void setup() {
  Serial.begin(115200);
  
  // Initialize Actuation and Alert Subsystems
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Disengage active-low relay array on boot
  digitalWrite(buzzerPin, LOW);

  // Initialize HMI Touch Interface Pins
  pinMode(touchSCL, OUTPUT);
  pinMode(touchSDO, INPUT);

  Serial.println("\n[ZARITECH SYSTEM] Initializing Multi-Architecture IoT Substrate...");
  
  // Establish Secure Wireless Interface
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  secureServer.begin();
  Serial.println("\n[ZARITECH SYSTEM] NodeMCU Wireless Interface Active.");
  Serial.print("[ZARITECH SYSTEM] Node IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Execute Analog Telemetry Read (LDR String Analysis)
  int ambientLight = analogRead(ldrAnalogPin);
  
  // Example Threshold Action: Auto-Actuate Systems in Low Light
  if (ambientLight < 200) {
    digitalWrite(relayPin, LOW); // Engage Relay
  }

  // Handle Enterprise Wireless Commands
  WiFiClient client = secureServer.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  // Route System Commands
  if (request.indexOf("/SYSTEM=ENGAGE") != -1) {
    digitalWrite(relayPin, LOW);
    triggerAcousticChime();
  }
  if (request.indexOf("/SYSTEM=DISENGAGE") != -1) {
    digitalWrite(relayPin, HIGH);
    triggerAcousticChime();
  }

  // Serve Responsive Infrastructure Control Dashboard
  sendHtmlDashboard(client, ambientLight);
  delay(1);
}

// Localized Diagnostic Notification Routine
void triggerAcousticChime() {
  digitalWrite(buzzerPin, HIGH);
  delay(80);
  digitalWrite(buzzerPin, LOW);
}

// Enterprise Dashboard Renderer
void sendHtmlDashboard(WiFiClient& client, int currentTelemetry) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  client.println("<!DOCTYPE HTML><html><head><title>Zaritech System Lab</title></head>");
  client.println("<body style='font-family:sans-serif; text-align:center;'>");
  client.println("<h1>Zaritech Interactive Multi-MCU Control Node</h1>");
  client.println("<p>Ambient Telemetry (LDR String Value): <strong>" + String(currentTelemetry) + "</strong></p>");
  client.println("<hr><p>Subsystem Control Array:</p>");
  client.println("<a href=\"/SYSTEM=ENGAGE\"><button style='padding:10px;'>ENGAGE RELAY</button></a> ");
  client.println("<a href=\"/SYSTEM=DISENGAGE\"><button style='padding:10px;'>DISENGAGE RELAY</button></a>");
  client.println("</body></html>");
}
