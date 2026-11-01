# Task 6 — Library versus Direct Registers

**Template prepared; sensor model, addresses, registers, library and hardware results Pending.** [Library template](code/task06_library_template/README.md) · [Register template](code/task06_register_template/README.md).

## Configuration Gate

Before enabling either sketch, identify the physical sensor and breakout revision; verify supply and pull-ups; scan its actual 7-bit address; obtain its datasheet, register map and appropriate library version. Fill initialization, conversion-ready checks and units. The supplied -1 address/pointer, empty unit strings and disabled booleans intentionally select no sensor. Compiling a stub does not verify a device driver.

Both existing sketch paths now support classic ESP32 (SDA21/SCL22) and WeMos ESP8266 (D2/GPIO4 SDA, D1/GPIO5 SCL); use the blue signal-source board if retaining the black analyzer firmware. [Chinese Task 5–7 guide](wiring-zh.md).

The direct template supports a 1- or 2-byte MSB-first register pointer and 1–4 returned bytes. It does not assume all sensors use this pattern: some need command framing, delayed conversions, checksums or API-managed sequences. Select repeated START versus STOP from the actual transaction diagram. Check every Wire status and short read; never reconstruct an uninitialized buffer or treat zero as a success default.

## Decoding Checklist

1. Confirm 7-bit address, pointer width/order, auto-increment and burst length. A register pointer is not the I2C target address.
2. Confirm byte order before combining; for a two-byte MSB-first field, `u=(uint16_t(b0)<<8)|b1`; reverse only if the map says so.
3. Remove reserved/status bits in the defined order; then sign-extend using the actual field width. Do not sign-cast an arbitrary high byte before knowing the encoding.
4. Apply documented gain/LSB/offset and configuration-dependent range; some sensors need calibration coefficients rather than one multiply.
5. Match library mode, conversion timing, precision and units. Reads occur sequentially, not simultaneously; compare under a stable condition and note latency.

**Datasheet teaching example only, not a configured kit sensor:** [TMP102](https://www.ti.com/lit/ds/symlink/tmp102.pdf) temperature data is MSB-first; normal and extended modes use different field widths, two's complement and a 0.0625 °C LSB. The configured mode must be known before shifting/sign extension. No TMP102 address/register was installed in our template. [VL53L0X](https://www.st.com/resource/en/datasheet/vl53l0x.pdf) illustrates that some ranging devices are better accessed through a documented API sequence than a guessed single temperature-like register.

## Prepared Method and Adapter Boundaries

1. Identify **one real sensor and exact breakout revision**. Confirm voltage/pull-ups, get its primary datasheet and register map, then complete Task 5 ACK/address identification. Existing NACK evidence proves none of these sensor-specific details.
2. Select and record an actual Arduino library/version and the physical quantity/unit. In `task06_library_template`, replace `beginSensorLibrary()` and `readSensorLibrary()` with that library's real API and failure handling; set verified `DEVICE_ADDRESS`, `VALUE_UNIT` and mode. Only then enable `SENSOR_CONFIGURATION_VERIFIED`. Until implemented, the hooks return false and cannot report a fake reading.
3. Configure the sensor identically for the direct run, including any required power-up/reset, range, conversion-ready timing and calibration. The generic direct transport does not initialize an unknown sensor. Add those device-specific steps only from its datasheet; do not rely on an earlier sketch's persistent mode without checking it.
4. In `task06_register_template`, fill actual device/pointer, pointer width, read length and transaction sequence before enabling `DEVICE_AND_REGISTER_VERIFIED`. Its optional **linear-field decoder** requires `DECODING_VERIFIED`, byte order, bit shift/width, signedness, scale, offset and a unit. Defaults deliberately prevent decoding. Calibration-coefficient devices and command/checksum protocols require an adapted driver, not guessed linear constants.
5. Save library output and direct raw bytes under the same stable condition, with configuration and separate timestamps. Calculate `direct − library`; choose a justified acceptance interval using resolution, conversion latency and sensor/library accuracy rather than imposing a fabricated tolerance. Explain any mismatch before concluding equivalence.

These are complete compilable program scaffolds, **not completed device-specific drivers**. A functional library example cannot be selected honestly until the sensor model/library is known. Compile checks cover the default disabled scaffolds, not a future adapter. No upload, sensor initialization or new reading is claimed.

| Configuration item | Actual verified selection |
|---|---|
| Sensor chip / breakout revision / photo | Pending |
| Datasheet/register-map URL and revision | Pending |
| Actual scanned 7-bit address and voltage/pull-ups | Pending |
| Library / version / class / API / physical unit | Pending |
| Initialization / range / conversion-ready sequence | Pending |
| Register pointer/order/length and bus sequence | Pending |
| Data order / field width / signedness / scale or compensation | Pending |
| Clock request / actual timing, if measured | Pending |

## Actual Comparison — Pending

| Date / model / datasheet revision | 7-bit address | Library/version/mode | Register pointer/bytes | Raw bytes in wire order | Signed reconstruction/formula | Library value/unit | Direct value/unit | Difference / timing / uncertainty | Evidence |
|---|---|---|---|---|---|---|---|---|---|
| | | | | | | | | | |

TODO: successful library and direct runs, matching configuration, error handling and a reasoned explanation of any difference. No measured agreement is claimed.

## Report Conclusion Template — Awaiting Hardware

- Quantity/units and configuration compared: Pending.
- Real library readings and direct raw reconstruction: Pending.
- Signed difference, acceptable interval and explanation: Pending.
- Evidence links and actual errors: Pending.
- Personal interpretation and limitations: Pending; no measured agreement is asserted.

Reuse the existing comparison table above; a new duplicated Task 6 report is not needed.

## Hardware Still Needed — Examples, Not Assigned Inventory

Task 5/6 needs at least one identified, 3.3 V-compatible I2C **sensor module** with a documented register map and a suitable library. Task 7 needs two sensor devices at distinct verified addresses. A possible teaching choice is a TMP102 breakout with accessible address selection: [TI TMP102 datasheet](https://www.ti.com/lit/ds/symlink/tmp102.pdf), sections 6.3.4 and Table 6-4, documents four address options. Two such modules with reviewed different address-pin settings could support Task 7; they are not confirmed owned, wired or scanned. Chip supply limits do not replace breakout power/pull-up checks. No model or register configuration has been enabled in the templates.
