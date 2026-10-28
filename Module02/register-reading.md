# Task 6 — Library versus Direct Registers

**Template prepared; sensor model, addresses, registers, library and hardware results Pending.** [Library template](code/task06_library_template/README.md) · [Register template](code/task06_register_template/README.md).

## Configuration Gate

Before enabling either sketch, identify the physical sensor and breakout revision; verify supply and pull-ups; scan its actual 7-bit address; obtain its datasheet, register map and appropriate library version. Fill initialization, conversion-ready checks and units. The supplied -1 address/pointer and disabled booleans intentionally select no sensor. Compiling a stub does not verify a device driver.

The direct template supports a 1- or 2-byte MSB-first register pointer and 1–4 returned bytes. It does not assume all sensors use this pattern: some need command framing, delayed conversions, checksums or API-managed sequences. Select repeated START versus STOP from the actual transaction diagram. Check every Wire status and short read; never reconstruct an uninitialized buffer or treat zero as a success default.

## Decoding Checklist

1. Confirm 7-bit address, pointer width/order, auto-increment and burst length. A register pointer is not the I2C target address.
2. Confirm byte order before combining; for a two-byte MSB-first field, `u=(uint16_t(b0)<<8)|b1`; reverse only if the map says so.
3. Remove reserved/status bits in the defined order; then sign-extend using the actual field width. Do not sign-cast an arbitrary high byte before knowing the encoding.
4. Apply documented gain/LSB/offset and configuration-dependent range; some sensors need calibration coefficients rather than one multiply.
5. Match library mode, conversion timing, precision and units. Reads occur sequentially, not simultaneously; compare under a stable condition and note latency.

**Datasheet teaching example only, not a configured kit sensor:** [TMP102](https://www.ti.com/lit/ds/symlink/tmp102.pdf) temperature data is MSB-first; normal and extended modes use different field widths, two's complement and a 0.0625 °C LSB. The configured mode must be known before shifting/sign extension. No TMP102 address/register was installed in our template. [VL53L0X](https://www.st.com/resource/en/datasheet/vl53l0x.pdf) illustrates that some ranging devices are better accessed through a documented API sequence than a guessed single temperature-like register.

## Actual Comparison — Pending

| Date / model / datasheet revision | 7-bit address | Library/version/mode | Register pointer/bytes | Raw bytes in wire order | Signed reconstruction/formula | Library value/unit | Direct value/unit | Difference / timing / uncertainty | Evidence |
|---|---|---|---|---|---|---|---|---|---|
| | | | | | | | | | |

TODO: successful library and direct runs, matching configuration, error handling and a reasoned explanation of any difference. No measured agreement is claimed.
