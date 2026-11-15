const int WATER_SENSOR_PIN = 27;
const unsigned long DEBOUNCE_MS = 300;

int lastRawState;
int stableState;
unsigned long lastChangeTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(WATER_SENSOR_PIN, INPUT);

  lastRawState = digitalRead(WATER_SENSOR_PIN);
  stableState = lastRawState;
  lastChangeTime = millis();

  Serial.println("HD-38 Water Sensor - Debounce Test");
}

void loop() {
  int rawState = digitalRead(WATER_SENSOR_PIN);

  if (rawState != lastRawState) {
    lastChangeTime = millis();
    lastRawState = rawState;
  }

  if (millis() - lastChangeTime >= DEBOUNCE_MS) {
    if (stableState != rawState) {
      stableState = rawState;

      if (stableState == LOW) {
        Serial.println("WATER DETECTED");
      } else {
        Serial.println("DRY");
      }
    }
  }

  delay(10);
}
