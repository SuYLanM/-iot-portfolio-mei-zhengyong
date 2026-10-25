#include <Arduino.h>

constexpr uint8_t ADC_PIN = 34;
constexpr uint8_t ADC_BITS = 12;
constexpr uint32_t SAMPLE_MS = 100;
constexpr bool PLOT_OUTPUT = false;
// TODO: measure actual supply and resistor before reporting resistance.
constexpr float SUPPLY_V = 3.3f;
constexpr float FIXED_RESISTOR_OHM = 10000.0f;
constexpr float ASSUMED_FULL_SCALE_V = 3.3f;
uint32_t lastSample = 0;

void setup() {
  Serial.begin(115200);
  analogReadResolution(ADC_BITS);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);
  if (!PLOT_OUTPUT) Serial.println("t_ms,raw,ideal_V,api_mV,R_est_ohm,R_valid");
}

void loop() {
  const uint32_t now = millis();
  if (now - lastSample < SAMPLE_MS) return;
  lastSample = now;
  const uint16_t raw = analogRead(ADC_PIN);
  const uint32_t apiMv = analogReadMilliVolts(ADC_PIN); // second conversion
  const float idealV = raw * ASSUMED_FULL_SCALE_V / (1U << ADC_BITS);
  const float v = apiMv / 1000.0f;
  // Conservative working band, NOT a guarantee of accuracy.
  const bool valid = raw > 0 && raw < 4095 && v >= 0.15f && v <= 2.9f && v < SUPPLY_V;
  const float resistance = valid ? FIXED_RESISTOR_OHM * (SUPPLY_V - v) / v : -1.0f;
  if (PLOT_OUTPUT) {
    Serial.printf("raw:%u ideal_mV:%.2f api_mV:%lu R_valid:%d\n", raw,
                  idealV * 1000, (unsigned long)apiMv, valid);
  } else {
    Serial.printf("%lu,%u,%.6f,%lu,%.2f,%d\n", (unsigned long)now, raw, idealV,
                  (unsigned long)apiMv, resistance, valid);
  }
}
