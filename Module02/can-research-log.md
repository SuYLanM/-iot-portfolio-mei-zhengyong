# CAN Research Process — AI Assistance and Verification

**Research Prepared; student review, hardware verification and classroom sharing Pending.** This records this preparation session, not a fabricated personal learning diary. [Profile](bus-can.md) · [Technical sources](can-technical-notes.md).

The following are actual excerpts from the user's request to the assistant, grouped into research questions. They are not invented conversations between the two students. Responses are summarized from the documentation/code produced in this session; no student “aha”, experiment or agreement is inferred.

| Actual prompt excerpt | Assistant answer summarized | Verification performed / remaining |
|---|---|---|
| “CAN 的发展历史，以及 Bosch 最初开发 CAN 的背景。” | Automotive controller networking motivated Bosch's work; substantial development in 1984, public SAE presentation in February 1986. | Read CiA's *The dawn of CAN* retrospective; avoided unsupported exact start date and first-car anecdotes. Student explanation Pending. |
| “CAN 与 UART、I²C、SPI、RS-485 的区别。” | CAN provides message priority/arbitration and error confinement; the other links differ in physical signaling, target selection and upper-layer responsibility. | Compared CiA CAN CC and TI physical-layer guidance with the repository's already sourced comparison. Actual comparative experiments Pending. |
| “CAN 的多主机访问、标识符优先级、无破坏性位仲裁。” | Dominant wins over recessive; loser becomes receiver without destroying the winner. Equal-base standard/extended frames need SRR/IDE handling. | Checked CiA arbitration explanation and NXP-hosted Bosch protocol reproduction. No arbitration waveform observed. |
| “CAN 数据帧结构、CRC、ACK、错误检测与错误处理。” | CRC/bit/stuff/form/ACK checks and fault confinement detect communication errors; ACK does not imply application processing. | Checked Bosch protocol reproduction and installed Espressif error/state definitions; real counters and negative tests Pending. |
| “使用 ESP32 内部 TWAI 自测模式或驱动明确支持的自接收测试机制。” | No-ACK and self-reception flags alone do not supply classic ESP32 TX-to-RX feedback. Same-pin GPIO-matrix feedback was prepared with GPIO5 disconnected. | Checked installed Core 3.3.12 / IDF 5.5.5 headers and v5.5.5 driver IO configuration. Compilation recorded in sketch READMEs; runtime success remains Pending. |

Additional checks in this session: Bosch's CAN FD page separates up-to-8 versus up-to-64-byte payloads; classic ESP32 documentation excludes FD support; TI SN65HVD230 data sheet specifies 3.3 V supply and up to 1 Mbit/s and explains RS mode behavior; CiA qualifies the theoretical 40 m / 1 Mbit/s example. The original Bosch-hosted CAN 2.0 PDF fetch failed, so the NXP-hosted reproduction was used and identified in the notes.

All pods still need to explain I2C open-drain signaling. [Existing I2C study material](study-log.md) provides preparation; each student must explain why devices pull LOW or release, how pull-ups and capacitance determine rise time, why incompatible pull-up voltages are unsafe, and how a stuck-low device affects the bus. No discussion or learning experience has been marked completed.
