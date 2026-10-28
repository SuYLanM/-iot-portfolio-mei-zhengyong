#include <Arduino.h>

// Classic ESP32 only: GPIO35 = ADC1_CH7, input-only.
// 3V3 -> R1 10k -> Vout -> R2 10k -> GND; GPIO35 connects to Vout.
// Prepared baseline; neither upload nor hardware validation is implied.
constexpr int ADC_PIN = 35;
constexpr int NUM_SAMPLES = 32;
constexpr uint32_t PRINT_INTERVAL_MS = 1000;
uint32_t lastRoundStart = 0;
bool firstRound = true;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);

  Serial.println("Module 2 - Task 1: Voltage Divider ADC Test");
  Serial.println("Theory only: 3.3 V / (10k + 10k) gives approximately 1.650 V.");
  Serial.println("Hardware validation pending. API voltage is an estimate, not a DMM measurement.");
}

void loop() {
  const uint32_t now = millis();
  // Approximately one second between burst starts, including acquisition time.
  if (!firstRound && static_cast<uint32_t>(now - lastRoundStart) < PRINT_INTERVAL_MS) {
    delay(1);
    return;
  }
  firstRound = false;
  lastRoundStart = now;

  uint32_t rawSum = 0;
  uint32_t mvSum = 0;
  for (int i = 0; i < NUM_SAMPLES; ++i) {
    // These calls perform separate conversions: 32 raw and 32 API-mV samples.
    rawSum += analogRead(ADC_PIN);
    mvSum += analogReadMilliVolts(ADC_PIN);
    delay(5);
  }

  const float rawAverage = rawSum / static_cast<float>(NUM_SAMPLES);
  const float voltageEstimate = mvSum / static_cast<float>(NUM_SAMPLES) / 1000.0f;
  Serial.printf("Raw mean: %.3f | API voltage estimate: %.3f V\n",
                rawAverage, voltageEstimate);

  if (rawAverage < 5.0f) {
    Serial.println("WARNING: Input near zero. Check wiring, ground and input voltage.");
  } else if (rawAverage > 4090.0f) {
    Serial.println("WARNING: ADC near full scale. Check wiring and input voltage; stop if overrange.");
  }
}
