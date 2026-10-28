# Task 9 A — Source and Software Validation Record

**Software preparation record, now supplemented by a real static acquisition stage.** [LOW/HIGH originals, replay screenshots and checks](evidence/task09-logic-analyzer/README.md) document subsequent tests; PWM/I2C/timebase/class decision remain Pending. [Chinese manual](task09-logic-analyzer-guide-zh.md) · [English decision record](scope-decision.md).

## Version and Dependencies

Reviewed 2026-10-28: the requested repository's fetched default-branch HEAD was **5c4f2b9fd85ac09aca3e42596e6e7e0b640cdab3**. Use this commit, not an assumed release number. [Upstream README](https://github.com/lmcapacho/ESP32_LogicAnalyzer/blob/5c4f2b9fd85ac09aca3e42596e6e7e0b640cdab3/README.md) states Arduino Core 3.3.7 and PlatformIO `espressif32@6.11.0` as tested environments; those are separate environments, not equivalent version numbers. The installed Arduino Core is **3.3.12**. We checked this version directly instead of downgrading it. PlatformIO and Core 3.3.7 were not installed/tested in this session.

The complete upstream `ESP32_LogicAnalyzer` sketch folder contains the .ino, LogicAnalyzer.cpp/.h, config header and I2S DMA/RLE sources. It uses Arduino and ESP32-supplied low-level I2S/GPIO/DMA/LEDC APIs, with no additional third-party Arduino library declared. It targets classic ESP32 register layout; an S3/C3 board is not an assumed replacement. Firmware is GPL-3.0; source remains upstream, with only a small attributable patch stored here. No upstream screenshot/binary is submitted as student evidence.

## Verified Core 3 Issue and Prepared Fix

Upstream `enable_out_clock()` calls `ledcAttach(CLK_OUT, rate, 1)` for every SUMP divider command. The installed `esp32-hal-ledc.c` rejects an already attached pin. Thus a successful source build does **not** establish that a later host-requested rate has changed the physical sampling clock. This is particularly relevant because the host may set a default rate before the user chooses another rate.

The [patch](patches/task09-core3-clock-retune.patch) retains first-call attach and calls `ledcChangeFrequency()` for subsequent changes, then reapplies duty. It modifies only the Core 3 branch; GPIO/configuration and Core 2 branch remain unchanged. [Official LEDC API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/ledc.html) describes retuning. Patch application was checked against the pinned source, and both source variants were compiled. Hardware timing, LEDC quantization, failed retunes and waveform correctness remain **Pending**; the fix is source-reviewed and compile-checked, not runtime proven.

## Build Checks

Commands use the installed Arduino CLI and classic `esp32:esp32:esp32` target. Build directories and downloaded external source were kept in a temporary directory outside this portfolio; no generated firmware was committed. The analyzer uses `:DebugLevel=none`; `_DEBUG_MODE_` stays commented out.

| Build | Result | Flash bytes | Global RAM bytes | Hardware status |
|---|---|---|---|---|
| Pinned upstream, unmodified, Core 3.3.12 | PASS | 286927 | 118488 | Not flashed/tested |
| Pinned upstream + Core 3 retune patch | PASS | 287415 | 118488 | Not flashed/tested |
| Portfolio Task 9 PWM source | PASS | 278043 | 22312 | Not flashed/tested |
| Portfolio Task 9 I2C capture source | PASS | 294480 | 23508 | Not flashed/tested |

```text
arduino-cli compile --fqbn esp32:esp32:esp32:DebugLevel=none --jobs 4 <upstream>/ESP32_LogicAnalyzer
arduino-cli compile --fqbn esp32:esp32:esp32 --jobs 4 Module02/code/task09_pwm_reference
arduino-cli compile --fqbn esp32:esp32:esp32 --jobs 4 Module02/code/task09_i2c_capture_source
```

