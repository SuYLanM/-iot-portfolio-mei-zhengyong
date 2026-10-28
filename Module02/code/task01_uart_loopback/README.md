# Task 1 — UART Loopback

## Parts / Pins / Wiring

Classic ESP32 WROOM Dev Module, USB data cable, short jumper. UART0 remains the USB console. UART2 uses RX=GPIO16, TX=GPIO17; after verifying availability, connect **TX17 → RX16 on the same board**. No sensor or 5 V wire is needed. Some PSRAM/module variants reserve these pins; change them only after checking the pinout. External peers require crossed TX/RX, compatible 3.3 V logic and an appropriate ground/reference; RS-232 needs its transceiver and must never connect directly.

## Operation / Output

Every two seconds the sketch sends a numbered LOOP_ token. It prints TX and actual RX, and MATCH only for an equal returned line; missing matches eventually print TIMEOUT. Enter short console lines with Newline to send additional text. It does not feed received UART bytes back to UART output, avoiding an infinite echo loop. Automatic tokens can interleave with manual text: stop typing and use a clean run for the match check.

Save no-jumper and loopback logs, actual configuration and wiring photo. Both ends of this same-UART loopback share the baud setting; a genuine peer-baud mismatch needs another independently configured UART/device. A wrong Serial Monitor baud tests only console decoding, and must be labelled that way. No UART success, voltage or latency has been measured here. Record actual results in [pins-and-dividers.md](../../pins-and-dividers.md).

## Run and Status

Prepared and compile-checked for **ESP32 Dev Module / Espressif Core 3.3.12**, Arduino IDE 2.3.10. Open the same-name `.ino` in this folder, select the actual board/port and verify. After inspecting actual hardware, power off to wire, reconnect and upload yourself; no upload was performed during preparation. Open Serial Monitor at **115200 baud**. Compilation is not hardware testing. [Chinese wiring guide](../../wiring-zh.md) · [Evidence register](../../evidence/README.md).
