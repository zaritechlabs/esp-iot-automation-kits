/*
 * ====================================================================
 * Project: Zaritech Secure Matrix Access Control & Servo Actuation Framework
 * Subsystem: Kit 10 - Zaritech Access Control Platform
 * Hardware: NodeMCU V3, 4x4 Keypad, SG90 Servo, Active Buzzer, 18650 Unit
 * Maintainer: Zaritech Labs (A division of Zaritech Dynamics)
 * License: MIT License
 * Official Distribution & Support: https://instagram.com
 * ====================================================================
 * Copyright (c) 2026 Zaritech Dynamics. All rights reserved.
 */

#include <ESP8266WiFi.h>
#include <Servo.h>
#include <Keypad.h>

// Enterprise Network Credentials
const char* ssid = "ZARITECH_SECURE_WIFI";
const char* password = "ZARITECH_ENTERPRISE_PASS";

// Cryptographic Access Token Configuration
const int PASSWORD_LENGTH = 4;
char masterPassword[PASSWORD_LENGTH] = {'1', '2', '3', '4'};
char enteredPassword[PASSWORD_LENGTH];
int inputCounter = 0;

// Keypad Structural Topology Map
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {16, 5, 4, 0};   // D0, D1, D2, D3
byte colPins[COLS] = {2, 3, 1, 15};   // D4, RX, TX, D8

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Actuator Instantiation
Servo lockServo;
const int servoPin = 14;  // D5
const int buzzerPin = 12; // D6
const int statusLed = 13; // D7

WiFiServer securityServer(80);
String systemSecureState = "SECURED_LOCKED";

void setup() {
  Serial.begin(115200);
  
  // Initialize Control Peripherals
  pinMode(buzzerPin, OUTPUT);
  pinMode(statusLed, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(statusLed, LOW);

  // Bind Mechanical Servo Node & Return to Safe Baseline Home
  lockServo.attach(servoPin);
  lockServo.write(0); // 0 Degrees = Secure Bolt Locked state

  Serial.println("\n[ZARITECH SYSTEM] Initializing Access Control Substrate...");

  // Establish Network Matrix
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  securityServer.begin();
  Serial.println("\n[ZARITECH SYSTEM] Security Loop Active. Awaiting Authentication...");
}

void loop() {
  // Read Matrix Keypad Interconnect Bus
  char pressedKey = customKeypad.getKey();

  if (pressedKey) {
    executeAcousticChime(60); // Haptic feedback chime
    Serial.print("[ZARITECH ENTRY] Key Token Logged: ");
    Serial.println(pressedKey);

    if (pressedKey == '#') { // System Reset Command
      resetInputBuffer();
    } else {
      enteredPassword[inputCounter] = pressedKey;
      inputCounter++;

      if (inputCounter == PASSWORD_LENGTH) {
        verifyCryptographicToken();
      }
    }
  }

  // Monitor Remote Administrator Commands over Wi-Fi
  WiFiClient client = securityServer.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/LOCK=OVERRIDE") != -1) {
    engageLockRelease();
  }

  serveSecurityDashboard(client);
  delay(1);
}

// Token Verification Subroutine
void verifyCryptographicToken() {
  bool tokenMatch = true;
  for (int i = 0; i < PASSWORD_LENGTH; i++) {
    if (enteredPassword[i] != masterPassword[i]) {
      tokenMatch = false;
    }
  }

  if (tokenMatch) {
    engageLockRelease();
  } else {
    systemSecureState = "ACCESS_DENIED_ALARM";
    Serial.println("[ZARITECH WARNING] Cryptographic Mismatch! Access Denied.");
    for (int r = 0; r < 3; r++) {
      executeAcousticChime(300);
      delay(100);
    }
    resetInputBuffer();
  }
}

// Unlock Matrix Execution Routine
void engageLockRelease() {
  systemSecureState = "AUTHORIZED_UNLOCKED";
  Serial.println("[ZARITECH SYSTEM] Access Token Verified. Disengaging Mechanical Lock...");
  digitalWrite(statusLed, HIGH);
  lockServo.write(90); // Rotate 90 Degrees to release secure bolt
  executeAcousticChime(500);
  
  delay(5000); // Hold perimeter entry point open for 5 seconds
  
  lockServo.write(0); // Re-engage lock
  digitalWrite(statusLed, LOW);
  systemSecureState = "SECURED_LOCKED";
  resetInputBuffer();
}

void resetInputBuffer() {
  inputCounter = 0;
  Serial.println("[ZARITECH SYSTEM] Local Entry Buffer Cleared.");
}

void executeAcousticChime(int durationMs) {
  digitalWrite(buzzerPin, HIGH);
  delay(durationMs);
  digitalWrite(buzzerPin, LOW);
}

// Enterprise Remote Admin Interface Renderer
void serveSecurityDashboard(WiFiClient& client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println("");
  
  client.println("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<title>Zaritech Secure Entry Node</title>");
  client.println("<style>body{font-family:Arial,sans-serif; text-align:center; background-color:#020617; color:#f1f5f9;}");
  client.println(".panel{max-width:550px; margin:50px auto; padding:30px; background:#0f172a; border-radius:6px; border: 1px solid #ef4444;}");
  client.println(".state{font-size:24px; font-weight:bold; color:#ef4444; margin:20px 0;} .state-ok{color:#22c55e;}");
  client.println(".override-btn{display:inline-block; padding:12px 24px; background:#dc2626; color:#fff; text-decoration:none; border-radius:4px; font-weight:bold;}</style></head>");
  
  client.println("<body><div class=\"panel\">");
  client.println("<h1>Zaritech Secure Access Hub</h1>");
  client.println("<p>Infrastructure Core: <strong>Zaritech Dynamics</strong></p><hr>");
  client.println("<p>Node Lock Status Status:</p>");
  if (systemSecureState == "SECURED_LOCKED") {
    client.println("<div class=\"state\">SYSTEM LOCKED</div>");
  } else {
    client.println("<div class=\"state state-ok\">ACCESS OPENED</div>");
  }
  client.println("<a href=\"/LOCK=OVERRIDE\" class=\"override-btn\">REMOTE OVERRIDE UNLOCK</a>");
  client.println("</div></body></html>");
}
