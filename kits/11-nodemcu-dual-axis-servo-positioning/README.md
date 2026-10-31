# Zaritech Dual-Axis Servo Actuation & Kinematic Positioning Framework 🌐🦾

Welcome to the official repository for the **Zaritech Dual-Axis Servo Actuation Hub** — a high-efficiency wireless cinematic and positioning infrastructure engineered by **Zaritech Labs** (A division of **Zaritech Dynamics**).

### ⚡ The Problem: High-Current Back-EMF Voltages & Single-Axis Limits
Standard automated robotic joints and positioning sensors (such as Pan-Tilt tracking cameras or mechanical barriers) suffer from low degrees of freedom when confined to a single axis. Furthermore, driving multiple inductive servo actuators directly from a microcontroller's logic power rail introduces massive voltage drops, reset loops, and back-EMF feedback noise that can permanently degrade processing cores.

### 🛡️ The Zaritech Solution: Decoupled Multi-Axis Power & Dual PWM Tracking
The **Zaritech Dual-Axis Positioning Platform** systematically isolates these risks. By engineering a split power routing topology using a high-capacity **18650 Energy Substrate**, the inductive load of two independent **SG90 Micro Servo Motors** is entirely decoupled from the NodeMCU processing core. Operating via dual synchronized internal hardware PWM channels over Wi-Fi networks, the subsystem delivers flawless 2-axis (X-Y / Pan-Tilt) mechanical manipulation without logic-level degradation.

---

## 🛠️ Hardware & Topology Specs
Our mission-critical kinematic manipulation framework is engineered for simultaneous multi-axis steering, clean power distribution, and grid-independent operation:

*   **Core Processing Unit:** NodeMCU V3 ESP8266 high-frequency wireless microcontroller managing dual independent hardware-timed PWM duty cycles.
*   **Kinematic Actuation Array:** 2x SG90 Micro Servo Motors delivering 0-to-180 degree mechanical physical block, panning, and tilt motions with included structural control horns.
*   **Decoupled Power Architecture:** 1x Factory-Verified High-Capacity 18650 Lithium-Ion Battery Node with a secure single cell dock supplying dedicated high-current output to the servo rail.
*   **Interconnect Matrix:** 10-piece premium high-durability jumper wire array optimized to guarantee clean logic routing and avoid line resistance degradation.

---

## 🗺️ Visual Architecture & Pin Topology
The hardware signal pathways, decoupled power rails, and input-output registers for this robotic framework are detailed below:

### Central Interconnection Matrix

| Sub-System Node | Component Pin Name | Target Destination | Operational Logic Context |
| :--- | :--- | :--- | :--- |
| **Servo 1 (Axis-X / Pan)** | PWM Signal Line | NodeMCU D1 (GPIO5) | Primary Angular Actuation Pulse |
| | VCC (Power Input) | 18650 Battery (+) Rail | Decoupled High-Current Current Path |
| | GND (Ground) | Shared System Ground | Common Ground Reference Plane |
| **Servo 2 (Axis-Y / Tilt)**| PWM Signal Line | NodeMCU D2 (GPIO4) | Secondary Angular Actuation Pulse |
| | VCC (Power Input) | 18650 Battery (+) Rail | Decoupled High-Current Current Path |
| **NodeMCU Core** | GND Pin | Shared System Ground | Common Ground Reference Plane |

### Industrial Firmware Implementation
The full, modular multi-layered firmware solution executing debounced network packet listeners, dual-axis pulse position hold routines, and responsive HTML remote manipulation panels is safely isolated within our production codebase.

🔗 **[Click here to view the full operational source code](./firmware/zaritech_dual_servo_positioning.ino)**

---

## 📸 Hardware Layout
![Zaritech Dual Servo Layout](schematics/hardware_layout.png)

---

## 📥 Commercial Acquisition & Distribution
To purchase factory-certified **Zaritech Dual-Axis Servo Actuation Hubs**, multi-axis pan-tilt brackets, or to license custom smart tracking software templates, please coordinate with our logistics network via our official [Zaritech Labs Instagram](https://instagram.com).

---

## ⚖️ License & Corporate Governance
This infrastructure is maintained under the **MIT License** and governed under the rigorous engineering frameworks of **Zaritech Dynamics**.

*May Allah grant us success on this path.*
