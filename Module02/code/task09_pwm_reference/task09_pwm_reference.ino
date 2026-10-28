#include <Arduino.h>

// Use a separate target board, not the board running analyzer firmware.
constexpr uint8_t PWM_PIN = 25;
constexpr uint32_t TARGET_HZ = 1000;
constexpr uint8_t RESOLUTION_BITS = 8;
constexpr uint32_t DUTY_CODE = 128;  // 128 / 256 = requested 50% duty.
bool ready = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Task 9 TARGET board only; separate ESP32 runs analyzer firmware.");
  const bool ok = ledcAttach(PWM_PIN, TARGET_HZ, RESOLUTION_BITS);
  if (!ok) { Serial.println("FAIL: LEDC attach; no valid PWM configuration claimed."); return; }
  ready = ledcWrite(PWM_PIN, DUTY_CODE);
  Serial.printf("GPIO=%u target_hz=%lu duty_code=%lu/256 target_duty=50%% setup_ok=%u\n",
                PWM_PIN, (unsigned long)TARGET_HZ, (unsigned long)DUTY_CODE, ready);
  Serial.println("Configured parameters only. Actual frequency/duty and capture remain PENDING.");
}

void loop() {
  static uint32_t lastReport = 0;
  if (millis() - lastReport >= 3000) {
    lastReport = millis();
    Serial.println(ready ? "PWM configured on target GPIO25; measure with analyzer."
                         : "PWM configuration failed; inspect board/core and reset.");
  }
  delay(10);
}
