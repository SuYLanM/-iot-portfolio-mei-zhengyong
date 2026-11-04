"""Software-only regression checks with explicitly synthetic fixtures."""
import json
import math
from pathlib import Path
import subprocess
import sys
from tempfile import TemporaryDirectory

from analyze import table


def run(folder, name, header, rows, mode, *options):
    source = folder / f"synthetic-{name}.csv"
    output = folder / f"synthetic-{name}"
    table(source, header.split(","), rows)
    subprocess.run([sys.executable, str(Path(__file__).with_name("analyze.py")),
                    mode, str(source), str(output), *options], check=True, capture_output=True, text=True)
    return json.loads((output / "summary.json").read_text(encoding="utf-8"))


def main():
    with TemporaryDirectory(prefix="module03-synthetic-") as tmp:
        folder = Path(tmp)
        # R = 10000 * E^-0.5; voltage is constructed from the divider equation.
        ldr_header = "level,role,distance_cm,lux,supply_v,vout_v,fixed_ohm"
        ladder = [(e, "ladder", "", e, 3.3, 3.3/(1+e**-0.5), 10000) for e in range(1, 7)]
        repeats = [tuple([1, "repeat", *ladder[0][2:]])] * 3
        result = run(folder, "ladder", ldr_header, ladder + repeats, "ldr", "--adc-step-v", "0.001")
        assert math.isclose(result["gamma"], 0.5) and result["repeat_range_ohm"] == 0
        assert result["power_rmse_ohm"] < result["linear_rmse_ohm"]
        fit = [(r[0], role, *r[2:]) for r, role in zip(ladder, ("fit1", "fit2", "validation"))]
        result = run(folder, "light-fit", ldr_header, fit, "calibrate", "--kind", "ldr")
        assert abs(result["power_error_ohm"]) < 1e-8
        result = run(folder, "voltage", "role,raw_mean,baseline_v,reference_v",
                     [("fit1", 100, 0.2, 0.3), ("fit2", 300, 0.6, 0.7), ("validation", 200, 0.4, 0.5)],
                     "calibrate", "--kind", "pot")
        assert math.isclose(result["error_after_v"], 0, abs_tol=1e-12)
        assert math.isclose(result["error_before_v"], -0.1)
        result = run(folder, "temperature", "role,sensor_c,reference_c",
                     [("cold", 2.25, 2), ("indoor", 20.125, 20), ("ice", 0.25, 0)],
                     "calibrate", "--kind", "temperature")
        assert result["ice_error_after_c"] == 0

        sweep = [("fast", direction, level, 3.3, voltage, 10000)
                 for direction, voltages in (("up", (1.1, 1.65)), ("down", (1.1, 2.2)))
                 for level, voltage in enumerate(voltages)]
        result = run(folder, "sweep", "sweep,direction,light_level,supply_v,vout_v,fixed_ohm",
                     sweep, "repeat", "--kind", "ldr")
        assert math.isclose(result["fast"]["max_gap_ohm"], 5000)
        result = run(folder, "returns", "reference_v,raw_mean", [(1, 500)]*10, "repeat", "--kind", "pot")
        assert result["raw_mean"]["sd"] == 0
        result = run(folder, "resolutions", "rom,bits,temp_c,status,conversion_ms",
                     [("ROM", 9, 5, "ok", 90)]*10 + [("ROM", 9, 85, "suspect_85c", 91)],
                     "repeat", "--kind", "ds")
        assert result["ROM/9"]["step_c"] == 0.5
        assert result["ROM/9"]["n"] == 10 and result["ROM/9"]["invalid"] == 1
        assert result["ROM/9"]["conversion_ms"]["mean"] == 90

        thermal = [(i*1000, "ROM", "ok", 20 if i <= 5 else 30-10*math.exp(-(i-5)/4))
                   for i in range(50)]
        result = run(folder, "thermal", "t_ms,rom,status,temp_c", thermal, "step", "--rom", "ROM",
                     "--step-s", "5", "--final-start-s", "39")
        assert math.isclose(result["tau_s"], 4, abs_tol=0.01)
        assert result["observed_5_percent_settling_s"] == 12

        result = run(folder, "noise", "t_ms,raw", [(20*i, 1000) for i in range(500)],
                     "noise", "--spike-codes", "100", "--threshold", "1000")
        peaks = result["synthetic_spike"]["peak_deviation_codes"]
        assert peaks["median5"] == 0 and peaks["mean5"] == 20
        result = run(folder, "lag", "t_ms,raw", [(20*i, 1000 if i < 250 else 1200) for i in range(500)],
                     "noise", "--step-s", "4.98")
        assert math.isclose(result["delay_vs_raw_s"]["mean5"], 0.04)
        result = run(folder, "budget", "contribution,estimate,distribution,unit,basis,source",
                     [("reference", 0.03, "standard", "V", "synthetic", "fixture"),
                      ("quantization", 0.04*math.sqrt(3), "rectangular_half_width", "V", "synthetic", "fixture")],
                     "budget", "--value", "1", "--unit", "V")
        assert math.isclose(result["expanded_uncertainty"], 0.1)
    print("PASS: 11 synthetic analysis cases; no hardware samples used.")


if __name__ == "__main__":
    main()
