# Module 02 — Hardware Evidence Register

**Task 9 static CH0 LOW/HIGH captures are now verified:** [original files, replay screenshots and checks](task09-logic-analyzer/README.md). The later [real ESP8266 PWM capture](task09-esp8266-pwm.md) is also available. Other rows remain at their existing status; this register is not a declaration that all labs are complete. [Task checklist](../task-checklist.md).

For each actual experiment record: date, board/module markings, sketch/settings, source/library version, wiring/power/pull-ups, operators and their actual roles, raw observations, calculations, uncertainty/limitations, conclusions and remaining work. Put original pictures in [pictures](../pictures/README.md) and use valid relative links only after files exist.

| Task | Required actual evidence | Result / artefact link |
|---|---|---|
| 1 | Defined divider topology/parts, paired input/output readings and comparison | [Two original voltage photos](task01-voltage-divider/README.md) documented; nodes, resistance values and original 5 V divider proof Pending |
| 2 | Actual assigned bus profile, pitch notes and real shared artefact | Pending assignment/discussion |
| 3 | Shared class table and actual discussion/personal note | [Pod 4 CAN row and research answers](../bus-comparison.md) Prepared; classroom artefact and personal confirmation Pending |
| 4a/4b | Student study/verification, neighbouring-pod agreement and Discord post | Pending |
| 5 | First scan, changed connection scan, address-to-module identification | [Before/after comparison](#sda-disconnection-comparison--2026-11-02): **Completed — LCD scanning and SDA-disconnection comparison**; 0x27 disappears in four full error-free scans; full module removal/restoration not performed |
| 6 | Library value and raw register reconstruction under matched settings | [Report / empty result table](../register-reading.md) Prepared; sensor/library/configuration/readings Pending |
| 7 | A-only/B-only/both scans and actual readings of both devices | [Report / address table](../two-devices.md) Prepared; actual two-device tests Pending |
| 8 | Conflict/fix evidence if hardware available; otherwise explicitly theoretical | Theory prepared; physical experiment Pending |
| 9 | Install/version/pins, PWM/I2C captures, measured timebase and class decision | [CH0 LOW/HIGH verified](task09-logic-analyzer/README.md), 10,000 samples each; [ESP8266 PWM verified](task09-esp8266-pwm.md); [I2C NACK-only capture verified](task09-esp8266-i2c.md); sensor ACK/calibrated timebase/class decision Pending |
| 10 | Optional decoded transaction and measured SCL | Optional / not tested |

## Scanner Report — Task 5 Partial Baseline

**Completed — LCD I²C scanning and SDA-disconnection address-disappearance comparison.** Completion applies to this documented single-LCD variant. Only SDA was disconnected; full module removal, restoration, a sensor-specific exercise and independent voltage measurements are not claimed. This is not an instructor grading/acceptance statement. Earlier blocked-bus/pause records below are historical.

### SDA Disconnection Comparison — 2026-11-02

The student reports powering off, disconnecting **only LCD SDA from ESP32 GPIO21**, leaving LCD VCC=3V3, GND and SCL=GPIO22 connected, then restoring USB. No remote physical manipulation was performed. COM3 remained the CP210x ESP32. The existing Scanner source was unchanged: [fresh compile](task05-i2c-scanning/sda-disconnect/compile-20261102.txt) PASS (294644 flash / 23508 RAM bytes); [upload](task05-i2c-scanning/sda-disconnect/upload-20261102.txt) succeeded with Hash of data verified. The board now runs Scanner, replacing the extension firmware on-board; all stored extension and other task files remain unchanged.

| Configuration / original log | Complete sweeps | Address set in every sweep | Per-sweep probes | ACK | NACK | Other errors |
|---|---:|---|---:|---:|---:|---:|
| [Before: LCD SDA connected](ds1302-lcd-extension/scanner-serial-20261102.txt) | 5 (scan 3–7) | {0x27} | 112 | 1 | 111 | 0 |
| [After: LCD SDA disconnected](task05-i2c-scanning/sda-disconnect/serial-20261102.txt) | 4 (scan 2–5) | {} | 112 | 0 | 112 | 0 |

All four new sweeps print **No I2C devices found** and end normally. No BUS_NOT_IDLE or bus-error record occurs. The unchanged scanner only enters its address loop when both sampled inputs are HIGH, so these complete sweeps demonstrate **SDA/SCL HIGH at each pre-scan idle check**. The log does not print the HIGH values separately, and no voltage or continuous waveform measurement is claimed. The internal SDA pull-up may maintain the now-isolated ESP32 input; this does not validate an external resistor value or qualify the pull-up network for arbitrary hardware.

`disappeared = {0x27} − {} = {0x27}`. This reproduces the expected address disappearance when the LCD's data connection is broken, strengthening the address-to-LCD association. It does **not** mean the LCD was unpowered, all its wires were removed, DS1302 became an I2C device, or no physical peripheral remains attached to SCL. A power-off SDA-only disconnection was sufficient to make the LCD unable to ACK on the controller's data line.

The new serial file preserves received bytes from a bounded 13-second read at 115200 8N1; capture starts after boot, so scan 1 is not included. All four counted BEGIN/END pairs were validated for 112 probes, zero detected ACKs, 112 NACKs and zero errors. [Verification and SHA-256 record](task05-i2c-scanning/sda-disconnect/verification-20261102.json) compares both originals. No screenshot, reconnection, further physical change or full-removal result was fabricated. Task 6 and Task 9 files were not changed.

**Completion scope:** the requested LCD scan and SDA-disconnection comparison is **Completed**. The professor's original step says to unplug a sensor and rescan; this LCD/SDA-only adaptation is explicitly documented rather than presented as an exact full-sensor-removal test. Instructor acceptance of the adaptation is not assumed; a literal full-sensor removal/restoration or sensor-specific test remains unperformed. No additional physical test is requested here.

### Latest Scan After Rewiring — 2026-11-02

The student reported LCD VCC→3V3, GND→GND, SDA→GPIO21, SCL→GPIO22, and moved the now-confirmed DS1302 to DAT25/CLK26/CE27 (three-wire, not I2C). No remote physical changes were made. The unchanged Scanner compiled (294644 flash / 23508 RAM bytes), then successfully uploaded to COM3 after USB was restored. The initial port-not-found failure is preserved alongside success logs in the [extension evidence folder](ds1302-lcd-extension/README.md).

The [new original Scanner serial log](ds1302-lcd-extension/scanner-serial-20261102.txt) contains **five complete sweeps, scan 3–7**. Every sweep returned **ACK 0x27**, **112 probed / 1 detected address / 111 NACKs / 0 errors**. The extension later measured **20/20 HIGH on each input**, re-ACKed 0x27 and successfully initialized/read back the LCD controller memory. Together with the known I2C-only LCD topology, these associate 0x27 with the LCD backpack assembly. An ACK alone is not a chip-identification transaction.

The earlier persistent LOW state was absent from these samples. Requested clock remains 100 kHz, not independently measured; no actual bus-voltage measurement or unique root-cause proof was obtained. The changed physical wiring preceded the improvement; no forced recovery pulses were used. At this earlier connected-only stage, no disconnection comparison had been performed. The later SDA-only comparison above now verifies address disappearance; full module removal/restoration remain unperformed. [DS1302 + LCD extension report](ds1302-lcd-extension/README.md) is separate from the unchanged Task 6 sensor report.

### Historical Blocked Attempt and Pause

**Attempted — Hardware I²C bus blocked (SDA/SCL LOW).**

At the earlier checkpoint, hardware testing was paused at the student's request on 2026-11-02; the later rewired run above supersedes that pause. The existing circuit is being retained; no further rewiring, upload, recovery or scan is planned for this stage. The isolation instructions below are a possible future diagnostic step, not an action performed or currently requested. All authentic logs, screenshots and software-diagnostic results are preserved. The LCD address, ACK and device-removal comparison remain unverified.

**Compile/upload/serial diagnostics Completed; LCD address discovery Blocked; removal comparison Pending.** The actual 2026-11-02 run found both configured bus inputs LOW before any address probe. This is not a successful scan, a NACK result, or evidence that no LCD is present. [Scanner source and instructions](../code/task05_i2c_scanner/README.md) · [Chinese wiring guide](../wiring-zh.md).

### Objective

Use an ESP32 to scan ordinary 7-bit I2C addresses and identify the LCD1602 PCF8574 backpack's actual ACK address. After discovery, power off and remove the LCD, repeat the scan and compare address sets. No RTC display extension is included in this run.

### Hardware

| Item | Confirmed information / limit |
|---|---|
| Controller | Upload tool identified ESP32-D0WD-V3 revision v3.1; ESP32 Dev Module build target |
| Software | Windows 11; Arduino IDE 2.3.10; installed Espressif Arduino Core 3.3.12; Arduino CLI compile/upload |
| Port | COM3, Silicon Labs CP210x USB to UART Bridge; console 115200 baud, 8N1 |
| Display | Student identifies LCD1602 with PCF8574 backpack; actual ACK address unknown |
| Supply | Student confirms LCD VCC connected to ESP32 3V3; ESP32 USB powered. This is a wiring statement, not a measured voltage |
| RTC | Student has now confirmed **DS1302**, not DS3231. DS1302 is three-wire, not an I2C target; it is excluded from this test. Physical isolation of its wires remains unverified |
| Physical wiring limits | LCD GND, actual SDA/SCL landing points, old GPIO22–23 jumper and absence of other powered/pull-up paths are not independently verified. No suitable wiring photo or DMM readings are available |

### Wiring

The intended LCD-only connections are **3V3→VCC, GND→GND, GPIO21→SDA, GPIO22→SCL**. These are the required connections, not a claim that all four have been visually checked. No 5 V is intended. Software digital HIGH/LOW readings cannot determine the voltage, pull-up value or electrical safety.

With USB/power disconnected, trace only the relevant four LCD wires. Isolate DS1302 DAT/CLK/RST if attached to the ESP32/shared lines. Remove an obsolete GPIO22–GPIO23 analyzer jumper if present; it is unnecessary here, but its mere presence does not prove a short. Keep unrelated circuits intact unless they actually share or drive these lines. Check for any remaining static-test GND/3V3 lead on GPIO21/22 and any external board driving the bus. Check the backpack pull-ups and their supply rail; do not introduce 5 V pull-ups. A powered, open-drain I2C bus needs suitable pull-ups to a compatible rail.

A PCF8574 can communicate even if LCD text is not visible at 3.3 V. Backlight/contrast/text are not substitutes for observing ACK. If a future setup requires a 5 V backpack or pull-ups, reassess it before GPIO attachment and use appropriate bidirectional I2C level translation where needed.

### Current Preflight — 2026-11-02

The earlier preflight compiled the original scanner (294564 flash bytes / 23508 global RAM bytes) and withheld upload while hardware identity was unresolved. Its genuine [IDE screenshot](task05-i2c-scanning/arduino-ide-preflight.jpg) remains preserved. The latest user clarification identifies the RTC as DS1302 and requests a real LCD-only attempt; the following results supersede the earlier upload-not-attempted status.

| Step | Current status | Real observation |
|---|---|---|
| Source inspection | Completed | Wire.begin(21,22), requested 100000 Hz, Serial 115200, 0x08–0x77 range, operation timeout 50 ms; no assumed LCD address |
| Original source compile/upload | Completed | 294564 flash bytes / 23508 global RAM bytes; COM3 upload finished with Hash of data verified and RTS reset |
| Original firmware serial read | Completed | Five complete diagnostic attempt records, scan numbers 3–7; every attempt reported BUS_NOT_IDLE, no address probes |
| Minimal diagnostic change | Completed | Read each idle input once and print separate SDA/SCL HIGH/LOW states; retained the existing skip-on-non-idle guard |
| Diagnostic build/upload | Completed | Core 3.3.12: 294644 flash bytes / 23508 global RAM bytes; upload Hash of data verified, port COM3 |
| Diagnostic serial read | Completed | Four complete attempt records, scan numbers 3–6; SDA_GPIO21=LOW and SCL_GPIO22=LOW every time |
| Full address sweep / ACK mapping | Blocked | No SCAN_END records or address probes; ACK/NACK totals unavailable, not zero measured responses |
| Removal comparison | Pending | Requires a stable responding LCD first, followed by user-performed power-off removal |
| DS1302 / RTC-LCD extension | Excluded from this run | DS1302 cannot be read with the DS3231/Wire plan; no RTC read or LCD text result is claimed |

Uploading replaced the selected ESP32's on-board firmware with Scanner. No stored LogicAnalyzer source, captures, screenshots or reports were changed. Serial access was closed after recording the diagnostic screenshot; no forced bus-clear pulses or bypass of the idle guard were attempted. The uploaded sketch continues its normal periodic idle checks while powered; disconnect USB before touching wiring.

### I2C Addresses

| Module | Actual 7-bit address | Identification status |
|---|---|---|
| LCD1602 PCF8574 backpack | **Pending / unknown** | No address was probed because the bus was not idle |
| DS1302 | Not applicable | Not an I2C device; do not assign an I2C address |

No 0x27/0x3F default is substituted for an actual observation. Even a later ACK will not independently identify a chip model; controlled LCD-only/removal evidence is needed for mapping.

### Scanner Results

| Run | Raw evidence | Observed attempts | Result |
|---|---|---|---|
| Original scanner, LCD intended connected | [Original serial bytes](task05-i2c-scanning/lcd-precheck-serial-20261102.txt) | 3, 4, 5, 6, 7 | Five BUS_NOT_IDLE records; no address probes |
| Scanner with per-line diagnostic | [Original serial bytes](task05-i2c-scanning/lcd-line-diagnostic-serial-20261102.txt) | 3, 4, 5, 6 | Four records, both inputs LOW; no address probes |

The first capture used a bounded 15-second read; the diagnostic capture used 12 seconds at 115200 8N1 through System.IO.Ports.SerialPort. Files preserve received bytes without invented headers, edited values or line-ending normalization. Capture started after boot and may end with a partial record (the diagnostic file ends with an incomplete `S`); partial records are not counted. `t_ms` is firmware uptime, not a wall-clock timestamp. The JSON [verification record](task05-i2c-scanning/lcd-diagnostic-verification-20261102.json) records hashes and parsed record counts. There are at least three real attempt records per file, but **zero complete 112-address sweeps**. No measured 100 kHz clock is claimed.

![Real Serial Monitor at 115200: repeated SDA GPIO21 LOW and SCL GPIO22 LOW](task05-i2c-scanning/lcd-bus-not-idle-monitor-20261102.jpg)

The screenshot was captured later from live Arduino Serial Monitor and shows attempts 10–16, not the same sequence numbers as the earlier raw capture. Both show the same diagnostic states. It is not a reconstructed display of the text files.

**Historical evidence, not this LCD scan:** the preserved [Task 9 ESP8266 NACK-only experiment](task09-esp8266-i2c.md) and [serial log](task09-esp8266-i2c/source-single-scan-115200.txt) record 0 ACK / 112 NACK / 0 other errors. Its [raw acquisition](task09-esp8266-i2c/pulseview-i2c-esp8266-nack-01.sr) and [decoder screenshot](task09-esp8266-i2c/i2c-nack-detail.jpg) remain untouched. Those NACK counts must not be attributed to this blocked LCD run.

### Device Removal Test

**Completed for the SDA-disconnection variant:** see the [actual Before/After comparison](#sda-disconnection-comparison--2026-11-02). The following earlier full-module removal/restoration plan remains unperformed; it is not the physical procedure used for the successful SDA-only test.

**Earlier full-removal plan — not performed.** After the bus is idle and at least three full LCD-connected scans show a stable address set, the student must power off, remove the LCD's bus/power connections, and restore ESP32 USB power. Capture at least three complete scans again and compare `disappeared = connected_ACK_set − disconnected_ACK_set`. Check idle pull-ups after removing the backpack; a stuck/floating empty bus is not a valid zero-device scan. No physical disconnection or address disappearance was performed here.

### RTC and LCD Communication

**Not performed; outside this LCD-address-only run.** The earlier conditional DS3231 plan (RTClib by Adafruit + Adafruit BusIO, hd44780 by Bill Perry / hd44780_I2Cexp) is not applicable to the now-confirmed DS1302. No combined sketch was created, installed or uploaded; no RTC timestamp, default initial time or LCD text output is claimed. A future DS3231 extension requires an actual I2C RTC, verified electrical compatibility, observed addresses and separately documented library versions. It must not silently set compiler time and present it as an independently read clock.

### Troubleshooting

| Software observation | Supported conclusion / next physical check |
|---|---|
| COM3 opens; compile/upload Hash verified; readable serial text | Toolchain, upload and serial path work; this does not validate LCD wiring |
| Both GPIO21 and GPIO22 repeatedly read LOW before scanning | Bus is not idle at those inputs; inspect actual pin landings, common ground, power, pull-ups and attached devices. Software cannot distinguish missing pull-ups, a short, a miswire or a device holding lines LOW |
| No ACK/NACK lines or SCAN_END | Scanner deliberately skipped address probes. Do not report 112 NACKs or No I2C devices found for these attempts |
| Old GPIO22–23 jumper possibly present | Remove only that obsolete connection with power off if present; do not claim it is the proven cause |
| DS1302 wires possibly attached | Disconnect its signal lines from the shared bus/ESP32 while powered off; DAT/CLK are not SDA/SCL |
| LCD has no text | Not diagnostic of PCF8574 address discovery; verify I2C independently after electrical/wiring correction |

No definite short, 5 V input or particular failed component was proven. Testing was not extended to active recovery or forced probing. Remaining hardware checks are consolidated above; a real wiring photo and, when available, VCC/SDA/SCL voltage measurements would help establish the cause.

### Passive GPIO Diagnostic Follow-up — 2026-11-02

To test whether the earlier LOW reports were a software artefact, a [separate passive diagnostic sketch](../code/task05_gpio_diagnostic/README.md) was compiled and uploaded to the same COM3 ESP32. It waits for serial commands, uses no GPIO push-pull outputs, and performs no explicit bus recovery pulses. Three complete diagnostics were actually run: [first original serial capture](task05-i2c-scanning/lcd-passive-gpio-diagnostic-20261102.txt), [two repeated diagnostics](task05-i2c-scanning/lcd-passive-gpio-repeat-20261102.txt), [SHA-256 / parsed verification](task05-i2c-scanning/lcd-passive-gpio-verification-20261102.json).

| Phase | Actual result in each of three runs |
|---|---|
| GPIO INPUT, no pull-up/pull-down | SDA LOW 100/100, SCL LOW 100/100; InputEn=1, OutputEn=0 |
| GPIO21 internal weak pull-up only | Both LOW 100/100; SDA Pullup=1 |
| Both internal weak pull-ups | Both LOW 100/100; Pullup=1, Pulldown=0 for both |
| Wire.begin(21,22,100000) | Returned true; configured clock=100000; both LOW 100/100; I2C routing, open-drain and input enabled |
| Explicit gpio_input_enable on both pins | Both calls ESP_OK; still both LOW 100/100 |

Compilation PASS (Core 3.3.12, 298152 flash bytes / 23500 global RAM bytes); upload Hash of data verified. An initial build lacked a temporary partitions.csv and failed at partition generation; copying the installed default partition CSV to the temporary build directory and recompiling resolved that build issue. This did not require a Core downgrade or a course partition change.

The installed Core warns that digitalRead on peripheral-owned pins may be inconsistent; the [GPIO API](https://docs.espressif.com/projects/esp-idf/en/v5.5.3/esp32/api-reference/peripherals/gpio.html) requires input enable. The measured configuration and input-only tests here exclude a disabled input path or Wire-only GPIO reading problem as the explanation for these runs. Weak pull-ups were enabled in configuration but could not raise either input; this does not prove a particular short or component failure. Internal pull-ups are not a substitute for qualified external I2C pull-ups.

**Not performed:** address probes, ACK/NACK measurements, bus-clear pulses, removal comparison, RTC/LCD display. The manual scan command was not sent because the bus remained LOW. The board now holds the command-driven diagnostic firmware, not the repeating scanner; no periodic bus transactions are running. Existing scanner source and all Task 9 materials remain unchanged by this follow-up.

**Next physical isolation:** disconnect USB, then detach only signal wiring attached to ESP32 GPIO21 and GPIO22 (also detach an obsolete GPIO22→GPIO23 branch if present). Leave LCD 3V3/GND and unrelated circuits intact. Reconnect USB for a passive weak-pull-up test of the isolated inputs. This physical step is not yet performed. Likely causes to distinguish are a grounding/mislanding/old static-test lead on either signal, an external module loading the lines because of power/ground/wiring or a module fault, and less likely an ESP32 input/board problem. Simply missing external pull-ups is insufficient to explain the inability of enabled weak pull-ups to raise either input without some additional load/fault. No unique cause, actual bus voltage or 5 V exposure was established.

### Evidence

| Artefact | Status |
|---|---|
| Original preflight IDE screenshot | Preserved, linked above |
| Two raw serial captures and parsed SHA-256/count record | Saved and linked above |
| Live 115200 Serial Monitor screenshot | Saved and linked above |
| Actual wiring photograph / electrical measurements | Pending / not available |
| Actual LCD ACK address and three full scans | Blocked by both input lines LOW |
| Power-off LCD removal / address disappearance | Pending |

**Historical blocked-attempt conclusion:** the software was compiled, uploaded and exercised on the real ESP32, but at that earlier stage the LCD-address test was blocked and Task 5 was not complete. The later connected scan and SDA-disconnection comparison above supersede that status within their stated scope. No historical log was overwritten.

Sources: [TI PCF8574](https://www.ti.com/lit/ds/symlink/pcf8574.pdf), [ADI DS1302](https://www.analog.com/media/en/technical-documentation/data-sheets/DS1302.pdf), [ADI DS3231](https://www.analog.com/media/en/technical-documentation/data-sheets/DS3231.pdf), [NXP PCA9306](https://www.nxp.com/docs/en/data-sheet/PCA9306.pdf).
