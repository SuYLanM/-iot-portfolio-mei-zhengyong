# Task 2 — Candidate Bus Research (Assignment Pending)

No bus has been assigned or chosen for Mei Zhengyong and Felix in the supplied information. These are preparatory notes, not seven completed pod presentations. After actual assignment, use [the profile template](bus-profile-template.md) to create **bus-<name>.md**, choose the relevant evidence and keep it to about one page. [Comparison table](bus-comparison.md) covers wiring, rates and diagnosis.

| Candidate | History/background and design purpose | Distinctive feature / relation to other buses | Electrical / addressing notes | Example and diagnosis |
|---|---|---|---|---|
| RS-232 | EIA, 1962: DTE/modem interconnection [R5](references.md#r5-rs232) | Legacy connector/handshake roles survive even when only TX/RX are used | Bipolar single-ended, no device address, transceiver before MCU | Legacy instrument; verify DTE/DCE cable and inversion, not merely baud |
| RS-485 | Standardized balanced industrial electrical interface; exact first standard year Verification Pending [R6](references.md#r6-rs485) | Shared pair does not define messages; Modbus can run above it | Differential, driver enable and termination; no native node addresses | Industrial transceiver needed; inspect A/B convention, stubs, reference/common mode and simultaneous drivers |
| Modbus RTU | Modicon 1979; process-data exchange [R7](references.md#r7-modbus) | Separates register/function semantics from physical network; RTU is not synonymous with RS-485 | Serial framing and checksum above a serial electrical layer; exact standard timing details pending retrieval | Meter example, not kit inventory; check device register numbering, parity, turnaround and CRC |
| CAN | Bosch introduced CAN February 1986 [R8](references.md#r8-can) | Bitwise priority arbitration preserves a winning frame; IDs name messages, not merely recipient addresses | High-speed CAN differential transceiver; two-ended termination | Networked motor drive; check rate, termination, missing ACK, bus-off; UART adapter alone is insufficient |
| I2C / I3C | Philips I2C original release 1982; MIPI I3C v1.0 2016, Basic v1.0 2018 [R4](references.md#r4-i2c-standard), [R9](references.md#r9-i3c-and-spi) | I2C trades speed/rise-time limits for shared wires; I3C adds dynamic addressing and push-pull transfer phases | I2C fixed/configurable addresses and pull-ups; I3C coexistence must satisfy both device specs | Sensor/display candidates, not scanned devices; inspect pull-ups, voltage, ACK and stuck LOW; Wire is not an I3C controller |
| SPI | Motorola lineage; precise origin year Verification Pending [R9](references.md#r9-i3c-and-spi) | Fast shifted data with explicit CS and clock; no universal packet/address scheme | Normally push-pull; inactive MISO must release; CPOL/CPHA per target | ADC/SD candidate; wrong mode or two selected MISO drivers can corrupt data |
| OneWire / 1-Wire | Dallas/Maxim device-family interface; exact first-release date Verification Pending [R10](references.md#r10-sensor-and-switch-datasheets) | Unique 64-bit ROM identifiers allow discovery; parasite power complicates signal/power sharing | Open-drain DQ plus ground, often VDD too; strong pull-up may be needed | DS18B20 documented example only; inspect reset/presence, rise times, power during conversion |

## Every Pod: I2C Open-drain Question

No target should actively drive SDA HIGH in ordinary I2C. Several targets can release the shared wire while one pulls LOW. Pull-ups, combined capacitance, voltage compatibility and sink-current limits determine whether that works physically. A defective device holding SDA/SCL LOW can block the whole segment. Changing the software address does not cure a stuck line. See [study log](study-log.md).

## Verification and Publication Gate

The table deliberately avoids universal cable lengths/rates. Hardware-specific maxima, transceiver common-mode limits, exact kit part numbers and disputed history dates remain **Verification Pending** until matched to the actual parts and primary documentation. Do not infer kit ownership from an example.

Official Modbus legacy PDF retrieval failed; consult the current specification portal before writing an RTU implementation. No Modbus timing parameter here is a tested configuration.

Next: record the actual assignment, prepare one page, rehearse the [pitch](bus-pitch.md), collect other pods' notes, and post an actually agreed artefact to Discord. None of those classroom actions is claimed complete.
