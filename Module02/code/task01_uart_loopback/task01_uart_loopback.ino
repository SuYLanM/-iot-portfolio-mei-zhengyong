#include <Arduino.h>

// Classic ESP32 WROOM Dev Module only after checking the physical pinout.
// UART0 remains the USB console. Power off, jumper GPIO17 (TX2) to GPIO16 (RX2).
constexpr int RX_PIN = 16, TX_PIN = 17;
constexpr uint32_t BAUD = 115200;
HardwareSerial TestPort(2);
uint32_t lastPing = 0, sequence = 0;
String received, expected;
bool waiting = false;

void setup() {
  Serial.begin(BAUD);
  TestPort.begin(BAUD, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("UART loopback prepared: TX17 -> RX16, 3.3 V only; console 115200 8N1");
}

void loop() {
  const uint32_t now = millis();
  if (now - lastPing >= 2000) {
    if (waiting) Serial.println("TIMEOUT: no matching line; inspect wiring/baud");
    lastPing = now;
    expected = "LOOP_" + String(sequence++);
    TestPort.println(expected);
    waiting = true;
    Serial.print("TX: "); Serial.println(expected);
  }
  // Console input is transmitted once; received data is never sent back again.
  while (Serial.available()) TestPort.write(Serial.read());
  while (TestPort.available()) {
    const char c = static_cast<char>(TestPort.read());
    if (c == '\r') continue;
    if (c == '\n') {
      Serial.print("RX: "); Serial.println(received);
      if (waiting && received == expected) {
        Serial.println("MATCH: returned test line; not a voltage/latency measurement");
        waiting = false;
      }
      received = "";
    } else if (received.length() < 120) received += c;
    else { received = ""; Serial.println("RX line too long; buffer discarded"); }
  }
}
