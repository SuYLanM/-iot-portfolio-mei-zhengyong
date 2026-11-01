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

constexpr bool TWO_DEVICE_CONFIGURATION_VERIFIED = false;
constexpr int ADDRESS_A = -1, ADDRESS_B = -1; // TODO verified distinct addresses
constexpr const char *UNIT_A = "", *UNIT_B = ""; // verified physical units
bool ready = false;
uint32_t lastRead = 0;

// TODO: implement actual device-specific library/register adapters and init.
bool initializeDevices() { return false; }
bool readDeviceA(float &value) { (void)value; return false; }
bool readDeviceB(float &value) { (void)value; return false; }
bool addressResponds(int address) {
  Wire.beginTransmission(static_cast<uint8_t>(address));
  const uint8_t status = Wire.endTransmission(true);
  Serial.printf("probe_7bit=0x%02X wire_status=%u\n", address, status);
  return status == 0;
}

void setup() {
  Serial.begin(115200);
  if (!TWO_DEVICE_CONFIGURATION_VERIFIED || ADDRESS_A < 0x08 || ADDRESS_A > 0x77 ||
      ADDRESS_B < 0x08 || ADDRESS_B > 0x77 || ADDRESS_A == ADDRESS_B ||
      UNIT_A[0] == 0 || UNIT_B[0] == 0) {
    Serial.println("PENDING_CONFIG: two verified different addresses and adapters required");
    return;
  }
  if (!beginConfiguredBus()) return;
  const bool a = addressResponds(ADDRESS_A), b = addressResponds(ADDRESS_B);
  ready = a && b && initializeDevices();
  if (!ready) Serial.println("NOT_READY: missing ACK, initialization or adapter implementation");
}

void loop() {
  if (!ready) { delay(100); return; }
  const uint32_t now = millis();
  if (now - lastRead < 1000) return;
  lastRead = now;
  float a = NAN, b = NAN;
  const bool okA = readDeviceA(a) && isfinite(a);
  const bool okB = readDeviceB(b) && isfinite(b);
  Serial.printf("t_ms=%lu a_valid=%d b_valid=%d", (unsigned long)now, okA, okB);
  if (okA) Serial.printf(" a=%.6f a_unit=%s", a, UNIT_A);
  if (okB) Serial.printf(" b=%.6f b_unit=%s", b, UNIT_B);
  Serial.println();
}
