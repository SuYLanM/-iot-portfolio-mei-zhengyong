#include <Arduino.h>

constexpr uint8_t LED_PIN = 18, BUTTON_PIN = 27;
// Default: GPIO27 -- button -- GND, internal pull-up.
constexpr bool PULL_UP = true;
constexpr bool EXTERNAL_RESISTOR = false; // true for external 10k pull-up/down
constexpr uint32_t DEBOUNCE_MS = 30;
int rawLevel = HIGH, stableLevel = HIGH;
uint32_t changedAt = 0, lastReport = 0;
uint32_t rawEdges = 0, stableEdges = 0, presses = 0;
bool ledOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUTTON_PIN, EXTERNAL_RESISTOR ? INPUT : (PULL_UP ? INPUT_PULLUP : INPUT_PULLDOWN));
  rawLevel = stableLevel = digitalRead(BUTTON_PIN);
  changedAt = millis();
}

void loop() {
  const uint32_t now = millis();
  const int level = digitalRead(BUTTON_PIN);
  if (level != rawLevel) {
    rawLevel = level; ++rawEdges; changedAt = now;
  }
  // Accept only after a stable 30 ms interval, with no blocking delay.
  if (stableLevel != rawLevel && now - changedAt >= DEBOUNCE_MS) {
    stableLevel = rawLevel;
    ++stableEdges;
    if (stableLevel == (PULL_UP ? LOW : HIGH)) {
      ++presses; ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    }
  }
  if (now - lastReport >= 100) {
    lastReport = now;
    Serial.printf("raw:%d stable:%d led:%d raw_edges:%lu debounced_edges:%lu presses:%lu\n",
                  rawLevel, stableLevel, ledOn, (unsigned long)rawEdges,
                  (unsigned long)stableEdges, (unsigned long)presses);
  }
}
