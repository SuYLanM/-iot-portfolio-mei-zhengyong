# Task 9 A — Actual Capture and Class Record

**Blank / Pending.** [English report](../scope-decision.md) · [Chinese manual](../task09-logic-analyzer-guide-zh.md) · [Completed software checks](../task09-analyzer-build.md).

## Setup Metadata

| Date / actual operators | Analyzer A board / COM | Target T board / COM | IDE / Core / firmware commit + patch | Windows / PulseView / libsigrok | Setup start / first usable capture / elapsed |
|---|---|---|---|---|---|
| | | | | | |

| Power / voltage checks | Actual A22→A23 clock jumper | Signal / ground wiring | I2C pull-ups / real sensor model | Actual input protection / boot-pin check | Problems / corrective actions |
|---|---|---|---|---|---|
| | | | | | |

## PWM Measurements

Configured target: GPIO25, 1000 Hz, 50%; these are design parameters only. Capture on A CH0/GPIO21. The table contains no measured values.

| Run | Requested sample rate | Samples / duration shown | Trigger / channels / RLE | Period measured | f=1/T | High time / duty | Repeatability / missed edges | Screenshot / .sr |
|---|---|---|---|---|---|---|---|---|
| | | | | | | | | |

| Requested-rate change sequence | Same target signal / independent reference | Observed timebase agreement | Retune patch behavior / limitation |
|---|---|---|---|
| | | | |

Without an independent reference, repeated capture of the same configured PWM checks consistency only; it does not establish a calibrated absolute sample rate.

## I2C Decode

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

No final adoption, personal contribution, screenshot or hardware pass is pre-filled.
