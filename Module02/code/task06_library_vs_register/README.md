# Task 6 — BME280 Library versus Direct Register Reading

**Prepared / Not Hardware Verified.** Requires an additional genuine BME280 sensor; the current LCD PCF8574 and DS1302 are not substitutes. Compiled only; not uploaded and no actual sensor values obtained.

Open the same-name .ino here. ESP32 Dev Module / Core 3.3.12, SDA21/SCL22, requested 100 kHz, Serial 115200. Dependencies installed: Adafruit BME280 Library 2.3.0, Adafruit BusIO 1.17.4, Adafruit Unified Sensor 1.1.15. Compile PASS: flash 304116 bytes, RAM 23700 bytes.

Future hardware must have verified 3.3 V-compatible module supply, common GND and SDA/SCL pull-ups, and a working bus. No rewiring is requested now. The program refuses a blocked bus before device transactions. Candidate addresses 0x76/0x77 require chip ID 0x60; no match on a working bus prints SENSOR_NOT_FOUND, not fake values.

The library configures temperature x1, humidity/pressure off, filter off. The direct path reads calibration (little-endian, signed T2/T3), triggers a forced conversion with 100 ms bounded status polling, reads the 20-bit ADC code at 0xFA–0xFC and applies Bosch compensation. Library/direct values refer to the same completed sample; raw reads before/after the library call must match. Output includes bytes, ADC code, library_C, direct_C and direct-minus-library difference. Short reads/errors suppress data.

Limitations: the upstream library's initialization calibration poll has no overall deadline, and readTemperature has no independent bus-success flag. Our bounded polling and surrounding checks do not change those API limits. No numerical agreement or accuracy is claimed before actual hardware validation. See the report for registers, formulas, sources and the empty comparison table.

[Sketch](task06_library_vs_register.ino) · [English report](../../register-reading.md) · [Chinese guide](../../wiring-zh.md#task-6库读取与直接寄存器读取).
