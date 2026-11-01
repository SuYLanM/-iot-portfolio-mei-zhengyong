#include <Arduino.h>
#include <Wire.h>

// 扫描器不预设 LCD 或 RTC 的地址；先确认 3.3 V 供电和总线电平。
constexpr uint32_t BUS_HZ = 100000, SCAN_INTERVAL_MS = 3000;

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
  Wire.setClock(BUS_HZ);
  Wire.setClockStretchLimit(50000);  // microseconds, ESP8266 API
  return true;  // begin() is void: this does not prove a device is present.
#else
  // 经典 ESP32：SDA=GPIO21，SCL=GPIO22；显式设置 100 kHz。
  const bool ok = Wire.begin(21, 22);
  const bool clockOk = ok && Wire.setClock(BUS_HZ);
  Wire.setTimeOut(50);  // milliseconds, ESP32 API
  return clockOk;
#endif
}

uint32_t lastScan = 0, scanNumber = 0;
bool ready = false, firstScan = true;

void scanBus() {
  unsigned found = 0, nacks = 0, errors = 0, probed = 0;
  ++scanNumber;
  Serial.printf("SCAN_BEGIN scan=%lu t_ms=%lu clock_config_hz=%lu\n",
                (unsigned long)scanNumber, (unsigned long)millis(),
                (unsigned long)BUS_HZ);
  if (digitalRead(SDA_PIN) == LOW || digitalRead(SCL_PIN) == LOW) {
    Serial.println("BUS_NOT_IDLE: check wiring/pull-ups; no addresses probed");
    return;
  }
  // 只扫描普通 7-bit 地址 0x08–0x77，跳过保留地址。
  for (uint8_t address = 0x08; address <= 0x77; ++address) {
    ++probed;
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
  if (found == 0) Serial.println("No I2C devices found");
  Serial.printf("SCAN_END scan=%lu probed=%u detected_addresses=%u nacks=%u errors=%u\n",
                (unsigned long)scanNumber, probed, found, nacks, errors);
  if (errors != 0) Serial.println("Bus errors occurred; zero ACKs does not prove no device is connected.");
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
  // 首次扫描后，每轮结束约 3 秒再扫描；串口发送 s 可立即请求。
  if (firstScan || requested || now - lastScan >= SCAN_INTERVAL_MS) {
    firstScan = false;
    scanBus();
    lastScan = millis();
  }
  delay(1);
}
