# Task 5 — Noise and filtering

## Objective

Measure fixed-input ADC scatter, compare three causal filters on the same new capture, and quantify the cost in response time when the potentiometer step is measured.

## Hardware setup

The operator reported 3V3 → LDR → GPIO34 node → 10 kΩ → GND, with the potentiometer disconnected from GPIO34. The ESP32 Dev Module uses 12-bit ADC, 11 dB attenuation and 115200 baud. The [existing analog sketch](../code/analog_capture/analog_capture.ino) was compiled with ESP32 Core 3.3.12 and uploaded on COM3.

## Task 5.2 — LDR noise measurement

The first 32-sample hardware check returned raw ADC code 0 for every sample, and the voltage API returned 142 mV. The acquisition guard stopped before the 500-sample command because the input was at the ADC floor. This record is preserved. A new non-clipped capture under fixed illumination is required.

## Task 5.3 — Filter comparison

The [Python analysis](../analyze.py) implements a trailing mean of five samples, a trailing median of five samples and EMA with alpha 0.2. The first four rows are excluded from every comparison statistic, including the raw comparison, so all filters use the same 496 timestamps in a 500-row run. The EMA starts at the first raw code. All original samples remain archived.

After a successful capture, run:

```text
python Module03/analyze.py noise Module03/evidence/task05-noise/ldr-fixed-500.csv Module03/figures/task05-noise --spike-codes 1000
```

The statistics include sample SD and its percentage decrease relative to raw over the shared interval. The 500-sample raw summary is reported separately. Firmware and Python replay differences are also recorded.

## Synthetic spike test

The command above adds 1000 codes to a single sample in an independent software copy. The exported synthetic CSV identifies the original and perturbed values. Peak filter deviations compare each perturbed output with its own unperturbed output. The synthetic plot is labeled explicitly. The original serial CSV remains the hardware record.

## Limitations

Fixed lamp placement and shielding do not establish perfectly constant illumination. Drift, flicker, sensor physics, source impedance and ADC electronics can all contribute. The separate API voltage conversion estimates node voltage and does not provide an independent DMM reference. Fixed-input SD reduction alone does not establish the latency on a real step.

## Pending potentiometer measurements

Task 5.1 and 5.4 are **Pending Hardware**. At the later wiring change, disconnect USB and detach the LDR node from GPIO34. Connect the pot outer terminals to 3V3 and GND and the wiper to GPIO34. Keep the current LDR circuit for this session.

Use the same sketch and collector for both future potentiometer runs. The GPIO34 channel is named LDR in the existing sketch; the actual circuit connected determines the source.

```text
powershell -File Module03/code/capture_ldr.ps1 -OutputDirectory Module03/evidence/task05-noise -Run pot-fixed-500 -Samples 500
powershell -File Module03/code/capture_ldr.ps1 -OutputDirectory Module03/evidence/task05-noise -Run pot-step-1000 -Samples 1000
python Module03/analyze.py noise POT_FIXED.csv POT_FIXED_OUTPUT
python Module03/analyze.py noise POT_STEP.csv POT_STEP_OUTPUT --step-s OBSERVED_STEP_SECONDS
```

For the step, hold the first voltage during the initial baseline, turn quickly to a second voltage, then hold it for the rest of the capture. The 1000-point window targets 20 seconds. Use the trace and actual timestamps to identify onset. The Python script compares half-step crossing delays relative to raw. A five-point mean has a nominal two-sample group delay (about 40 ms at this cadence). EMA and median latency depend on the waveform and must be measured.

Task 5.5 is **Software Prepared**. The existing `--threshold` and `--band` arguments replay a two-threshold decision. A real slow crossing and justified thresholds are pending. A dead zone can instead retain the previous output until the absolute input change exceeds a chosen band.

## Figures and raw data

[Compile, upload and original captures](../evidence/task05-noise/)
