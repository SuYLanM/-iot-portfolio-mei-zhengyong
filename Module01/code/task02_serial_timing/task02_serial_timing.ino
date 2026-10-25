#include <Arduino.h>

constexpr uint8_t LED_PIN = 18, BUTTON_PIN = 27;
// Repeat all four combinations. No commands needed in Serial Plotter.
constexpr bool PRINT_OUTPUT = true;
constexpr bool BLOCKING_DELAY = false;
constexpr uint32_t PERIOD_MS = 200;
uint32_t previousLoopUs = 0, reportAt = 0, ledAt = 0;
uint32_t count = 0, maximumLoopUs = 0;
uint64_t totalLoopUs = 0;
double lastMeanUs = 0;
uint32_t lastMaxUs = 0;
uint32_t rawEdges = 0;
int previousButton = HIGH;
bool ledOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  previousButton = digitalRead(BUTTON_PIN);
  previousLoopUs = micros();
  reportAt = ledAt = millis();
}

void loop() {
  const uint32_t currentUs = micros();
  const uint32_t intervalUs = currentUs - previousLoopUs;
  previousLoopUs = currentUs;
  totalLoopUs += intervalUs; ++count;
  if (intervalUs > maximumLoopUs) maximumLoopUs = intervalUs;
  const int button = digitalRead(BUTTON_PIN);
  if (button != previousButton) { ++rawEdges; previousButton = button; }
  const uint32_t now = millis();
  if (now - ledAt >= PERIOD_MS) {
    ledAt = now; ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  }
  if (now - reportAt >= PERIOD_MS) {
    reportAt = now;
    lastMeanUs = (double)totalLoopUs / count;
    lastMaxUs = maximumLoopUs;
    if (PRINT_OUTPUT) {
      Serial.printf("button:%d led:%d mean_loop_us:%.2f max_loop_us:%lu raw_edges:%lu blocking:%d\n",
                    button, ledOn, (double)totalLoopUs / count,
                    (unsigned long)maximumLoopUs, (unsigned long)rawEdges, BLOCKING_DELAY);
    }
    totalLoopUs = 0; count = 0; maximumLoopUs = 0;
  }
  // With periodic printing disabled, request the LAST completed window once.
  // The query's own overhead affects the following window: discard that window.
  if (!PRINT_OUTPUT && Serial.available() && Serial.read() == 'r') {
    Serial.printf("mean_loop_us:%.2f max_loop_us:%lu raw_edges:%lu blocking:%d\n",
                  lastMeanUs, (unsigned long)lastMaxUs, (unsigned long)rawEdges, BLOCKING_DELAY);
  }
  if (BLOCKING_DELAY) delay(PERIOD_MS);
}
