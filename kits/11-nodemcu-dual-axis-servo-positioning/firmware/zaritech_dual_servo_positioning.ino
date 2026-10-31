/*
 * ====================================================================
 * Project: Zaritech Dual-Axis Servo Actuation & Kinematic Positioning Framework
 * Subsystem: Kit 11 - Zaritech Dual-Axis Positioning Platform
 * Hardware: NodeMCU V3, 2x SG90 Servo Motors, 18650 Power Substrate
 * Maintainer: Zaritech Labs (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution & Support: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <ESP8266WiFi.h>
#include <Servo.h>

// Enterprise Network Credentials
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

// Actuator Instances
Servo servoX; // Axis-X Pan Motor
Servo servoY; // Axis-Y Tilt Motor

// Pin Configurations
const int pinServoX = 5; // D1 (GPIO5)
const int pinServoY = 4; // D2 (GPIO4)

// Spatial Metrics Default Baselines
int axisPositionX = 90; // Center 90 Degrees
int axisPositionY = 90; // Center 90 Degrees

WiFiServer positioningServer(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Bind Kinematic Nodes to Target Logic Output Channels
  servoX.attach(pinServoX);
  servoY.attach(pinServoY);

  // Calibrate System to Safe Default Home Coordinates
  servoX.write(axisPositionX);
  servoY.write(axisPositionY);

  Serial.println("\n[ZARITECH SYSTEM] Initializing Kinematic Positioning Substrate...");

  // Establish Wireless Connection Matrix
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  positioningServer.begin();
  Serial.println("\n[ZARITECH SYSTEM] Dual-Axis Actuator Loop Active and Synchronized.");
  Serial.print("[ZARITECH SYSTEM] Assigned Local Node IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Listen for Inbound Network Kinematic Commands over Wi-Fi
  WiFiClient client = positioningServer.available();
  if (!client) return;

  // Read the Inbound HTTP Request Stream
  String request = client.readStringUntil('\r');
  client.flush();

  // Parse Vector Shifts for Axis-X (Pan)
  if (request.indexOf("/AXIS_X=DECREASE") != -1) {
    axisPositionX = max(0, axisPositionX - 15);
    servoX.write(axisPositionX);
  }
  if (request.indexOf("/AXIS_X=INCREASE") != -1) {
    axisPositionX = min(180, axisPositionX + 15);
    servoX.write(axisPositionX);
  }

  // Parse Vector Shifts for Axis-Y (Tilt)
  if (request.indexOf("/AXIS_Y=DECREASE") != -1) {
    axisPositionY = max(0, axisPositionY - 15);
    servoY.write(axisPositionY);
  }
  if (request.indexOf("/AXIS_Y=INCREASE") != -1) {
    axisPositionY = min(180, axisPositionY + 15);
    servoY.write(axisPositionY);
  }

  // Serve Responsive Kinematic Management Dashboard
  servePositioningDashboard(client);
  delay(1); // Yield control to background network stacks
}

// Enterprise Remote Manipulation Dashboard Renderer
void servePositioningDashboard(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  
  client.println("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<title>Zaritech Kinematic Node</title>");
  client.println("<style>body{font-family:Arial,sans-serif; text-align:center; background-color:#0b1329; color:#f8fafc;}");
  client.println(".panel{max-width:550px; margin:40px auto; padding:30px; background:#1c2541; border-radius:8px; border: 1px solid #48cae4;}");
  client.println(".coord-display{font-size:20px; font-weight:bold; color:#48cae4; margin:15px 0;}");
  client.println(".nav-btn{display:inline-block; padding:12px 20px; background:#00b4d8; color:#fff; text-decoration:none; border-radius:4px; font-weight:bold; margin:5px;}</style></head>");
  
  client.println("<body><div class=\"panel\">");
  client.println("<h1>Zaritech Multi-Axis Kinematic Hub</h1>");
  client.println("<p>Infrastructure Core: <strong>Zaritech Dynamics</strong></p><hr>");
  
  client.println("<div class=\"coord-display\">X-AXIS (PAN) POSITION: " + String(axisPositionX) + "°</div>");
  client.println("<a href=\"/AXIS_X=DECREASE\" class=\"nav-btn\"><- PAN LEFT</a>");
  client.println("<a href=\"/AXIS_X=INCREASE\" class=\"nav-btn\">PAN RIGHT -></a><br><br>");
  
  client.println("<div class=\"coord-display\">Y-AXIS (TILT) POSITION: " + String(axisPositionY) + "°</div>");
  client.println("<a href=\"/AXIS_Y=DECREASE\" class=\"nav-btn\">\/ TILT DOWN</a>");
  client.println("<a href=\"/AXIS_Y=INCREASE\" class=\"nav-btn\">/\ TILT UP</a>");
  
  client.println("</div></body></html>");
}
