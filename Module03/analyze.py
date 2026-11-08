"""Analyze Module 03 CSVs. See README for units and run preparation."""
import argparse
import csv
import json
import math
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def read(path):
    with path.open(newline="", encoding="utf-8-sig") as stream:
        rows = list(csv.DictReader(line for line in stream if not line.startswith("#")))
    if not rows:
        raise ValueError(f"No samples in {path}")
    return rows


def col(rows, key):
    values = np.array([float(row[key]) for row in rows])
    if not np.isfinite(values).all():
        raise ValueError(f"Non-finite {key}")
    return values


def resistance(rows):
    supply, node, fixed = (col(rows, k) for k in ("supply_v", "vout_v", "fixed_ohm"))
    if np.any((node <= 0) | (node >= supply) | (fixed <= 0)):
        raise ValueError("Require 0 < Vout < Vs and positive fixed resistance")
    return fixed * (supply - node) / node


def light(rows):
    if all(row["lux"].strip() for row in rows):
        values, label = col(rows, "lux"), "lux (meter estimate)"
    else:
        distance = col(rows, "distance_cm")
        if np.any(distance <= 0):
            raise ValueError("Require positive distances")
        values, label = (distance[0] / distance) ** 2, "relative illuminance (first distance = 1)"
    if np.any(values <= 0):
        raise ValueError("Require positive illuminance")
    return values, label


def ldr(rows, out, args):
    x, label = light(rows)
    y = resistance(rows)
    ladder = np.array([r["role"] == "ladder" for r in rows])
    if len(set(x[ladder])) < 6:
        raise ValueError("Use at least six distinct ladder levels")
    log_slope, log_a = np.polyfit(np.log(x[ladder]), np.log(y[ladder]), 1)
    linear = np.polyfit(x[ladder], y[ladder], 1)
    xx = np.geomspace(min(x[ladder]), max(x[ladder]), 200)
    def power(value):
        return np.exp(log_a) * value ** log_slope

    fig, (ax, comparison) = plt.subplots(1, 2, figsize=(10, 4))
    ax.loglog(x, y, "o")
    ax.loglog(xx, power(xx))
    comparison.plot(x, y, "o")
    comparison.plot(xx, power(xx), label="Power law")
    comparison.plot(xx, np.polyval(linear, xx), label="Linear")
    comparison.legend()
    for panel in (ax, comparison):
        panel.set(xlabel=label, ylabel="Resistance (ohm)")
    save(fig, out, "ldr")
    table(out / "ldr-residuals.csv", ["level", "role", "light", "resistance_ohm", "power_residual_ohm", "linear_residual_ohm"],
          zip([r["level"] for r in rows], [r["role"] for r in rows], x, y, y-power(x), y-np.polyval(linear, x)))
    repeat = y[[r["role"] == "repeat" for r in rows]]
    repeat_levels = {r["level"] for r in rows if r["role"] == "repeat"}
    if len(repeat) != 3 or len(repeat_levels) != 1:
        raise ValueError("Enter exactly three repeat rows at one level")
    result = {"light_axis": label, "log_slope": log_slope, "gamma": -log_slope,
              "A": math.exp(log_a), "linear_slope": linear[0], "linear_intercept": linear[1],
              "power_rmse_ohm": rms(y[ladder] - power(x[ladder])),
              "linear_rmse_ohm": rms(y[ladder] - np.polyval(linear, x[ladder])),
              "repeat_range_ohm": float(np.ptp(repeat)), "repeat_sd_ohm": sd(repeat)}
    if all(r["distance_cm"].strip() and r["lux"].strip() for r in rows):
        distance = col(rows, "distance_cm")
        if np.any(distance <= 0):
            raise ValueError("Require positive distances")
        table(out / "illuminance-ratios.csv", ["level", "measured_ratio", "inverse_square_ratio"],
              zip([r["level"] for r in rows], x/x[0], (distance[0]/distance)**2))
    if args.adc_step_v is not None:
        if args.adc_step_v <= 0:
            raise ValueError("ADC step must be positive")
        ref = [r for r in rows if r["role"] == "repeat"]
        node = col(ref, "vout_v").mean()
        supply = col(ref, "supply_v").mean()
        fixed = col(ref, "fixed_ohm").mean()
        result["one_adc_step_ohm_estimate"] = fixed * supply * args.adc_step_v / (node * (node + args.adc_step_v))
    return result


