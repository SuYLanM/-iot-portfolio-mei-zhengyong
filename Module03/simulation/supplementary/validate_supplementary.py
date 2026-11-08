"""Validate the supplementary outputs without changing original evidence."""
from pathlib import Path
import csv
import hashlib
import json
import re
from urllib.parse import unquote
import numpy as np
from PIL import Image

HERE = Path(__file__).resolve().parent
MODULE = HERE.parents[1]
LABEL = "SUPPLEMENTARY_SIMULATION_NOT_HARDWARE_MEASUREMENT"


def read(name):
    with (HERE/name).open(encoding="utf-8-sig", newline="") as handle:
        return list(csv.DictReader(handle))


def run():
    counts = {}
    for path in sorted(HERE.rglob("*.csv")):
        rows = read(path.relative_to(HERE))
        assert rows and all(row["provenance"] == LABEL for row in rows), path
        for row in rows:
            for value in row.values():
                assert value is not None and value != "", path
                try:
                    number = float(value)
                except ValueError:
                    continue
                assert np.isfinite(number), (path, value)
        counts[path.relative_to(HERE).as_posix()] = len(rows)
    source_hashes = json.loads((HERE/"source_hashes.json").read_text())
    for relative, expected in source_hashes.items():
        assert hashlib.sha256((MODULE/relative).read_bytes()).hexdigest() == expected, relative
    warming = read("task02-ds18b20/simulated-warming-resolutions.csv")
    for bits, q, maximum_ms in [(9,.5,93.75), (10,.25,187.5), (11,.125,375), (12,.0625,750)]:
        rows = [r for r in warming if int(r["resolution_bits"]) == bits]
        assert len(rows) >= 20
        times = np.array([float(r["simulated_t_s"]) for r in rows])
        y = np.array([float(r["simulated_reported_c"]) for r in rows])
        truth = np.array([float(r["assumed_true_c"]) for r in rows])
        assert np.all(np.diff(times)*1000 >= maximum_ms)
        assert np.allclose(y/q, np.round(y/q))
        assert np.max(abs(y-truth)) <= q/2 + 1e-12
        assert np.ptp(truth) > 0 and np.all(np.diff(truth) >= 0)
    thermal = read("task02-ds18b20/simulated-water-air-response.csv")
    cfg = json.loads((HERE/"simulation_parameters.json").read_text())
    for medium in ("water_model", "air_model"):
        rows = [r for r in thermal if r["medium"] == medium]
        times = np.array([float(r["simulated_t_s"]) for r in rows])
        y = np.array([float(r["modeled_temperature_c"]) for r in rows])
        tau = float(rows[0]["tau_s"])
        derived = json.loads((MODULE/"figures/task02-ds18b20/water-step/summary.json").read_text())
        expected = derived["initial_c"] + (derived["final_c"]-derived["initial_c"])*(1-np.exp(-1))
        # Verify the plotted model reaches 63.2% at one tau, within sampling interpolation tolerance.
        assert abs(np.interp(cfg["task02"]["response_step_s"]+tau, times, y)-expected) < .03
        assert np.all(np.diff(y) >= -1e-12)
    calibration = read("task03c-calibration/simulated-calibration-validation.csv")
    assert sum(r["role"] == "calibration" for r in calibration) == 1
    assert sum(r["role"] == "independent_simulated_validation" for r in calibration) == 2
    for row in calibration:
        assert np.isclose(float(row["simulated_corrected_c"]), float(row["simulated_uncorrected_c"])+float(row["simulated_offset_c"]))
        if row["role"] != "calibration":
            assert abs(float(row["simulated_after_error_c"])) < abs(float(row["simulated_before_error_c"]))
    sensitivity = read("task01-quantization/modeled-one-code-sensitivity.csv")
    for row in sensitivity:
        # Finite step should be close to analytic differential at these small increments.
        actual = float(row["modeled_plus_one_code_delta_ohm"])
        linearized = float(row["derivative_ohm_per_v"])*float(row["modeled_step_v"])
        assert actual < 0 and abs(actual/linearized-1) < .005
    gaps = read("task04-hysteresis/simulated-matched-level-gaps.csv")
    maxgap = {mode:max(abs(float(r["modeled_signed_gap_ohm"])) for r in gaps if r["mode"]==mode)
              for mode in ("fast", "slow", "fixed_dwell")}
    assert maxgap["fast"] > maxgap["slow"] > maxgap["fixed_dwell"]
    virtual = read("task04-hysteresis/simulated-virtual-probes.csv")
    assert {r["virtual_probe"] for r in virtual} == {"VIRTUAL_A", "VIRTUAL_B", "VIRTUAL_C"}
    for row in virtual:
        q = float(row["datasheet_step_c"])
        assert np.isclose(float(row["simulated_reported_c"])/q, round(float(row["simulated_reported_c"])/q))
    images = []
    for path in sorted(HERE.rglob("*.png")):
        with Image.open(path) as im:
            im.load()
            assert im.width >= 1000 and im.height >= 500
            images.append(path.relative_to(HERE).as_posix())
    link_count = 0
    for path in HERE.rglob("*.md"):
        text = path.read_text(encoding="utf-8")
        assert "Supplementary Simulation — Not Hardware Measurement" in text
        for target in re.findall(r"!?\[[^\]]*\]\(([^)]+)\)",text):
            if re.match(r"https?://",target):
                continue
            target = unquote(target.split("#")[0])
            if target:
                resolved = (path.parent/target).resolve()
                # This report is emitted only after validation succeeds below.
                if resolved != HERE/"validation_results.json":
                    assert resolved.exists(), (path,target)
                link_count += 1
    result = {"status":"PASS", "classification":LABEL,"csv_rows":counts,
              "source_hashes_verified":len(source_hashes),"images_decoded":images,
              "local_links_verified":link_count,
              "checks":["finite and labeled CSV values",">=20 synthetic samples per resolution",
                        "datasheet quantization grid and conversion-time scheduling",
                        "first-order 63.2% response","held-out synthetic calibration",
                        "finite-difference sensitivity","rate/dwell comparison",
                        "virtual units are labeled","original source hashes unchanged"]}
    (HERE/"validation_results.json").write_text(json.dumps(result,indent=2)+"\n",encoding="utf-8")
    print(json.dumps(result,indent=2))


if __name__ == "__main__":
    run()
