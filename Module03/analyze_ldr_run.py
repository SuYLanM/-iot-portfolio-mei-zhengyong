"""Summarize real Task 1 captures listed in a bench manifest."""
import argparse
import json
from pathlib import Path

import numpy as np

from analyze import col, plt, read, rms, save, spread, table, times


def fit(distance, resistance, output):
    light = (min(distance) / distance) ** 2
    log_r = np.log(resistance)
    slope, intercept = np.polyfit(np.log(light), log_r, 1)
    linear = np.polyfit(light, resistance, 1)
    power_prediction = np.exp(intercept) * light**slope
    linear_prediction = np.polyval(linear, light)
    power_error = resistance - power_prediction
    linear_error = resistance - linear_prediction
    total = np.sum((resistance - np.mean(resistance))**2)
    xx = np.geomspace(min(light), max(light), 200)
    fig, (left, right) = plt.subplots(1, 2, figsize=(10, 4))
    left.loglog(light, resistance, "o")
    left.loglog(xx, np.exp(intercept) * xx**slope)
    right.plot(light, resistance, "o", label="Measured ADC-derived estimates")
    right.plot(xx, np.exp(intercept) * xx**slope, label="Power law")
    right.plot(xx, np.polyval(linear, xx), label="Linear")
    right.legend()
    for ax in (left, right):
        ax.set(xlabel="Relative illuminance (nearest point = 1)", ylabel="Estimated LDR resistance (ohm)")
    save(fig, output, "transfer-fit")
    table(output / "fit-residuals.csv",
          ["distance_cm", "relative_light", "estimated_ohm", "power_residual_ohm", "linear_residual_ohm"],
          zip(distance, light, resistance, power_error, linear_error))
    return {"gamma": float(-slope), "A_ohm": float(np.exp(intercept)),
            "power_rmse_ohm": rms(power_error), "linear_rmse_ohm": rms(linear_error),
            "power_r2_in_ohms": None if total == 0 else float(1 - np.sum(power_error**2)/total),
            "linear_r2_in_ohms": None if total == 0 else float(1 - np.sum(linear_error**2)/total)}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("manifest", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    runs, ladder, repeats = [], [], {}
    fig, ax = plt.subplots()
    for row in read(args.manifest):
        data = read(args.manifest.parent / row["csv"])
        t, raw, voltage = times(data), col(data, "raw"), col(data, "ldr_mv") / 1000
        if len(raw) < 100 or not np.array_equal(col(data, "sample"), np.arange(len(raw))):
            raise ValueError(f"Missing samples in {row['run']}")
        v_adc = float(np.mean(voltage))
        supply = float(row["supply_v"])
        v_meter = float(row["dmm_vout_v"]) if row["dmm_vout_v"] else None
        vout = v_adc if v_meter is None else v_meter
        clipped = bool(np.any((raw == 0) | (raw == 4095)))
        valid = 0 < vout < supply and (not clipped or v_meter is not None)
        ohms = float(row["fixed_ohm"]) * (supply-vout) / vout if valid else None
        distance = float(row["distance_cm"]) if row["distance_cm"] else None
        if distance is not None and (not np.isfinite(distance) or distance <= 0):
            raise ValueError(f"Invalid distance in {row['run']}")
        stats = {"run": row["run"], "role": row["role"], "distance_cm": distance, "n": len(raw),
                 "raw_min": int(min(raw)), "raw_max": int(max(raw)), "raw_mean": float(np.mean(raw)),
                 "raw_sd": spread(raw)["sd"], "adc_mean_v": v_adc, "dmm_vout_v": v_meter,
                 "supply_v": supply, "supply_basis": row["supply_basis"], "fixed_ohm": float(row["fixed_ohm"]),
                 "resistance_ohm": ohms, "adc_rail_code_observed": clipped, "notes": row["notes"]}
        runs.append(stats)
        if distance is not None and row["role"] == "ladder":
            ladder.append((distance, vout, ohms))
        if distance is not None and ohms is not None and row["role"] in ("ladder", "repeat"):
            repeats.setdefault(distance, []).append(ohms)
        if row["role"] == "check":
            ax.plot(t, voltage, label=row["run"])
    if ax.lines:
        ax.set(xlabel="Time within capture (s)", ylabel="ESP32 voltage estimate (V)")
        ax.legend()
        save(fig, args.output, "functional-check")
    else:
        plt.close(fig)
    result = {"runs": runs, "ladder_distances": len({row[0] for row in ladder}),
              "fit": None, "repeatability_ohm": {d: spread(v) for d, v in repeats.items() if len(v) >= 3}}
    if ladder:
        fig, ax = plt.subplots()
        ax.plot(*zip(*[(d, v) for d, v, _ in sorted(ladder)]), "o-")
        ax.set(xlabel="Flashlight distance (cm)", ylabel="Output voltage (V)")
        save(fig, args.output, "distance-voltage")
    eligible = [(d, r) for d, _, r in ladder if r is not None]
    if len({d for d, _ in eligible}) >= 3:
        result["fit"] = fit(*np.array(eligible).T, args.output)
        result["fit"]["n"] = len(eligible)
    table(args.output / "run-summary.csv", list(runs[0]), (row.values() for row in runs))
    report = json.dumps(result, indent=2, allow_nan=False)
    (args.output / "summary.json").write_text(report + "\n", encoding="utf-8")
    print(report)


if __name__ == "__main__":
    main()
