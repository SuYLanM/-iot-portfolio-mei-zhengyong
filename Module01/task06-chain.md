# Task 6 — VL53L0X Signal Chain

Status: literature-based preparation diagram; **no VL53L0X wiring or physical test claimed**. This extends the sensor choice already researched in [Module 0](../00-pre-study.md#31-vl53l0x-time-of-flight-distance-sensor), rather than replacing that work.

Source: [ST VL53L0X datasheet](https://www.st.com/resource/en/datasheet/vl53l0x.pdf). TODO confirm the actual breakout board, regulator, pinout and electrical limits before any future wiring.

```mermaid
flowchart TD
  A[Physical quantity: target distance] --> B[940 nm light emitted and reflected]
  B --> C[SPAD photon detection and internal ToF ranging]
  C --> D[Internal signal processing / ranging configuration]
  D --> E[Digital I2C interface]
  E --> F[ESP32 reads distance and validity/status]
  F --> G[MCU checks validity, filters and applies a presence rule]
  G --> H[System: workstation status / optional future logging]
  H --> I[Optional WS2812 status light via suitable power/interface]
  X[Reflectance, geometry and ambient IR] -. error/noise .-> B
  Y[Cover glass / crosstalk / timing budget / offset] -. error/noise .-> D
  Z[Wiring / voltage levels / address / lost transactions] -. communication errors .-> E
  Q[Stale values / invalid status / thresholds / filter latency] -. interpretation errors .-> G
```

| Stage | Explanation |
|---|---|
| Measurand / transduction | Optical time-of-flight uses emitted light and detected photons to estimate distance; classification: active optical sensor, digital output |
| Conditioning/conversion | Detection, ranging and associated processing occur inside the sensor; this is not an external LDR divider or an ESP32 GPIO34 ADC measurement |
| Interface | I2C; ST's address-byte notation 0x52 corresponds to common 7-bit address 0x29. TODO verify actual device/address if hardware is used |
| MCU processing | Read result/status, handle invalid or stale data, then filter/threshold only valid readings |
| System use | Proposed workstation presence indication; no deployed system claimed |
| Actuator | Optional future WS2812 indicator; needs reviewed data-level interface, adequate external supply and common ground. No actuator hardware verified |

TODO: explain the chain in my own words; choose one error source and design a real test (e.g. fixed distance, varying target/ambient light). Do not insert numerical accuracy claims without datasheet conditions or actual measurements. The MCU's 12-bit ADC resolution does not determine this digital sensor's ranging accuracy.