def calibrate(rows, out, args):
    kind = args.kind
    expected = {"fit1", "fit2", "validation"}
    if kind == "temperature":
        expected = {"cold", "indoor", "ice"}
    roles = {r["role"]: r for r in rows}
    if set(roles) != expected or len(rows) != 3:
        raise ValueError(f"Provide one row per role: {sorted(expected)}")
    order = ("cold", "indoor", "ice") if kind == "temperature" else ("fit1", "fit2", "validation")
    rows = [roles[k] for k in order]
    if kind == "temperature":
        raw, ref = col(rows, "sensor_c"), col(rows, "reference_c")
        offset = ref[0] - raw[0]
        return {"offset_c": offset, "indoor_offset_c": ref[1]-raw[1],
                "cold_residual_using_indoor_offset_c": raw[0]+ref[1]-raw[1]-ref[0],
                "ice_error_before_c": raw[2]-ref[2], "ice_error_after_c": raw[2]+offset-ref[2]}
    if kind == "ldr":
        x, label = light(rows)
        y = resistance(rows)
        gain, offset = line(np.log(x[:2]), np.log(y[:2]))
        predicted = math.exp(offset + gain * math.log(x[2]))
        a, b = line(x[:2], y[:2])
        return {"light_axis": label, "A": math.exp(offset), "gamma": -gain, "validation_ohm": y[2],
                "power_prediction_ohm": predicted, "linear_prediction_ohm": a*x[2]+b,
                "power_error_ohm": predicted-y[2], "linear_error_ohm": a*x[2]+b-y[2]}
    raw, ref = col(rows, "raw_mean"), col(rows, "reference_v")
    gain, offset = line(raw[:2], ref[:2])
    if gain <= 0 or not min(raw[:2]) < raw[2] < max(raw[:2]):
        raise ValueError("Use increasing voltage with ADC code and an interior validation point")
    corrected = gain * raw[2] + offset
    baseline = col(rows, "baseline_v")
    fig, ax = plt.subplots()
    ax.plot(raw[:2], ref[:2], "s-", label="DMM calibration and fit")
    ax.scatter([raw[2]]*3, [ref[2], baseline[2], corrected], marker="o",
               c=["black", "tab:orange", "tab:green"])
    for value, label in zip((ref[2], baseline[2], corrected), ("DMM", "ADC API", "Corrected")):
        ax.annotate(label, (raw[2], value), xytext=(5, 0), textcoords="offset points")
    ax.set(xlabel="Mean raw ADC code", ylabel="Voltage (V)")
    ax.legend()
    save(fig, out, "adc-validation")
    return {"gain_v_per_code": gain, "offset_v": offset, "validation_reference_v": ref[2],
            "corrected_v": corrected, "error_after_v": corrected-ref[2],
            "error_before_v": baseline[2]-ref[2]}


def line(x, y):
    if x[0] == x[1]:
        raise ValueError("Fit inputs must be distinct")
    gain = (y[1]-y[0]) / (x[1]-x[0])
    return gain, y[0]-gain*x[0]


def sd(x):
    return float(np.std(x, ddof=1))


def rms(x):
    return float(np.sqrt(np.mean(x*x)))


def table(path, header, rows):
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.writer(stream)
        writer.writerow(header)
        writer.writerows(rows)


def save(fig, out, name):
    fig.tight_layout()
    fig.savefig(out / f"{name}.png", dpi=160)
    plt.close(fig)


def spread(x):
    return {"n": len(x), "mean": float(np.mean(x)), "sd": sd(x), "range": float(np.ptp(x))}


