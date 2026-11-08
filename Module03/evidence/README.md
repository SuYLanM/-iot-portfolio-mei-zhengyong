# Evidence index

| Evidence | Status |
|---|---|
| ESP32 sketch compilation | See the [verification record](verification.md) |
| LDR ladder, repeats and excluded attempts | [Task 1 archive](task01-ldr/) |
| DS18B20 ROM, resolutions and water response | [Task 2 archive](task02-ds18b20/). Independent reference and still-air comparison pending |
| ADC paired calibration, DMM photos | [Task 3a archive](task03-calibration/adc/). Photo discrepancies are explained in the report |
| DS18B20 calibration stability check | [Task 3c archive](task03c-ds18b20/). Reference Pending |
| LDR sweep, pot returns and probe repeats | [LDR](task04a-ldr/), [pot](task04b-pot/), [one probe](task04c-ds18b20/) |
| ADC diagnosis, real noise and transition records | [Task 5 archive](task05-noise/) |
| Conditional uncertainty and source integrity | [Task 6 results](../data/task06-analysis/) |
| Peer validation, shared artefact and Discord link | Pending |

Keep original terminal captures here. For analysis, copy a single CSV header and its rows into a run-specific CSV under [data](../data/README.md). Preserve invalid sensor rows with their status. Keep each thermal run separate. Use run identifiers to connect captures, reference measurements, photos and plots.

Most acquired CSVs are stored beside their original UART logs in this directory. Header-only input templates are under `data/`. Pure synthetic inputs and the labeled real-data-plus-drift demonstration are under [simulation/task05](../simulation/task05/). Injected-spike copies are named `synthetic-spike.csv` in the corresponding analysis outputs.
