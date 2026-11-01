# Task 6 — Sensor Library Adapter Template

## Parts / Pins / Wiring

Actual sensor and library are **not selected**. Required later: identified 3.3 V-compatible module/breakout, ESP32, USB, shared ground and reviewed I2C power/pull-ups. Proposed SDA21/SCL22 must match the physical board. Do not wire an unidentified module. No third-party library is needed to compile the disabled template.

## Configuration / Output

Default SENSOR_CONFIGURATION_VERIFIED=false prints PENDING_CONFIG and performs no I2C transaction. After datasheet review, include the actual library, instantiate its device object, implement beginSensorLibrary/readSensorLibrary and their failure checks, set the verified actual address through that library, set DEVICE_ADDRESS and VALUE_UNIT, then enable the guard. The stubs return false rather than fake zero readings. If initialization/reading fails or an adapter returns a nonfinite value, no physical value is reported. The adapter hooks still return false until implemented. Specify units and library/version/mode in the report; the generic output field has no implied unit.

Compile success covers the scaffold only, not an unselected library or sensor driver. Compare against [direct-register template](../task06_register_template/README.md) under matched conversion settings in [the blank Task 6 comparison](../../register-reading.md). Hardware readings and agreement remain Pending.

## Board Selection, Run and Status

The same sketch supports **classic ESP32 Dev Module / Core 3.3.12** (SDA21/SCL22) and **ESP8266 LOLIN(WEMOS) D1 R2 & mini / Core 3.1.2** (SDA=D2/GPIO4, SCL=D1/GPIO5). ESP8266 uses two-argument Wire.begin plus setClock; ESP32 uses its clock/timeout API. Both request 100 kHz. Use the blue WeMos when retaining the black analyzer's firmware; never flash this sketch to the protected analyzer just because it is listed on COM3.

Open this folder's same-name `.ino` in Arduino IDE 2.3.10, choose the actual board/port and compile. Serial Monitor uses **115200 baud**. No additional library is required for the default **disabled** scaffold. Device-specific library includes and hooks must be selected, implemented, compiled again and tested after the actual model is known. Blank units and -1 addresses are intentional blockers. A successful compile is not a successful sensor reading.

[Chinese Task 5–7 guide](../../wiring-zh.md) · [Follow-up compile record](../../validation.md#follow-up-preparation-checks--2026-11-02). No new upload, module ACK or sensor reading was performed in this preparation.
