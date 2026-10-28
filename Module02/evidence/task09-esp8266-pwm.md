# Task 9 A — ESP8266 PWM Dynamic Capture Record

**Real PWM capture verified — 2026-10-28; independent timebase calibration Pending.** [Source and instructions](../code/task09_esp8266_pwm_generator/README.md) · [Task 9 report](../scope-decision.md). Preserved LOW/HIGH files and screenshots are unchanged.

## Setup

| Item | Configuration / evidence status |
|---|---|
| Analyzer A | Black classic ESP32, official LogicAnalyzer v0.31 firmware retained; COM3 CP210x / 921600 baud |
| Source B | Blue WeMos D1 mini / ESP8266, USB-SERIAL CH340 / COM4 confirmed by user |
| Board package | ESP8266 Community 3.1.2; LOLIN(WEMOS) D1 R2 & mini (`esp8266:esp8266:d1_mini`) |
| Source output | D1 / GPIO5, range 0–1023, code 512, requested 1000 Hz and approximately 50% duty |
| Source console | 115200 baud, separate from analyzer transport |
| Wiring | B D1→A GPIO21/CH0; B GND→A GND; A GPIO22→A GPIO23 clock jumper retained |
| Safety / confirmation | User confirmed common ground and removal of former GPIO21 static GND/3V3 tie; separate USB power, no supply-rail interconnection |
| Capture request | 1 MHz, 10 k samples, CH0, no trigger, RLE off, External Clock off, Pattern=None |

Pin wiring/common ground are user-confirmed, not independently photographed or continuity-meter verified. Target parameters are not measured results. No analyzer firmware upload is authorized or required for this experiment.

## Build / Upload / Capture — Actual Evidence

- **Compile PASS:** installed ESP8266 Community 3.1.2, `esp8266:esp8266:d1_mini`; 28392 bytes global RAM, 61183 bytes instruction RAM including reserved cache, 237892 bytes flash code.
- **Upload PASS, blue board COM4 only:** the user explicitly confirmed COM4 and wiring before upload. esptool identified ESP8266EX / 4 MB flash, wrote the binary and reported `Hash of data verified`. No upload or firmware change was made to black analyzer A / COM3.
- **Real startup output:** [115200-baud application text](task09-esp8266-pwm/source-serial-115200.txt), read through COM4 after reset. Preceding reset bytes were omitted; only the received application message is saved. It confirms requested parameters, not measured frequency.
- **Original data:** [PulseView `.sr`](task09-esp8266-pwm/pulseview-pwm-esp8266-10000.sr), copied unchanged from the original local acquisition. ZIP integrity passed; metadata has 16 probes, 2 bytes/sample, 1 MHz; logic data has **20,000 bytes / 10,000 samples**. Only CH0 was driven and analyzed; the other stored channels are not validated.
- **Real screenshots:** live capture overview below, plus a separately labelled replay of the same saved file. No synthetic PWM diagram or Demo acquisition is used.
- **Timebase:** requested/stored 1 MHz, **not independently calibrated**. No frequency counter or independent scope was used.

| Run | Actual samples | Rising-edge period(s) | HIGH duration(s) | Frequency from capture timebase | Duty cycle | Variation / missed edges / evidence |
|---|---|---|---|---|---|---|
| Original acquisition | 10,000 | 999–1000 samples = 0.999–1.000 ms at configured rate | 500–501 samples = 0.500–0.501 ms | 1000.000–1001.001 Hz per complete cycle | 50.000–50.100% | Nine complete cycles; edge list in [measurement JSON](task09-esp8266-pwm/pwm-measurements.json) |

Rising edges (zero-based sample index): **729, 1729, 2728, 3728, 4728, 5728, 6728, 7727, 8727, 9727**. There are nine complete rising-to-rising intervals; partial cycles at record boundaries are excluded. Mean period is **999.7778 samples ≈ 0.999778 ms**, giving approximately **1000.22 Hz** from the inverse mean period. Mean HIGH duration is **500.3333 samples**, and the mean of the nine per-cycle duty ratios is **50.0445%**.

Example independently recountable within the original file: rising index 729, falling index 1229, next rising index 1729 → period 1000 points, HIGH 500 points → 1.000 ms and 50.000% using the configured rate. These are calculations from actual sample indices, not manually claimed cursor measurements. At this rate one sample represents a nominal 1 µs; short-record quantization, source software PWM and analyzer timing can all contribute to observed variation. This one record does not prove long-term jitter bounds or zero missed edges.

Archive SHA-256: `94cdc3d80c2f904140df032849705de47dfa47da1441f5b6f7dc0026c5ab0526`. [Full edge indices and per-cycle calculations](task09-esp8266-pwm/pwm-measurements.json).

## Genuine Screenshots

Live PulseView acquisition with the real ESP32 device selected, 1 MHz and 10 k samples visible:

![Real PulseView acquisition of WeMos ESP8266 D1 PWM on CH0](task09-esp8266-pwm/pwm-live-overview.jpg)

Replay of the same saved `.sr`, reopened successfully in PulseView (not another acquisition):

![Actual PulseView replay of the preserved ESP8266 PWM capture](task09-esp8266-pwm/pwm-replay.jpg)

A real two-board wiring photograph and original Arduino IDE/Serial Monitor screenshots have **not** been supplied. Wiring/common ground are user-confirmed; upload and startup text were observed through CLI/serial tools. Do not substitute diagrams or screenshots from upstream for those missing photographs.

For a genuine capture, use multiple adjacent rising edges: `period=(next_index-index)/configured_sample_rate`; `duty=HIGH_samples/period_samples`. Report raw sample counts as well as time conversions. Unless independently calibrated, timing/frequency is an estimate based on the configured analyzer rate; both source and analyzer clock errors can contribute. ESP8266 uses software PWM, so observed jitter and load effects must be recorded rather than assumed absent.

## Conclusion and Remaining Work

Under the stated setup, the ESP8266 source produced a repeated signal captured by the unchanged ESP32 analyzer, with sample-derived period and duty consistent with the approximately 1 kHz / 50% target. This verifies one dynamic PWM acquisition, not absolute timing calibration or all analyzer channels/rates. I2C capture, long-term repeatability, independent reference comparison, wiring photography and final class decision remain Pending.
