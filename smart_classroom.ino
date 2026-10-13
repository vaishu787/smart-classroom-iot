/*
  Project: Smart Classroom Monitoring and Automatic Lighting System
  Board: Arduino Uno

  Connections:
  PIR sensor OUT -> D2
  PIR sensor VCC -> 5V
  PIR sensor GND -> GND

  LDR voltage divider:
  5V -> LDR -> A0 junction -> 10k ohm resistor -> GND

  LED:
  D8 -> 220 ohm resistor -> LED anode (long leg)
  LED cathode (short leg) -> GND

  Notes:
  - Adjust DARK_THRESHOLD using values in the Serial Monitor.
  - PIR sensors may need 30-60 seconds to stabilize after power-up.
  - This sketch performs local Arduino automation and USB Serial monitoring;
    Wi-Fi/cloud connectivity requires additional hardware.
*/

const int PIR_PIN = 2;
const int LDR_PIN = A0;
const int LED_PIN = 8;

const int DARK_THRESHOLD = 400;  // Calibrate for your room and LDR circuit
const unsigned long NO_MOTION_TIMEOUT = 30000UL;  // 30 seconds

unsigned long lastMotionTime = 0;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Smart Classroom System Started");
  Serial.println("Allow the PIR sensor to stabilize.");
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);
  int lightLevel = analogRead(LDR_PIN);

  if (motionDetected == HIGH) {
    lastMotionTime = millis();
  }

  // Keep occupancy active briefly after the last detected movement.
  bool personPresent =
      (motionDetected == HIGH) ||
      (millis() - lastMotionTime < NO_MOTION_TIMEOUT);

  // With the stated LDR divider orientation, lower readings usually mean darker.
  bool roomIsDark = (lightLevel < DARK_THRESHOLD);

  bool shouldTurnOnLight = personPresent && roomIsDark;
  digitalWrite(LED_PIN, shouldTurnOnLight ? HIGH : LOW);

  Serial.print("PIR: ");
  Serial.print(motionDetected);
  Serial.print(" | LDR: ");
  Serial.print(lightLevel);
  Serial.print(" | Occupancy: ");
  Serial.print(personPresent ? "DETECTED" : "NOT DETECTED");
  Serial.print(" | Room: ");
  Serial.print(roomIsDark ? "DARK" : "BRIGHT");
  Serial.print(" | Light: ");
  Serial.println(shouldTurnOnLight ? "ON" : "OFF");

  delay(500);
}
