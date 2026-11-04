#include <Arduino.h>

constexpr int LDR_PIN = 34;
constexpr int POT_PIN = 35;
constexpr uint32_t INTERVAL_US = 20000;

void capture(int pin, int count) {
  Serial.println("sample,t_ms,raw,baseline_mv,mean5,median5,ema");
  const uint32_t start = micros();
  int window[5] = {};
  float ema = 0;
  for (int i = 0; i < count; ++i) {
    const uint32_t target = start + i * INTERVAL_US;
    while (static_cast<int32_t>(micros() - target) < 0) {
      delayMicroseconds(100);
    }
    const uint32_t timestamp = micros() - start;
    const int raw = analogRead(pin);
    window[i % 5] = raw;
    ema = i == 0 ? raw : 0.2f * raw + 0.8f * ema;
    Serial.printf("%d,%.3f,%d,", i, timestamp / 1000.0, raw);
    if (pin == POT_PIN && count == 32) {
      // Separate conversion for the uncorrected ESP32 API baseline.
      Serial.print(analogReadMilliVolts(pin));
    }
    if (i < 4) {
      Serial.printf(",,,%.4f\n", ema);
      continue;
    }
    int sorted[5];
    memcpy(sorted, window, sizeof(sorted));
    int sum = 0;
    for (int j = 0; j < 5; ++j) {
      sum += sorted[j];
      for (int k = j; k > 0 && sorted[k] < sorted[k - 1]; --k) {
        const int tmp = sorted[k];
        sorted[k] = sorted[k - 1];
        sorted[k - 1] = tmp;
      }
    }
    Serial.printf(",%.2f,%d,%.4f\n", sum / 5.0, sorted[2], ema);
  }
  Serial.println("# END");
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetPinAttenuation(LDR_PIN, ADC_11db);
  analogSetPinAttenuation(POT_PIN, ADC_11db);
  Serial.println("# l/p: 32 LDR/pot samples; L/P: 500; S: 1000 pot samples");
}

void loop() {
  switch (Serial.read()) {
    case 'l': capture(LDR_PIN, 32); break;
    case 'p': capture(POT_PIN, 32); break;
    case 'L': capture(LDR_PIN, 500); break;
    case 'P': capture(POT_PIN, 500); break;
    case 'S': capture(POT_PIN, 1000); break;
  }
  delay(1);
}
