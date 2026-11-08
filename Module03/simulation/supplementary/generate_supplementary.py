"""Reproducible supplementary models; never writes outside this directory.

Run: python Module03/simulation/supplementary/generate_supplementary.py
All original input files are read-only. No serial, Arduino or hardware access.
"""
from pathlib import Path
import csv
import hashlib
import json
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

HERE = Path(__file__).resolve().parent
MODULE = HERE.parents[1]
P = json.loads((HERE / "simulation_parameters.json").read_text(encoding="utf-8"))
LABEL = P["classification"]
HEADING = "# Supplementary Simulation — Not Hardware Measurement\n\n"
SOURCES = {}


def read_source(relative):
    path = MODULE / relative
    SOURCES[relative] = hashlib.sha256(path.read_bytes()).hexdigest()
    if path.suffix == ".json":
        return json.loads(path.read_text(encoding="utf-8-sig"))
    return list(csv.DictReader(line for line in path.read_text(encoding="utf-8-sig").splitlines()
                               if not line.startswith("#")))


def column(rows, key):
    return np.array([float(row[key]) for row in rows])


def output_csv(group, name, rows):
    path = HERE / group / name
    path.parent.mkdir(parents=True, exist_ok=True)
    rows = [{"provenance": LABEL, **row} for row in rows]
    with path.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(rows[0]))
        writer.writeheader()
        writer.writerows(rows)


def figure(group, name, fig):
    fig.suptitle("Supplementary Simulation — Not Hardware Measurement", fontsize=10)
    fig.tight_layout(rect=(0, 0, 1, 0.95))
    fig.savefig(HERE / group / name, dpi=160, metadata={"Software": "Matplotlib supplementary model"})
    plt.close(fig)


def note(group, text):
    (HERE / group / "README.md").write_text(HEADING + text, encoding="utf-8")


def quantize(x, step):
    return np.round(np.asarray(x) / step) * step


