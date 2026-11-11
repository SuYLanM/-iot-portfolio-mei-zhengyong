#include <Arduino.h>

constexpr uint8_t ADC_PIN = 34;
constexpr uint32_t SAMPLE_INTERVAL_MS = 500;
uint32_t lastSampleMs = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ADC_PIN, INPUT);
  analogReadResolution(12);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);

  Serial.println("# Module 4 Task 2 - Capacitive Liquid Level Measurement");
  Serial.println("time_ms,adc_raw");
}

void loop() {
  const uint32_t now = millis();
  if (now - lastSampleMs < SAMPLE_INTERVAL_MS) {
    delay(1);
    return;
  }
  lastSampleMs = now;

  const int raw = analogRead(ADC_PIN);
  Serial.printf("%lu,%d\n", static_cast<unsigned long>(now), raw);
}
