#include <Arduino.h>
#include <Wire.h>

#if !defined(ESP8266)
#error "Select ESP8266 LOLIN(WEMOS) D1 R2 & mini, not analyzer ESP32."
#endif

constexpr uint8_t SDA_PIN = D2;  // GPIO4 -> analyzer CH0 / GPIO21
constexpr uint8_t SCL_PIN = D1;  // GPIO5 -> analyzer CH1 / GPIO19
constexpr uint32_t BUS_HZ = 100000;
bool continuous = false;
uint32_t lastScanEnd = 0;

void scanBus() {
  if (digitalRead(SDA_PIN) == LOW || digitalRead(SCL_PIN) == LOW) {
    Serial.println("BUS_NOT_IDLE: check wiring and pull-ups; scan skipped.");
    return;
  }
  unsigned acks = 0, nacks = 0, errors = 0;
  Serial.println("SCAN_BEGIN: 7-bit addresses 0x08..0x77, empty writes");
  for (uint8_t address = 0x08; address <= 0x77; ++address) {
    Wire.beginTransmission(address);
    const uint8_t status = Wire.endTransmission(true);
    if (status == 0) {
      ++acks;
      Serial.printf("ACK address_7bit=0x%02X (identity not verified)\n", address);
    } else if (status == 2) {
      ++nacks;
    } else {
      ++errors;
      Serial.printf("BUS_ERROR address_7bit=0x%02X status=%u\n", address, status);
    }
    yield();
  }
  Serial.printf("SCAN_END ACK=%u NACK=%u other_errors=%u\n", acks, nacks, errors);
  Serial.println("Check ninth SCL pulse in the actual capture for ACK/NACK.");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);  // ESP8266 third argument is NOT clock frequency.
  Wire.setClock(BUS_HZ);
  Wire.setClockStretchLimit(50000);  // microseconds in ESP8266 Core 3.1.2
  Serial.println();
  Serial.println("Task 9 ESP8266 I2C source: SDA=D2/GPIO4; SCL=D1/GPIO5");
  Serial.println("Requested clock=100000 Hz; console=115200; capture PENDING.");
  Serial.println("3.3 V pull-ups only; verify common ground and probe mapping.");
  Serial.println("Initially paused: s=one scan, c=continuous, p=pause.");
}

void loop() {
  bool once = false;
  while (Serial.available()) {
    const char command = Serial.read();
    if (command == 's') once = true;
    if (command == 'c') continuous = true;
    if (command == 'p') continuous = false;
  }
  if (once || (continuous && millis() - lastScanEnd >= 100)) {
    scanBus();
    lastScanEnd = millis();
  }
  delay(1);
}
