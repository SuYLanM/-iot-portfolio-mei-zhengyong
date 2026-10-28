#include <Arduino.h>

#if !defined(ESP8266)
#error "Select an ESP8266 LOLIN(WEMOS) D1 R2 & mini board, not ESP32."
#endif

constexpr uint8_t PWM_PIN = D1;  // WeMos D1 mini: D1 = GPIO5.
constexpr uint32_t PWM_HZ = 1000;
constexpr uint16_t PWM_RANGE = 1023;
constexpr uint16_t PWM_DUTY = 512;

void setup() {
  Serial.begin(115200);
  pinMode(PWM_PIN, OUTPUT);
  analogWriteRange(PWM_RANGE);
  analogWriteFreq(PWM_HZ);
  analogWrite(PWM_PIN, PWM_DUTY);
  Serial.println();
  Serial.println("Task 9 PWM source: WeMos D1 mini (ESP8266)");
  Serial.printf("Pin=D1 GPIO%u | target_frequency=%lu Hz | range=0-%u | duty_code=%u (about 50%%)\n",
                PWM_PIN, static_cast<unsigned long>(PWM_HZ), PWM_RANGE, PWM_DUTY);
  Serial.println("Configured values only; measure period and duty with the separate ESP32 analyzer.");
}

void loop() {
  delay(1000);  // Yield to ESP8266 background tasks; PWM runs independently.
}
