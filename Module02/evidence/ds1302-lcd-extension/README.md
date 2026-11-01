# DS1302 + LCD1602 — Extension Experiment

**Actual hardware run, 2026-11-02.** This is a three-wire RTC plus I2C display extension, **not** the professor's Task 6 I2C sensor/library/register comparison. [Sketch](../../code/ds1302_lcd_extension/ds1302_lcd_extension.ino) · [Task 5 scanner report](../README.md#latest-scan-after-rewiring--2026-11-02) · [Task 6 remains unchanged](../../register-reading.md).

## Hardware and Software

ESP32 Dev Module / COM3 CP210x, Windows 11, Arduino IDE 2.3.10, Espressif Core 3.3.12; Serial 115200 baud, 8N1. Installed libraries: **Rtc by Makuna 2.5.0**, **hd44780 by Bill Perry 1.3.2**; [actual library inventory](library-versions-20261102.txt).

| Peripheral | ESP32 connection reported by student |
|---|---|
| LCD1602 PCF8574 VCC / GND | 3V3 / GND |
| LCD SDA / SCL | GPIO21 / GPIO22 |
| DS1302 VCC / GND | 3V3 / GND |
| DS1302 DAT / CLK / RST (CE) | GPIO25 / GPIO26 / GPIO27 |

The student physically rewired the assembly before this run; software automation made no physical changes. No VCC/SDA/SCL voltage measurement was made. Internal pull-ups and HIGH input readings are not proof of all electrical conditions. No 5 V supply is intended.

## Actual Execution and Results

The first Scanner upload failed because Windows had no serial port. The [failure log](scanner-upload-20261102.txt) is preserved. After the student restored USB, COM3 CP210x returned; [Scanner upload succeeded](scanner-upload-usb-restored-20261102.txt), with flash verification. No other board was flashed.

1. The unchanged Task 5 Scanner [compiled successfully](scanner-compile-20261102.txt): 294644 flash bytes / 23508 global RAM bytes. Its [raw serial capture](scanner-serial-20261102.txt) contains **five full sweeps**, scan numbers 3–7. Each probes 112 addresses and returns **0x27 ACK, 111 address NACKs, 0 other errors**. There is one responding address. It was not a guessed default.
2. The extension's separate idle checks returned **SDA_HIGH=20, SCL_HIGH=20 out of 20** before LCD initialization and during recorded display writes. Thus the previous persistent LOW state did not recur in these samples. The clock request is 100 kHz; physical frequency was not measured.
3. The extension compiled/uploaded in two iterations. [Initial compile](extension-compile-20261102.txt): 300364 flash / 23580 RAM bytes; [initial upload](extension-upload-20261102.txt) verified. [Readback-enabled final compile](extension-readback-compile-20261102.txt): **300760 flash / 23580 RAM bytes**; [final upload](extension-readback-upload-20261102.txt) verified. Both real serial runs are retained below.
4. LCD address 0x27 re-ACKed (`status=0`); `lcd.begin(16,2)` returned **0**, ready=1. The final program's LCD DDRAM readback returned **read_ok=1, match=1**, row 1 `2026-11-01` padded to 16 columns, row 2 `23:48:59` padded to 16 columns. This verifies controller-memory content, not independent visual legibility/contrast/backlight. No physical-screen photo was supplied or fabricated.

## DS1302 Read-Only Results

| Run / original evidence | Valid reads | First timestamp read | Last timestamp read | Checked progression |
|---|---:|---|---|---|
| [Initial extension serial capture](extension-serial-20261102.txt) | 15 | 2026-11-01 23:47:40 | 2026-11-01 23:47:54 | All 14 adjacent deltas +1 s |
| [Final readback serial capture](extension-readback-serial-20261102.txt) | 10 | 2026-11-01 23:48:59 | 2026-11-01 23:49:08 | All 9 adjacent deltas +1 s, including minute rollover |

Example real DS1302 burst bytes from the final run: `59,48,23,01,11,07,26,00`. The program records raw BCD bytes, validates their fields and the library calendar result, reads the clock-halt/write-protect flags and checks timestamp progression. These are **DS1302 three-wire transactions**, not Wire/I2C register measurements. Raw burst and library reads are sequential, not an atomic combined snapshot.

The observed clock was running and write protection was off. **No date/time, clock-halt, write-protect or trickle-charge setting was changed.** No compile-time date, initial/default time, network time or fabricated zero value was substituted. The date/time above is what the RTC stored; its timezone, alignment with current civil time and calibrated accuracy remain unverified. A valid calendar and one-second progression do not establish correct wall-clock time.

## Program Operation and Limits

Open the same-name sketch folder in Arduino IDE. The final firmware is now on COM3. Sending `g` at 115200 starts read-only RTC sampling about once per second, with LCD output when its actual address ACKs and initialization succeeds. It continues after serial capture closes. Rebooting returns to command-wait mode. It never calls RTC Set methods. If RTC data is invalid it prints raw diagnostics and displays an invalid warning, rather than setting the clock. If LCD communication is blocked/initialization fails, RTC reading continues independently on GPIO25/26/27.

The LCD address constant is **0x27, obtained from the preceding actual Scanner run**. No automatic search of unrelated addresses is used by the extension. No DS1302 I2C address is assigned. The extension does not set the RTC even if it is stopped/invalid; setting it would be a separate user-authorized action.

## Course Status

- **Task 5:** address scanning and LCD address association now have genuine positive evidence. Power-off removal/address disappearance remains **Pending**, so the entire task is not declared complete.
- **Extension:** read-only DS1302 calendar/progression, LCD ACK/initialization and DDRAM write/readback **verified in software logs**; visual screen confirmation and calibrated timing remain **Pending**.
- **Task 6:** the BME280 example/report remain unchanged. Real I2C sensor-library values, direct register reconstruction and their comparison are still **Pending**. This RTC/display extension does not replace them.
- Task 9 originals, other tasks and group-member information were not changed.

## Evidence Integrity and References

Original serial bytes are retained, including any partial final record; only complete records are counted. CLI output preserves failed/successful upload and compile attempts. [Verification JSON](verification-20261102.json) records parsed scan counts, timestamp windows, progression and SHA-256 hashes. No synthetic screenshots or sensor values were created.

Sources: [Makuna DS1302 API](https://github.com/Makuna/Rtc/blob/master/src/RtcDS1302.h), [Makuna ThreeWire example](https://github.com/Makuna/Rtc/blob/master/examples/DS1302_Simple/DS1302_Simple.ino), [hd44780 I2Cexp example](https://github.com/duinoWitchery/hd44780/blob/master/examples/ioClass/hd44780_I2Cexp/HelloWorld/HelloWorld.ino), [ADI DS1302 datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/DS1302.pdf). The upstream RTC example's automatic time-setting code was deliberately not used.
