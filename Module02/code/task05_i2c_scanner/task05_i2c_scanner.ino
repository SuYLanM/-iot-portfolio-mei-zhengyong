#include <Arduino.h>
#include <Wire.h>

constexpr int SDA_PIN = 21, SCL_PIN = 22;
constexpr uint32_t BUS_HZ = 100000, SCAN_INTERVAL_MS = 3000;
uint32_t lastScan = 0;
bool ready = false, firstScan = true;

void scanBus() {
  unsigned found = 0, errors = 0;
  Serial.printf("SCAN_BEGIN t_ms=%lu clock_config_hz=%lu\n", (unsigned long)millis(),
                (unsigned long)Wire.getClock());
  // Reserved address groups are deliberately excluded. These are 7-bit values.
  for (uint8_t address = 0x08; address <= 0x77; ++address) {
    Wire.beginTransmission(address);
    const uint8_t status = Wire.endTransmission(true);
    if (status == 0) {
      Serial.printf("ACK address_7bit=0x%02X\n", address);
      ++found;
    } else if (status != 2) {
      Serial.printf("BUS_ERROR address_7bit=0x%02X wire_status=%u\n", address, status);
      ++errors;
    }
    delay(1);
  }
  Serial.printf("SCAN_END acknowledgements=%u errors=%u\n", found, errors);
  Serial.println("ACK is not device identity or a count of physical devices.");
}

void setup() {
  Serial.begin(115200);
  ready = Wire.begin(SDA_PIN, SCL_PIN, BUS_HZ);
  Wire.setTimeOut(50);
  if (!ready) Serial.println("I2C initialization failed; inspect pin configuration");
}

void loop() {
  if (!ready) { delay(100); return; }
  bool requested = false;
  while (Serial.available()) if (Serial.read() == 's') requested = true;
  const uint32_t now = millis();
  if (firstScan || requested || now - lastScan >= SCAN_INTERVAL_MS) {
    firstScan = false;
    scanBus();
    lastScan = millis();
  }
}
