/*
 * ====================================================================
 * Project: Zaritech Climate-Aware Power Actuation Hub & Local Telemetry String
 * Subsystem: Kit 08 - Zaritech Climate Control Suite
 * Hardware: NodeMCU V3, DHT11 Sensor, 5V Relay, 18650 Supply, LED Matrix
 * Maintainer: Zaritech Labs (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution & Support: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <ESP8266WiFi.h>
#include <DHT.h>

// DHT Sensor Topology Configuration
#define DHTPIN 2       // D4 (GPIO2)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Enterprise Network Credentials
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

// Hardware Actuator and LED Mapping
const int relayPin  = 5;  // D1 - Actuator Relay
const int blueLED   = 14; // D5 - Humidity / Cooling State
const int greenLED  = 12; // D6 - Normal Standby State
const int redLED    = 13; // D7 - Thermal Alert State

// Automation Thresholds
const float CRITICAL_TEMP_THRESHOLD = 30.0; // Celsius

WiFiServer climateServer(80);

void setup() {
  Serial.begin(115200);
  
  // Initialize Digital Output Nodes
  pinMode(relayPin, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  // Set Safe Default Topologies (Active-Low Relay Disengaged)
  digitalWrite(relayPin, HIGH);
  digitalWrite(blueLED, LOW);
  digitalWrite(greenLED, HIGH); // System safe indicator active
  digitalWrite(redLED, LOW);

  // Initialize Climate Telemetry Bus
  dht.begin();
  Serial.println("\n[ZARITECH SYSTEM] Initializing Climate Telemetry Substrate...");

  // Establish Secure Network Connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  climateServer.begin();
  Serial.println("\n[ZARITECH SYSTEM] Local Infrastructure Node Online.");
  Serial.print("[ZARITECH SYSTEM] Assigned Local IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Read Local Environmental Sensory Strings
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Validate Sensor Integrity Check
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("[ZARITECH ERROR] Failed to parse DHT telemetry string!");
    return;
  }

  // Autonomous Threshold Control Core
  if (temperature >= CRITICAL_TEMP_THRESHOLD) {
    digitalWrite(relayPin, LOW);  // Engage Relay (Cooling/Exhaust On)
    digitalWrite(redLED, HIGH);   // Fire Alarm Signal
    digitalWrite(greenLED, LOW);
  } else {
    digitalWrite(relayPin, HIGH); // Disengage Relay
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
  }

  // Handle Wireless Server Connections
  WiFiClient client = climateServer.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  // Render Industrial Responsive Telemetry Web Panel
  serveClimateDashboard(client, temperature, humidity);
  delay(1);
}

// Technical Web Server HTML Dashboard Renderer
void serveClimateDashboard(WiFiClient& client, float temp, float hum) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  
  client.println("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<title>Zaritech Climate Hub</title>");
  client.println("<style>body{font-family:Arial,sans-serif; text-align:center; background-color:#111827; color:#f9fafb;}");
  client.println(".panel{max-width:550px; margin:50px auto; padding:30px; background:#1f2937; border-radius:8px;}");
  client.println(".data-grid{display:flex; justify-content:space-around; margin:25px 0;}");
  client.println(".data-card{background:#374151; padding:20px; border-radius:6px; width:40%;}");
  client.println(".val{font-size:32px; font-weight:bold; color:#10b981;}</style></head>");
  
  client.println("<body><div class=\"panel\">");
  client.println("<h1>Zaritech Environmental Actuation Terminal</h1>");
  client.println("<p>Infrastructure Core: <strong>Zaritech Dynamics</strong></p><hr>");
  client.println("<div class=\"data-grid\">");
  client.println("<div class=\"data-card\"><h3>Temperature</h3><div class=\"val\">" + String(temp, 1) + " °C</div></div>");
  client.println("<div class=\"data-card\"><h3>Humidity</h3><div class=\"val\">" + String(hum, 1) + " %</div></div>");
  client.println("</div>");
  client.println("<p>Automated Grid Limit Target: <strong>" + String(CRITICAL_TEMP_THRESHOLD, 1) + " °C</strong></p>");
  client.println("</div></body></html>");
}
