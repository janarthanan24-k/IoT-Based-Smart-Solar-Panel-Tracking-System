// Blynk Template Info (MUST be placed before any includes)
#define BLYNK_TEMPLATE_ID "xxxxx"
#define BLYNK_TEMPLATE_NAME "Name "
#define BLYNK_AUTH_TOKEN "xxxx"

// Libraries
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// Blynk and Wi-Fi Credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WIFI";      // Your mobile hotspot or router SSID
char pass[] = "password";          // Wi-Fi password

// Pin Definitions
#define LDR1_PIN 34           // LDR1 connected to GPIO 34
#define LDR2_PIN 35           // LDR2 connected to GPIO 35
#define LM35_PIN 32           // LM35 temperature sensor on GPIO 32
#define VOLTAGE_SENSOR_PIN 33 // Voltage sensor on GPIO 33
#define SERVO_PIN 13          // Servo motor on GPIO 13

// Servo Object
Servo solarServo;

// Variables
int ldr1Value = 0;
int ldr2Value = 0;
float temperature = 0.0;
float voltage = 0.0;
int servoAngle = 90;            // Start in middle position
const int ldrThreshold = 50;    // Difference needed to move
const int angleStep = 5;        // How much to move each time
String weatherCondition = "";   // For combined weather report

// Blynk Timer
BlynkTimer timer;

// Function to connect to Wi-Fi
void connectWiFi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, pass);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected!");
    Serial.println("IP Address: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nFailed to connect to Wi-Fi. Retrying...");
  }
}

void setup() {
  Serial.begin(115200);

  // Attach servo and center it
  solarServo.attach(SERVO_PIN);
  solarServo.write(servoAngle);

  // Connect to Wi-Fi and Blynk
  connectWiFi();
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.config(auth);
    Blynk.connect();
  } else {
    Serial.println("Wi-Fi connection failed. Blynk will not connect.");
  }

  // Send sensor data to Blynk every 2 seconds
  timer.setInterval(2000L, sendDataToBlynk);
  
  // Check Wi-Fi and Blynk connection every 10 seconds
  timer.setInterval(10000L, checkConnections);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && Blynk.connected()) {
    Blynk.run();
  }
  timer.run();
  trackSun();
}

// Function to check Wi-Fi and Blynk connection
void checkConnections() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected! Reconnecting...");
    connectWiFi();
  }
  if (WiFi.status() == WL_CONNECTED && !Blynk.connected()) {
    Serial.println("Blynk disconnected! Reconnecting...");
    Blynk.connect();
  }
}

void trackSun() {
  ldr1Value = analogRead(LDR1_PIN);
  ldr2Value = analogRead(LDR2_PIN);

  int ldrDiff = ldr1Value - ldr2Value;

  if (ldrDiff > ldrThreshold && servoAngle < 180) {
    servoAngle += angleStep;
    if (servoAngle > 180) servoAngle = 180;
    solarServo.write(servoAngle);
    Serial.println("Moving toward LDR1, Angle: " + String(servoAngle));
  } else if (ldrDiff < -ldrThreshold && servoAngle > 0) {
    servoAngle -= angleStep;
    if (servoAngle < 0) servoAngle = 0;
    solarServo.write(servoAngle);
    Serial.println("Moving toward LDR2, Angle: " + String(servoAngle));
  }
  delay(100); // Stabilization delay
}

void sendDataToBlynk() {
  // Only send data if connected to Blynk
  if (!Blynk.connected()) {
    Serial.println("Not connected to Blynk. Skipping data send.");
    return;
  }

  // Read LDRs
  ldr1Value = analogRead(LDR1_PIN);
  ldr2Value = analogRead(LDR2_PIN);

  // Read and convert LM35 temperature
  int lm35Raw = analogRead(LM35_PIN);
  float lm35Voltage = (lm35Raw / 4095.0) * 3.3;
  temperature = lm35Voltage * 100.0;

  // Read and convert voltage sensor
  int voltageRaw = analogRead(VOLTAGE_SENSOR_PIN);
  voltage = (voltageRaw / 4095.0) * 25.0; // Adjust if using a different sensor

  // Calculate light and temperature condition
  int ldrAverage = (ldr1Value + ldr2Value) / 2;
  String lightCondition = (ldrAverage > 2000) ? "Sunny" :
                          (ldrAverage >= 500) ? "Cloudy" : "Dark";
  String tempCondition = (temperature > 30.0) ? "Hot" :
                         (temperature >= 15.0) ? "Mild" : "Cool";
  weatherCondition = lightCondition + " and " + tempCondition;

  // Send to Blynk virtual pins
  Blynk.virtualWrite(V0, ldr1Value);
  Blynk.virtualWrite(V1, ldr2Value);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, voltage);
  Blynk.virtualWrite(V4, servoAngle);
  Blynk.virtualWrite(V5, weatherCondition);

  // Debug
  Serial.println("LDR1: " + String(ldr1Value) + ", LDR2: " + String(ldr2Value));
  Serial.println("Temperature: " + String(temperature) + " °C");
  Serial.println("Voltage: " + String(voltage) + " V");
  Serial.println("Servo Angle: " + String(servoAngle));
  Serial.println("Weather: " + weatherCondition);
}