# Task 6 — Sensor Library Adapter Template

## Parts / Pins / Wiring

Actual sensor and library are **not selected**. Required later: identified 3.3 V-compatible module/breakout, ESP32, USB, shared ground and reviewed I2C power/pull-ups. Proposed SDA21/SCL22 must match the physical board. Do not wire an unidentified module. No third-party library is needed to compile the disabled template.

## Configuration / Output

Default SENSOR_CONFIGURATION_VERIFIED=false prints PENDING_CONFIG and performs no I2C transaction. After datasheet review, include the actual library, instantiate its device object, implement beginSensorLibrary/readSensorLibrary and their failure checks, set the verified actual address through that library, then enable the guard. The stubs return false rather than fake zero readings. If initialization/reading fails, no physical value is reported. Specify units and library/version/mode in the report; the generic output field has no implied unit.

Compile success covers the scaffold only, not an unselected library or sensor driver. Compare against [direct-register template](../task06_register_template/README.md) under matched conversion settings in [the blank Task 6 comparison](../../register-reading.md). Hardware readings and agreement remain Pending.

## Run and Status

Prepared and compile-checked for **ESP32 Dev Module / Espressif Core 3.3.12**, Arduino IDE 2.3.10. Open the same-name `.ino` in this folder, select the actual board/port and verify. After inspecting actual hardware, power off to wire, reconnect and upload yourself; no upload was performed during preparation. Open Serial Monitor at **115200 baud**. Compilation is not hardware testing. [Chinese wiring guide](../../wiring-zh.md) · [Evidence register](../../evidence/README.md).
