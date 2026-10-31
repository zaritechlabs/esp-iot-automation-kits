/*
 * ====================================================================
 * Project: Zaritech NodeMCU Web Automation & Security Ecosystem
 * Hardware: NodeMCU ESP8266, HC-SR501, 5V Relay, Active Buzzer
 * Maintainer: Zaritech Labs (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */
/*
 * ====================================================================
 * Project: Zaritech NodeMCU Web Automation & Security Ecosystem
 * Subsystem: Kit 01 - NodeMCU Web Automation & Security Kit
 * Hardware: NodeMCU ESP8266, HC-SR501 PIR Sensor, 5V Relay, Active Buzzer
 * Maintainer: Zaritech Labs (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution & Support: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <ESP8266WiFi.h>

// Enterprise Network Credentials - Configure based on local environment
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

// Hardware Pin Topologies (NodeMCU ESP8266 Mapping)
const int relayPin = 5;     // D1 (GPIO5) - High-Voltage Grid Actuation Control
const int pirPin = 4;       // D2 (GPIO4) - Real-Time Intrusion Telemetry Input
const int buzzerPin = 14;   // D5 (GPIO14)- Localized Acoustic Diagnostic Alert

WiFiServer secureServer(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Initialize Actuation and Diagnostic Alarm Interfaces
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Safe State Initializations: Disengage active-low relay array on boot
  digitalWrite(relayPin, HIGH); 
  digitalWrite(buzzerPin, LOW);

  // Initialize Sensory Interface
  pinMode(pirPin, INPUT);

  Serial.println("\n[ZARITECH SYSTEM] Initializing NodeMCU Web Automation Substrate...");
  
  // Establish Secure Wireless Local Area Network Handshake
  Serial.print("[ZARITECH SYSTEM] Connecting to Infrastructure SSID: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Start Standalone Localized IoT Web Server
  secureServer.begin();
  Serial.println("\n[ZARITECH SYSTEM] Wireless Interface Status: CONNECTED.");
  Serial.print("[ZARITECH SYSTEM] Assigned Local Node IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Execute Real-Time Intrusion Telemetry (PIR Sensor Array Check)
  int motionState = digitalRead(pirPin);
  if (motionState == HIGH) {
    Serial.println("[ZARITECH ALERT] Motion Vector Detected in Monitored Perimeter!");
    triggerAcousticChime(150); // Instantly fire the active buzzer
  }

  // Monitor and Accept Inbound Enterprise Wireless Control Commands
  WiFiClient client = secureServer.available();
  if (!client) {
    return;
  }

  // Read the first line of the inbound HTTP request string
  String request = client.readStringUntil('\r');
  client.flush();

  // Route Actuation Node Commands based on Endpoint Parsers
  if (request.indexOf("/ACTUATOR=ON") != -1) {
    digitalWrite(relayPin, LOW); // Engage Relay (Active-Low Topology)
    Serial.println("[ZARITECH SYSTEM] Relay Actuator State: ENGAGED");
    triggerAcousticChime(50);   // Brief audible diagnostic confirmation
  }
  if (request.indexOf("/ACTUATOR=OFF") != -1) {
    digitalWrite(relayPin, HIGH); // Disengage Relay
    Serial.println("[ZARITECH SYSTEM] Relay Actuator State: DISENGAGED");
    triggerAcousticChime(50);    // Brief audible diagnostic confirmation
  }

  // Render Industrial IoT Web Interface Dashboard
  renderWebDashboard(client);
  delay(1); // Yield to background network stacks
}

// Localized Acoustic Notification Routine
void triggerAcousticChime(int durationMs) {
  digitalWrite(buzzerPin, HIGH);
  delay(durationMs);
  digitalWrite(buzzerPin, LOW);
}

// Production-Grade Responsive Dashboard Renderer
void renderWebDashboard(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println(""); // Critical blank line separating headers from body
  
  client.println("<!DOCTYPE HTML><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<title>Zaritech NodeMCU Ecosystem</title>");
  client.println("<style>body{font-family:Arial,sans-serif; text-align:center; background-color:#f4f6f9; color:#333;}");
  client.println(".container{max-width:500px; margin:50px auto; padding:20px; background:#fff; border-radius:8px; box-shadow:0 4px 6px rgba(0,0,0,0.1);}");
  client.println(".btn{display:inline-block; padding:15px 25px; margin:10px; font-size:16px; border:none; border-radius:5px; cursor:pointer; text-decoration:none; color:#fff;}");
  client.println(".btn-on{background-color:#2ecc71;} .btn-off{background-color:#e74c3c;}</style></head>");
  
  client.println("<body><div class=\"container\">");
  client.println("<h1>Zaritech Labs IoT Web Automation</h1>");
  client.println("<p>Infrastructure Division: <strong>Zaritech Dynamics</strong></p>");
  client.println("<hr><p>Subsystem Power Control Array (10A Relay):</p>");
  client.println("<a href=\"/ACTUATOR=ON\" class=\"btn btn-on\">ENGAGE ACTUATOR</a>");
  client.println("<a href=\"/ACTUATOR=OFF\" class=\"btn btn-off\">DISENGAGE ACTUATOR</a>");
  client.println("</div></body></html>");
}
