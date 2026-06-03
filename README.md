# ☀️ IoT-Based Smart Solar Panel Tracking System

![Platform](https://img.shields.io/badge/Platform-ESP32-blue.svg)
![Framework](https://img.shields.io/badge/Framework-Arduino%20IDE-00979D.svg)
![IoT](https://img.shields.io/badge/IoT-Blynk-11BF71.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

## 📖 Introduction

Static solar panels only operate at peak efficiency for a small window of the day. The **IoT-Based Smart Solar Panel Tracking System** is an automated, single-axis solar tracking solution designed to solve this problem. By utilizing light-dependent resistors (LDRs) and an ESP32 microcontroller, the system actively orientates the solar panel toward the most intense light source, maximizing energy harvesting throughout the day. 

Beyond mechanical tracking, this project acts as a complete smart-energy node. It features real-time environmental and power monitoring pushed directly to a custom **Blynk IoT Dashboard**, allowing users to track voltage output, ambient temperature, and weather conditions from anywhere in the world.

---

## ✨ Key Features

* **Dynamic Light Tracking:** Utilizes a dual-LDR sensory feedback loop to calculate light differentials and precisely adjust the panel via a servo motor.
* **Real-Time IoT Telemetry:** Live streaming of system metrics (voltage, temperature, servo angle, and LDR readings) to the Blynk mobile/web dashboard.
* **Smart Environmental Logic:** Processes sensor data to output human-readable weather states (e.g., *"Sunny and Hot"*, *"Cloudy and Mild"*).
* **Robust Connectivity:** Built-in fail-safes and auto-reconnect logic ensure the system recovers gracefully from Wi-Fi drops or cloud service interruptions.
* **Low Power Footprint:** Optimized sensor reading intervals prevent unnecessary polling and save energy.

---

## 🏗️ System Architecture

### Hardware Requirements
* **Microcontroller:** ESP32 (NodeMCU or similar)
* **Actuator:** MG995 (High Torque) or SG90 (Mini) Servo Motor
* **Sensors:** 
  * 2 × Light Dependent Resistors (LDR)
  * 1 × LM35 Analog Temperature Sensor
  * 1 × Voltage Sensor Module (0-25V range)
* **Power Source:** Mini Solar Panel (Size scaled to match your voltage divider and servo capacity)
* **Miscellaneous:** Breadboard, jumper wires, 10kΩ resistors (for LDR voltage dividers).

### Software & Libraries
Compiled using the **Arduino IDE**. The following libraries must be installed:
* `WiFi.h` (Standard ESP32 package)
* `BlynkSimpleEsp32.h` (IoT communication)
* `ESP32Servo.h` (Hardware PWM control for ESP32)

---

## 🔌 Pin Configuration

| Component | ESP32 Pin | Type | Function |
| :--- | :--- | :--- | :--- |
| **LDR 1 (Left/Top)** | `GPIO 34` | Analog In | Measures light intensity on side A |
| **LDR 2 (Right/Bottom)**| `GPIO 35` | Analog In | Measures light intensity on side B |
| **LM35 Sensor** | `GPIO 32` | Analog In | Monitors ambient system temperature |
| **Voltage Sensor** | `GPIO 33` | Analog In | Tracks raw power output from the panel |
| **Servo Motor (PWM)** | `GPIO 13` | PWM Out | Drives the single-axis tracking mechanism |

> **Note:** Ensure your ESP32 is powered adequately, especially if using a high-torque servo like the MG995, as current spikes during motor movement can cause the ESP32 to reset if powered only via USB.

---

## 🚀 Installation & Setup

### 1. Blynk Cloud Configuration
1. Log in to the [Blynk Web Console](https://blynk.cloud/).
2. Create a new **Template** (Hardware: ESP32, Connection: WiFi).
3. Navigate to **Datastreams** and set up the following Virtual Pins:
   * `V0`, `V1` -> Integer (LDR Raw Values)
   * `V2` -> Double (Temperature in °C)
   * `V3` -> Double (Voltage Output in V)
   * `V4` -> Integer (Current Servo Angle)
   * `V5` -> String (Weather Condition Status)
4. Copy the `BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME`, and `BLYNK_AUTH_TOKEN`.

### 2. Microcontroller Setup
1. Clone this repository to your local machine:
```bash
   git clone [https://github.com/yourusername/IoT-Smart-Solar-Tracker.git](https://github.com/yourusername/IoT-Smart-Solar-Tracker.git)
