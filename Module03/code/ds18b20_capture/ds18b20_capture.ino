#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

constexpr int DATA_PIN = 18;
constexpr int MAX_PROBES = 8;
constexpr uint16_t CONVERSION_MS[] = {94, 188, 375, 750};
OneWire bus(DATA_PIN);
DallasTemperature sensors(&bus);

void printRom(const uint8_t *rom) {
  for (int i = 0; i < 8; ++i) {
    Serial.printf("%02X", rom[i]);
  }
}

int discover(DeviceAddress (&roms)[MAX_PROBES]) {
  bus.reset_search();
  uint8_t rom[8];
  int count = 0;
  while (count < MAX_PROBES && bus.search(rom)) {
    if (rom[0] != 0x28 || OneWire::crc8(rom, 7) != rom[7]) {
      continue;
    }
    memcpy(roms[count++], rom, sizeof(rom));
    Serial.print("# DS18B20 ROM ");
    printRom(rom);
    Serial.println();
  }
  return count;
}

void capture(int bits, int samples) {
  DeviceAddress roms[MAX_PROBES];
  const int count = discover(roms);
  if (count == 0) {
    Serial.println("# NO_DS18B20");
    return;
  }
  for (int i = 0; i < count; ++i) {
    sensors.setResolution(roms[i], bits);
    if (sensors.getResolution(roms[i]) != bits) {
      Serial.println("# RESOLUTION_CHECK_FAILED");
      return;
    }
  }

  Serial.println("sample,t_ms,rom,bits,temp_c,status");
  const uint32_t start = millis();
  for (int i = 0; i < samples; ++i) {
    if (!bus.reset()) {
      Serial.println("# BUS_NO_PRESENCE; capture aborted");
      break;
    }
    sensors.requestTemperatures();
    const uint32_t conversionStart = millis();
    // Allow a manual transfer marker while the externally powered probes convert.
    while (millis() - conversionStart < CONVERSION_MS[bits - 9] + 1U) {
      if (Serial.read() == 'm') {
        Serial.printf("# TRANSFER t_ms=%lu\n", static_cast<unsigned long>(millis() - start));
      }
      delay(1);
    }
    const uint32_t elapsed = millis() - start;
    for (int j = 0; j < count; ++j) {
      const float value = sensors.getTempC(roms[j]);
      Serial.printf("%d,%lu,", i, static_cast<unsigned long>(elapsed));
      printRom(roms[j]);
      Serial.printf(",%d,", bits);
      if (!(value >= -55.0f && value <= 125.0f)) {
        Serial.println(",read_error");
      } else {
        Serial.printf("%.4f,ok\n", value);
      }
    }
  }
  Serial.println("# END");
}

void setup() {
  Serial.begin(115200);
  sensors.begin();
  sensors.setAutoSaveScratchPad(false);
  sensors.setWaitForConversion(false);
  Serial.println("# r: scan ROM IDs; 9/a/b/c: 10 conversions at 9/10/11/12 bits");
  Serial.println("# s: 240 conversions at 12 bits for a step response");
  Serial.println("# m: mark transfer during a capture (DQ GPIO18)");
}

void loop() {
  const int command = Serial.read();
  if (command == 'r') {
    DeviceAddress roms[MAX_PROBES];
    if (discover(roms) == 0) {
      Serial.println("# NO_DS18B20");
    }
  } else {
    switch (command) {
      case '9': capture(9, 10); break;
      case 'a': capture(10, 10); break;
      case 'b': capture(11, 10); break;
      case 'c': capture(12, 10); break;
      case 's': capture(12, 240); break;
    }
  }
  delay(1);
}