def task01():
    group = "task01-quantization"
    cfg = P["task01"]
    points = read_source("evidence/task03-calibration/adc/paired-points.csv")
    low = next(x for x in points if x["role"] == "fit1")
    high = next(x for x in points if x["role"] == "fit2")
    secant = (float(high["reference_v"]) - float(low["reference_v"])) / (
        float(high["raw_mean"]) - float(low["raw_mean"]))
    q = cfg["illustrative_ideal_adc_span_v"] / 2 ** cfg["adc_bits"]
    manifest = read_source("evidence/task01-ldr/manifest.csv")
    ladder = sorted([r for r in manifest if r["role"] == "ladder"], key=lambda r: float(r["distance_cm"]))
    results, codes, e, resistances = [], [], [], []
    for entry in ladder:
        rows = read_source("evidence/task01-ldr/" + entry["csv"])
        v = column(rows, "ldr_mv").mean() / 1000
        raw = column(rows, "raw").mean()
        vs, rf = float(entry["supply_v"]), float(entry["fixed_ohm"])
        r = rf * (vs - v) / v
        e.append((10 / float(entry["distance_cm"])) ** 2)
        resistances.append(r)
        for basis, step in [("hypothetical_3p3V_span", q), ("extrapolated_task03a_secant", secant)]:
            delta = rf * (vs - v - step) / (v + step) - r
            results.append(dict(source_file=entry["csv"], distance_cm=float(entry["distance_cm"]),
                measured_api_mean_v=v, measured_raw_mean=raw, nominal_supply_v=vs,
                nominal_fixed_ohm=rf, derived_resistance_ohm=r, voltage_step_basis=basis,
                modeled_step_v=step, derivative_ohm_per_v=-rf * vs / v ** 2,
                modeled_plus_one_code_delta_ohm=delta))
            for code_delta in range(-2, 3):
                shifted = v + code_delta * step
                codes.append(dict(source_file=entry["csv"], voltage_step_basis=basis,
                    simulated_code_delta=code_delta, modeled_v=shifted,
                    modeled_resistance_ohm=rf * (vs - shifted) / shifted))
    output_csv(group, "modeled-one-code-sensitivity.csv", results)
    output_csv(group, "simulated-code-steps.csv", codes)
    fig, ax = plt.subplots(1, 2, figsize=(10, 4))
    for basis, label in [("hypothetical_3p3V_span", "Hypothetical span / 4096"),
                         ("extrapolated_task03a_secant", "Task 3a secant (extrapolated)")]:
        selected = [r for r in results if r["voltage_step_basis"] == basis]
        ax[0].plot([r["measured_api_mean_v"] for r in selected],
                   [-r["modeled_plus_one_code_delta_ohm"] for r in selected], "o", label=label)
    ax[0].set(xlabel="Real API mean used as operating point (V)", ylabel="Modeled |delta R| per +1 code (ohm)")
    ax[0].legend(fontsize=8)
    selected = [r for r in codes if r["source_file"] == "distance-30cm.csv"]
    for basis in ("hypothetical_3p3V_span", "extrapolated_task03a_secant"):
        rows = [r for r in selected if r["voltage_step_basis"] == basis]
        ax[1].plot([r["simulated_code_delta"] for r in rows], [r["modeled_resistance_ohm"] / 1000 for r in rows], "o-", label=basis)
    ax[1].set(xlabel="Simulated code change about real 30 cm mean", ylabel="Modeled LDR resistance (kohm)")
    figure(group, "quantization-sensitivity.png", fig)
    gamma, log_a = np.polyfit(np.log(e), np.log(resistances), 1)
    fit = {"A_ohm": float(np.exp(log_a)), "gamma": float(-gamma)}
    note(group, f"""## Inputs and method

The six operating voltages are freshly averaged from the original Task 1 ladder CSVs. Resistance uses the recorded nominal rail and 10 kohm resistor, not new DMM measurements. The two step scenarios are a hypothetical 3.3 V ADC span divided by 4096 (**{q*1000:.6f} mV/code**) and the measured Task 3a two-point secant (**{secant*1000:.6f} mV/code**).

For R = Rfixed(Vs/V - 1), dR/dV = -Rfixed Vs/V². The CSV evaluates finite +1-code changes and a -2 to +2 code ladder. Lower node voltages amplify the resistance effect of the same voltage step. A nominal supply does not establish the ADC's effective full-scale span.

## Results and limits

The modeled +1-code resistance magnitude spans **{min(abs(r['modeled_plus_one_code_delta_ohm']) for r in results):.2f}–{max(abs(r['modeled_plus_one_code_delta_ohm']) for r in results):.2f} ohm** across these points and scenarios. These are sensitivity calculations, not observed individual ADC transitions. The Task 3a secant is used below its 0.568–2.398 V calibration range and does not validate local ADC linearity. Nominal rail, resistor uncertainty, ADC API error and point-source illumination limits remain. Independent LDR-session DMM verification remains **Pending External Evidence**.

[One-code sensitivity CSV](modeled-one-code-sensitivity.csv) · [Synthetic code steps](simulated-code-steps.csv)

![Modeled quantization sensitivity](quantization-sensitivity.png)

The static LDR fit is also recomputed from these six means for Task 4: A = {fit['A_ohm']:.6f} ohm, gamma = {fit['gamma']:.9f}, with Erel = (10 cm / d)². This is an empirical relative-light model; original failed captures and non-monotonic points remain in the hardware archive. No point is removed to improve this supplementary fit.
""")
    return fit, {"ideal_step_v": q, "empirical_secant_v_per_code": secant}


