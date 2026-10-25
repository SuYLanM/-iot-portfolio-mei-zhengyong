#include <Arduino.h>

constexpr uint8_t ADC_PIN = 34;
constexpr uint32_t SAMPLE_MS = 20;
constexpr bool PLOT_OUTPUT = true;
constexpr float ALPHA = 0.2f;
// Illustrative validity band: TODO justify for the actual signal.
constexpr int GATE_LOW = 205, GATE_HIGH = 3890;
int samples[20] = {}, gatedSamples[5] = {};
uint8_t used = 0, head = 0, gatedUsed = 0, gatedHead = 0;
uint32_t lastSample = 0, previousSample = 0;
bool injectNext = false, emaReady = false;
float ema = 0;

float meanRecent(uint8_t window) {
  const uint8_t count = used < window ? used : window;
  long sum = 0;
  for (uint8_t i = 0; i < count; ++i) sum += samples[(head + 19 - i) % 20];
  return count ? (float)sum / count : -1;
}

float medianRecent() {
  const uint8_t count = used < 5 ? used : 5;
  int sorted[5];
  for (uint8_t i = 0; i < count; ++i) sorted[i] = samples[(head + 19 - i) % 20];
  for (uint8_t i = 1; i < count; ++i) {
    int value = sorted[i];
    int j = i - 1;
    while (j >= 0 && sorted[j] > value) { sorted[j + 1] = sorted[j]; --j; }
    sorted[j + 1] = value;
  }
  if (!count) return -1;
  return count % 2 ? sorted[count / 2] : (sorted[count / 2 - 1] + sorted[count / 2]) / 2.0f;
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetPinAttenuation(ADC_PIN, ADC_11db);
  if (!PLOT_OUTPUT)
    Serial.println("t_ms,dt_ms,measured_raw,input_raw,synthetic,ma5,ma10,ma20,median5,gated_ma5,gate_accept,gated_valid,ema,full_window");
}

void loop() {
  // Optional SOFTWARE spike: always flagged synthetic; not hardware evidence.
  if (Serial.available() && Serial.read() == 's') injectNext = true;
  const uint32_t now = millis();
  if (now - lastSample < SAMPLE_MS) return;
  lastSample = now;
  const uint32_t dt = previousSample ? now - previousSample : 0;
  previousSample = now;
  const int measured = analogRead(ADC_PIN);
  const bool synthetic = injectNext;
  const int input = synthetic ? 4095 : measured;
  injectNext = false;
  samples[head] = input; head = (head + 1) % 20;
  if (used < 20) ++used;
  const bool accept = input >= GATE_LOW && input <= GATE_HIGH;
  if (accept) {
    gatedSamples[gatedHead] = input; gatedHead = (gatedHead + 1) % 5;
    if (gatedUsed < 5) ++gatedUsed;
  }
  long gatedSum = 0;
  for (uint8_t i = 0; i < gatedUsed; ++i) gatedSum += gatedSamples[i];
  const float gatedMean = gatedUsed ? (float)gatedSum / gatedUsed : -1;
  if (!emaReady) { ema = input; emaReady = true; }
  else ema += ALPHA * (input - ema);
  const float ma5 = meanRecent(5), ma10 = meanRecent(10), ma20 = meanRecent(20);
  const float med5 = medianRecent();
  if (PLOT_OUTPUT) {
    Serial.printf("raw:%d ma5:%.2f ma10:%.2f ma20:%.2f median5:%.2f gated_ma5:%.2f ema:%.2f synthetic:%d gate_accept:%d full_window:%d\n",
                  input, ma5, ma10, ma20, med5, gatedMean, ema, synthetic, accept, used == 20);
  } else {
    Serial.printf("%lu,%lu,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%.2f,%d\n",
                  (unsigned long)now, (unsigned long)dt, measured, input, synthetic,
                  ma5, ma10, ma20, med5, gatedMean, accept, gatedUsed > 0, ema, used == 20);
  }
}
