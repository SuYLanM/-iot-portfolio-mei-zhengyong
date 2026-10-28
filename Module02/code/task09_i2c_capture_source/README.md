# Task 9 — I2C Capture Source (Target Board)

**Prepared / Awaiting hardware validation.** [Sketch](task09_i2c_capture_source.ino) · [Chinese operation manual](../../task09-logic-analyzer-guide-zh.md) · [English report](../../scope-decision.md) · [Blank evidence record](../../evidence/task09-logic-analyzer-record.md).

This independent sketch leaves the existing Task 5 scanner unchanged. It runs on target ESP32 **T**, while separate ESP32 **A** runs upstream analyzer firmware. Select classic ESP32 Dev Module, installed Espressif Core 3.3.12, actual target COM port and Serial Monitor 115200 baud. Open the same-name .ino folder; do not combine it with the analyzer or PWM sketch.

## Parts / Wiring

Two classic ESP32 boards and USB cables, jumper wires, and appropriate SDA/SCL pull-ups **to target 3.3 V only**. A real compatible I2C sensor is optional for NACK-only traffic and required to demonstrate a real device ACK; confirm its model/address/voltage from its datasheet. Inspect any onboard pull-ups before adding more. A starting pull-up proposal is 4.7 kΩ per line, subject to capacitance/rise-time/sink-current checks, not a verified universal value.

| Target T | Analyzer A | Purpose |
|---|---|---|
| GPIO21 SDA | GPIO21 / CH0 | Probe data input |
| GPIO22 SCL | GPIO19 / CH1 | Probe clock input |
| GND | GND | Common reference |
| No target connection | A GPIO22 → A GPIO23 | Mandatory analyzer-local clock jumper |

Never connect target SCL to analyzer GPIO22, which is an **output**. Do not join the boards' 3V3/5V outputs. Power off before rewiring. The optional sensor and both pull-ups belong to T's local supply/ground. All bare analyzer inputs accept only compatible 3.3 V logic.

## Operation / Evidence

The sketch configures `Wire.begin(21,22,100000)`, a 50 ms Wire timeout and repeated scans approximately three seconds after the last scan ends. Sending `s` on **T's** console requests an additional scan. It probes 7-bit addresses 0x08–0x77 and prints the actual result of each address: ACK for status 0, address NACK for status 2, or other errors separately. No output address is pre-filled as a real device.

In PulseView on **A's** COM port, start with CH0/CH1, requested 1 MS/s, 16k samples, RLE off. Free-running windows can fall in the three-second idle gap; use a supported **LOW level** trigger on CH1/SCL or arm then request a scan on T. Add the I2C decoder with SDA=CH0 and SCL=CH1. A scanner transaction is START, seven address bits, write bit 0, ACK/NACK on the ninth clock, STOP; there are no register/data bytes in this empty write probe. A real sensor pulls SDA LOW to ACK; no responder leaves it HIGH for NACK. Compare the selected transaction with its actual serial result and use seven-bit address display where offered.

Save a real wiring photograph, serial log, decoder screenshot with START/address/RW/ACK-or-NACK/STOP, and original `.sr` acquisition. Record requested rate separately from independently verified timing. If no sensor is connected, report only the actual NACK observation; it is not an ACK demonstration. No capture or device discovery has yet occurred.

## Software Validation

**Compile PASS — 2026-10-28:** Arduino CLI, `esp32:esp32:esp32`, installed Core 3.3.12. Output: 294480 bytes flash / 23508 bytes global RAM. Build artifacts stayed in a temporary directory. No upload, scan or capture was performed.
