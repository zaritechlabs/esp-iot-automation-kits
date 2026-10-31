# Zaritech NodeMCU Web Automation & Security Ecosystem 🌐

Welcome to the official repository for the **Zaritech NodeMCU Web Automation Set** — a modular, high-reliability Internet of Things (IoT) hardware ecosystem engineered by **Zaritech Labs** (A division of **Zaritech Dynamics**).

Unlike standard bare-board development environments, this integrated infrastructure provides all the necessary sensors, signal routing, and power actuation layers in a unified package designed for rapid deployment of smart home automation and localized web security frameworks.

---

## 🛠️ Hardware & Topology Specs
Our modular prototyping and automation setups are compiled for optimal sensor telemetry, stable logic-level shifting, and dependable low-voltage switching capabilities:

*   **Core Microcontroller:** NodeMCU ESP8266 Wi-Fi enabled development board executing on safe, low-power logical structures.
*   **Actuation Layer:** 5V Single-Channel Relay Module supporting up to **10A (250VAC / 30VDC)** synchronous grid or heavy-load power control.
*   **Telemetry & Motion Tracking:** HC-SR501 PIR Motion Sensor equipped with dedicated potentiometers for adjusting sensitivity and delay times.
*   **Acoustic Telemetry:** Active Buzzer unit for immediate audible status verification and real-time security alerts.
*   **Interconnect Infrastructure:** 18-piece multi-color high-durability jumper cable array optimized for safe signal paths.

---

## 🗺️ Visual Architecture & Production
The recommended electrical routing, pin mapping parameters, and system topology are detailed inside this infrastructure:

### Sample Interconnection Map

| Peripheral Component | Target Pinout | NodeMCU Node | Operational Context |
| :--- | :--- | :--- | :--- |
| **5V Relay Module** | VCC / IN / GND | VIN / D1 / GND | High-Voltage Actuation Control |
| **HC-SR501 PIR Sensor** | VCC / OUT / GND | VIN / D2 / GND | Real-Time Intrusion Telemetry |
| **Active Buzzer Alert** | POS (+) / NEG (-) | D5 / GND | Audible Notification Output |

### Functional Demonstration Code
```cpp
#include <ESP8266WiFi.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const int relayPin = 5;    // D1
const int pirPin = 4;      // D2
const int buzzerPin = 14;  // D5

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pirPin, INPUT);
  
  digitalWrite(relayPin, HIGH); // Disengage active-low relay state

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  
  server.begin();
}

void loop() {
  if (digitalRead(pirPin) == HIGH) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
  }

  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil\(('\r');\)
  client.flush();

  if (request.indexOf("/RELAY=ON") != -1)  digitalWrite(relayPin, LOW);
  if (request.indexOf("/RELAY=OFF") != -1) digitalWrite(relayPin, HIGH);

  client.println("HTTP/1.1 200 OK\nContent-Type: text/html\n\n<!DOCTYPE HTML><html>");
  client.println("<h1>Zaritech Labs IoT Web Automation</h1>");
  client.println("<a href=\"/RELAY=ON\"><button>Turn ON</button></a>");
  client.println("<a href=\"/RELAY=OFF\"><button>Turn OFF</button></a></html>");
}
```
🔗 **[Click here to view the full industrial firmware source code](./firmware/zaritech_web_automation.ino)**

---

## 📥 Commercial Acquisition & Distribution
To purchase complete, factory-verified **Zaritech NodeMCU Web Automation Sets**, individual components, or to request customized firmware implementations, please contact our international distribution team through our official [Zaritech Labs Instagram](https://instagram.com).

---

## ⚖️ License & Governance
This infrastructure is maintained under the **MIT License** and governed under the engineering standards of **Zaritech Dynamics**.

*May Allah grant us success on this path.*
