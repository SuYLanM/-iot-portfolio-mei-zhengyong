# Task 4 — I2C Study Log

**AI-assisted theory preparation, not a completed pod learning experience.** Mei Zhengyong and Felix are the named group members; actual personal explanations, discussion roles and hardware verification remain Pending. [Course Task 4/4a/4b](references.md#r1-course) · [Primary sources](references.md).

## Concepts to Explain and Verify

| Concept | Research explanation | How to verify later |
|---|---|---|
| SDA / SCL | Shared data / clock plus a common ground. Controller creates clock and transaction boundaries; a stretching target may hold SCL LOW. | Check board/module pinout and capture both lines |
| Open-drain / pull-up | Assert LOW or release; compatible external pull-ups create HIGH. This differs from ordinary push-pull GPIO. | Meter idle level, inspect module pull-ups, capture analog rise time |
| START / STOP | SDA HIGH→LOW while SCL HIGH starts; SDA LOW→HIGH while SCL HIGH stops. Data normally stays stable while clock is HIGH. | Mark the actual edge pair on a capture |
| ACK / NACK | Receiver pulls SDA LOW on ninth clock for ACK; released HIGH is NACK. A controller normally NACKs the last byte of a read to finish it. | Decode address ACK separately from final read NACK |
| 7-bit / address byte | Wire expects 7-bit addresses. Address byte is (address<<1)|R/W, not a second device identity. | Compare scanner with datasheet notation; do not shift twice |
| Repeated START | New START without STOP keeps the logical transfer together; often used after selecting a register pointer. | Check actual device transaction diagram; Wire endTransmission(false) then requestFrom(...,true) |
| Clock stretching | Target delays the next rising edge by holding SCL LOW; controller timeout and device support still limit it. | Capture an actually stretching device; a slow loop is not proof of stretching |
| Register map | Device-specific command/config/data layout; pointer width, byte order, side effects and scaling vary. | Check the selected sensor's exact datasheet and revision |
| Address conflict | Two physical devices may both ACK one address; scanner cannot distinguish them. Open-drain read bits can combine into plausible wrong data. | Separate devices, scan/read each, then controlled shared test if available |

Detailed standards: [NXP UM10204](https://www.nxp.com/docs/en/user-guide/UM10204.pdf). API behavior: [Arduino Wire](https://docs.arduino.cc/language-reference/en/functions/communication/wire/) and [Espressif Wire implementation](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/i2c.html). The Arduino page body was not extractable in this research session; the local Core 3.3.12 signatures and Espressif API page were checked instead.

## Address Example — Documentation Only

[ST VL53L0X datasheet](https://www.st.com/resource/en/datasheet/vl53l0x.pdf) lists address byte 0x52. In 7-bit notation that is 0x29; write/read address bytes would be 0x52/0x53. This is not a scan result and does not select our actual Task 6 device. Normal Wire code should not be given 0x52 merely because the datasheet printed an address byte.

Reserved I2C groups mean not all 128 bit patterns are ordinary target addresses; the prepared scanner probes 0x08–0x77. A missing ACK may indicate wiring, power, notation, sleep/reset or a stuck bus, not necessarily a dead sensor. Some devices dislike general scanning; check their datasheets.

## Pull-up and Multi-device Reasoning

Parallel module pull-ups lower effective resistance: two 4.7k resistors make 2.35k (calculation, not our inventory). Approximate RC rise-time reasoning is `tr≈0.8473×Rp×Cb`; sink-current/VOL constraints set a lower resistance bound while rise-time/capacitance sets an upper bound. Use the actual mode/device limits. Do not add pull-ups blindly or use the ESP32's weak internal pull-ups as a verified bus design.

A generic 3.3 V sensor bus must have pull-ups to a level every participant accepts. A board powered at 5 V may also pull SDA/SCL to 5 V even if its product name says “Arduino compatible”. Confirm module circuitry, regulator and pull-up rails separately. I3C compatibility adds further restrictions; plain Wire is not an I3C implementation.

## Task 4a — Actual Research in This Preparation Session

Sources actually consulted by Codex: full official Module 2; NXP UM10204 Rev. 7; Espressif Wire/UART APIs and local Core headers; ST VL53L0X and TI TMP102 datasheets; TI RS-232/RS-485/CAN references; MIPI I3C pages; pinned instrument README/config/source files. See [reference verification register](references.md) for failed retrievals and version limits.

Actual search examples used by Codex on 2026-10-28 (not attributed to student browsing):

| Search query | Finding / verification |
|---|---|
| site.modbus.org Modbus serial line specification V1.02 pdf | Legacy PDF URL found but retrieval failed; current portal/introduction read; exact framing parameters remain open |
| site.mipi.org I3C specification 12.5 MHz SDR | MIPI official FAQ/specification pages distinguish clock/raw rate and protocol modes |
| site.analog.com SPI interface tutorial clock polarity phase | ADI SPI documentation explains CPOL/CPHA; actual peripheral timing still required |

## LLM Q&A — Actual User Requests, 2026-10-28

The excerpts below are from the user's requests in this chat. Answers summarize this preparation; they are not invented prior pod prompts or claims of personal insight.

| Actual prompt excerpt | Answer summary | Verification and open issue |
|---|---|---|
| “注意教授提供的 10 kΩ / 20 kΩ 示例…约 3.33 V…给出更保守的分压设计” | 10k/12k at nominal 5 V gives 2.727 V; tolerance limits and ADC headroom must be checked. | Formula independently recalculated; ESP32 electrical tables reviewed; all meter readings Pending |
| “解释 SDA、SCL、START、STOP、ACK/NACK、7-bit 与 8-bit 地址…” | Separate transaction edges, acknowledge role and address-byte notation; repeated START is device-specific. | NXP, Wire API and sensor datasheets compared; no capture obtained |
| “实际传感器型号尚未确定…不得擅自指定寄存器地址作为已验证配置” | Templates use -1/disabled guards and unimplemented library hooks. | Local source and compile checks; actual model/map/library Pending |
| “部分逻辑分析仪固件占用 GPIO22…先核对引脚映射” | Pinned analyzer source reserves output22/input23 and CH0=21/CH1=19. Use separate target/instrument boards. | Source checked; actual wiring and captures Pending |

Overconfidence corrected during research: an ACK cannot identify a device; a requested sample-rate field is not measured timing; the web scope's README image is explicitly simulated, and its Core 2 LEDC calls are not directly compatible with local Core 3.3.12. The analyzer README's 912600 typo differs from source 921600. No bench verification is claimed.

## Personal / Pod Learning — To Fill After Actual Study

| Reference actually used by me | Concept I can now explain | My own explanation / question | Verification performed / evidence |
|---|---|---|---|
| TODO | TODO | TODO | Pending |

My remarkable observation/counterexample: TODO. My actual additional search terms/URLs: TODO. My additional LLM prompts, short answers, errors and tests: TODO. Suggested future prompts are in the official course, not claimed as asked here.

Two candidate explanations to review: the NXP transaction diagrams (wire-level events) and the actual selected sensor register-map diagram (mapping bytes into physical values). Final personal best-two selection, what they taught me, and a second selected device diagram are TODO; no selected sensor is presumed.

## Agreed learning (pod discussion)

**Task 4b Pending — no neighbouring-pod discussion or Discord publication claimed.**

- Actual date, our pod and neighbouring-pod members: TODO.
- Bus profile we explained and questions/gaps found: TODO.
- 3–5 jointly accepted points: TODO.
- Changes to this log after the discussion: TODO.
- Shared artefact, its contributors and one-sentence selection reason: TODO.
- Actual course Discord post link: TODO.
- Individual agreed-learning notes from Mei / Felix: TODO, to be written by each member.
