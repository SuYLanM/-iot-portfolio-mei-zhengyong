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

constexpr bool DEVICE_AND_REGISTER_VERIFIED = false;
constexpr int DEVICE_ADDRESS = -1;   // TODO: actual non-reserved 7-bit address
constexpr int REGISTER_ADDRESS = -1; // TODO: datasheet pointer; never guessed
constexpr uint8_t POINTER_BYTES = 1; // TODO: this transport supports 1 or 2, MSB first
constexpr uint8_t READ_BYTES = 2;    // TODO: 1..4 consecutive bytes, device must allow it
constexpr bool REPEATED_START_SUPPORTED = false;
// Optional linear-field decoder; leave disabled until the actual datasheet
// confirms field layout AND the sensor's initialization/mode configuration.
constexpr bool DECODING_VERIFIED = false;
constexpr bool DATA_MSB_FIRST = true, FIELD_IS_SIGNED = false;
constexpr uint8_t RIGHT_SHIFT = 0, VALUE_BITS = 0; // 0 bits deliberately invalid
constexpr double SCALE = NAN, OFFSET = NAN;
constexpr const char *VALUE_UNIT = "";
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
  for (uint8_t i = 0; i < READ_BYTES; ++i) {
    const int received = Wire.read();
    if (received < 0) return false;
    bytes[i] = static_cast<uint8_t>(received);
  }
  return true;
}

bool decodeLinearField(const uint8_t *bytes, double &value) {
  if (!DECODING_VERIFIED || VALUE_BITS == 0 || VALUE_BITS > 32 ||
      RIGHT_SHIFT + VALUE_BITS > READ_BYTES * 8 ||
      !isfinite(SCALE) || !isfinite(OFFSET) || VALUE_UNIT[0] == 0) return false;
  uint32_t packed = 0;
  for (uint8_t i = 0; i < READ_BYTES; ++i) {
    const uint8_t index = DATA_MSB_FIRST ? i : READ_BYTES - 1 - i;
    packed = (packed << 8) | bytes[index];
  }
  const uint64_t modulus = uint64_t(1) << VALUE_BITS;
  const uint32_t field = (packed >> RIGHT_SHIFT) & (modulus - 1);
  int64_t integer = field;
  if (FIELD_IS_SIGNED && (uint64_t(field) & (modulus >> 1))) integer -= static_cast<int64_t>(modulus);
  value = double(integer) * SCALE + OFFSET;
  return isfinite(value);
}

void setup() {
  Serial.begin(115200);
  const bool valid = DEVICE_AND_REGISTER_VERIFIED && DEVICE_ADDRESS >= 0x08 &&
    DEVICE_ADDRESS <= 0x77 && REGISTER_ADDRESS >= 0 && REGISTER_ADDRESS <= 65535 &&
    (POINTER_BYTES == 1 || POINTER_BYTES == 2) &&
    (POINTER_BYTES == 2 || REGISTER_ADDRESS <= 255) && READ_BYTES >= 1 && READ_BYTES <= 4;
  if (!valid) { Serial.println("PENDING_CONFIG: no register selected and no bus traffic"); return; }
  ready = beginConfiguredBus();
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
  double value;
  if (decodeLinearField(bytes, value)) {
    Serial.printf("; reconstructed=%.6f unit=%s\n", value, VALUE_UNIT);
  } else {
    Serial.println("; scaling/signed decoding PENDING actual datasheet configuration");
  }
}
