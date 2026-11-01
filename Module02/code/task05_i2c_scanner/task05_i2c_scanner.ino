#include <Arduino.h>
#include <Wire.h>

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

constexpr uint32_t BUS_HZ = 100000, SCAN_INTERVAL_MS = 3000;
uint32_t lastScan = 0;
bool ready = false, firstScan = true;

void scanBus() {
  unsigned found = 0, nacks = 0, errors = 0;
  Serial.printf("SCAN_BEGIN t_ms=%lu clock_config_hz=%lu\n", (unsigned long)millis(),
                (unsigned long)BUS_HZ);
  if (digitalRead(SDA_PIN) == LOW || digitalRead(SCL_PIN) == LOW) {
    Serial.println("BUS_NOT_IDLE: check wiring/pull-ups; no addresses probed");
    return;
  }
  // Reserved address groups are deliberately excluded. These are 7-bit values.
  for (uint8_t address = 0x08; address <= 0x77; ++address) {
    Wire.beginTransmission(address);
    const uint8_t status = Wire.endTransmission(true);
    if (status == 0) {
      Serial.printf("ACK address_7bit=0x%02X\n", address);
      ++found;
    } else if (status == 2) {
      ++nacks;
    } else {
      Serial.printf("BUS_ERROR address_7bit=0x%02X wire_status=%u\n", address, status);
      ++errors;
    }
    delay(1);
  }
  Serial.printf("SCAN_END acknowledgements=%u nacks=%u errors=%u\n", found, nacks, errors);
  Serial.println("ACK is not device identity or a count of physical devices.");
}

void setup() {
  Serial.begin(115200);
  Serial.printf("Task 5 scanner: SDA=%d SCL=%d requested_clock=100000 Hz; Serial=115200\n", SDA_PIN, SCL_PIN);
  ready = beginConfiguredBus();
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
  delay(1);
}
