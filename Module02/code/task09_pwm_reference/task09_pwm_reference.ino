#include <Arduino.h>

// Use a separate target board, not the board running analyzer firmware.
constexpr uint8_t PWM_PIN = 25;
constexpr uint32_t TARGET_HZ = 1000;

void setup() {
  Serial.begin(115200);
  const bool ok = ledcAttach(PWM_PIN, TARGET_HZ, 8);
  if (!ok) { Serial.println("LEDC attach failed"); return; }
  const bool dutyOk = ledcWrite(PWM_PIN, 128);
  Serial.printf("configured_target_hz=%lu duty_code=128/256 setup_status=%d; measured rate PENDING\n",
                (unsigned long)TARGET_HZ, dutyOk);
}

void loop() { delay(100); }
