"""Reproducible Task 5 software simulation. No hardware samples are generated."""
import json
from pathlib import Path
from types import SimpleNamespace

import numpy as np

from analyze import noise, read, table


def main():
    root = Path(__file__).resolve().parent / "simulation" / "task05"
    root.mkdir(parents=True, exist_ok=True)
    rng = np.random.default_rng(20261109)
    n = 500
    index = np.arange(n)
    fixed = np.rint(1800 + rng.normal(0, 12, n)).astype(int)
    step = np.where(index < 250, 1000, 2000)
    drift = np.rint(np.linspace(1785, 1815, n) + rng.normal(0, 6, n)).astype(int)
    configurations = (
        ("fixed", fixed, None, 1000, None),
        ("step", step, 4.98, None, None),
        ("drift", drift, None, None, 1800),
    )
    for name, values, step_s, spike, threshold in configurations:
        source = root / f"synthetic-{name}.csv"
        table(source, ["sample", "t_ms", "raw", "provenance"],
              ((i, 20*i, int(value), "SYNTHETIC_SIMULATION") for i, value in enumerate(values)))
        output = root / name
        output.mkdir(exist_ok=True)
        args = SimpleNamespace(step_s=step_s, spike_codes=spike, threshold=threshold,
                               band=8, label="SIMULATION — synthetic ADC input")
        result = noise(read(source), output, args)
        result["provenance"] = "SYNTHETIC_SIMULATION"
        (output / "summary.json").write_text(json.dumps(result, indent=2, allow_nan=False)+"\n")
        print(name, json.dumps(result))
    (root / "provenance.json").write_text(json.dumps({
        "provenance": "SYNTHETIC_SIMULATION", "seed": 20261109, "samples_per_run": n,
        "sample_interval_ms": 20, "fixed": "1800 codes plus Gaussian noise, sigma 12, rounded",
        "step": "Ideal 1000 to 2000 code step at index 250; no hardware latency",
        "drift": "1785 to 1815 codes plus Gaussian noise, sigma 6, rounded",
        "voltage": "No voltage or resistance reference assigned",
        "spike": "Add 1000 codes at index 250 of a separate fixed-input copy"
    }, indent=2)+"\n")


if __name__ == "__main__":
    main()
