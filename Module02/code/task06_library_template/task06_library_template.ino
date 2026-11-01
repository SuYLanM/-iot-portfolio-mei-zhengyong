#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#if defined(ESP8266)
constexpr int SDA_PIN = D2, SCL_PIN = D1;  // WeMos: GPIO4 SDA, GPIO5 SCL
#elif defined(ESP32)
constexpr int SDA_PIN = 21, SCL_PIN = 22;  // classic ESP32 target, NOT analyzer
#else
#error "Select classic ESP32 Dev Module or ESP8266 LOLIN(WEMOS) D1 R2 & mini."
#endif

bool beginConfiguredBus() {
#if defined(ESP8266)
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);
  Wire.setClockStretchLimit(50000);  // microseconds, ESP8266 API
  return true;  // begin() is void: this does not prove a device is present.
#else
  const bool ok = Wire.begin(SDA_PIN, SCL_PIN, 100000);
  Wire.setTimeOut(50);  // milliseconds, ESP32 API
  return ok;
#endif
}

constexpr bool SENSOR_CONFIGURATION_VERIFIED = false;
constexpr int DEVICE_ADDRESS = -1;  // actual scanned address, not selected
constexpr const char *VALUE_UNIT = "";  // actual library quantity/unit required
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
  if (!SENSOR_CONFIGURATION_VERIFIED || DEVICE_ADDRESS < 0x08 ||
      DEVICE_ADDRESS > 0x77 || VALUE_UNIT[0] == 0) {
    Serial.println("PENDING_CONFIG: choose sensor/library; no bus transaction performed");
    return;
  }
  if (!beginConfiguredBus()) return;
  ready = beginSensorLibrary();
  if (!ready) Serial.println("PENDING_ADAPTER_OR_INIT_ERROR: implement library hooks");
}

void loop() {
  if (!ready) { delay(100); return; }
  const uint32_t now = millis();
  if (now - lastRead < 1000) return;
  lastRead = now;
  float value = NAN;
  if (readSensorLibrary(value) && isfinite(value))
    Serial.printf("t_ms=%lu library_value=%.6f unit=%s\n", (unsigned long)now, value, VALUE_UNIT);
  else Serial.println("READ_FAILED: no physical value reported");
}
