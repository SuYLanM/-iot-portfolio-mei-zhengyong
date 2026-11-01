# Task 1 — Original Voltage Measurement Photographs

**Photo evidence documented; divider topology and 5 V conversion verification Pending.** [English Task 1 report](../../pins-and-dividers.md). Imported on 2026-11-02 from the student's ZIP; the actual capture date/operator details are not established by this import. The two JPEGs are copied byte-for-byte without cropping, enhancement or recompression.

| Original file | Supported observation | Not established |
|---|---|---|
| [Photo A](Task1_photo_A_multimeter_3V3.jpg) | ESP32, breadboard, potentiometer and multimeter visible; main DC voltage display approximately **3.307 V** | Exact probe nodes, whether this is a supply measurement, source voltage, resistor values and divider ratio |
| [Photo B](Task1_photo_B_potentiometer_output.jpg) | Same types of equipment visible; student reports approximately **2.7 V**, with display partly obscured by glare | Exact numeric reading and probe nodes; whether this is a potentiometer wiper-to-ground measurement |

The filenames are supplied labels, not independent proof of the electrical nodes. Do not calculate a measured divider ratio by treating these two photographs as simultaneous Vin/Vout measurements. In particular, approximately 2.7 V does not prove the proposed 10 kΩ/12 kΩ design or a 5 V source.

## Photo A

![Original ESP32 and potentiometer setup with a visible approximately 3.307 V display; nodes unconfirmed](Task1_photo_A_multimeter_3V3.jpg)

## Photo B

![Original ESP32 and potentiometer setup; student-estimated approximately 2.7 V display partly obscured](Task1_photo_B_potentiometer_output.jpg)

## Integrity

| File | Original JPEG bytes | SHA-256 |
|---|---|---|
| Task1_photo_A_multimeter_3V3.jpg | 262325 | `a7a08605b6f9ba234b415c25eaa9d4e2284e9f4d3c71c09a64ddca5cb0dd726a` |
| Task1_photo_B_potentiometer_output.jpg | 286622 | `d19a71df9bffea1eedb69911b14c0672af85534041d6d2dd0eac3d3bfb59a0a9` |

## Required Follow-up

Record a labelled schematic, actual source and ground, both probe nodes for each reading, potentiometer terminal/wiper connections and resistance or actual R1/R2 values, meter range/uncertainty, and paired Vin/Vout readings for the professor's 5 V task. Verify a safe output with the ADC disconnected before any connection to ESP32. No new measurements are inferred here.
