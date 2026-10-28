# Task 5 — Repeating I2C Scanner

## Parts / Pins / Wiring

ESP32, USB data cable, a datasheet-identified compatible I2C module, breadboard/jumpers and reviewed pull-ups if needed. Default **SDA GPIO21, SCL GPIO22**, plus GND and module-specific supply. Verify physical board pins first. Modules powered at 5 V can have 5 V pull-ups: do not connect those lines to ESP32. Use compatible 3.3 V logic and inspect existing pull-ups before adding more. Never treat arbitrary VIN/VCC labels as equivalent.

## Operation / Output

Scan starts after setup, repeats about every three seconds after the previous scan, and can be requested with lowercase `s`. Only ordinary 7-bit address range **0x08–0x77** is probed; reserved groups are deliberately skipped, unlike the course's broader introductory loop. Probe timing is blocking for the duration of a scan and may be much longer with timeout/errors.

Runtime output includes SCAN_BEGIN, configured clock, actual ACK address_7bit, non-address-NACK Wire statuses and a final summary. No address output is prefilled here. Zero ACKs is not proof of a broken sensor, and one ACK can represent colliding targets; scanning is not a model identifier. Address probing can affect unusual devices: read the actual datasheet before using this generic scanner.

Record module identity, address convention, pins, supply/pull-ups, actual port and scan log. Power off, disconnect one device, restore power and rescan; compare actual changes without unsafe hot-plugging. Save one-/two-device configurations and map every observed address to the relevant datasheet. [Blank scan report](../../evidence/README.md) · [Study log](../../study-log.md). No hardware scan has been performed.

## Run and Status

Prepared and compile-checked for **ESP32 Dev Module / Espressif Core 3.3.12**, Arduino IDE 2.3.10. Open the same-name `.ino` in this folder, select the actual board/port and verify. After inspecting actual hardware, power off to wire, reconnect and upload yourself; no upload was performed during preparation. Open Serial Monitor at **115200 baud**. Compilation is not hardware testing. [Chinese wiring guide](../../wiring-zh.md) · [Evidence register](../../evidence/README.md).
