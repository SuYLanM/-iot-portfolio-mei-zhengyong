#include <Arduino.h>
#include <Wire.h>

constexpr int SDA_PIN = 21, SCL_PIN = 22;
constexpr bool SENSOR_CONFIGURATION_VERIFIED = false;
bool ready = false;
uint32_t lastRead = 0;

// TODO: include the actual sensor library, create its instance, and implement
// its initialization/error checks after checking model, address and voltage.
bool beginSensorLibrary() { return false; }
bool readSensorLibrary(float &value) {
  (void)value;
  return false; // Not a fabricated zero measurement.
}

void setup() {
  Serial.begin(115200);
  if (!SENSOR_CONFIGURATION_VERIFIED) {
    Serial.println("PENDING_CONFIG: choose sensor/library; no bus transaction performed");
    return;
  }
  if (!Wire.begin(SDA_PIN, SCL_PIN, 100000)) return;
  Wire.setTimeOut(50);
  ready = beginSensorLibrary();
  if (!ready) Serial.println("PENDING_ADAPTER_OR_INIT_ERROR: implement library hooks");
}

void loop() {
  if (!ready) { delay(100); return; }
  const uint32_t now = millis();
  if (now - lastRead < 1000) return;
  lastRead = now;
  float value;
  if (readSensorLibrary(value)) Serial.printf("t_ms=%lu library_value=%.6f\n", (unsigned long)now, value);
  else Serial.println("READ_FAILED: no physical value reported");
}