def task02():
    group, cfg = "task02-ds18b20", P["task02"]
    original = read_source("evidence/task02-ds18b20/timing-retest/resolution-12.csv")
    # Field name is taken from the archived CSV, not a synthetic device log.
    key = "temperature_c" if "temperature_c" in original[0] else "temp_c"
    anchor = column(original, key).mean()
    water = read_source("figures/task02-ds18b20/water-step/summary.json")
    n, dt = cfg["warming_points_per_resolution"], cfg["sample_interval_s"]
    assert n >= 20 and dt * 1000 >= max(cfg["datasheet_max_conversion_ms"].values())
    t = np.arange(n) * dt
    latent = anchor + np.linspace(0, cfg["assumed_warming_rise_c"], n)
    rows = []
    fig, ax = plt.subplots(1, 2, figsize=(11, 4))
    ax[0].plot(t, latent, "k--", label="Assumed continuous temperature")
    for bits in (9, 10, 11, 12):
        step = cfg["datasheet_step_c"][str(bits)]
        values = quantize(latent, step)
        ax[0].step(t, values, where="post", label=f"Simulated {bits}-bit")
        rows.extend(dict(simulated_sample=i, simulated_t_s=float(t[i]), resolution_bits=bits,
                    datasheet_step_c=step, datasheet_max_conversion_ms=cfg["datasheet_max_conversion_ms"][str(bits)],
                    assumed_true_c=float(latent[i]), simulated_reported_c=float(values[i])) for i in range(n))
    output_csv(group, "simulated-warming-resolutions.csv", rows)
    times = np.arange(0, cfg["response_duration_s"] + dt/2, dt)
    response_rows = []
    for medium, tau, origin in [("water_model", water["tau_s"], "derived_real_water_tau"),
                                ("air_model", cfg["assumed_air_tau_s"], "assumed_not_measured")]:
        elapsed = np.maximum(times - cfg["response_step_s"], 0)
        y = water["initial_c"] + (water["final_c"] - water["initial_c"]) * (1 - np.exp(-elapsed / tau))
        ax[1].plot(times, y, label=f"{medium}: tau={tau:.2f} s ({origin})")
        response_rows.extend(dict(medium=medium, simulated_t_s=float(s), modeled_temperature_c=float(v),
            simulated_12bit_c=float(quantize(v, .0625)), tau_s=tau, tau_basis=origin) for s, v in zip(times, y))
    output_csv(group, "simulated-water-air-response.csv", response_rows)
    ax[0].set(xlabel="Simulation time (s)", ylabel="Temperature (°C)")
    ax[1].set(xlabel="Simulation time (s)", ylabel="Modeled temperature (°C)")
    for a in ax: a.legend(fontsize=7)
    figure(group, "resolution-and-medium-models.png", fig)
    note(group, f"""## Parameters and provenance

There are **{n} synthetic readings per resolution** ({n*4} total). The assumed linear ramp starts at the real 12-bit bench mean, {anchor:.6f} °C, and rises by {cfg['assumed_warming_rise_c']} °C. This anchor is an observed sensor indication, not a known bath truth. No new ROM, configuration readback or physical conversion is claimed.

The [DS18B20 Rev 6 datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf), configuration register and conversion-time table, specifies 0.5/0.25/0.125/0.0625 °C steps and maximum conversion durations 93.75/187.5/375/750 ms for 9/10/11/12 bits. The 1 s simulated interval exceeds all four limits. Rounding models quantization; it does not model conversion scheduling jitter, internal filtering, or establish accuracy.

## Thermal model

T(t) = Tinitial + (Tfinal - Tinitial)[1 - exp(-(t - tstep)/tau)] after the ideal bath step, and Tinitial before it. The model uses archived water-run endpoints {water['initial_c']:.6f} and {water['final_c']:.6f} °C. Its **water tau = {water['tau_s']:.6f} s** is the previously estimated real-water parameter, not a new measurement. The original onset was inferred and the bath had no independent reference. The **air tau = {cfg['assumed_air_tau_s']:.1f} s is wholly assumed**. Both curves here are simulated; no air response was recorded. The common ideal temperature step isolates model time constants and does not describe a measured air environment or prove stirring was controlled in the real water run.

The model compares an idealized well-mixed/stirred water bath with still air. This mixing assumption belongs only to the simulation; it is not documented for the real water experiment. The model reaches 63.2% after one tau; air is slower solely because that parameter was chosen. A sealed probe's actual heat transfer depends on its housing, immersion, flow and mounting. A single first-order law need not fit the whole trace.

[Warming CSV](simulated-warming-resolutions.csv) · [Thermal model CSV](simulated-water-air-response.csv) · [Original water summary](../../../figures/task02-ds18b20/water-step/summary.json)

![Synthetic resolution and thermal response](resolution-and-medium-models.png)

## Completion boundary

Simulation Completed. The additional real 20-reading warming/configuration checks, independent temperature reference, field observations and air test remain outstanding. These models do not fulfill those hardware requirements.
""")
    return {"warming_anchor_observed_c": float(anchor), "water_tau_data_derived_s": water["tau_s"], "air_tau_assumed_s": cfg["assumed_air_tau_s"]}


