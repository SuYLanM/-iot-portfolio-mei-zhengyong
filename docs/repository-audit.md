# Repository Audit — 2026-10-26

## Scope and Baseline

This audit organizes Mei Zhengyong's HSBI Sensors & Actuators portfolio. It publishes existing Task 1/3 evidence and all prepared Arduino sources, improves English navigation and preserves pending preparation without inventing experiments.

Baseline: local `main` and fetched `origin/main` both at `5bc7945e98e6b08da1f233d1010de5af73118627`. Commands reviewed: `git status --short --branch`, `git log --oneline -10`, `git remote -v`, `git branch -vv`, remote fetch and tree inspection. The working tree already contained an edited Module 01 page and untracked reports, images, sketches and preparation documents. No staged changes were present. No applicable AGENTS.md was found.

## Before Organization: Published versus Local Only

| Experiment / material | Already on origin/main | Local-only content to publish |
|---|---|---|
| Task 1 LED | None of this experiment's report, source or real images | Task01_LED_Blink.md; task01_led_blink sketch/README; two real images; task01-electronics.md |
| Task 2 button | Task02_Button_Debounce.md and three original images, commit 5bc7945 | Original task01_button_debounce sketch/README; controlled follow-up record and navigation |
| Task 3 ADC | None of this experiment's report, source or real images | Task03_ADC.md; task03_adc sketch/README; five real images; task03-adc.md |
| Task 4 LDR | Task04_LDR.md and four original images, commit 0e21eea | task04_ldr sketch/README; remaining-measurement record and navigation |
| Initial hardware serial | None | Screenshot and serial-test.md; basic upload/communication evidence only |
| Other preparation | Module 0 notes, root Wokwi source/images, Reflection prompts, Module 02 template | Theory/profile/chain drafts, serial timing and Task 5 code, blank records; all clearly pending where appropriate |

The two remote reports and seven published images were retrieved from the fetched Git tree and checked against local files. Their contents remain unchanged. This confirms remote repository content, not completion of every course learning goal.

## Evidence Review

- [LED report](../Module01/evidence/Task01_LED_Blink.md): photograph shows green LED illuminated; serial commands alternate at 500 ms. 10 kΩ is a colour-band identification, not measured resistance/current. Full optical-cycle timing is not proved by a still photograph.
- [Button report](../Module01/evidence/Task02_Button_Debounce.md): GPIO27/18, internal pull-up and 30 ms stable-duration algorithm match the retained source. Snapshots 93/39/19 and 171/79/39 remain unchanged. Controlled physical-press repeatability and independent timing remain TODO.
- [ADC report](../Module01/evidence/Task03_ADC.md): real endpoint codes 0–1023 and 0–4095; ideal scaling and API estimates are explicitly distinct from DMM readings. Intermediate positions, complete sweep/plot and DMM checks remain TODO.
- [LDR report](../Module01/evidence/Task04_LDR.md): original ambient/brighter/covered evidence and nominal-resistance calculation retained. Invalid -1 markers are not resistance measurements. Controlled repetition and meter validation remain TODO.
- All 23 original image files (15 hardware/serial and eight Wokwi) passed image-decoding integrity checks. No source image was regenerated or edited.

## Minimal Structural Changes

- Rebuilt [portfolio home](../README.md) and [Module 01](../Module01/README.md) with direct report/code/photo/record links and a distinct Pre-study and Preparation section.
- All six original Arduino sketch filenames and source contents retained, including task01_button_debounce.ino. Task numbering is explained rather than changed.
- Normalized the previously untracked initial serial image from `esp32-serial-test.png.png` to `esp32-serial-test.png`. Image bytes are identical and all active references were updated. No tracked experimental image was moved.
- Original homepage, Module 02 and Hardware template text preserved (line endings and trailing whitespace normalized) as [plain-text archive files](archive/README.md). Template claims and nonexistent reflection/image links were removed from active navigation, not from the preserved template text.
- Original Module 0, root Wokwi files and Reflection content remain unchanged. Their self-reported historical checklist is not treated as independent verification of personal preparation.
- Two identical pre-existing picture README placeholders were found; no duplicate experimental images were found by content hash. The picture READMEs now explain their actual directory purpose. No raw data was deleted.
- Root README_IMPORT.txt is a package-import instruction, retained unchanged locally and intentionally not staged. It is not experimental evidence. No ambiguous course material was deleted or silently published.

## Status and Validation

- Six current ESP32 sketches compiled successfully on 2026-10-26 using the installed Core 3.3.12 and target esp32:esp32:esp32. Build products stayed outside the repository; no upload or new hardware measurement was performed. Default configurations only; alternative plotting/timing/resolution runs still require their documented experiments.
- No Arduino source was changed during this audit. Reports distinguish theoretical calculations, software estimates and physical measurements.
- Markdown local file and fragment links are checked across the repository and again against the staged Git tree before commit. Active Windows filesystem links are not used. Remote third-party reference URLs are retained; their ongoing availability is not certified by the local-link check.
- `.gitignore` now excludes build artifacts, caches, system files and common local credential files while retaining evidence images and source. Candidate text files were reviewed/scanned for credential material; no actual credentials were identified in the publication set.
- `git diff --check`, staged file review and protected-file checks are required before commit. Only portfolio organization, experiment evidence/source and explicitly labelled preparation files are included. No reset, history rewrite or forced push is used.

## Pending Work

Task 5 is **Pending / Not hardware tested**. Collect a real common input stream and compare raw, MA5/10/20, median and gated outputs, recording windows, actual sample intervals, rejection events and response delay. Separate synthetic spikes from physical disturbances.

The course's serial plotting/delay-versus-millis work, independent LED/ADC/LDR measurements, controlled button repeatability, Task 0 personal answers/pod agreement, Task A student verification, Task B discussion/sharing, Task 6 personal explanation, Reflection 1 and Module 02 remain TODO. No instructor grading is claimed.
