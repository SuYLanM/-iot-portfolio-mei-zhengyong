# Task 9 — Scope / Logic Analyzer Decision Record

**Research and test plan prepared. No class decision, external firmware build, flashing or capture has occurred.** Proposed pairing for evaluation: digital SUMP analyzer plus browser analog scope. Final adoption and ownership of instructions must be agreed by the class, not inferred from this document. [Reference/source register](references.md#r11-instrument-candidates).

## Pinned Candidates, Not Tested Instruments

| Item | A: ESP32_LogicAnalyzer + PulseView | B: ESP32 Web Oscilloscope |
|---|---|---|
| Source | [lmcapacho/ESP32_LogicAnalyzer](https://github.com/lmcapacho/ESP32_LogicAnalyzer/tree/5c4f2b9fd85ac09aca3e42596e6e7e0b640cdab3) | [GUELORD-MWENDERWA/esp32-web-oscilloscope](https://github.com/GUELORD-MWENDERWA/esp32-web-oscilloscope/tree/2d4c39d7568e394391ee14a5b4629574d7ae76ba) |
| Commit reviewed | 5c4f2b9fd85ac09aca3e42596e6e7e0b640cdab3 | 2d4c39d7568e394391ee14a5b4629574d7ae76ba |
| Role | Digital bus capture, SUMP transport to host decoder | Single-channel analog waveform display in browser, not an I2C decoder |
| Project claims / configuration | Up to 20 MHz capture, 8/16-bit operation, 128000 samples configured, RLE; none measured here | 100 S/s–50 kS/s design targets, 12-bit ADC, 512-sample frames, triggers/prehistory; none measured here |
| Toolchain stated upstream | Arduino Core 3.3.7 or PlatformIO espressif32@6.11.0, debug output off | PlatformIO espressif32@6.10.0, Arduino Core 2.0.x; local Core 3.3.12 is not a directly compatible replacement |
| Host / install | PulseView + ols/SUMP driver, USB-serial driver, source-configured 921600 baud | PlatformIO source build or a separately verified release binary, browser for Wi-Fi UI; Chromium-family Web Serial if browser flashing is chosen |
| GPIO | Captured channels below; GPIO22/23 reserved for clock loop; LED2 and UART0 used | ADC1 GPIO34 input, GPIO25 1 kHz test output, Wi-Fi active |
| Input limit | Bare GPIO: 3.3 V logic only, no negative or 5 V input; project PCB/level-shifter design is separate and under validation | Positive low-voltage ADC signal only, useful range depends on attenuation/nonlinearity; keep test amplitude comfortably below 3.0 V after verified conditioning |
| Main limitations | Digital levels only; no calibrated analog rise-time/amplitude measurement; claimed maximum rate needs reference tests | Wi-Fi/analogRead timing jitter and ADC error; requested rate is not an independently measured rate; cannot resolve 100 kHz I2C with its claimed analog rates |
| Current status | Source inspected; install/build/flash/PWM/I2C decode Pending | Source inspected; compatibility choice/build/flash/analog validation Pending |

These are project statements or code constants, **not our measured specifications**. The browser project's own README states its interface image uses a simulated capture and its timing figures are unvalidated design targets. No upstream image is imported as student evidence.

## Pin Map and Conflict Gate

Source checked: [LogicAnalyzerConfig.h](https://github.com/lmcapacho/ESP32_LogicAnalyzer/blob/5c4f2b9fd85ac09aca3e42596e6e7e0b640cdab3/ESP32_LogicAnalyzer/LogicAnalyzerConfig.h).

| Analyzer channel | GPIO | Analyzer channel | GPIO |
|---|---|---|---|
| CH0 | 21 | CH8 | 14 |
| CH1 | 19 | CH9 | 27 |
| CH2 | 18 | CH10 | 26 |
| CH3 | 5 | CH11 | 25 |
| CH4 | 4 | CH12 | 33 |
| CH5 | 15 | CH13 | 32 |
| CH6 | 13 | CH14 | 35 |
| CH7 | 12 | CH15 | 34 |

**Reserved on analyzer board A: GPIO22 is CLK_OUT (LEDC output), GPIO23 is CLK_IN; upstream requires 22→23 loopback. They are not probe inputs.** GPIO2 is the capture LED; UART0 is the host transport. Some channels are strapping pins and can affect boot if externally driven; confirm the physical module before using them. The initial plan uses CH0/CH1 rather than those straps.

Proposed two-board capture, only after board/pin/voltage verification:

```text
Target board T (runs our scanner)        Analyzer board A (SUMP firmware)
T GPIO21 SDA -------------------------- A CH0 / GPIO21 (input)
T GPIO22 SCL -------------------------- A CH1 / GPIO19 (input)
T GND -------------------------------- A GND
                                        A GPIO22 output --> A GPIO23 clock input ONLY
```

**Never connect T's SCL to A's GPIO22. Never run the default scanner and this analyzer on the same physical ESP32 with unchanged maps.** Reassigning one side requires source review and a new map; a second board is the simpler evaluation arrangement. Two power supplies need reviewed grounding; do not tie 3V3 outputs together. Actual board availability and physical wiring checks: Pending.

## Installation / Feasibility Notes

1. Keep external source/builds outside this portfolio; checkout the pinned commit and record any patch. No external firmware/binary is bundled here.
2. Digital candidate: install the compatible upstream board package/toolchain, build the original sketch/project and keep debug prints off UART0. Its README specifications have a **912600 typo**; config.h and the PulseView example use **921600**. Confirm that value in the firmware actually flashed. Close Serial Monitor before PulseView opens the same COM port. COM3 is a previous portfolio port, not necessarily the analyzer's current port.
3. Install PulseView from [sigrok](https://sigrok.org/wiki/PulseView), select Openbench Logic Sniffer/SUMP-compatible (ols), configure actual COM port and baud. Official website retrieval timed out during research: current installer version, USB driver and Windows compatibility remain Pending. Do not record them as installed.
4. Analog candidate: upstream platformio.ini pins espressif32@6.10.0. src/main.cpp calls ledcSetup/ledcAttachPin, which were removed in Core 3. Choose a separately installed compatible environment or prepare/test a Core 3 port; neither has been done here. [Espressif migration note](https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html).
5. GPIO34 is ADC1, suitable for a candidate using Wi-Fi, but voltage calibration/nonlinearity remains unverified. For a proposed 3.3 V PWM source use a verified 10k/10k divider to approximately 1.65 V, check node voltage before connecting and evaluate loading/bandwidth. Do not blindly copy the upstream high-voltage/biased input drawing or treat a 0–3.3 V rail test as calibrated amplitude. Viewing an AP-hosted page differs from Web Serial flashing; record browser/OS and actual route used.
6. The web sampler uses a micros()-paced loop; capture metadata is calculated from requested period, not measured timestamps for every ADC conversion. Its main.cpp header mentions a hardware timer, while sampler.cpp implements the polling loop. Inspect working code and measure real timing rather than trusting either label.

## Capture Test Plan

Use [our Core 3 PWM reference](code/task09_pwm_reference/README.md) on the separate target, configured for 1 kHz at GPIO25. The number printed by that sketch is a configured target, not a measured frequency.

- Digital baseline: capture a known PWM signal at several sample settings; measure period/duty stability, missing edges and duration. Start conservatively; check maximum claims against a reference if available.
- I2C: run the scanner at configured 100 kHz and decode SDA/SCL on correct channels. Course rule of thumb: at least 1 MS/s for 100 kHz and 4 MS/s for 400 kHz, subject to real sample reliability and decoder behavior. This is a margin recommendation, not a demonstrated acquisition rate or analog edge-quality guarantee.
- Analog: capture a conditioned PWM/slow RC signal at several requested rates, compare amplitude with a meter and timebase with an independent reference. A 50 kS/s analog claim cannot decode 100 kHz I2C.
- Optional OneWire: actual DS18B20/reset/presence capture if hardware is available. No device ownership or capture is assumed.
- Time actual setup from install to first usable capture; save errors as evidence. If instrument firmware fails, document the failure rather than marking adopted.

| Date / candidate / exact source+patch | Board / pins / input protection | Host OS/browser/version | Requested rate | Independently checked effective rate/timebase | PWM result | I2C decoder result | Setup minutes / failures | Evidence |
|---|---|---|---|---|---|---|---|---|
| Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending | Pending |

## Actual Class Decision — Pending

Provisional research preference: A for bus decoding, B for slow analog exploration. This is **not a class adoption decision**.

- Actual demo/date/participating pods: TODO.
- Agreed one or two solutions and reasons: TODO.
- Maintainers of install instructions: TODO; do not assign contributions to Mei/Felix without agreement.
- Captures, actual performance, limitations and fallback decision: TODO.
- If time is short: record an actual class decision and carry captures into portfolio time, as permitted by the course; no decision is fabricated here.