Flash/global-RAM totals are compiler output, not runtime heap availability or maximum acquisition performance. DMA buffers are allocated dynamically and can still fail in a real run.

## PulseView / Windows Check

The existing Windows executable was found at `D:\PulseView\pulseview.exe`; PE machine type `0x8664` confirms x64. `--version` executed successfully and reported **PulseView 0.5.0-git-e2fe9df**, runtime libsigrok **0.6.0-git-883c2ac**, libsigrokdecode **0.6.0-git-71f4514**. Its hardware-driver list includes **ols — Openbench Logic Sniffer & SUMP compatibles**. The installed I2C decoder files are present. `--help` confirms `-D` and `-d` options. This checks program startup/component availability only; GUI connection, serial transport and real decoding on Windows 11 remain Pending. An unrelated ASIX helper warning occurred during version listing; no ASIX device is part of this setup.

The [libsigrok ols source](https://github.com/sigrokproject/libsigrok/blob/master/src/hardware/openbench-logic-sniffer/api.c) was checked at file blob `988a157abd93913df9f41af7b693ea190370a5e1`: default serial settings are 115200/8n1 unless overridden; supported trigger matches are LOW/HIGH. Therefore explicitly use **921600/8n1** and a simple level trigger. This upstream driver review is not a claim that its whole source equals the installed binary's commit.

The upstream analyzer README's “912600” specification line conflicts with its own command example and `LogicAnalyzerConfig.h`; the configuration is **921600**. UART0/USB-serial carries SUMP binary data; close competing Serial Monitor/Plotter windows on analyzer A. Target T uses a different COM port at 115200. Keep Core Debug Level None and `_DEBUG_MODE_` off.

## Acquisition Limits From Code

The source advertises 20 MS/s, 128000 capture samples and RLE; all are upstream claims/configuration, not measured capability. The current default explicitly configures **16-bit I2S**; its 8-bit setup line is marked not implemented, despite broader README wording. Start with a subset of low channels and short captures rather than assume a verified alternate 8-bit acquisition mode or maximum memory depth. Firmware uses 128000 samples and a 96000-byte RLE buffer; host channel-group accounting may constrain the offered sample count. Use the actual device options, not a promised 128k usable record.

Trigger configuration comments specify only mask/value stage 0; advanced trigger configuration and many flags are ignored. RLE is off for the baseline because compression and flag interactions require separate tests. Transfer over 921600 baud is much slower than acquisition and is not continuous 20 MS/s streaming. The 100 MHz SUMP divider calculation programs a requested LEDC clock; independently compare timebase after any change. Do not interpret digital transitions as a calibrated analog amplitude or rise-time measurement.

Official course Task 9 was re-read at Git blob **4a473d222df687c66bb69b376958517c0dcf24e8**. It still requires actual build/flash, PWM/I2C captures, setup effort and a real class agreement on 1–2 tools; this software preparation alone does not complete it.

## Follow-up: Core 3.3.7 and Actual Static Acquisition

The complete original sketch was also compiled using an isolated Core **3.3.7**, classic Dev Module, Debug=None: **PASS**, 298391 bytes flash / 118440 bytes global RAM. The user confirmed successful Arduino IDE compilation/upload and `Hash of data verified`; an original upload screenshot was not supplied. The board's firmware was user-reported as official/unmodified, identity ESP32 Logic Analyzer v0.31. No assistant reflash occurred during recovery or LOW/HIGH acquisition.

COM3 CP210x / 921600 baud SUMP transfers and real PulseView captures now verified static CH0 LOW/HIGH with 10,000 samples each. Both .sr files were preserved unchanged and reopened. The earlier Pending host-connection statements above describe the software-only preparation phase; [the stage report](scope-decision.md) supersedes them for this specific static test, not for I2C decoding or timebase calibration. Later local robustness fixes compiled under Core 3.3.7 (299227 bytes flash / 118448 bytes global RAM) but were not flashed or used for the captured results, and are not included in this evidence commit.
