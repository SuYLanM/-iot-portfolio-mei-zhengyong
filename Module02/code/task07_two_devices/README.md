# Task 7 — Two-device Framework

## Parts / Pins / Wiring

ESP32/USB and two actual I2C devices with verified **different** 7-bit addresses, compatible logic, shared SDA21/SCL22/GND and reviewed module supplies/pull-ups. Models/addresses are Pending. No sensor libraries are bundled. Check capacitance, pull-ups and slowest device speed; begin at 100 kHz.

## Configuration / Output

Default configuration is disabled with addresses=-1, so output is PENDING_CONFIG and no device is assumed present. Fill actual distinct addresses, implement initializeDevices/readDeviceA/readDeviceB using the actual libraries or register maps, then enable the guard. Configured setup probes each specified address and reports raw Wire status; it is not a full discovery scan. Use [Task 5 scanner](../task05_i2c_scanner/README.md) to record A-only, B-only and combined discovery.

After successful initialization the loop reports independent validity flags and only prints values when the matching adapter succeeds. Failed reads are not zero measurements. Units and scaling belong to the implemented adapters. Fill the [actual device/address table and run record](../../two-devices.md); do not mistake compile-checked stubs for two tested sensor drivers.

## Run and Status

Prepared and compile-checked for **ESP32 Dev Module / Espressif Core 3.3.12**, Arduino IDE 2.3.10. Open the same-name `.ino` in this folder, select the actual board/port and verify. After inspecting actual hardware, power off to wire, reconnect and upload yourself; no upload was performed during preparation. Open Serial Monitor at **115200 baud**. Compilation is not hardware testing. [Chinese wiring guide](../../wiring-zh.md) · [Evidence register](../../evidence/README.md).
