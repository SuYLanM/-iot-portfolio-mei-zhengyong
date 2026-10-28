#include <Arduino.h>
#include <Wire.h>

// TARGET board only. A second ESP32 runs the SUMP analyzer firmware.
// Target SDA21 -> analyzer CH0/GPIO21; target SCL22 -> analyzer CH1/GPIO19.
// NEVER connect target SCL to analyzer GPIO22 (analyzer's clock OUTPUT).
constexpr int SDA_PIN = 21, SCL_PIN = 22;
constexpr uint32_t BUS_HZ = 100000, SCAN_INTERVAL_MS = 3000;
bool ready = false, firstScan = true;
uint32_t lastScanEnd = 0;

void scanBus() {
  unsigned acknowledgements = 0, nacks = 0, errors = 0;
  Serial.printf("SCAN_BEGIN t_ms=%lu SDA=%d SCL=%d configured_hz=%lu\n",
                (unsigned long)millis(), SDA_PIN, SCL_PIN, (unsigned long)Wire.getClock());
  // Probe only non-reserved 7-bit addresses. No device/address is assumed.
  for (uint8_t address = 0x08; address <= 0x77; ++address) {
    Wire.beginTransmission(address);
    const uint8_t status = Wire.endTransmission(true);
    if (status == 0) {
      ++acknowledgements;
      Serial.printf("ACK address_7bit=0x%02X\n", address);
    } else if (status == 2) {
      ++nacks;
      Serial.printf("NACK address_7bit=0x%02X (address not acknowledged)\n", address);
    } else {
      ++errors;
      Serial.printf("BUS_ERROR address_7bit=0x%02X Wire_status=%u\n", address, status);
    }
    delay(1);
  }
  Serial.printf("SCAN_END ACK=%u NACK=%u other_errors=%u\n", acknowledgements, nacks, errors);
  Serial.println("ACK is not device identity/count. Verify ninth SCL pulse in PulseView.");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Task 9 I2C target: 100 kHz configured; actual captures PENDING.");
  Serial.println("3.3 V pull-ups required; no 5 V pull-ups. Send s to request a scan.");
  ready = Wire.begin(SDA_PIN, SCL_PIN, BUS_HZ);
  Wire.setTimeOut(50);
  if (!ready) Serial.println("FAIL: Wire initialization; no scans will run.");
}

void loop() {
  if (!ready) { delay(100); return; }
  bool requested = false;
  while (Serial.available()) if (Serial.read() == 's') requested = true;
  if (firstScan || requested || millis() - lastScanEnd >= SCAN_INTERVAL_MS) {
    firstScan = false;
    scanBus();
    lastScanEnd = millis();
  }
  delay(1);
}
