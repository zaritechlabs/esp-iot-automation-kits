/*
 * ====================================================================
 * Project: Zaritech ESP32 Spatial Telemetry & Ultrasonic Radar Ecosystem
 * Subsystem: Kit 03 - Zaritech ESP32 Spatial Telemetry Suite
 * Hardware: ESP32 NodeMCU, HC-SR04 Ultrasonic Sensor, Active Buzzer, Push Button, LEDs
 * Maintainer: Zaritech Labs (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution & Support: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <WiFi.h>

// Enterprise Network Credentials - Configure based on local environment
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

// Hardware Pin Topologies (ESP32 NodeMCU Mapping)
const int trigPin = 5;      // Sonic Pulse Output
const int echoPin = 18;     // Time-of-Flight Input
const int buzzerPin = 19;   // Acoustic Warning Actuator
const int buttonPin = 4;    // Manual Override / Calibration Input

// Visual LED Array Configuration
const int led1 = 25;        // Zone 1 Indicator (Far)
const int led2 = 26;        // Zone 2 Indicator (Mid)
const int led3 = 27;        // Zone 3 Indicator (Critical Close)

WiFiServer secureServer(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Initialize Peripheral Pin Environments
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Secure logic high state

  // Initialize Visual LED Infrastructure
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Set Safe Default States
  digitalWrite(buzzerPin, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  Serial.println("\n[ZARITECH SYSTEM] Initializing ESP32 Spatial Telemetry Substrate...");
  
  // Establish Secure Wireless Local Area Network Connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  secureServer.begin();
  Serial.println("\n[ZARITECH SYSTEM] ESP32 Core Wireless Interface: ACTIVE.");
  Serial.print("[ZARITECH SYSTEM] Assigned Local Node IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Execute Precision Ultrasonic Time-of-Flight Mapping
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  // Calculate metric distance in centimeters based on speed of sound
  int distanceCm = duration * 0.034 / 2;

  // Manual Override Interrupt Read
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("[ZARITECH SYSTEM] Manual Calibration Pin Triggered.");
    executeSystemChime(300);
  }

  // Tiered Proximity Processing Loops
  processProximityZones(distanceCm);

  // Serve Enterprise Telemetry Dashboard over Wireless Web Interface
  WiFiClient client = secureServer.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  renderWebDashboard(client, distanceCm);
  delay(1);
}

// Tiered Proximity Management Routine
void processProximityZones(int distance) {
  if (distance > 0 && distance <= 10) { // Critical Zone
    digitalWrite(led1, HIGH); digitalWrite(led2, HIGH); digitalWrite(led3, HIGH);
    executeSystemChime(50); 
  } else if (distance > 10 && distance <= 25) { // Warning Zone
    digitalWrite(led1, HIGH); digitalWrite(led2, HIGH); digitalWrite(led3, LOW);
  } else if (distance > 25 && distance <= 50) { // Safe Detection Zone
    digitalWrite(led1, HIGH); digitalWrite(led2, LOW); digitalWrite(led3, LOW);
  } else { // Idle State
    digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);
  }
}

// Localized Acoustic Diagnostic Signal
void executeSystemChime(int durationMs) {
  digitalWrite(buzzerPin, HIGH);
  delay(durationMs);
  digitalWrite(buzzerPin, LOW);
}

// Production-Grade Responsive Spatial Dashboard Renderer
void renderWebDashboard(WiFiClient& client, int currentDistance) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  
  client.println("<!DOCTYPE HTML><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<title>Zaritech ESP32 Spatial Hub</title>");
  client.println("<style>body{font-family:Arial,sans-serif; text-align:center; background-color:#1e272e; color:#fff;}");
  client.println(".container{max-width:500px; margin:50px auto; padding:20px; background:#2f3640; border-radius:10px; box-shadow:0 4px 10px rgba(0,0,0,0.3);}");
  client.println(".telemetry{font-size:48px; color:#00d2d3; margin:20px 0;}</style></head>");
  
  client.println("<body><div class=\"container\">");
  client.println("<h1>Zaritech Labs Spatial Telemetry</h1>");
  client.println("<p>Architecture Substrate: <strong>ESP32 32-Bit Dual-Core</strong></p><hr>");
  client.println("<p>Real-Time Proximity Metric:</p>");
  client.println("<div class=\"telemetry\">" + String(currentDistance) + " cm</div>");
  client.println("<p>Status Monitored under Zaritech Dynamics Standards.</p>");
  client.println("</div></body></html>");
}
