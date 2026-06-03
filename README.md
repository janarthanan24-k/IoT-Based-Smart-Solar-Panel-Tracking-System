# ☀️ IoT-Based Smart Solar Panel Tracking System


## 📖 Introduction

Static solar panels only operate at peak efficiency for a small window of the day. The **IoT-Based Smart Solar Panel Tracking System** is an automated, single-axis solar tracking solution designed to solve this problem. By utilizing light-dependent resistors (LDRs) and an ESP32 microcontroller, the system actively orientates the solar panel toward the most intense light source, maximizing energy harvesting throughout the day. 

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

###  Blynk Cloud Configuration
1. Log in to the [Blynk Web Console](https://blynk.cloud/).
2. Create a new **Template** (Hardware: ESP32, Connection: WiFi).
3. Navigate to **Datastreams** and set up the following Virtual Pins:
   * `V0`, `V1` -> Integer (LDR Raw Values)
   * `V2` -> Double (Temperature in °C)
   * `V3` -> Double (Voltage Output in V)
   * `V4` -> Integer (Current Servo Angle)
   * `V5` -> String (Weather Condition Status)
4. Copy the `BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME`, and `BLYNK_AUTH_TOKEN`.


## 🧠 System Workflow (Logic Flow)

The solar tracking system operates on a continuous, closed-loop evaluation cycle to ensure optimal panel orientation and real-time monitoring:

1. **Data Acquisition:** The ESP32 continuously polls the analog pins to read the voltage drops across the two Light Dependent Resistors (LDRs), alongside raw data from the LM35 temperature sensor and the voltage module.
2. **Differential Analysis:** The microcontroller calculates the absolute difference in light intensity (`ldrDiff`) between the left and right sensors.
3. **Smart Actuation:** * If the `ldrDiff` exceeds the predefined sensitivity threshold, it indicates an imbalance in sunlight. The ESP32 sends a PWM signal to the servo motor, stepping it toward the brighter LDR.
   * If the light is balanced (difference is below the threshold), the motor holds its current position to conserve power and prevent mechanical jitter.
4. **Environmental Evaluation:** Internal logic translates the raw temperature and voltage data into human-readable weather conditions (e.g., determining if it is "Sunny and Hot" or "Cloudy").
5. **IoT Synchronization:** Every 2000 milliseconds, a non-blocking timer triggers a data push, sending the updated sensor arrays, servo angle, and weather status to the Blynk Cloud to update the live dashboard.

---

## 🔮 Future Scope & Upgrades

This project serves as a strong foundation for smart energy harvesting. Planned future enhancements include:

- [ ] **Dual-Axis Tracking Mechanism:** Upgrading the mechanical frame with an additional servo and two more LDRs to track the sun's elevation (up/down) as well as its azimuth (left/right).
- [ ] **MPPT (Maximum Power Point Tracking):** Integrating an MPPT algorithmic charge controller to mathematically extract the absolute maximum electrical power from the panel under varying conditions.
- [ ] **Battery Management System (BMS):** Adding a safe charging circuit for Li-ion or Lead-Acid batteries to store the harvested energy for nighttime use.
- [ ] **Advanced Data Analytics:** Implementing local SD card logging or exporting Blynk cloud data to a CSV for long-term seasonal efficiency analysis.
- [ ] **Automated Alerts:** Setting up push notifications or email alerts via Blynk to warn users of high-temperature hardware faults or extreme weather events.