def repeat(rows, out, args):
    if args.kind == "pot-adc":
        trials = col(rows, "trial")
        if len(rows) != 11 or set(trials) != set(range(11)):
            raise ValueError("Record baseline 0 and ten independent returns 1–10")
        if np.any(col(rows, "n") < 100):
            raise ValueError("Require at least 100 ADC samples at each position")
        rows = sorted(rows, key=lambda row: int(row["trial"]))
        x = col(rows, "raw_mean")
        voltage = col(rows, "adc_estimated_v")
        delta = x - x[0]
        table(out / "return-deviations.csv",
              ["trial", "started", "n", "raw_mean", "raw_sd", "raw_min", "raw_max",
               "adc_estimated_v", "delta_from_baseline_codes"],
              ([r[k] for k in ("trial", "started", "n", "raw_mean", "raw_sd", "raw_min",
                               "raw_max", "adc_estimated_v")] + [d] for r, d in zip(rows, delta)))
        fig, ax = plt.subplots()
        ax.plot(range(1, 11), delta[1:], "o-")
        ax.axhline(0, color="gray")
        ax.set(xlabel="Independent return", ylabel="Mean deviation from baseline (ADC codes)",
               xticks=range(1, 11))
        save(fig, out, "pot-returns")
        return {"baseline_raw_mean": float(x[0]), "baseline_raw_sd": float(rows[0]["raw_sd"]),
                "return_raw_means": spread(x[1:]), "return_adc_estimated_v": spread(voltage[1:]),
                "return_deviation_codes": spread(delta[1:]),
                "max_absolute_deviation_codes": float(np.max(np.abs(delta[1:])))}
    if args.kind == "pot":
        if len(rows) != 10:
            raise ValueError("Record ten independent returns to the mark")
        return {key: spread(col(rows, key)) for key in ("reference_v", "raw_mean")}
    if args.kind == "ds":
        groups = {}
        for row in rows:
            groups.setdefault((row["rom"], int(row["bits"])), []).append(row)
        result = {}
        for (rom, bits), group in sorted(groups.items()):
            valid = [r for r in group if r["status"] == "ok"]
            if len(valid) < 2 or bits not in (9, 10, 11, 12):
                raise ValueError("Require at least two valid samples and 9–12 bits")
            stats = spread(col(valid, "temp_c")) | {
                "step_c": 2.0**(8-bits), "invalid": len(group)-len(valid)}
            if "conversion_ms" in valid[0]:
                stats["conversion_ms"] = spread(col(valid, "conversion_ms"))
            result[f"{rom}/{bits}"] = stats
        return result
    y = resistance(rows)
    sweeps = {}
    for row, value in zip(rows, y):
        levels = sweeps.setdefault(row["sweep"], {}).setdefault(row["direction"], {})
        level = float(row["light_level"])
        if level in levels:
            raise ValueError("Enter one settled reading per sweep/direction/level")
        levels[level] = value
    fig, ax = plt.subplots()
    result = {}
    for name, directions in sweeps.items():
        up, down = directions["up"], directions["down"]
        if set(up) != set(down) or len(up) < 2:
            raise ValueError("Pair the same light levels in both directions")
        for direction, levels in directions.items():
            xx = sorted(levels)
            ax.plot(xx, [levels[x] for x in xx], "o-", label=f"{name} {direction}")
        result[name] = {"max_gap_ohm": float(max(abs(up[k]-down[k]) for k in up))}
    ax.set(xlabel="Matched light level (see bench log for units)", ylabel="Resistance (ohm)")
    ax.legend()
    save(fig, out, "hysteresis")
    return result


def times(rows):
    t = col(rows, "t_ms") / 1000
    if len(t) < 2 or np.any(np.diff(t) <= 0):
        raise ValueError("Timestamps must increase within a run")
    return t


def crossing(t, y, threshold, start, rise):
    for i in range(1, len(t)):
        if t[i] < start:
            continue
        a, b = y[i-1:i+1]
        crossed = a < threshold <= b if rise else a > threshold >= b
        if crossed:
            estimate = t[i-1] + (t[i]-t[i-1])*(threshold-a)/(b-a)
            if estimate >= start:
                return float(estimate)
    return None


