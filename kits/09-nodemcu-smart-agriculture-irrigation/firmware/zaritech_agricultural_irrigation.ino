/*
 * ====================================================================
 * Project: Zaritech Precision Smart-Agriculture Hub & Automated Irrigation Substrate
 * Subsystem: Kit 09 - Zaritech Smart-Agriculture Platform
 * Hardware: NodeMCU V3, Soil Moisture Sensor, DHT11, 5V Mini Pump, 5V Relay, 18650 Supply
 * Maintainer: Zaritech Labs (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution & Support: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <ESP8266WiFi.h>
#include <DHT.h>

// DHT Environmental Sensor Mapping
#define DHTPIN 2       // D4 (GPIO2)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Hardware Pin Topologies
const int soilMoisturePin = A0;  // Analog Telemetry Input
const int pumpRelayPin    = 5;   // D1 (GPIO5) - Fluid Pump Actuator

// Enterprise Network Credentials
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

// Smart Irrigation Automation Thresholds
const int CRITICAL_DRY_THRESHOLD = 400; // Calibrated raw analog mapping (Lower = Wetter, Higher = Drier)
const int REFRESH_RATE_MS = 2000;

WiFiServer agroServer(80);

void setup() {
  Serial.begin(115200);
  
  // Initialize Fluid Control Interface
  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, HIGH); // Disengage active-low relay array on boot (Pump OFF)

  // Initialize Sensory Infrastructure
  dht.begin();
  Serial.println("\n[ZARITECH SYSTEM] Initializing Agricultural Telemetry Substrate...");

  // Establish Secure Network Connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  agroServer.begin();
  Serial.println("\n[ZARITECH SYSTEM] Precision Agriculture Node Online.");
  Serial.print("[ZARITECH SYSTEM] Assigned Local IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Parse Subterranean Moisture Strings
  int rawSoilMoisture = analogRead(soilMoisturePin);
  
  // Parse Atmospheric Climate Strings
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Validate Sensor Integrity Check
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("[ZARITECH ERROR] Failed to parse DHT telemetry string!");
    return;
  }

  // Autonomous Closed-Loop Irrigation Core
  if (rawSoilMoisture >= CRITICAL_DRY_THRESHOLD) {
    Serial.println("[ZARITECH AUTOMATION] Critical Soil Depletion! Engaging Fluid Subsystem...");
    digitalWrite(pumpRelayPin, LOW);  // Engage Pump (Active-Low Topology)
  } else {
    digitalWrite(pumpRelayPin, HIGH); // Disengage Pump
  }

  // Listen for Inbound Network Telemetry Requests
  WiFiClient client = agroServer.available();
  if (!client) {
    delay(REFRESH_RATE_MS);
    return;
  }

  String request = client.readStringUntil('\r');
  client.flush();

  // Render Responsive Agro-Dashboard over Wireless Web Interface
  serveAgroDashboard(client, rawSoilMoisture, temperature, humidity);
  delay(1);
}

// Technical Web Server HTML Dashboard Renderer
void serveAgroDashboard(WiFiClient& client, int soilMoisture, float temp, float hum) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  
  client.println("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<title>Zaritech Agro Hub</title>");
  client.println("<style>body{font-family:Arial,sans-serif; text-align:center; background-color:#0f172a; color:#f8fafc;}");
  client.println(".panel{max-width:600px; margin:50px auto; padding:30px; background:#1e293b; border-radius:10px; border: 1px solid #10b981;}");
  client.println(".data-grid{display:flex; flex-wrap:wrap; justify-content:space-around; margin:25px 0;}");
  client.println(".data-card{background:#334155; padding:15px; border-radius:8px; width:28%; margin:5px;}");
  client.println(".val{font-size:24px; font-weight:bold; color:#34d399; margin-top:10px;}</style></head>");
  
  client.println("<body><div class=\"panel\">");
  client.println("<h1>Zaritech Precision Agriculture Node</h1>");
  client.println("<p>Infrastructure Core: <strong>Zaritech Dynamics</strong></p><hr>");
  client.println("<div class=\"data-grid\">");
  client.println("<div class=\"data-card\"><h4>Soil Moisture</h4><div class=\"val\">" + String(soilMoisture) + "</div></div>");
  client.println("<div class=\"data-card\"><h4>Temperature</h4><div class=\"val\">" + String(temp, 1) + " °C</div></div>");
  client.println("<div class=\"data-card\"><h4>Humidity</h4><div class=\"val\">" + String(hum, 1) + " %</div></div>");
  client.println("</div>");
  client.println("<p>Pump Safe Activation Boundary Point: <strong>> " + String(CRITICAL_DRY_THRESHOLD) + "</strong></p>");
  client.println("</div></body></html>");
}
