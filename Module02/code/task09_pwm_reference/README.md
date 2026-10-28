# Task 9 — Core 3 PWM Reference Source

## Parts / Pins / Wiring

A separate, verified classic ESP32 target board and USB cable; GPIO25 generates a 3.3 V logic PWM command. The instrument must have its own reviewed pin map. The first digital test uses **target GPIO25 → analyzer CH0/GPIO21**, common GND, and **analyzer-local GPIO22→GPIO23** clock jumper; never probe analyzer GPIO22. GPIO25 avoids the analyzer's reserved clock pins; on the separate analyzer it is also CH11, which is not used here. For the browser analog candidate GPIO34, first use a reviewed divider (e.g. 10k/10k nominally halves 3.3 V to 1.65 V), verify voltage before attachment and assess loading/bandwidth. Do not tie two boards' regulated 3V3 outputs together.

## Operation / Output

Uses Core 3 ledcAttach/ledcWrite to request 1 kHz, 8-bit duty code 128/256. Setup reports configuration success/error; printed target frequency is not independently measured output frequency. No external component is powered from GPIO25. Do not run this source on the board currently running SUMP firmware without a reviewed combined design.

Save actual measured frequency/duty, requested/measured instrument rates, wiring/protection, firmware commits, host versions and PWM/I2C captures in [scope-decision.md](../../scope-decision.md). This sketch is compiled preparation only; it is not installation or performance validation of either external instrument project.

## Run and Status

Prepared and compile-checked for **ESP32 Dev Module / Espressif Core 3.3.12**, Arduino IDE 2.3.10. Open the same-name `.ino` in this folder, select the actual board/port and verify. After inspecting actual hardware, power off to wire, reconnect and upload yourself; no upload was performed during preparation. Open Serial Monitor at **115200 baud**. Compilation is not hardware testing. [Chinese wiring guide](../../wiring-zh.md) · [Evidence register](../../evidence/README.md).

Recompiled 2026-10-28: **PASS**, 278043 bytes flash / 22312 bytes global RAM. Setup now prints GPIO, requested frequency, duty and success status explicitly; periodic messages do not claim measured performance. [Complete Task 9 Chinese manual](../../task09-logic-analyzer-guide-zh.md) · [Build audit](../../task09-analyzer-build.md) · [Blank capture record](../../evidence/task09-logic-analyzer-record.md).
