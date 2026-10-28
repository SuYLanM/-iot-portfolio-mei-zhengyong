# CAN Wiring and Test Procedure / CAN 接线与测试说明

**Prepared / CAN Hardware Test Pending.** Environment supplied by the user: classic ESP32 Dev Module, Windows 11, Arduino IDE 2.3.10, Serial Monitor 115200 baud. Installed Core verified locally: 3.3.12. Planned SN65HVD230 modules have not been physically inspected.

[Profile](bus-can.md) · [A self-test](code/task02_can_self_test/README.md) · [B external communication](code/task02_can_external/README.md) · [Blank records](evidence/task02-can-test-record.md).

## A — Local Feedback, No Transceiver

程序 A 不接任何 CAN 模块、CANH/CANL、终端电阻或跳线。GPIO5 必须空置：程序将 TWAI TX 和 RX 都映射到同一个 GPIO5，通过芯片 GPIO 矩阵与本地引脚输入反馈进行自接收。GPIO5 仍会输出逻辑信号，不能同时接其他驱动源。此方式不是两个独立 CAN 节点，也不验证差分电气信号、终端、外部 ACK 或线缆。

Upload only the A sketch to the verified classic ESP32. Open 115200 baud Serial Monitor. Save real TX/RX and PASS/FAIL lines later, including ID, DLC and payload. A PASS, if observed, means only that the locally sent frame matched the locally received frame. Compilation is not a recorded PASS.

## B — External Transceiver Connections

断电后按下表接线；先核对实物模块的丝印、芯片型号及原理图。TXD/D 是收发器输入，RXD/R 是收发器输出，不能只按模糊的 TX/RX 丝印猜方向。**GPIO5→TXD，GPIO4←RXD**。经典 ESP32 GPIO5 是启动配置相关引脚，应确认模块在复位/上电期间不会施加不兼容电平或妨碍启动。

| ESP32 / bus | SN65HVD230 module | Meaning |
|---|---|---|
| GPIO5 (TWAI TX) | TXD / D input | 3.3 V controller output to transceiver |
| GPIO4 (TWAI RX) | RXD / R output | Transceiver output to controller |
| 3V3 | VCC | 3.3 V supply; check module current requirements and board supply budget |
| GND | GND | Shared reference; connect reference ground between bench nodes |
| Bus CANH | CANH | High side of twisted pair |
| Bus CANL | CANL | Low side of twisted pair |
| Mode / RS / STB | **Pending actual module identification** | Do not invent a jumper or generic enable connection |

裸 SN65HVD230 的 RS（芯片第 8 脚）低电平用于高速模式，高电平进入接收有效而发送关闭的待机模式；接地电阻可改变斜率。数据手册没有通用的独立 STB 引脚。若模块标有 STB/模式跳线，必须按该模块原理图确认，不能机械套用裸芯片连接。Vref 也不能当成电源输出使用。参见 [TI 数据手册](https://www.ti.com/lit/ds/symlink/sn65hvd230.pdf)，§7、§10.4。

```text
Physical end A                                  Physical end B
CANH o================ twisted pair ==================o CANH
     |                                                |
   120 Ω                                            120 Ω
     |                                                |
CANL o================================================o CANL
     |                                                |
 SN65HVD230 A                                     transceiver B
     | short connection                               |
 ESP32 A                                          CAN controller B
GND  o-------------- common bench reference -----------o GND
```

两只终端电阻分别跨接两端 CANH/CANL；不要把 120 Ω 接到地，也不要在每个中间节点添加。先确认模块/分析仪上是否已经有可开关的终端。断电测量 H/L 间电阻时，两只 120 Ω 并联理论上约为 60 Ω；这不是本项目的实测记录。CANH/CANL 绝不能直接接 ESP32 GPIO。不要将测试电路接入实际汽车、正在运行的工业控制网络或未知电压线路。

## Two-Node Verification

1. Inspect both modules, voltage/mode/termination, short twisted-pair wiring and reference ground. Power off for each connection change. Do not run B while A's local feedback wiring/configuration is assumed.
2. On board A, leave `NODE_B=false`; on a second ESP32 running B, set `NODE_B=true` and recompile. This selects IDs 0x321 and 0x322 respectively. These are **chosen test identifiers**, not discovered nodes or observed traffic. Both configure 500 kbit/s Classical CAN.
3. Alternatively use a compatible CAN adapter/controller at 500 kbit/s in **active** mode with its transceiver and termination configured correctly. A passive/listen-only analyzer alone cannot ACK the sender.
4. Capture both consoles and, if available, a CAN analyzer trace. Compare the transmitting marker/sequence and all eight bytes with the other node's RX log in both directions. `TX queue ... ESP_OK` means queued, not transmitted. `TX_ACKED` means link-layer completion, not application execution.
5. Record actual ACK errors, TEC/REC, bus error counters, timeouts and wiring checks. A missing peer is a planned negative test, not a successful single-node external link. Disconnect/reconnect only with power off, then restart the appropriate setup.
6. If bus-off occurs, fix the physical/configuration fault before sending `r` in Serial Monitor. Recovery is not immediate verification. If a TX completion timeout remains pending, no new TX buffer is reused; correct the fault and reset the board if completion does not arrive.

No supply voltage, signal amplitude, achieved bit rate, trace or test pass is supplied yet. The ISR queues are finite; these low-rate teaching sketches are not lossless high-load CAN loggers or automotive-qualified devices.
