# Module 02 — Sources and Verification Register

Reviewed 2026-10-28. Primary references were used for technical claims. Source-checked means read against documentation/source, **not bench verified**. Some pages below could not be fully retrieved; that is recorded rather than represented as successful verification.

## R1 Course

[Official Module 2](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/02-i2c-and-sensor-addressing.md), entire file read at Git blob `25aff30988742c068340f554b1fdf951fac2db24`. Learning goals precede a draft boundary; Tasks 1–9 are marked core and Task 10 is an optional stretcher. Preserve the required pins-and-dividers.md, bus-comparison.md, study-log.md and scope-decision.md deliverables. A final bus-<name>.md awaits actual pod assignment.

## R2 ESP32 Electrical

[ESP32 Series datasheet v5.3](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf), electrical characteristics §5. The I/O domain matters: the DC table lists VIH minimum 0.75×VDD and maximum VDD+0.3 V. The 3.6 V absolute rating in the power-pin table is not a universal recommended GPIO operating voltage. Do not connect 5 V to a bare ESP32 GPIO or assume a whole development board shares every chip limit. ADC useful range is narrower and depends on attenuation. Board/module identity is still to be checked.

## R3 Arduino Wire and UART

- [Arduino Wire reference](https://docs.arduino.cc/language-reference/en/functions/communication/wire/) — official reference retained; the research tool returned only an iframe, so its body could not be independently extracted.
- [Espressif Arduino I2C API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/i2c.html) — reviewed for begin, setTimeOut, requestFrom and endTransmission. Generic ESP32 defaults are SDA21/SCL22; our examples explicitly configure them.
- [Espressif UART API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/serial.html) — reviewed for HardwareSerial and begin(baud, format, rx, tx).
- Installed Core 3.3.12 Wire.h and esp32-hal-ledc.h checked locally for actual signatures. Compilation is separately recorded in validation.md.
- [Core 2-to-3 migration](https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html) — old ledcSetup/ledcAttachPin APIs were removed; new reference generator uses ledcAttach/ledcWrite.

## R4 I2C Standard

[NXP UM10204 Rev. 7.0, 1 October 2021](https://www.nxp.com/docs/en/user-guide/UM10204.pdf). Reviewed transaction/addressing, open-drain behavior, timing and pull-up constraints. Standard/Fast modes are the initial laboratory scope; unusual UFm/Hs rules must not be generalized from simple I2C examples. Original I2C release is dated 1982 in the revision history. An ACK is not device identification or proof of unique physical ownership of an address.

## R5 RS232

[TI SLLA037A: Interface Circuits for TIA/EIA-232-F](https://www.ti.com/lit/an/slla037a/slla037a.pdf). EIA introduced RS-232 in 1962; the legacy DTE/DCE context explains connector/handshake conventions. Receiver decision regions are outside ±3 V; these bipolar/inverted line levels require a transceiver and must never be connected directly to ESP32 logic. Cable/data-rate claims depend on capacitance and implementation, not a universal 15 m guarantee.

## R6 RS485

[TI RS-485 Design Guide SLLA272D](https://www.ti.com/lit/an/slla272d/slla272d.pdf) and [TI SLLA070D](https://www.ti.com/lit/an/slla070d/slla070d.pdf). Reviewed differential signaling, linear-bus topology, termination, stubs, common-mode limits and rate/length trade-offs. The familiar 1200 m figure is a cable-specific low-rate example, not a guarantee at maximum rate. Exact founding year was not checked in these documents: Verification Pending.

## R7 Modbus

[Modbus Organization introduction](https://www.modbus.org/introduction-to-modbus) and [FAQ](https://www.modbus.org/faq): verified Modicon origin in 1979 and distinction between an application protocol and electrical transport. [Official specification portal](https://www.modbus.org/specifications) is available, but legacy direct PDF URLs for Application Protocol V1.1b3 and Serial Line V1.02 returned 404 in this session. Exact RTU timing/address-range/CRC parameter details must be checked in an obtained official specification and the actual instrument manual before implementation; no Modbus driver is supplied here. No third-party mirror is presented as a successfully retrieved official standard.

## R8 CAN

[TI SLOA101B introduction](https://www.ti.com/lit/an/sloa101b/sloa101b.pdf): reviewed classical CAN, message-ID arbitration, transceiver separation and termination. [CiA history](https://can-cia.org/can-knowledge/history-of-can-technology) documents Bosch's February 1986 introduction. Classical CAN, CAN FD and higher-layer CANopen are not interchangeable configurations; ESP32 controller capability alone does not supply a physical transceiver.

## R9 I3C and SPI

- [MIPI I3C/I3C Basic](https://www.mipi.org/specifications/i3c-sensor-specification) and [MIPI FAQ](https://www.mipi.org/resources/I3C-frequently-asked-questions): reviewed dynamic addressing, push-pull phases, coexistence restrictions and SDR clock up to 12.5 MHz. MIPI lists I3C v1.0 (2016), Basic v1.0 (2018). I3C is not an operating mode supplied by the classic ESP32 Wire controller.
- [Analog Devices SPI introduction](https://www.analog.com/en/resources/analog-dialogue/articles/introduction-to-spi-interface.html) and [RTC interface note](https://www.analog.com/en/resources/design-notes/interfacing-spi-realtime-clocks-rtcs-with-a-microcontroller.html): clock phase/polarity, per-device chip select and Motorola SPI lineage. A precise first-publication year is Verification Pending; there is no one universal SPI cable/speed rating.

## R10 Sensor and Switch Datasheets

- [ADI DS18B20 Rev. 6](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf): 1-Wire, unique 64-bit ROM ID, external/parasite power constraints. No DS18B20 is assumed present in our kit.
- [ST VL53L0X DS11555 Rev. 6](https://www.st.com/resource/en/datasheet/vl53l0x.pdf): digital I2C sensor with configurable address, address-byte notation 0x52 (7-bit 0x29). This is a documented example, not a found device.
- [TI TMP102 SBOS397I](https://www.ti.com/lit/ds/symlink/tmp102.pdf): example of two-byte, MSB-first signed data; normal 12-bit versus extended 13-bit mode and 0.0625 °C/LSB. It is an instructional example, not the selected sensor or verified hardware configuration.
- [TI TCA9548A SCPS207H](https://www.ti.com/lit/ds/symlink/tca9548a.pdf): eight switched downstream buses, strap-selected 7-bit address range 0x70–0x77, channel-bit control and deselection/reset behavior. Actual breakout and straps remain Pending.

## R11 Instrument Candidates

- [lmcapacho source snapshot](https://github.com/lmcapacho/ESP32_LogicAnalyzer/tree/5c4f2b9fd85ac09aca3e42596e6e7e0b640cdab3): README, LogicAnalyzerConfig.h and platformio.ini read. 921600 baud in source conflicts with a README specification typo of 912600; use the source and host configuration, then verify.
- [Web scope snapshot](https://github.com/GUELORD-MWENDERWA/esp32-web-oscilloscope/tree/2d4c39d7568e394391ee14a5b4629574d7ae76ba): README, config.h, main.cpp, sampler.cpp and platformio.ini read. README expressly calls timing figures design targets and its interface image a simulated capture. It is not our hardware evidence.
- [PulseView](https://sigrok.org/wiki/PulseView): official page request timed out. SUMP/ols setup is source-supported by the analyzer project, but current host installer/version/driver compatibility remains Verification Pending.

No external firmware was flashed or claimed built. No credentials, downloaded binaries or external project screenshots are included.