def task03c():
    group, cfg = "task03c-calibration", P["task03c"]
    refs = cfg["hypothetical_reference_c"]
    cold = refs["cold_calibration"]
    def model(t):
        return float(quantize(t + cfg["assumed_bias_at_cold_c"] + cfg["assumed_bias_slope_c_per_c"] * (t-cold), cfg["quantization_step_c"]))
    offset = cold - model(cold)
    rows = [dict(scenario=name, role="calibration" if name == "cold_calibration" else "independent_simulated_validation",
                assumed_reference_c=t, simulated_uncorrected_c=model(t), simulated_offset_c=offset,
                simulated_corrected_c=model(t)+offset, simulated_before_error_c=model(t)-t,
                simulated_after_error_c=model(t)+offset-t) for name, t in refs.items()]
    output_csv(group, "simulated-calibration-validation.csv", rows)
    sensitivity = []
    for width in cfg["illustrative_reference_halfwidths_c"]:
        sensitivity.append(dict(assumed_reference_rectangular_halfwidth_c=width,
            assumed_reference_standard_uncertainty_c=width / np.sqrt(3),
            illustrative_quantization_standard_uncertainty_c=cfg["quantization_step_c"] / np.sqrt(12),
            conditional_offset_standard_uncertainty_c=np.hypot(width/np.sqrt(3), cfg["quantization_step_c"]/np.sqrt(12)),
            assumption="independent uniform reference and uniform rounding phase; no bias-model uncertainty included"))
    output_csv(group, "illustrative-reference-sensitivity.csv", sensitivity)
    fig, ax = plt.subplots(1, 2, figsize=(10, 4))
    x = np.arange(3)
    ax[0].bar(x-.18, [r["simulated_before_error_c"] for r in rows], width=.36, label="Before")
    ax[0].bar(x+.18, [r["simulated_after_error_c"] for r in rows], width=.36, label="After cold offset")
    ax[0].set_xticks(x, ["Cold (fit)", "Indoor (held out)", "Ideal ice (held out)"])
    ax[0].set(ylabel="Simulated signed error (°C)")
    ax[0].axhline(0, color="black", lw=.6)
    ax[0].legend()
    ax[1].plot([r["assumed_reference_rectangular_halfwidth_c"] for r in sensitivity],
               [r["conditional_offset_standard_uncertainty_c"] for r in sensitivity], "o-")
    ax[1].set(xlabel="Assumed reference halfwidth (°C)", ylabel="Conditional offset u (°C)")
    figure(group, "simulated-offset-and-reference-sensitivity.png", fig)
    note(group, f"""## Hypothetical experiment

All values in this folder are synthetic. Assumed true reference scenarios are cold **5 °C**, indoor **20 °C**, and ideal ice **0 °C**. They are not readings from an actual thermometer or bath. The assumed indication before quantization is T + 0.300 + 0.015(T - 5) °C, rounded to a 12-bit step of 0.0625 °C.

The synthetic cold indication is {model(cold):.4f} °C, giving offset **{offset:+.4f} °C**. Only this point sets the correction; the indoor and ideal-ice points are excluded from calibration.

| Synthetic scenario | Reference (°C) | Before (°C) | Corrected (°C) | Error before (°C) | Error after (°C) |
|---|---:|---:|---:|---:|---:|
""" + "\n".join(f"| {r['scenario']} | {r['assumed_reference_c']:.4f} | {r['simulated_uncorrected_c']:.4f} | {r['simulated_corrected_c']:.4f} | {r['simulated_before_error_c']:+.4f} | {r['simulated_after_error_c']:+.4f} |" for r in rows) + """

## Reference sensitivity and limitations

For an assumed rectangular reference bound ±a, uref = a/sqrt(3). With an independently varying rounding phase, uq = 0.0625/sqrt(12). The chart combines these two under the stated independence assumption. Rounding at a fixed temperature may be deterministic, so this is an illustrative uncertainty scenario, not a measured Type A uncertainty. A systematic reference error transfers directly into the offset; repeating the same biased reference cannot remove it. A constant offset also leaves the model's temperature-dependent bias. Real calibration needs simultaneous stable readings and an independent validation condition.

No reference-instrument accuracy, certificate or measured uncertainty is supplied. This model does not improve the real probe's verified accuracy. The formal Task 3c status remains **Reference Pending / Hardware Partially Completed**, even though this supplementary simulation is complete. A real ice bath has not been certified as 0 °C.

[Simulation CSV](simulated-calibration-validation.csv) · [Illustrative reference sensitivity](illustrative-reference-sensitivity.csv)

![Synthetic offset and reference sensitivity](simulated-offset-and-reference-sensitivity.png)
""")
    return {"assumed_model_offset_c": offset, "simulated_validation_rows": rows}


