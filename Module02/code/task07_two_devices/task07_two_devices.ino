#include <Arduino.h>
#include <Wire.h>

constexpr int SDA_PIN = 21, SCL_PIN = 22;
constexpr bool TWO_DEVICE_CONFIGURATION_VERIFIED = false;
constexpr int ADDRESS_A = -1, ADDRESS_B = -1; // TODO verified distinct addresses
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
      ADDRESS_B < 0x08 || ADDRESS_B > 0x77 || ADDRESS_A == ADDRESS_B) {
    Serial.println("PENDING_CONFIG: two verified different addresses and adapters required");
    return;
  }
  if (!Wire.begin(SDA_PIN, SCL_PIN, 100000)) return;
  Wire.setTimeOut(50);
  const bool a = addressResponds(ADDRESS_A), b = addressResponds(ADDRESS_B);
  ready = a && b && initializeDevices();
  if (!ready) Serial.println("NOT_READY: missing ACK, initialization or adapter implementation");
}

void loop() {
  if (!ready) { delay(100); return; }
  const uint32_t now = millis();
  if (now - lastRead < 1000) return;
  lastRead = now;
  float a, b;
  const bool okA = readDeviceA(a), okB = readDeviceB(b);
  Serial.printf("t_ms=%lu a_valid=%d b_valid=%d", (unsigned long)now, okA, okB);
  if (okA) Serial.printf(" a=%.6f", a);
  if (okB) Serial.printf(" b=%.6f", b);
  Serial.println();
}
