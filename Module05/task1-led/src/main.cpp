#include <Arduino.h>

// Task 1 — one LED, PWM fade on GPIO 16
const int LED_PIN = 13; // for the lazy ones, you can also use 2 for the inverted blue onboard led.

void setup() {
  Serial.begin(115200);
  Serial.print("Task 1: PWM fade on GPIO ");
  Serial.println(LED_PIN);
}

void loop() {
  for (int duty = 0; duty <= 255; duty++) {
    analogWrite(LED_PIN, duty);
    delay(8);
  }
  for (int duty = 255; duty >= 0; duty--) {
    analogWrite(LED_PIN, duty);
    delay(8);
  }
}