def task04(fit):
    group, cfg = "task04-hysteresis", P["task04"]
    A, gamma, tau = fit["A_ohm"], fit["gamma"], cfg["assumed_ldr_tau_s"]
    levels = np.array([1/36, 1/16, 1/9, 1/4, 4/9, 1.0])
    equilibrium = lambda e: A * np.asarray(e) ** -gamma
    all_rows, gaps = [], []
    fig, ax = plt.subplots(1, 2, figsize=(11, 4))
    for mode, duration in [("fast", cfg["fast_leg_s"]), ("slow", cfg["slow_leg_s"]), ("fixed_dwell", cfg["fixed_dwell_s"])]:
        dt = cfg["integration_step_s"]
        if mode != "fixed_dwell":
            leg_t = np.linspace(0, duration, round(duration/dt)+1)
            up = np.linspace(levels[0], levels[-1], len(leg_t))
            down = up[-2::-1]
            light = np.r_[up, down]
            times = np.arange(len(light))*dt
            directions = np.array(["up"]*len(up) + ["down"]*len(down))
        else:
            n_dwell = round(duration/dt)
            staircase = np.r_[levels, levels[::-1]]
            light = np.repeat(staircase, n_dwell)
            times = np.arange(len(light))*dt
            directions = np.repeat(["up"]*len(levels)+["down"]*len(levels), n_dwell)
        state = float(equilibrium(levels[0]))
        modeled = []
        for e in light:
            state = float(equilibrium(e) + (state-equilibrium(e))*np.exp(-dt/tau))
            modeled.append(state)
        modeled = np.asarray(modeled)
        for direction in ("up", "down"):
            mask = directions == direction
            if mode == "fixed_dwell":
                # Plot reported hold endpoints, rather than the transient within each hold.
                mask &= (np.arange(len(light)) + 1) % n_dwell == 0
            ax[0].plot(light[mask], modeled[mask]/1000, label=f"{mode} {direction}", ls="-" if direction=="up" else "--")
        # Dwell scenarios use the final observation at each commanded level.
        if mode == "fixed_dwell":
            indices = np.arange(n_dwell-1, len(light), n_dwell)
            up_v = modeled[indices[:len(levels)]]
            down_v = modeled[indices[len(levels):]][::-1]
        else:
            up_v = np.interp(levels, light[directions=="up"], modeled[directions=="up"])
            down_light = np.r_[light[len(up)-1], light[directions=="down"]]
            down_state = np.r_[modeled[len(up)-1], modeled[directions=="down"]]
            down_v = np.interp(levels, down_light[::-1], down_state[::-1])
        for e, u, d in zip(levels, up_v, down_v):
            gaps.append(dict(mode=mode, relative_light=float(e), modeled_up_ohm=float(u),
                        modeled_down_ohm=float(d), modeled_signed_gap_ohm=float(d-u), assumed_tau_s=tau))
        all_rows.extend(dict(mode=mode, simulated_t_s=float(t), direction=direction,
                        relative_light=float(e), equilibrium_resistance_ohm=float(equilibrium(e)),
                        simulated_resistance_ohm=float(r), assumed_tau_s=tau)
                        for t,e,r,direction in zip(times,light,modeled,directions))
    output_csv(group, "simulated-ldr-rate-sweeps.csv", all_rows)
    output_csv(group, "simulated-matched-level-gaps.csv", gaps)
    ax[0].set(xlabel="Relative light (model, not lux)", ylabel="Modeled resistance (kohm)")
    ax[0].legend(fontsize=7)
    modes = ["fast", "slow", "fixed_dwell"]
    max_gaps = [max(abs(r["modeled_signed_gap_ohm"]) for r in gaps if r["mode"]==mode) for mode in modes]
    ax[1].bar(modes, np.array(max_gaps)/1000)
    ax[1].set(ylabel="Maximum reported matched-level |gap| (kohm)")
    figure(group, "simulated-ldr-speed-and-dwell.png", fig)
    original = read_source("evidence/task04c-ds18b20/resolution-12.csv")
    key = "temperature_c" if "temperature_c" in original[0] else "temp_c"
    center = column(original, key).mean()
    rng = np.random.default_rng(P["seed"])
    virtual, summary = [], []
    fig, ax = plt.subplots(1, 2, figsize=(10, 4))
    for bits, a in zip((9, 12), ax):
        q = P["task02"]["datasheet_step_c"][str(bits)]
        for index, bias in enumerate(cfg["virtual_probe_biases_c"]):
            n = cfg["virtual_probe_samples"]
            y = quantize(center+bias+rng.normal(0,cfg["assumed_virtual_probe_noise_sd_c"],n),q)
            identifier = "VIRTUAL_" + chr(65+index)
            virtual.extend(dict(virtual_probe=identifier, simulated_t_s=i, resolution_bits=bits,
                           assumed_true_c=float(center), assumed_bias_c=bias, simulated_reported_c=float(v),
                           datasheet_step_c=q) for i,v in enumerate(y))
            summary.append(dict(virtual_probe=identifier,resolution_bits=bits,assumed_bias_c=bias,
                                simulated_mean_c=float(y.mean()),simulated_sd_c=float(y.std(ddof=1)),
                                simulated_range_c=float(np.ptp(y)),simulated_mean_error_c=float(y.mean()-center)))
            a.step(np.arange(n),y,where="post",label=f"{identifier}, bias {bias:+.1f} °C")
        a.set(xlabel="Simulation time (s)",ylabel="Synthetic temperature (°C)", title=f"{bits}-bit virtual probes")
        a.legend(fontsize=7)
    output_csv(group,"simulated-virtual-probes.csv",virtual)
    output_csv(group,"simulated-virtual-probe-statistics.csv",summary)
    figure(group,"simulated-virtual-probes.png",fig)
    note(group, f"""## LDR dynamics: data-based equilibrium, assumed time constant

The equilibrium law R = {A:.6f} Erel^(-{gamma:.9f}) is refitted to all six accepted real Task 1 ladder points. Erel = (10 cm / distance)² is a geometry approximation, not lux. Non-monotonic hardware points and failed original captures remain disclosed in Task 1. A **wholly assumed tau = {tau:.1f} s** is used in dR/dt = (Req(E) - R)/tau. Each integration step uses the exact exponential solution for a locally constant input.

Fast and slow legs last {cfg['fast_leg_s']:.0f} and {cfg['slow_leg_s']:.0f} s. The staircase waits {cfg['fixed_dwell_s']:.0f} s per level, reporting its endpoint. All start at low-light equilibrium. This model produces rate-dependent apparent hysteresis, not intrinsic static hysteresis; it cannot identify what caused the real manual-card gap. Interpolation matches the same commanded light levels. At the fast/slow turnaround the same observation is shared, so it is not an independent repeat. The fixed-dwell trial includes a second high-light hold.

| Synthetic protocol | Maximum matched-level absolute gap (ohm) |
|---|---:|
""" + "\n".join(f"| {mode} | {gap:.3f} |" for mode,gap in zip(modes,max_gaps)) + f"""

Longer dwell reduces the modeled directional gap because the assumed dynamic state approaches equilibrium. The dwell curves and bar use only the endpoint of each hold; all intermediate samples are retained in the CSV. These times and gains are not recovered from the hardware sweep. Sensitivity to real light geometry, sensor asymmetry and history remains unknown.

[Sweep CSV](simulated-ldr-rate-sweeps.csv) · [Matched-level CSV](simulated-matched-level-gaps.csv)

![Synthetic sweep-speed comparison](simulated-ldr-speed-and-dwell.png)

## Virtual DS18B20 units

The real single-probe 12-bit mean ({center:.6f} °C) only anchors a synthetic bath. It is not an independent true temperature. Three virtual units have assumed biases -0.2/0/+0.2 °C and independent Gaussian pre-quantization noise SD {cfg['assumed_virtual_probe_noise_sd_c']} °C. Each produces {cfg['virtual_probe_samples']} observations at both 9 and 12 bits, spaced 1 s apart (longer than the datasheet conversion maxima). Seed {P['seed']} fixes the noise. Labels VIRTUAL_A/B/C are not ROM IDs.

The two charts separate quantization, short-run scatter and persistent bias. A constant quantized code can have zero repeatability SD while remaining biased. Finer resolution can expose noise without worsening accuracy. The assumed biases are examples, not a model of the real probe population or a certified ±0.5 °C distribution.

[Virtual readings](simulated-virtual-probes.csv) · [Virtual statistics](simulated-virtual-probe-statistics.csv)

![Synthetic multiple-probe comparison](simulated-virtual-probes.png)

**Simulation Completed; Hardware Partially Completed.** Actual evidence still contains one probe, ten real potentiometer returns and one manual LDR sweep. Controlled sweep-speed and real multi-probe evidence remain missing.
""")
    return {"assumed_tau_s":tau,"max_modeled_gaps_ohm":dict(zip(modes,max_gaps)),"virtual_bath_anchor_observed_c":float(center)}


def main():
    for name in ("task01-quantization", "task02-ds18b20", "task03c-calibration", "task04-hysteresis"):
        (HERE / name).mkdir(exist_ok=True)
    fit, adc = task01()
    results = {"classification":LABEL,"task01":adc,"ldr_static_fit":fit,
               "task02":task02(),"task03c":task03c(),"task04":task04(fit)}
    (HERE / "derived_results.json").write_text(json.dumps(results,indent=2)+"\n",encoding="utf-8")
    (HERE / "source_hashes.json").write_text(json.dumps(SOURCES,indent=2,sort_keys=True)+"\n",encoding="utf-8")
    print(json.dumps(results,indent=2))


if __name__ == "__main__":
    main()
