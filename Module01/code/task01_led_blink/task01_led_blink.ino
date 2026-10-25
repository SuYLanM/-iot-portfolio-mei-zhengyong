#include <Arduino.h>

// External LED. TODO: confirm series resistor and polarity before wiring.
constexpr uint8_t LED_PIN = 18;
constexpr uint32_t HALF_PERIOD_MS = 500;
uint32_t lastChange = 0;
bool ledOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  lastChange = millis();
}

void loop() {
  const uint32_t now = millis();
  if (now - lastChange >= HALF_PERIOD_MS) {
    lastChange = now;
    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    Serial.printf("t_ms:%lu led:%d\n", (unsigned long)now, ledOn);
  }
}
