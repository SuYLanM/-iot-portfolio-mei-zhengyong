# Task 7 — Two-device Framework

## Parts / Pins / Wiring

ESP32/USB and two actual I2C devices with verified **different** 7-bit addresses, compatible logic, shared SDA21/SCL22/GND and reviewed module supplies/pull-ups. Models/addresses are Pending. No sensor libraries are bundled. Check capacitance, pull-ups and slowest device speed; begin at 100 kHz.

## Configuration / Output

Default configuration is disabled with addresses=-1, so output is PENDING_CONFIG and no device is assumed present. Fill actual distinct addresses, implement initializeDevices/readDeviceA/readDeviceB using the actual libraries or register maps, set UNIT_A/UNIT_B, then enable the guard. Configured setup probes each specified address and reports raw Wire status; it is not a full discovery scan. Use [Task 5 scanner](../task05_i2c_scanner/README.md) to record A-only, B-only and combined discovery.

After successful initialization the loop reports independent validity flags and only prints values when the matching adapter succeeds. Failed or nonfinite reads are not zero measurements. Units and scaling belong to the implemented adapters. Fill the [actual device/address table and run record](../../two-devices.md); do not mistake compile-checked stubs for two tested sensor drivers.

## Board Selection, Run and Status

The same sketch supports **classic ESP32 Dev Module / Core 3.3.12** (SDA21/SCL22) and **ESP8266 LOLIN(WEMOS) D1 R2 & mini / Core 3.1.2** (SDA=D2/GPIO4, SCL=D1/GPIO5). ESP8266 uses two-argument Wire.begin plus setClock; ESP32 uses its clock/timeout API. Both request 100 kHz. Use the blue WeMos when retaining the black analyzer's firmware; never flash this sketch to the protected analyzer just because it is listed on COM3.

Open this folder's same-name `.ino` in Arduino IDE 2.3.10, choose the actual board/port and compile. Serial Monitor uses **115200 baud**. No additional library is required for the default **disabled** scaffold. Device-specific library includes and hooks must be selected, implemented, compiled again and tested after the actual model is known. Blank units and -1 addresses are intentional blockers. A successful compile is not a successful sensor reading.

[Chinese Task 5–7 guide](../../wiring-zh.md) · [Follow-up compile record](../../validation.md#follow-up-preparation-checks--2026-11-02). No new upload, module ACK or sensor reading was performed in this preparation.
