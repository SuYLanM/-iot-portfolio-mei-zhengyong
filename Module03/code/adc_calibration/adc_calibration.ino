#include <Arduino.h>

constexpr int ADC_PIN = 34;
constexpr int SAMPLES = 150;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);
  Serial.println("# Send t for one calibration or validation point (GPIO34).");
}

void loop() {
  if (Serial.read() != 't') {
    delay(1);
    return;
  }
  Serial.println("sample,t_ms,raw,baseline_mv");
  const uint32_t start = millis();
  for (int i = 0; i < SAMPLES; ++i) {
    const uint32_t elapsed = millis() - start;
    const int code = analogRead(ADC_PIN);
    const uint32_t mv = analogReadMilliVolts(ADC_PIN);
    Serial.printf("%d,%lu,%d,%lu\n", i, static_cast<unsigned long>(elapsed), code,
                  static_cast<unsigned long>(mv));
    delay(20);
  }
  Serial.println("# END");
}
