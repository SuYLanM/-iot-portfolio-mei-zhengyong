#include <Arduino.h>

constexpr int ADC_PIN = 34;

void setup() {
  Serial.begin(115200);
  pinMode(ADC_PIN, INPUT);
  analogReadResolution(12);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);
  Serial.println("# GPIO34 ADC1; 12 bit; 11 dB; send d for 30 readings");
}

void loop() {
  if (Serial.read() != 'd') {
    delay(1);
    return;
  }
  Serial.println("sample,t_ms,raw,api_mv");
  const uint32_t started = millis();
  for (int i = 0; i < 30; ++i) {
    const uint32_t elapsed = millis() - started;
    const int raw = analogRead(ADC_PIN);
    const uint32_t mv = analogReadMilliVolts(ADC_PIN);
    Serial.printf("%d,%lu,%d,%lu\n", i, static_cast<unsigned long>(elapsed), raw,
                  static_cast<unsigned long>(mv));
    delay(100);
  }
  Serial.println("# END");
}
