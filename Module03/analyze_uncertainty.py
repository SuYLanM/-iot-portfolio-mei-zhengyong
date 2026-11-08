"""Task 6 analysis from archived measurements; no acquisition or source-data writes."""
import hashlib
import csv
import json
from pathlib import Path

import numpy as np

from analyze import col, crossing, filters, plt, read, save, sd, table

ROOT = Path(__file__).resolve().parent
OUT = ROOT / "data" / "task06-analysis"
FIG = ROOT / "figures" / "task06-analysis"


def voltage(z):
    x1, x2, x, v1, v2 = z
    return v1 + (v2-v1)*(x-x1)/(x2-x1)


def adc_budget():
    source = ROOT / "evidence/task03-calibration/adc"
    paired = read(source / "paired-points.csv")
    raw = [col(read(source / f"{role}.csv"), "raw") for role in ("fit1", "fit2", "validation")]
    means = np.array([a.mean() for a in raw])
    sigmas = np.array([sd(a) for a in raw])
    se = sigmas / np.sqrt([len(a) for a in raw])
    v1, v2, ref = [float(row["reference_v"]) for row in paired]
    x1, x2, x = means
    m = (v2-v1)/(x2-x1)
    b = v1-m*x1
    w = (x-x1)/(x2-x1)
    sensitivity = np.array([m*(w-1), -m*w, m, 1-w, w])
    z = np.r_[means, v1, v2]
    # Check every analytic sensitivity against a central finite difference.
    numerical = []
    for i in range(5):
        dz = np.zeros(5)
        dz[i] = 1e-4
        numerical.append((voltage(z+dz)-voltage(z-dz))/(2e-4))
    assert np.allclose(sensitivity, numerical, rtol=1e-6, atol=1e-9)
    contributions = np.abs(sensitivity[:3])*se
    uc = float(np.linalg.norm(contributions))
    single = float(np.linalg.norm(np.r_[contributions[:2], m*sigmas[2]]))
    # Slope and intercept derive from the same endpoint code means.
    jacobian = np.array([[m/(x2-x1), -m/(x2-x1)],
                         [-m*x2/(x2-x1), m*x1/(x2-x1)]])
    coefficient_covariance = jacobian @ np.diag(se[:2]**2) @ jacobian.T
    design = np.array([x, 1])
    propagated = design @ coefficient_covariance @ design + m*m*se[2]**2
    assert np.isclose(propagated, uc*uc)
    budget = []

    def add(name, estimate, unit, distribution, u_mv, basis, treatment, evidence):
        budget.append([name, estimate, unit, distribution, u_mv, basis, treatment, evidence])

    for i, role in enumerate(("low calibration noise", "high calibration noise", "validation noise")):
        add(role, se[i], "ADC code (standard error)", "Type A; independent stationary samples assumed",
            1000*contributions[i], "Measured SD; assumed independence", "Included in conditional mean budget",
            f"Task3a {('fit1','fit2','validation')[i]}.csv, n=150")
    add("Reference accuracy and calibration", "Unknown", "mV", "Unknown", "",
        "Unknown", "Excluded; full uncertainty pending", "UT139B visible in photos; accuracy/range certificate unavailable")
    add("ADC quantization", 1000*abs(m)/np.sqrt(12), "mV per single reading",
        "Uniform half-width 0.5 code assumed", "", "Assumed model",
        "Diagnostic scale only; already contained in observed ADC scatter", "Two-point fitted voltage per code")
    add("Independent reset repeatability", "Unknown", "mV", "Unknown", "", "Unknown",
        "Excluded; do not reuse the same within-capture noise", "Task4 marked returns are different positions/voltages")
    add("Calibration coefficient uncertainty", "Covariance matrix in summary.json", "mixed coefficient units",
        "First-order propagation of endpoint code means", "", "Measured plus independence assumption",
        "Already included through the first two noise rows", "Two endpoints; two-point fit has zero residual degrees of freedom")
    add("Validation signed residual", 1000*(voltage(z)-ref), "mV", "Observed signed error", "", "Measured comparison",
        "Evidence of model/reference discrepancy; not assigned a random distribution", "Held-out Task3a validation")
    for name, evidence in (
        ("Supply variation", "Single reported 3.296 V rail reading; no rail time series"),
        ("Temperature dependence and long-term drift", "No paired temperature or long-duration calibration runs"),
        ("Reference reading timing and settling", "Operator read DMM before each ADC burst"),
        ("Component tolerance and loading", "Pot resistance tolerance not needed as an input; loading/contact effects unbounded"),
        ("Self-heating", "No controlled comparison or appropriate circuit-specific bound"),
    ):
        add(name, "Unknown", "mV", "Unknown", "", "Unknown", "Excluded; full uncertainty pending", evidence)
    table(OUT / "uncertainty-budget.csv",
          ["contribution", "estimate", "unit", "distribution", "output_standard_uncertainty_mv",
           "basis", "treatment", "source"], budget)
    table(OUT / "sensitivities.csv", ["input", "value", "sensitivity_v_per_input_unit"],
          zip(("x1", "x2", "x_validation", "V1", "V2"), z, sensitivity))
    table(OUT / "raw-adc-statistics.csv", ["point", "n", "mean_code", "sample_sd_code", "iid_se_code", "lag1_correlation"],
          ((name, len(a), a.mean(), sd(a), e, np.corrcoef(a[:-1], a[1:])[0, 1])
           for name, a, e in zip(("fit1", "fit2", "validation"), raw, se)))

    batch_rows = []
    for size in (10, 15, 25, 30, 50):
        batch_se = [sd(a.reshape(-1, size).mean(axis=1))/np.sqrt(len(a)//size) for a in raw]
        batch_uc = float(np.linalg.norm(sensitivity[:3]*batch_se))*1000
        batch_rows.append([size, len(raw[0])//size, *batch_se, batch_uc, 2*batch_uc])
    table(OUT / "batch-sensitivity.csv",
          ["batch_size", "batches_per_point", "se_fit1_codes", "se_fit2_codes", "se_validation_codes",
           "conditional_uc_mv", "conditional_U_k2_mv"], batch_rows)
    grid = []
    for reference_u_mv in (0, 0.5, 1, 2, 5):
        for rho in (0, 0.5, 1):
            extra = ((1-w)**2+w*w+2*rho*w*(1-w))*reference_u_mv**2
            grid.append([reference_u_mv, rho, np.sqrt((1000*uc)**2+extra)])
    table(OUT / "illustrative-reference-sensitivity.csv",
          ["assumed_endpoint_standard_u_mv", "assumed_endpoint_correlation", "conditional_uc_mv"], grid)

    fig, ax = plt.subplots()
    ax.bar(["Low fit mean", "High fit mean", "Validation mean"], 1000*contributions)
    ax.set(ylabel="Propagated standard uncertainty (mV)",
           title="Conditional ADC random contributions\nUnknown systematic terms excluded")
    save(fig, FIG, "contributions")
    fig, ax = plt.subplots()
    for rho in (0, 0.5, 1):
        selected = [r for r in grid if r[1] == rho]
        ax.plot([r[0] for r in selected], [r[2] for r in selected], "o-", label=f"Assumed rho={rho}")
    ax.set(xlabel="Illustrative endpoint reference standard uncertainty (mV)",
           ylabel="Conditional combined standard uncertainty (mV)", title="Sensitivity scenarios — reference values are assumptions")
    ax.legend()
    save(fig, FIG, "reference-sensitivity")
    baseline = float(col(read(source/"validation.csv"), "baseline_mv").mean()/1000)
    return dict(corrected_v=voltage(z), reference_v=ref, baseline_v=baseline,
                residual_mv=1000*(voltage(z)-ref), gain_v_per_code=m, offset_v=b,
                w=w, conditional_mean_uc_mv=1000*uc, conditional_mean_U_k2_mv=2000*uc,
                conditional_single_uc_mv=1000*single, conditional_single_U_k2_mv=2000*single,
                output_contributions_mv=(1000*contributions).tolist(),
                coefficient_covariance=coefficient_covariance.tolist(),
                coefficient_correlation=float(coefficient_covariance[0, 1] /
                    np.sqrt(coefficient_covariance[0, 0]*coefficient_covariance[1, 1])),
                batch_U_k2_range_mv=[min(r[-1] for r in batch_rows), max(r[-1] for r in batch_rows)])


def filtering():
    results = []
    for name, path in (
        ("LDR", "hardware-ldr/ldr-fixed-500.csv"),
        ("Pot", "hardware-pot/pot-fixed-500-retry.csv"),
    ):
        rows = read(ROOT/"evidence/task05-noise"/path)
        x = col(rows, "raw")
        assert len(x) == 500
        traces = filters(x)
        spike = x.copy()
        spike[250] += 1000
        injected = filters(spike)
        for kind, values in traces.items():
            peak = float(np.max(np.abs(injected[kind][4:]-values[4:])))
            results.append([name, kind, len(x)-4, np.mean(values[4:]), sd(values[4:]),
                            100*(1-sd(values[4:])/sd(x[4:])), peak])
    table(OUT/"filter-comparison.csv",
          ["hardware_source", "filter", "n_common", "mean_code", "sample_sd_codes",
           "sd_reduction_percent", "synthetic_1000_code_spike_peak_effect"], results)
    rows = read(ROOT/"evidence/task05-noise/hardware-pot/pot-step-clean-20261109-034900.csv")
    t, x = col(rows, "t_ms")/1000, col(rows, "raw")
    threshold = (x[t < 85].mean()+x[-20:].mean())/2
    crossed = {name: crossing(t, values, threshold, 85, False) for name, values in filters(x).items()}
    table(OUT/"latency.csv", ["filter", "half_span_crossing_s", "additional_delay_ms"],
          ((name, value, 1000*(value-crossed["raw"])) for name, value in crossed.items()))
    fig, ax = plt.subplots()
    for source in ("LDR", "Pot"):
        selected = [r for r in results if r[0] == source]
        ax.plot([r[1] for r in selected], [r[4] for r in selected], "o-", label=source)
    ax.set(ylabel="Sample SD (ADC codes)", title="Real fixed-input data; shared valid interval")
    ax.legend()
    save(fig, FIG, "filter-noise")
    return {"latency_ms": {name:1000*(value-crossed["raw"]) for name, value in crossed.items()}}


def evidence_review():
    ldr = ROOT/"evidence/task01-ldr"
    levels, repeats = [], []
    for row in read(ldr/"manifest.csv"):
        if row["role"] not in ("ladder", "repeat"):
            continue
        values = col(read(ldr/row["csv"]), "ldr_mv")/1000
        r = float(row["fixed_ohm"])*(float(row["supply_v"])-values.mean())/values.mean()
        if row["role"] == "ladder":
            levels.append((float(row["distance_cm"]), r))
        if float(row["distance_cm"]) == 30:
            repeats.append(r)
    d, r = np.array(levels).T
    gamma = -np.polyfit(np.log((10/d)**2), np.log(r), 1)[0]
    chosen = {distance: value for distance, value in levels}
    fit_gamma = np.log(chosen[60]/chosen[10])/np.log(36)
    predicted = chosen[10]*(1/9)**(-fit_gamma)
    pot_means = [col(read(ROOT/f"evidence/task04b-pot/trial-{i:02}.csv"), "raw").mean()
                 for i in range(1, 11)]
    probes = {}
    for bits in (9, 10, 11, 12):
        rows = read(ROOT/f"evidence/task02-ds18b20/timing-retest/resolution-{bits}.csv")
        valid = [row for row in rows if row["status"] == "ok"]
        probes[str(bits)] = {"valid_n": len(valid), "mean_c": col(valid, "temp_c").mean(),
                             "sd_c": sd(col(valid, "temp_c"))}
    task4_ds = {}
    for bits in (9, 12):
        rows = read(ROOT/f"evidence/task04c-ds18b20/resolution-{bits}.csv")
        task4_ds[str(bits)] = {"n": len(rows), "mean_c": col(rows, "temp_c").mean(),
                               "sd_c": sd(col(rows, "temp_c"))}
    water = read(ROOT/"evidence/task02-ds18b20/water-step/20261104-173236.csv")
    sweep = read(ROOT/"evidence/task04a-ldr/sweep.csv")
    by_level = {}
    for row in sweep:
        samples = read(ROOT/"evidence/task04a-ldr"/row["source_csv"])
        v = col(samples, "ldr_mv").mean()/1000
        by_level.setdefault(row["light_level"], {})[row["direction"]] = 10000*(3.3-v)/v
    review = {
        "task1": {"ladder_n": len(levels), "six_point_gamma": gamma,
                  "three_30cm_resistance_sd_ohm": sd(repeats),
                  "voltage_basis": "ADC estimate; nominal 3.3 V and 10 kohm"},
        "task2": {"resolution_runs": probes, "water_n": len(water),
                  "water_first_c": float(water[0]["temp_c"]),
                  "water_last_c": float(water[-1]["temp_c"]),
                  "water_reference": "None recorded; dynamic data excluded from stable calibration"},
        "task3b": {"two_point_gamma": fit_gamma, "A_ohm": chosen[10],
                    "30cm_prediction_ohm": predicted, "30cm_error_ohm": predicted-chosen[30]},
        "task3c": {"paired_temperature_reference_rows": sum(
                    len(list(csv.DictReader(p.open(encoding="utf-8-sig"))))
                    for p in (ROOT/"data/temperature-reference.csv", ROOT/"data/temperature-calibration.csv")),
                    "basis": "temperature-reference.csv and temperature-calibration.csv have headers only"},
        "task4": {"ten_return_means_sd_codes": sd(np.array(pot_means)),
                  "ldr_max_directional_gap_ohm": max(abs(v["up"]-v["down"]) for v in by_level.values()),
                  "one_probe_resolution_runs": task4_ds,
                  "limitations": "One probe; card positions lack lux reference and controlled dwell"},
    }
    (OUT/"evidence-review.json").write_text(json.dumps(review, indent=2, allow_nan=False)+"\n")


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    FIG.mkdir(parents=True, exist_ok=True)
    original = sorted((ROOT/"evidence").rglob("*.csv"))
    original += sorted((ROOT/"data").glob("*.csv"))
    hashes = [(str(p.relative_to(ROOT)), hashlib.sha256(p.read_bytes()).hexdigest()) for p in original]
    result = {"scope": "PARTIAL CONDITIONAL UNCERTAINTY; full metrological coverage unavailable",
              "adc": adc_budget(), "filtering": filtering()}
    evidence_review()
    table(OUT/"source-hashes.csv", ["module03_path", "sha256"], hashes)
    assert all(hashlib.sha256((ROOT/p).read_bytes()).hexdigest() == digest for p, digest in hashes)
    (OUT/"summary.json").write_text(json.dumps(result, indent=2, allow_nan=False)+"\n")
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
