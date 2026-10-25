#include <Arduino.h>

constexpr uint8_t ADC_PIN = 34;
constexpr uint8_t ADC_BITS = 12;;; // Repeat with 10, upload, reset and sweep again.
static_assert(ADC_BITS >= 9 && ADC_BITS <= 12, "Classic ESP32 comparison: 9..12 bits");
constexpr bool PLOT_OUTPUT = false;
constexpr float ASSUMED_FULL_SCALE_V = 3.3f; // Classroom model, NOT internal Vref.
constexpr uint32_t SAMPLE_MS = 50;
bool seen[1U << ADC_BITS] = {};
uint16_t minCode = (1U << ADC_BITS) - 1, maxCode = 0, distinct = 0;
uint32_t lastSample = 0;

void setup() {
  Serial.begin(115200);
  analogReadResolution(ADC_BITS);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);
  if (!PLOT_OUTPUT)
    Serial.println("t_ms,bits,raw,ideal_V,api_mV,min_raw,max_raw,distinct_observed");
}

void loop() {
  const uint32_t now = millis();
  if (now - lastSample < SAMPLE_MS) return;
  lastSample = now;
  const uint16_t raw = analogRead(ADC_PIN);
  // SECOND conversion: hold the potentiometer still for comparison.
  const uint32_t apiMv = analogReadMilliVolts(ADC_PIN);
  if (!seen[raw]) { seen[raw] = true; ++distinct; }
  if (raw < minCode) minCode = raw;
  if (raw > maxCode) maxCode = raw;
  const float idealV = raw * ASSUMED_FULL_SCALE_V / (1U << ADC_BITS);
  if (PLOT_OUTPUT) {
    Serial.printf("raw:%u ideal_mV:%.2f api_mV:%lu\n", raw, idealV * 1000,
                  (unsigned long)apiMv);
  } else {
    Serial.printf("%lu,%u,%u,%.6f,%lu,%u,%u,%u\n", (unsigned long)now, ADC_BITS,
                  raw, idealV, (unsigned long)apiMv, minCode, maxCode, distinct);
  }
}
