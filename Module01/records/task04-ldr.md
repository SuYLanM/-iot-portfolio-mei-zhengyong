# Task 4 — Remaining LDR Measurement Record

Status: qualitative hardware response is documented in the [Task 4 report](../evidence/Task04_LDR.md). Its original readings and worked nominal-resistance calculation remain the source of results. The blank tables below are for additional controlled trials and independent checks, not a second copy of those screenshots.

| Metadata | Value |
|---|---|
| Date / operator / board / Core / source settings | |
| Bare LDR model, fixed resistor actual value/tolerance | |
| Measured supply / voltage source used for calculation | |
| Divider orientation / GPIO / wiring photo | |
| DMM specification / voltage uncertainty | |
| Lighting geometry, distance, ambient light, settling time | |
| CSV / plot / Task 5 shared data links | |

Condition/repetition labels specify planned sampling only. Keep actual light conditions reproducible; add room-light and flashlight as distinct conditions when useful.

| Condition | Repetition | Raw code | Ideal estimate V | API mV | DMM V if measured | R estimate ohm | R valid? / assumptions |
|---|---|---|---|---|---|---|---|
| Dark | 1 | | | | | | |
| Dark | 2 | | | | | | |
| Dark | 3 | | | | | | |
| Shade | 1 | | | | | | |
| Shade | 2 | | | | | | |
| Shade | 3 | | | | | | |
| Bright | 1 | | | | | | |
| Bright | 2 | | | | | | |
| Bright | 3 | | | | | | |

| Condition | Raw min/max | Voltage min/max V | Repeatability metric / units | Observed settling behaviour |
|---|---|---|---|---|
| Dark | | | | |
| Shade | | | | |
| Bright | | | | |

An existing worked calculation is in the report. For an independently checked follow-up calculation, TODO insert `R_fixed`, `V_supply`, `Vout`, substitution into `R_LDR=R_fixed*(V_supply−Vout)/Vout`, units and uncertainty/assumptions. Never treat an invalid sentinel of -1 as resistance. Discuss repeatability, nonlinear response, speed and temperature dependence based on observation and actual sensor sources: TODO.
