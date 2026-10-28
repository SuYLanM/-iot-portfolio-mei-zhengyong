#include <Arduino.h>
#include <Wire.h>

constexpr int SDA_PIN = 21, SCL_PIN = 22;
constexpr bool DEVICE_AND_REGISTER_VERIFIED = false;
constexpr int DEVICE_ADDRESS = -1;   // TODO: actual non-reserved 7-bit address
constexpr int REGISTER_ADDRESS = -1; // TODO: datasheet pointer; never guessed
constexpr uint8_t POINTER_BYTES = 1; // TODO: this transport supports 1 or 2, MSB first
constexpr uint8_t READ_BYTES = 2;    // TODO: 1..4 consecutive bytes, device must allow it
constexpr bool REPEATED_START_SUPPORTED = false;
bool ready = false;
uint32_t lastRead = 0;

bool readRegisterBytes(uint8_t *bytes) {
  Wire.beginTransmission(static_cast<uint8_t>(DEVICE_ADDRESS));
  if (POINTER_BYTES == 2) Wire.write(static_cast<uint8_t>(REGISTER_ADDRESS >> 8));
  Wire.write(static_cast<uint8_t>(REGISTER_ADDRESS));
  if (Wire.endTransmission(!REPEATED_START_SUPPORTED) != 0) return false;
  const size_t got = Wire.requestFrom(static_cast<uint8_t>(DEVICE_ADDRESS),
                                      static_cast<size_t>(READ_BYTES), true);
  if (got != READ_BYTES) {
    while (Wire.available()) Wire.read();
    return false;
  }
  for (uint8_t i = 0; i < READ_BYTES; ++i) bytes[i] = Wire.read();
  return true;
}

void setup() {
  Serial.begin(115200);
  const bool valid = DEVICE_AND_REGISTER_VERIFIED && DEVICE_ADDRESS >= 0x08 &&
    DEVICE_ADDRESS <= 0x77 && REGISTER_ADDRESS >= 0 && REGISTER_ADDRESS <= 65535 &&
    (POINTER_BYTES == 1 || POINTER_BYTES == 2) &&
    (POINTER_BYTES == 2 || REGISTER_ADDRESS <= 255) && READ_BYTES >= 1 && READ_BYTES <= 4;
  if (!valid) { Serial.println("PENDING_CONFIG: no register selected and no bus traffic"); return; }
  ready = Wire.begin(SDA_PIN, SCL_PIN, 100000);
  Wire.setTimeOut(50);
}

void loop() {
  if (!ready) { delay(100); return; }
  const uint32_t now = millis();
  if (now - lastRead < 1000) return;
  lastRead = now;
  uint8_t bytes[4];
  if (!readRegisterBytes(bytes)) { Serial.println("READ_FAILED_OR_SHORT: no value reconstructed"); return; }
  Serial.printf("t_ms=%lu raw_bytes_in_wire_order=", (unsigned long)now);
  for (uint8_t i = 0; i < READ_BYTES; ++i) Serial.printf("%02X ", bytes[i]);
  Serial.println("; scaling/signed decoding PENDING actual datasheet configuration");
}
