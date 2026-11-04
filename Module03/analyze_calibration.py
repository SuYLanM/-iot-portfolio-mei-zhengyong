"""Build the Task 3b result from the original Task 1 CSVs."""
import argparse
import json
from pathlib import Path
from types import SimpleNamespace

import numpy as np

from analyze import calibrate, col, line, plt, read, resistance, save, table


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("manifest", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)

    roles = {10: "fit1", 60: "fit2", 30: "validation"}
    points = []
    for row in read(args.manifest):
        if row["role"] != "ladder":
            continue
        samples = read(args.manifest.parent / row["csv"])
        distance = float(row["distance_cm"])
        raw = col(samples, "raw")
        if np.any((raw == 0) | (raw == 4095)):
            raise ValueError(f"ADC rail code in {row['csv']}")
        points.append(dict(row, role=roles.get(distance, "context"), distance_cm=distance,
                           lux="", raw_mean=float(raw.mean()), n=len(raw),
                           vout_v=float(col(samples, "ldr_mv").mean()/1000)))
    selected = [p for p in points if p["role"] in roles.values()]
    result = calibrate(selected, args.output, SimpleNamespace(kind="ldr"))
    points.sort(key=lambda p: p["distance_cm"])
    measured = resistance(points)
    light = (10 / col(points, "distance_cm"))**2
    mask = np.array([p["role"].startswith("fit") for p in points])
    slope, intercept = line(light[mask], measured[mask])
    result.update(linear_slope_ohm=float(slope), linear_intercept_ohm=float(intercept))
    power = result["A"] * light**(-result["gamma"])
    linear = slope * light + intercept
    table(args.output / "points.csv",
          ["source_csv", "role", "distance_cm", "n", "raw_mean", "adc_v", "nominal_supply_v",
           "nominal_fixed_ohm", "relative_light", "resistance_ohm", "power_ohm", "linear_ohm",
           "power_error_ohm", "linear_error_ohm"],
          ([p["csv"], p["role"], p["distance_cm"], p["n"], p["raw_mean"], p["vout_v"],
            p["supply_v"], p["fixed_ohm"], e, r, pw, ln, pw-r, ln-r]
           for p, e, r, pw, ln in zip(points, light, measured, power, linear)))
    (args.output / "summary.json").write_text(json.dumps(result, indent=2, allow_nan=False)+"\n")
    print(json.dumps(result, indent=2))

    xx = np.geomspace(min(light), max(light), 200)
    fig, ax = plt.subplots()
    ax.plot(xx, result["A"] * xx**(-result["gamma"]), label="Power law")
    ax.plot(xx, slope * xx + intercept, label="Linear")
    for role, marker in (("fit", "s"), ("validation", "D"), ("context", "o")):
        indices = [i for i, p in enumerate(points) if p["role"].startswith(role)]
        ax.scatter(light[indices], measured[indices], marker=marker, label=role)
    ax.set(xscale="log", xlabel="Relative illuminance (10 cm = 1)", ylabel="Estimated resistance (ohm)")
    ax.legend()
    save(fig, args.output, "models")

    fig, ax = plt.subplots()
    distances = col(points, "distance_cm")
    ax.plot(distances, power-measured, "o", label="Power law")
    ax.plot(distances, linear-measured, "x", label="Linear")
    ax.axvline(30, linestyle=":", color="gray", label="Held-out validation")
    ax.axhline(0, color="gray")
    ax.set(xlabel="Distance (cm)", ylabel="Predicted minus observed (ohm)")
    ax.legend()
    save(fig, args.output, "residuals")


if __name__ == "__main__":
    main()
