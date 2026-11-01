# Task 6 — Direct Wire Register Template

## Parts / Pins / Wiring

No sensor/address/register is selected. Required later: actual datasheet-identified sensor, ESP32/USB, common ground, verified supply/pull-ups; default SDA21/SCL22 subject to board confirmation. The template compiles with built-in Wire only. No hardware traffic should be enabled until configuration review.

## Configuration / Output

DEVICE_AND_REGISTER_VERIFIED=false and address/pointer=-1 are deliberate blockers. Fill a valid ordinary 7-bit address, exact register pointer, pointer width and read length from the actual map. This helper supports 1/2-byte MSB-first pointers and 1–4 consecutive read bytes; command-only sensors, LSB-first pointers or special checksums need a different adapter. Set REPEATED_START_SUPPORTED according to the device transaction diagram, not a blanket assumption. The template performs no arbitrary configuration-register writes beyond the selected pointer.

Default output is PENDING_CONFIG. Once correctly enabled, it prints only actual raw bytes in wire order; it refuses short/failed reads. It deliberately does not invent signed decoding, scale, offset or physical units. An optional linear-field decoder now supports verified byte order, right shift, 1–32-bit fields, signed extension and scale/offset. It requires DECODING_VERIFIED, a nonempty unit and finite constants; defaults prevent reconstruction. Add the actual settings only after checking the register bit fields/mode and sensor initialization. Sensors requiring calibration coefficients, checksums or non-linear compensation need a different decoder. [Decoding checklist and blank comparison](../../register-reading.md). An implemented adapter needs a fresh compile and hardware test; none is claimed here.

## Board Selection, Run and Status

The same sketch supports **classic ESP32 Dev Module / Core 3.3.12** (SDA21/SCL22) and **ESP8266 LOLIN(WEMOS) D1 R2 & mini / Core 3.1.2** (SDA=D2/GPIO4, SCL=D1/GPIO5). ESP8266 uses two-argument Wire.begin plus setClock; ESP32 uses its clock/timeout API. Both request 100 kHz. Use the blue WeMos when retaining the black analyzer's firmware; never flash this sketch to the protected analyzer just because it is listed on COM3.

Open this folder's same-name `.ino` in Arduino IDE 2.3.10, choose the actual board/port and compile. Serial Monitor uses **115200 baud**. No additional library is required for the default **disabled** scaffold. Device-specific library includes and hooks must be selected, implemented, compiled again and tested after the actual model is known. Blank units and -1 addresses are intentional blockers. A successful compile is not a successful sensor reading.

[Chinese Task 5–7 guide](../../wiring-zh.md) · [Follow-up compile record](../../validation.md#follow-up-preparation-checks--2026-11-02). No new upload, module ACK or sensor reading was performed in this preparation.
