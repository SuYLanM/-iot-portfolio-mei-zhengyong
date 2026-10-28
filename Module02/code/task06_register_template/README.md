# Task 6 — Direct Wire Register Template

## Parts / Pins / Wiring

No sensor/address/register is selected. Required later: actual datasheet-identified sensor, ESP32/USB, common ground, verified supply/pull-ups; default SDA21/SCL22 subject to board confirmation. The template compiles with built-in Wire only. No hardware traffic should be enabled until configuration review.

## Configuration / Output

DEVICE_AND_REGISTER_VERIFIED=false and address/pointer=-1 are deliberate blockers. Fill a valid ordinary 7-bit address, exact register pointer, pointer width and read length from the actual map. This helper supports 1/2-byte MSB-first pointers and 1–4 consecutive read bytes; command-only sensors, LSB-first pointers or special checksums need a different adapter. Set REPEATED_START_SUPPORTED according to the device transaction diagram, not a blanket assumption. The template performs no arbitrary configuration-register writes beyond the selected pointer.

Default output is PENDING_CONFIG. Once correctly enabled, it prints only actual raw bytes in wire order; it refuses short/failed reads. It deliberately does not invent signed decoding, scale, offset or physical units. Add those only after checking the actual register bit fields/mode. [Decoding checklist and blank comparison](../../register-reading.md). An implemented adapter needs a fresh compile and hardware test; none is claimed here.

## Run and Status

Prepared and compile-checked for **ESP32 Dev Module / Espressif Core 3.3.12**, Arduino IDE 2.3.10. Open the same-name `.ino` in this folder, select the actual board/port and verify. After inspecting actual hardware, power off to wire, reconnect and upload yourself; no upload was performed during preparation. Open Serial Monitor at **115200 baud**. Compilation is not hardware testing. [Chinese wiring guide](../../wiring-zh.md) · [Evidence register](../../evidence/README.md).
