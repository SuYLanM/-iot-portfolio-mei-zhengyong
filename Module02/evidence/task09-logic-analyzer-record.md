# Task 9 A — Actual Capture and Class Record

**Static CH0 LOW/HIGH and a real ESP8266-source PWM capture verified; I2C NACK-only capture verified; sensor ACK/calibrated timing/class decision Pending.** [Actual PWM results](task09-esp8266-pwm.md). [Preserved real files and verification](task09-logic-analyzer/README.md). The remaining tables below are blank future-work templates, not completed measurements. [English report](../scope-decision.md) · [Chinese manual](../task09-logic-analyzer-guide-zh.md) · [Completed software checks](../task09-analyzer-build.md).

## Completed Static Input Stage

On 2026-10-28, one classic ESP32 with GPIO22→GPIO23 captured GPIO21/CH0 connected to GND, then to 3V3 after power-off. PulseView ols, COM3 CP210x, 921600/8N1, requested 1 MHz, 10 k samples, no trigger, RLE off and External Clock off. Each preserved .sr contains 20,000 logic bytes / 10,000 16-bit samples: LOW run all CH0=0; HIGH run all CH0=1. Only CH0 is validated. Original-firmware capture succeeded without an assistant reflash; optional local fixes were not used. Actual timebase, independent voltage, wiring/upload screenshots and setup elapsed time remain unmeasured/unprovided.

[LOW original](task09-logic-analyzer/pulseview-original-low-10000.sr) · [HIGH original](task09-logic-analyzer/pulseview-original-high-10000.sr) · [Genuine replay screenshots / provenance](task09-logic-analyzer/README.md).

## Further Setup Metadata — Pending

| Date / actual operators | Analyzer A board / COM | Target T board / COM | IDE / Core / firmware commit + patch | Windows / PulseView / libsigrok | Setup start / first usable capture / elapsed |
|---|---|---|---|---|---|
| | | | | | |

| Power / voltage checks | Actual A22→A23 clock jumper | Signal / ground wiring | I2C pull-ups / real sensor model | Actual input protection / boot-pin check | Problems / corrective actions |
|---|---|---|---|---|---|
| | | | | | |

## PWM Measurements — Real WeMos Capture Linked Separately

The actual blue WeMos D1 mini source uses D1/GPIO5; [complete raw data, screenshots and sample-derived measurements](task09-esp8266-pwm.md) record its real acquisition. The blank table below is retained for further tests; the earlier GPIO25 source is an untested ESP32 alternative.

| Run | Requested sample rate | Samples / duration shown | Trigger / channels / RLE | Period measured | f=1/T | High time / duty | Repeatability / missed edges | Screenshot / .sr |
|---|---|---|---|---|---|---|---|---|
| | | | | | | | | |

| Requested-rate change sequence | Same target signal / independent reference | Observed timebase agreement | Retune patch behavior / limitation |
|---|---|---|---|
| | | | |

Without an independent reference, repeated capture of the same configured PWM checks consistency only; it does not establish a calibrated absolute sample rate.

## I2C Decode

[Real ESP8266 NACK-only capture, serial text and decoder screenshots](task09-esp8266-i2c.md) are available. The following blank table remains for future sensor ACK/data tests.

Configured target: SDA21/SCL22, 100 kHz. Analyzer SDA=CH0/GPIO21, SCL=CH1/GPIO19. Record a real sensor only if physically identified; a configured/probed address is not device discovery.

| Run | Real sensor / absent-device condition | Requested rate / samples / trigger / RLE | START / 7-bit address / R-W | Observed ninth-clock ACK or NACK | STOP | Actual SCL period / frequency / method | Serial log / screenshot / .sr |
|---|---|---|---|---|---|---|---|
| | | | | | | | |

## Evidence Files to Create Later

Suggested filenames only; these files **do not exist yet** and are not image links:

- `task09-wiring.jpg`: both boards, pins and A-local clock jumper.
- `task09-build-and-versions.png`: actual firmware/Core/patch and PulseView versions.
- `task09-ols-settings.png`: actual device, COM, serial parameters, sample count/rate and trigger.
- `task09-pwm-cursors.png`, `task09-pwm.sr`: actual waveform and cursor measurement.
- `task09-i2c-decode.png`, `task09-i2c.sr`: original capture plus SDA/SCL and protocol annotations.
- `task09-i2c-target-serial.txt`: actual scanner output; distinguish ACK, NACK and bus errors.

Choose a consistent repository evidence location after files exist; link real paths in the report. Reopen the .sr and verify samples; keep session configuration and decoder text/CSV as supplementary artifacts. Record export limitations rather than substitute a simulated capture. Preserve failed captures when useful.

## Experimental Conclusion and Class Decision — Pending

- Actual measured performance, failures and limitations:
- PWM / I2C pass criteria met, with evidence:
- Actual demo date and participants:
- Class-selected 1–2 tools, roles A/B and reasons:
- Actual setup-instruction owners and agreed next steps:
- Mei Zhengyong's actual contribution:
- Felix's actual contribution:

No final adoption or individual group contribution is pre-filled. The verified static stage above does not fill the PWM/I2C/class-decision templates.
