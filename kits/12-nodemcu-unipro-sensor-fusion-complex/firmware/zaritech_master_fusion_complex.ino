/*
 * ====================================================================
 * Project: Zaritech UniPro Sensor Fusion Complex & Multi-Tier IoT Gateway
 * Subsystem: Kit 12 - Zaritech Flagship Sensor Fusion Laboratory
 * Hardware: NodeMCU Core + Base Shield, HC-SR04, TTP223, KY-039, Relay, C002 Siren, LDR
 * Maintainer: Zaritech Labs (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution & Support: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <ESP8266WiFi.h>

// Enterprise Network Credentials
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

// Pin Configuration Matrix (Seated on Base Shield)
const int pinTouch     = 4;  // D2 - Capacitive Input
const int pinTrig      = 5;  // D1 - Ultrasonic Trig
const int pinEcho      = 12; // D6 - Ultrasonic Echo
const int pinRelay     = 14; // D5 - Power Actuation Relay
const int pinSiren     = 13; // D7 - C002 High-Power Siren Array
const int pinAnalogBus = A0; // Shared Telemetry Analog Node (KY-039 / LDR via layout)

WiFiServer masterFusionServer(80);
String globalSecurityStatus = "SYSTEM_INTEGRITY_OK";

void setup() {
  Serial.begin(115200);
  delay(10);

  // Initialize Input / Output Peripheral Nodes
  pinMode(pinTouch, INPUT);
  pinMode(pinEcho, INPUT);
  pinMode(pinTrig, OUTPUT);
  pinMode(pinRelay, OUTPUT);
  pinMode(pinSiren, OUTPUT);

  // Safe Default State Executions
  digitalWrite(pinRelay, HIGH); // Disengage active-low relay grid
  digitalWrite(pinSiren, LOW);  // Silence industrial siren array

  Serial.println("\n[ZARITECH MASTER] Booting Flagship Sensor Fusion Complex Substrate...");

  // Establish Network Matrix
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  masterFusionServer.begin();
  Serial.println("\n[ZARITECH MASTER] Multi-Tier Wireless Gateway Node: OPERATIONAL.");
  Serial.print("[ZARITECH MASTER] Central Complex IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // 1. Parse Spatial Proximity Telemetry
  digitalWrite(pinTrig, LOW); delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH); delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  long duration = pulseIn(pinEcho, HIGH);
  int distanceMetric = duration * 0.034 / 2;

  // 2. Parse HMI Capacitive Interface States
  int touchState = digitalRead(pinTouch);

  // 3. Parse Volumetric Analog Telemetry (Biometric / Luminance String)
  int rawAnalogStream = analogRead(pinAnalogBus);

  // Autonomous Defensive Rule Matrix
  if (touchState == HIGH || (distanceMetric > 0 && distanceMetric < 15)) {
    globalSecurityStatus = "PERIMETER_BREACH_ALERT";
    digitalWrite(pinRelay, LOW);   // Actuate Power Grid
    digitalWrite(pinSiren, HIGH);  // Fire Industrial Warning Siren
  } else {
    globalSecurityStatus = "SYSTEM_INTEGRITY_OK";
    digitalWrite(pinRelay, HIGH);
    digitalWrite(pinSiren, LOW);
  }

  // Listen for Inbound Supervisory Network Connections
  WiFiClient client = masterFusionServer.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  // Render Master Fusion Control Console
  serveMasterFusionDashboard(client, distanceMetric, touchState, rawAnalogStream);
  delay(1);
}

// Production-Grade Comprehensive Operational Control Dashboard Renderer
void serveMasterFusionDashboard(WiFiClient& client, int dist, int touch, int analogVal) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  
  client.println("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<title>Zaritech Master Console</title>");
  client.println("<style>body{font-family:Arial,sans-serif; text-align:center; background-color:#090d16; color:#f1f5f9;}");
  client.println(".panel{max-width:700px; margin:40px auto; padding:35px; background:#111827; border-radius:8px; border: 2px solid #3b82f6;}");
  client.println(".matrix-grid{display:flex; flex-wrap:wrap; justify-content:space-around; margin:25px 0;}");
  client.println(".node-card{background:#1f2937; padding:20px; border-radius:6px; width:28%; margin:10px; border-top:4px solid #3b82f6;}");
  client.println(".alert-active{border-top:4px solid #ef4444;} .metric{font-size:28px; font-weight:bold; margin-top:15px; color:#60a5fa;}</style></head>");
  
  client.println("<body><div class=\"panel\">");
  client.println("<h1>Zaritech Master Sensor Fusion Console</h1>");
  client.println("<p>Corporate Division: <strong>Zaritech Dynamics Framework</strong></p><hr>");
  
  client.println("<div class=\"matrix-grid\">");
  client.println("<div class=\"node-card\"><h4>Proximity Metric</h4><div class=\"metric\">" + String(dist) + " cm</div></div>");
  client.println("<div class=\"node-card\"><h4>HMI Touch State</h4><div class=\"metric\">" + String(touch ? "ACTIVE" : "IDLE") + "</div></div>");
  client.println("<div class=\"node-card\"><h4>Analog Telemetry</h4><div class=\"metric\">" + String(analogVal) + "</div></div>");
  client.println("</div>");
  
  client.print("<p>Gateway Automation Core Status: <strong>");
  client.print(globalSecurityStatus);
  client.println("</strong></p></div></body></html>");
}
