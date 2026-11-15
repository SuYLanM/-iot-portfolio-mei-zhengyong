#include <Arduino.h>

constexpr uint8_t AO_PIN = 34;
constexpr uint8_t DO_PIN = 18;
constexpr uint32_t SAMPLE_INTERVAL_MS = 500;
constexpr int UNRECORDED = -1;

int dryLevel = UNRECORDED;
int wetLevel = UNRECORDED;
uint32_t lastSampleMs = 0;

void handleCommand(char command) {
  switch (command) {
    case 'd':
      dryLevel = digitalRead(DO_PIN);
      Serial.printf("# Dry reference DO=%d\n", dryLevel);
      break;
    case 'w':
      wetLevel = digitalRead(DO_PIN);
      Serial.printf("# Wet reference DO=%d\n", wetLevel);
      break;
    case 'r':
      dryLevel = UNRECORDED;
      wetLevel = UNRECORDED;
      Serial.println("# Calibration cleared");
      return;
    default:
      return;
  }

  if (dryLevel == UNRECORDED || wetLevel == UNRECORDED) {
    Serial.println("# Record both references to enable DRY/WET labels");
  } else if (dryLevel == wetLevel) {
    Serial.println("# Same DO in both references. Check the comparator threshold and recalibrate.");
  } else {
    Serial.printf("# Calibrated: DRY=%d, WET=%d\n", dryLevel, wetLevel);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(AO_PIN, INPUT);
  pinMode(DO_PIN, INPUT);
  analogReadResolution(12);
  analogSetPinAttenuation(AO_PIN, ADC_11db);

  Serial.println("# Module 4 Task 1 - Conductive Water Level Detection");
  Serial.println("# Commands: d = record dry, w = record wet, r = clear calibration");
  Serial.println("time_ms,AO,DO,state");
}

void loop() {
  if (Serial.available() > 0) {
    handleCommand(static_cast<char>(Serial.read()));
  }

  const uint32_t now = millis();
  if (now - lastSampleMs < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleMs = now;

  const int ao = analogRead(AO_PIN);
  const int digitalLevel = digitalRead(DO_PIN);
  const char* state = "UNCALIBRATED";
  if (dryLevel != UNRECORDED && wetLevel != UNRECORDED && dryLevel != wetLevel) {
    state = digitalLevel == wetLevel ? "WET" : "DRY";
  }
  Serial.printf("%lu,%d,%d,%s\n", static_cast<unsigned long>(now), ao, digitalLevel, state);
}