def step(rows, out, args):
    selected = [r for r in rows if r["rom"].upper() == args.rom.upper()]
    good = [r for r in selected if r["status"] == "ok"]
    t, y = times(good), col(good, "temp_c")
    if not t[0] < args.step_s < args.final_start_s < t[-1]:
        raise ValueError("Supply transfer and stabilized-tail times in seconds")
    initial, final = np.mean(y[t < args.step_s]), np.mean(y[t >= args.final_start_s])
    change = final-initial
    if abs(change) < 0.0625:
        raise ValueError("No resolvable step")
    target = initial + (1-math.exp(-1))*change
    reach = crossing(t, y, target, args.step_s, change > 0)
    tau = None if reach is None else reach-args.step_s
    # First sample after the last excursion, with at least three in-band samples.
    after = np.flatnonzero(t >= args.step_s)
    outside = after[np.abs(y[after]-final) > 0.05*abs(change)]
    settled_index = outside[-1]+1 if len(outside) else after[0]
    settled = float(t[settled_index]-args.step_s) if len(t)-settled_index >= 3 else None
    fig, ax = plt.subplots()
    ax.plot(t, y, "o-", markersize=2)
    ax.axvline(args.step_s, color="gray", linestyle="--")
    ax.axhline(target, color="gray", linestyle=":")
    ax.set(xlabel="Time (s)", ylabel="Temperature (°C)")
    save(fig, out, "thermal-step")
    return {"tau_s": tau, "settling_4_to_5_tau_s": None if tau is None else [4*tau, 5*tau],
            "observed_5_percent_settling_s": settled, "initial_c": initial, "final_c": final,
            "invalid_samples": len(selected)-len(good)}


def filters(x):
    windows = np.lib.stride_tricks.sliding_window_view(x, 5)
    ema = np.empty(len(x))
    ema[0] = x[0]
    for i in range(1, len(x)):
        ema[i] = 0.2*x[i] + 0.8*ema[i-1]
    return {"raw": x, "mean5": np.r_[np.full(4, np.nan), windows.mean(axis=1)],
            "median5": np.r_[np.full(4, np.nan), np.median(windows, axis=1)], "ema": ema}


def noise(rows, out, args):
    t, x = times(rows), col(rows, "raw")
    if len(x) < 500 or np.any((x < 0) | (x > 4095) | (x != np.floor(x))):
        raise ValueError("Require at least 500 valid 12-bit raw samples")
    traces = filters(x)
    stats = {}
    if args.step_s is None:
        stats = {key: spread(y[4:]) for key, y in traces.items()}
        raw_sd = stats["raw"]["sd"]
        for values in stats.values():
            values["sd_reduction_percent"] = None if raw_sd == 0 else 100*(1-values["sd"]/raw_sd)
        if args.spike_codes is not None:
            modified = x.copy()
            index = len(x)//2
            modified[index] += args.spike_codes
            spiked = filters(modified)
            table(out / "synthetic-spike.csv",
                  ["t_s", "original_raw", "synthetic_raw", "synthetic_mean5", "synthetic_median5", "synthetic_ema"],
                  zip(t, x, modified, spiked["mean5"], spiked["median5"], spiked["ema"]))
            stats["synthetic_spike"] = {"index": index, "injected_codes": args.spike_codes,
                                        "peak_deviation_codes": {
                                            k: float(np.max(np.abs(spiked[k][4:]-v[4:])))
                                            for k, v in traces.items()}}
            fig, ax = plt.subplots()
            for key, y in spiked.items():
                ax.plot(t, y, label=key)
            ax.set(xlabel="Time (s)", ylabel="ADC codes", title="Synthetic single-sample injection")
            ax.set_xlim(t[max(0, index-20)], t[min(len(t)-1, index+30)])
            ax.legend()
            save(fig, out, "synthetic-spike")
        table(out / "noise-statistics.csv", ["signal", "n", "mean", "sd", "range", "sd_reduction_percent"],
              ([key, v["n"], v["mean"], v["sd"], v["range"], v["sd_reduction_percent"]]
               for key, v in stats.items() if "n" in v))
        stats["raw_all_samples"] = spread(x) | {"min": float(min(x)), "max": float(max(x))}
    else:
        if not t[4] < args.step_s < t[-20]:
            raise ValueError("Record a pre-step baseline and a stabilized tail")
        initial = np.mean(x[t < args.step_s])
        final = np.mean(x[-20:])
        if initial == final:
            raise ValueError("No step between baseline and tail")
        halfway = (initial+final)/2
        crossings = {k: crossing(t, y, halfway, args.step_s, final > initial) for k, y in traces.items()}
        raw = crossings["raw"]
        stats["half_step_crossing_s"] = crossings
        stats["delay_vs_raw_s"] = {k: None if raw is None or v is None else v-raw for k, v in crossings.items()}
    fig, ax = plt.subplots()
    for key, y in traces.items():
        ax.plot(t, y, label=key)
    if args.step_s is not None:
        ax.axvline(args.step_s, color="gray", linestyle="--")
    ax.set(xlabel="Time (s)", ylabel="ADC codes")
    ax.legend()
    save(fig, out, "filters")
    if args.threshold is not None:
        threshold, band = args.threshold, args.band
        if band <= 0:
            raise ValueError("Hysteresis half-width must be positive")
        direct = x >= threshold
        decision = [bool(direct[0])]
        for value in x[1:]:
            decision.append(value >= threshold+band if not decision[-1] else value > threshold-band)
        fig, ax = plt.subplots()
        ax.step(t, direct.astype(int), where="post", label="Single threshold")
        ax.step(t, np.array(decision, dtype=int), where="post", label="Hysteresis")
        ax.set(xlabel="Time (s)", ylabel="Decision (0/1)")
        ax.legend()
        save(fig, out, "decision")
        stats["decision_transitions"] = {"single": int(np.count_nonzero(np.diff(direct))),
                                          "hysteresis": int(np.count_nonzero(np.diff(decision)))}
        traces["decision_single"] = direct.astype(int)
        traces["decision_hysteresis"] = np.array(decision, dtype=int)
    if all(key in rows[0] for key in ("mean5", "median5", "ema")):
        stats["firmware_replay_max_difference_codes"] = {
            key: float(np.max(np.abs(col(rows[4:], key)-traces[key][4:])))
            for key in ("mean5", "median5", "ema")
        }
    table(out / "filtered.csv", ["t_s", *traces], zip(t, *traces.values()))
    return stats


