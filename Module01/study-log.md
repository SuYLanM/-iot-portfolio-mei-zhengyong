# Task A — Theory Study Log

Status: **AI-assisted preparation draft**, not proof of completed pod study. Student paraphrasing, reading, discussion and experimental verification are TODO. [Module 0](../00-pre-study.md) remains the original pre-study record.

## References consulted during preparation

| Source | Concept used in this draft | Student learning / verification |
|---|---|---|
| [Official Module 01](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/01-foundations-measurement-and-sensor-chain.md) | Transduction, conditioning, sampling, uncertainty and experiment requirements | TODO explain to pod |
| [Espressif ADC API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html) | Raw codes versus calibrated millivolts; resolution and attenuation | Documentation checked in preparation; actual ADC/DMM comparison TODO |
| [Espressif GPIO API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/gpio.html) | Input pull-up/pull-down, output and digital readings | Documentation checked; actual button/LED test TODO |

Local Core 3.3.12 ADC header/source was also inspected for the APIs used. Documentation is evidence for intended API behaviour, not proof of this board's accuracy.

## Vocabulary — draft explanations and illustrative examples

| Term | Explanation | Illustration (not measured here) |
|---|---|---|
| Sensor / transducer | Converts a physical quantity into another useful quantity; an electrical sensor is a transducer | An LDR changes resistance with illumination |
| Conditioning | Adapts a sensor output to the converter using a divider, amplifier, filter or reference | LDR divider converts resistance into voltage |
| True value | Ideal value sought; usually not known exactly | Actual wiper voltage, approximated by a reference instrument |
| Error | Measured value minus reference value | A voltage reading consistently higher than a DMM comparison |
| Uncertainty | Quantified doubt in a result, with stated method/conditions | Report voltage with DMM specification and repeatability, not unjustified decimals |
| Accuracy | Closeness to the reference/true value | Correct voltage after checking bias; fine digits alone do not prove it |
| Precision | Agreement among repeated readings under stated conditions | A stable but biased ADC may be precise |
| Resolution | Smallest reported distinguishable increment | 12-bit codes have finer ideal spacing than 10-bit codes |
| Range / span | Input interval supported / difference between endpoints | A specified ADC input range depends on attenuation and chip |
| Sensitivity | Change in output per input change | A hypothetical 10 mV/°C sensor changes 10 mV for 1 °C |
| Systematic / random error | Persistent bias / fluctuating scatter | Offset needs correction; averaging can reduce random scatter under suitable assumptions |

TODO: rewrite key terms in my own words and give personal examples before claiming learning.

## Digital signal chain and calculated examples

Physical quantity → transducer → conditioning → ADC sampling/quantization → MCU scaling/filtering/calibration → system decision → actuator driver.

- With an **assumed ideal 3.3 V full-scale model**, `q=3.3/2^n`: 12-bit q≈0.806 mV, 10-bit q≈3.223 mV. These are theoretical calculations, not observed hardware steps. Codes run 0 to `2^n-1`.
- Ideal rounding quantization error is bounded by about ±q/2 under the ideal quantizer model; real offset, gain, nonlinearity and noise add errors. Rail saturation breaks a simple full-range interpretation.
- On classic ESP32, 9–12-bit settings affect SAR resolution. Attenuation sets input scaling. The supply rail is not the internal ADC reference; `raw*3.3/2^n` is a classroom comparison, not a calibrated voltage guarantee.
- A band-limited signal needs a sample rate above twice its highest relevant frequency for ideal reconstruction; practical anti-alias filtering and margin are needed. Filtering aliased samples afterwards cannot generally recover the original signal.
- Moving averages trade noise reduction for delay. Median filters target isolated impulses. Validity gates reject implausible data but may discard real events or hold stale outputs.

**Deeper question:** high resolution can coexist with poor accuracy because code spacing does not remove ADC gain/offset/nonlinearity, divider tolerances, supply drift or sensor bias. In the potentiometer experiment, compare the ADC conversion stage against the DMM before assigning a cause. No particular fault has been observed yet.

## Things I searched — actual student log, TODO

Do not list proposed searches as completed searches. Candidate terms: ADC attenuation, input pull-up, contact bounce, median filter, aliasing.

| Actual search term / date | What I found | Source URL | My explanation |
|---|---|---|---|
| TODO | TODO | TODO | TODO |

## Remarkable things — candidate discussion prompts

These are draft observations to consider, not reported personal discoveries:

- More bits do not guarantee less bias.
- A very smooth gated plot can hide rejected real changes.
- Raw button transitions count both press and release; they are not the same as presses.
- A slow manual ADC sweep need not observe every theoretical code.

TODO: record my actual surprise, counterexample or corrected misconception and its source.

## LLM Q&A — this preparation session

Prompt (summary of the student's actual request): prepare ESP32 Task 1–5 examples, theory and blank records aligned with official Module 01, without fabricating hardware results.

Answer summary: independent sketches were drafted for LED/button, serial timing, ADC, LDR and filters. Ideal values, API estimates and independent reference readings are separated. At that preparation stage, only the initial serial test had hardware evidence; later Task 1–4 observations are linked from the [current module guide](README.md).

Verification: teacher task text, Espressif documentation and local Core 3.3.12 APIs were checked; see [software validation](validation.md). **Theoretical understanding and pod validation remain TODO. Later hardware observations do not by themselves complete this study log; independent component/voltage checks are still pending.** No claim that an LLM answer was confirmed by a new hardware experiment.

| Additional actual prompt | 2–3 line answer summary | Verification performed and evidence | Wrong / vague / overconfident claims |
|---|---|---|---|
| TODO | TODO | TODO; distinguish planned checks from completed checks | TODO |

## Two explanations/diagrams to review and select

1. [Official Module 01, “The chain you must always be able to draw”](https://github.com/iotempire/iotempire-teaching/blob/main/full-classes/hsbi-sensors-actuators-10d/modules/01-foundations-measurement-and-sensor-chain.md#the-chain-you-must-always-be-able-to-draw): selected in this draft because it separates transduction, conditioning, conversion and system use. TODO: student explanation of what it taught me.
2. [Espressif ADC API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html): selected in this draft because raw counts, millivolts, resolution and attenuation have different meanings. TODO: student explanation and comparison with our board.

These are preparation candidates, not claimed jointly selected best sources.

## Agreed learning (pod discussion)

**TODO — no discussion or agreement is claimed.**

- Actual date and my pod / neighbouring pod members: TODO.
- Explain our chain and logs to each other; questions and gaps found: TODO.
- 3–5 points we jointly stand behind: TODO.
- What I changed after the discussion: TODO.
- Shared artefact and contributors: TODO.
- Actual course Discord post link and one-sentence selection reason: TODO.

Do not fill this section from AI-generated imagined conversations.
