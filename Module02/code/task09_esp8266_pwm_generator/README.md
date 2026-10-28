# Task 9 — WeMos D1 mini ESP8266 PWM Generator

**Compile/upload and a real PWM acquisition verified — 2026-10-28.** [Actual capture and measurements](../../evidence/task09-esp8266-pwm.md). Existing ESP32 LOW/HIGH evidence remains unchanged. This sketch runs only on the **blue ESP8266 board B**; never upload it to black analyzer ESP32 A / COM3.

[Sketch](task09_esp8266_pwm_generator.ino) · [Task 9 report](../../scope-decision.md) · [Preserved static LOW/HIGH evidence](../../evidence/task09-logic-analyzer/README.md)

## Board and Program

Select **LOLIN(WEMOS) D1 R2 & mini**, from the official **ESP8266 Community** board package. The target is `esp8266:esp8266:d1_mini`, not an ESP32 target. Board B was identified as USB-SERIAL CH340 on **COM4**, confirmed by the user before upload. Recheck after future USB changes; do not select COM3.

The sketch explicitly sets D1=GPIO5 as output, `analogWriteRange(1023)`, `analogWriteFreq(1000)` and duty code 512, approximately 50%. Serial Monitor uses **115200 baud** and prints pin, requested frequency, range and duty code after reset. These are configuration values, not measurements. The ESP8266 core implements software PWM; timing/jitter still require observation. No Wi-Fi application is started.

Official sources: [ESP8266 analog output API](https://arduino-esp8266.readthedocs.io/en/3.1.2/reference.html#analog-output), [Boards Manager index](https://arduino.esp8266.com/stable/package_esp8266com_index.json). Explicit range selection matters because newer cores default to 0–255 rather than 0–1023.

## Wiring Gate — Confirm Before Upload

Power off before changing connections. **Remove GPIO21's former static GND/3V3 test connection** before attaching an actively driven PWM source.

| Connection | Required state |
|---|---|
| Blue B D1 / GPIO5 → black A GPIO21 / CH0 | PWM output to analyzer input only |
| Blue B GND → black A GND | Shared reference; User confirmed shared-ground wire before upload |
| Black A GPIO22 → A GPIO23 | Keep existing analyzer-local clock jumper |
| Both USB cables | Separate board power; do not join 5V/3V3 outputs |

All logic is compatible 3.3 V. No direct 5 V or negative input. Black A keeps its successful LogicAnalyzer firmware unchanged. The board has a D1 header label; it must not be confused with GPIO1 or the analyzer's GPIO5.

## Arduino IDE and PulseView

1. Open this same-name sketch folder in Arduino IDE 2.3.10, select the ESP8266 board and **confirmed blue-board port**, then Verify.
2. Upload only after the user confirms the port and shared ground. Do not modify analyzer firmware. Open B's Serial Monitor at 115200 and reset B if its startup message was missed.
3. In PulseView select the **real ESP32 Logic Analyzer v0.31**, ols, **COM3 / 921600 / 8N1**. Close any competing monitor on COM3; B's separate monitor can remain open.
4. Enable CH0, request **1 MHz**, **10 k samples**, no trigger, RLE off, External Clock off, Pattern=None. Run one acquisition; after USB reconnection release/reselect the real device if an old connection is stale.
5. Measure multiple adjacent rising edges: period `T`, frequency `1/T`; measure HIGH duration `tH`, duty `100*tH/T`. Target values are approximately **1 ms period**, **0.5 ms HIGH** and **50% duty**, not pre-filled observations. The nominal 10 ms record should contain about ten target cycles if the timebase is correct; the analyzer's configured rate is not independent calibration.
6. Save an original `.sr`, a genuine waveform/cursor screenshot, a real two-board wiring photo and B's startup Serial Monitor output. Keep actual settings and observed variation. Do not label captures completed until genuine data exists.

## Actual PWM Record — Verified Capture, Uncalibrated Timebase

| Blue-board COM / package / upload evidence | Common GND / static tie removed | Requested capture rate / samples | Observed period(s) | HIGH time / duty | Evidence |
|---|---|---|---|---|---|
| COM4, ESP8266 Core 3.1.2; ESP8266EX identified, upload hash verified | User confirmed | 1 MHz / 10,000 samples | 999–1000 samples ≈ 0.999–1.000 ms | HIGH 500–501 samples; duty 50.00–50.10% | [Original capture / analysis](../../evidence/task09-esp8266-pwm.md) |

## Software Validation

ESP8266 Community **3.1.2** installed from the official index and compiled for `esp8266:esp8266:d1_mini`: **PASS**. Compiler output: 28392 bytes global RAM, 61183 bytes instruction RAM including reserved cache, 237892 bytes flash code. Upload to user-confirmed **COM4** identified ESP8266EX / 4 MB flash and returned `Hash of data verified`; startup parameters were read at 115200 baud. Black ESP32 / COM3 firmware was not flashed or modified.

The real capture has 10,000 16-bit samples and nine complete rising-edge-to-rising-edge cycles. Calculated time/frequency uses the configured 1 MHz analyzer rate; no independent timebase calibration or long-duration jitter/reliability test was performed. Existing ESP32 PWM reference remains a separate untested alternative.