def budget(rows, out, args):
    if not math.isfinite(args.value) or not args.unit.strip():
        raise ValueError("Provide a finite measured value and its unit")
    items = []
    for row in rows:
        if row["unit"] != args.unit or not row["source"].strip() or not row["basis"].strip():
            raise ValueError("Every contribution needs the same unit, a basis and a source")
        divisor = {"rectangular_half_width": math.sqrt(3), "standard": 1}.get(row["distribution"])
        amount = float(row["estimate"])
        if divisor is None or not math.isfinite(amount) or amount < 0:
            raise ValueError("Specify a valid nonnegative estimate and distribution")
        items.append((row["contribution"], amount/divisor))
    combined = math.hypot(*(u for _, u in items))
    table(out / "uncertainty.csv", ["contribution", "standard_uncertainty", "unit"],
          ((name, u, args.unit) for name, u in items))
    return {"value": args.value, "unit": args.unit, "standard_uncertainty": combined,
            "k": 2, "expanded_uncertainty": 2*combined, "dominant": max(items, key=lambda p: p[1])[0]}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="mode", required=True)
    handlers = {"ldr": ldr, "calibrate": calibrate, "repeat": repeat, "step": step, "noise": noise, "budget": budget}
    modes = {key: sub.add_parser(key) for key in handlers}
    for mode in modes.values():
        mode.add_argument("csv", type=Path)
        mode.add_argument("output", type=Path)
    modes["ldr"].add_argument("--adc-step-v", type=float)
    modes["calibrate"].add_argument("--kind", choices=["pot", "ldr", "temperature"], required=True)
    modes["repeat"].add_argument("--kind", choices=["pot", "pot-adc", "ldr", "ds"], required=True)
    modes["step"].add_argument("--rom", required=True)
    modes["step"].add_argument("--step-s", type=float, required=True)
    modes["step"].add_argument("--final-start-s", type=float, required=True)
    modes["noise"].add_argument("--step-s", type=float)
    modes["noise"].add_argument("--spike-codes", type=float)
    modes["noise"].add_argument("--threshold", type=float)
    modes["noise"].add_argument("--band", type=float, default=3)
    modes["budget"].add_argument("--value", type=float, required=True)
    modes["budget"].add_argument("--unit", required=True)
    args = parser.parse_args()
    try:
        rows = read(args.csv)
        args.output.mkdir(parents=True, exist_ok=True)
        result = handlers[args.mode](rows, args.output, args)
        report = json.dumps(result, indent=2, allow_nan=False)
        (args.output / "summary.json").write_text(report + "\n", encoding="utf-8")
        print(report)
    except (ValueError, KeyError, TypeError, OSError) as exc:
        parser.exit(1, f"Analysis error: {exc}\n")


if __name__ == "__main__":
    main()
