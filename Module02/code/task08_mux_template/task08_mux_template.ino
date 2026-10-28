#include <Arduino.h>
#include <Wire.h>

constexpr bool MUX_CONFIGURATION_VERIFIED = false;
constexpr int MUX_ADDRESS = -1; // TODO: actual TCA9548A A2/A1/A0 strap selection
constexpr uint8_t CHANNEL = 0; // One channel only for conflicting devices.

void setup() {
  Serial.begin(115200);
  if (!MUX_CONFIGURATION_VERIFIED || MUX_ADDRESS < 0x70 || MUX_ADDRESS > 0x77 || CHANNEL > 7) {
    Serial.println("PENDING_CONFIG: TCA9548A model, wiring/address and channel unverified");
    return;
  }
  if (!Wire.begin(21, 22, 100000)) return;
  Wire.setTimeOut(50);
  Wire.beginTransmission(static_cast<uint8_t>(MUX_ADDRESS));
  Wire.write(static_cast<uint8_t>(1U << CHANNEL));
  const uint8_t status = Wire.endTransmission(true);
  Serial.printf("channel_selection_write_status=%u; downstream sensor read PENDING\n", status);
  // Return to all channels deselected. ACK is not proof of downstream sensing.
  Wire.beginTransmission(static_cast<uint8_t>(MUX_ADDRESS));
  Wire.write(static_cast<uint8_t>(0));
  Serial.printf("deselect_status=%u\n", Wire.endTransmission(true));
}

void loop() { delay(100); }
