# Module 02 中文接线与实测前检查

状态：本次只补充文档和程序，未执行新硬件操作。已收到 [Task 1 真实照片](evidence/task01-voltage-divider/README.md)；原有 [Task 9 NACK 扫描](evidence/task09-esp8266-i2c.md)可复用作空总线基线。Task 5 的从设备 ACK/拔插、Task 6/7 的传感器读取仍未验证。先确认开发板丝印、传感器型号、模块原理图和引脚定义。未知模块不能仅凭排针顺序接线。

## Task 5：ESP32、DS3231 与 PCF8574 LCD1602

### 当前方案与尚待核验项

用户最新补充：模块标称按“DS3231”记录，但芯片顶面丝印尚未核实；目前没有万用表，VCC、DAT、CLK对GND均没有实测读数。LCD和RTC信号线均已与ESP32 GPIO断开，未猜接DAT/CLK。这是用户提供的接线状态，不是电压或接口验证结果。完整丝印可用肉眼或手机放大查看，不需要万用表；电压实测仍需合适仪表。继续暂停烧录及带模块扫描，不将“暂无法测量”记录成“已通过”。

**最新识别阻断：RTC排针为DAT、CLK、RST，不能据此认定它是DS3231或把DAT/CLK直接接GPIO21/22。** DS3231是SDA/SCL的I²C器件；DS1302是I/O、SCLK、CE三线接口。现有标识提示可能为三线模块，但芯片丝印和实际模块走线尚未提供，型号仍Pending。RST标记也不能直接当作DS3231的可闲置复位脚。先断电，核对芯片完整丝印/模块原理图；下面的DS3231接线表和RTClib方案只在型号及引脚确认后适用。[DS1302官方资料](https://www.analog.com/media/en/technical-documentation/data-sheets/DS1302.pdf)。

2026-11-02软件预检已确认IDE为task05_i2c_scanner、ESP32 Dev Module、COM3；端口为Silicon Labs CP210x。Core 3.3.12编译通过。实际供电和总线电压未提供，因此未上传、未打开串口扫描、未替换板上固件。请在信号线与ESP32隔离时完成DMM直流电压检查；GPIO读到HIGH或地址ACK都不能证明线电压没有超过3.3 V逻辑域。

用户确认：本次使用 ESP32，不使用 LogicAnalyzer；没有双向 I²C 电平转换器；两个模块都支持 3.3 V，所有器件统一使用 **3.3 V，不接 5 V**。这作为本次主接线方案。模块兼容性来自用户确认；实际 SDA/SCL 上拉电压、阻值、接线与扫描结果仍待现场验证。本次只准备文件，不烧录、不接 GPIO。

PCF8574 芯片支持 2.5–6 V，DS3231 芯片支持 2.3–5.5 V；芯片允许3.3 V不等于任意LCD玻璃/背光/模块都一定工作，但本次按用户确认的3.3 V模块配置。资料：[TI PCF8574](https://www.ti.com/lit/ds/symlink/pcf8574.pdf)、[ADI DS3231](https://www.analog.com/media/en/technical-documentation/data-sheets/DS3231.pdf)。

### 第一步：GPIO 尚不连接时，检查上拉电压

1. 先断电，核对两个模块各自的 VCC/GND/SDA/SCL 丝印及模块供电说明，不能凭四针顺序猜接法。两模块先分别检查，SDA/SCL 与 ESP32、彼此均保持断开。使用确认的3.3 V电源给一个模块供电，并接该电源GND。
2. 万用表使用 **直流电压档**：黑表笔接模块 GND，红表笔依次测模块 SDA、SCL，记录空闲电压；同时记录实际 VCC。此步骤不使用电流档，不在上电电路上测电阻。GPIO 不参与这些测量。
3. 在已确认的3.3 V上拉方案中，SDA/SCL空闲应由合适上拉保持接近其3.3 V电源。若接近5 V、超出所选供电/逻辑域、来源不明、浮空或持续LOW，先停止，不接ESP32；不能因为电压为0就认定安全可通信。
4. 断电后按模块原理图/电阻标记核对上拉接向何处。若需电阻测量，先隔离所有电源，包括会保留供电的电池路径，遵从仪表及模块说明；不以在路测得的数值直接认定某一只电阻。没有外部上拉时，需依据电容/速率/允许灌电流选择；ESP32内部弱上拉不能代替这个核验。
5. 分别确认后，按下表在断电状态组合供电/总线。首次上电仍让 ESP32 GPIO 与总线断开，复查组合后 SDA/SCL 的空闲电压，再断电连接 GPIO。两模块自带上拉会并联，不能盲目再加两组。DMM只能检查直流空闲电压，不能证明边沿上升时间或协议正常。

| 实测项 | DS3231 单独供电 | LCD 单独供电 | 两模块组合后 |
|---|---|---|---|
| VCC→GND | Pending | Pending | Pending |
| SDA→GND，GPIO未连接 | Pending | Pending | Pending |
| SCL→GND，GPIO未连接 | Pending | Pending | Pending |
| 已有上拉阻值及上拉电源 | Pending | Pending | Pending |

### 全部3.3 V接线表——完成上述核验后再接 GPIO

| ESP32 Dev Module | DS3231 RTC 模块 | PCF8574 LCD1602 转接板 | 说明 |
|---|---|---|---|
| 3V3 | VCC | VCC | 用户确认两模块支持3.3 V；核对ESP32稳压器可承担两模块及背光总电流 |
| GND | GND | GND | 所有设备共地 |
| GPIO21 / SDA | SDA | SDA | 同一条已核验的3.3 V上拉数据线 |
| GPIO22 / SCL | SCL | SCL | 同一条已核验的3.3 V上拉时钟线，程序请求100 kHz |
| 不接 | INT/SQW、32K等非本实验引脚 | A0/A1/A2保持实际模块配置 | 不猜跳线状态，不在软件预填LCD地址 |

模块支持3.3 V的确认不包含实际耗电测量；若背光导致复位/欠压，应先查供电能力与模块说明，不把GPIO当电源。RTC模块若带电池充电电路，需核对电池类型和板电路，不给未知电池充电。LCD背光亮但无字并不等于I²C失败，还要检查对比度和实际LCD规格。

### 哪些情况需要双向 I²C 电平转换器

当前全部3.3 V且实测兼容时，不需要只为本方案增加转换器。若后续LCD/任一模块必须5 V供电且上拉到5 V，则不要直连ESP32：采用已确认支持开漏双向I²C的转换模块，在LV侧3.3 V、HV侧5 V分别设置合适上拉并共地。不能用单向电阻分压器代替双向SDA/SCL转换，也不能直接把两侧信号跨接短路。

| 混合电压示意（非当前接线） | 连接关系 |
|---|---|
| ESP32 3V3 / GPIO21 / GPIO22 | 转换器LV参考 / LV-SDA / LV-SCL |
| 5 V LCD 的 VCC / SDA / SCL | 已核实5 V电源 / HV-SDA / HV-SCL |
| 可在3.3 V工作的RTC | 留在LV侧，3.3 V供电/上拉 |
| GND | ESP32、模块、转换器参考地相连 |

按具体转换模块原理图接线，不将裸PCA9306的参考/使能电路误当成通用“LV/HV排针”。[NXP PCA9306](https://www.nxp.com/docs/en/data-sheet/PCA9306.pdf)说明双侧需上拉。也不能仅将5 V PCF8574上拉改到3.3 V就认定合规：TI规定VIH最低为0.7×VCC，5 V供电时为3.5 V，3.3 V不满足保证值。当前PCF8574使用3.3 V供电，不存在这一5 V阈值假设。

### Task 5 实验顺序

1. 完成供电/电平检查，选择 ESP32 Dev Module、Core 3.3.12及实际串口，打开原路径 [task05_i2c_scanner.ino](code/task05_i2c_scanner/task05_i2c_scanner.ino)。Serial Monitor=115200。本次没有执行上传。
2. 若选择的板就是以前的黑色ESP32，上传Scanner会**覆盖板上LogicAnalyzer固件**，但不会删除GitHub保存的程序、源码或采样。先备份/记录固件来源，断电移除旧的分析仪22→23时钟跳线及与新电路冲突的线。仅凭本说明不自动烧录或切换固件。
3. **Run A：只接DS3231**，运行扫描器。记录全部真实响应地址、NACK/错误数、实际接线和串口截图，不预设RTC一定响应。RTC模块可能还带其他I²C芯片，一个模块不一定只出现一个地址。
4. **Run B：全部断电后接LCD**，按3.3 V表共线接入。重新上电扫描并记录完整地址集合；对照Run A识别新增地址，不能预设0x27/0x3F，也不保证总共只有两个地址。必要时再断电做LCD单独扫描来归属地址。
5. **Run C：全部断电后移除一个完整模块**（包括可能通过信号线回灌的连接），再上电扫描，记录消失的地址集合。扫描时保留的器件不能处于断电但信号仍连接的未知状态。
6. **Run D：断电恢复模块**后扫描，核对地址是否恢复。用集合差值比较B与C，不靠猜测映射型号。
7. 保存A/B/C/D真实Serial Monitor截图、原始串口文本、接线照片和空闲电压记录。扫描器地址ACK不等于RTC时间已正确读取或LCD已显示成功。

### RTC读取与LCD显示：地址和电气条件确认后的第二阶段

目前尚无真实扫描或电平测量，**第二个独立Sketch暂不创建**，以遵守先扫描后确认地址的顺序。计划使用Arduino Library Manager安装 **RTClib by Adafruit（研究版本2.1.4）**、依赖 **Adafruit BusIO**，以及 **hd44780 by Bill Perry（研究版本1.3.2）**。菜单：Sketch → Include Library → Manage Libraries，按上述名称/作者核对并记录实际安装版本；本次未安装或编译第二个程序。

届时用RTClib的RTC_DS3231读取，并用hd44780_I2Cexp的**显式实扫地址构造函数**初始化16×2 LCD；不依赖默认或自动猜测LCD地址。GPIO仍21/22，100 kHz。第一行显示YYYY-MM-DD，第二行HH:MM:SS，串口同步输出同一份RTC读取结果，每秒更新，无Wi-Fi/联网校时。具体代码需核对实际库版本和模块映射后编译。

默认不调用rtc.adjust()，尤其不能自动用__DATE__/__TIME__覆盖RTC。若lostPower标志、读取失败或日期无效，显示未设置/无效状态并保留Pending，不拿编译时刻或任意初始时间当实测。即便RTC寄存器返回有效时间，也不能仅凭LCD显示宣称其与现实时间一致；手动设置需要另行说明来源和设置行为。参考：[RTClib](https://github.com/adafruit/RTClib)、[hd44780](https://github.com/duinoWitchery/hd44780)。

## UART 回环

[回环程序说明](code/task01_uart_loopback/README.md)。在确认 GPIO16/17 可用的经典 WROOM 板上，断电后用短线把 GPIO17 TX2 接 GPIO16 RX2；UART0 保持 USB 串口输出，两个串口均配置 115200、8N1。部分带 PSRAM 的板会占用 16/17，必须先查板型。不要接真正 RS-232 正负电压接口；USB-TTL 适配器也须确认 3.3 V 电平。两块板通信时 TX 接对方 RX，且须有合适共地；不能把 TX 与 TX 对接。

## 分压器

[英文计算与空白记录](pins-and-dividers.md)。待核验的保守方案为上臂 10k、下臂 12k、1% 电阻；在明确 5 V 信号上限与容差的前提下，标称输出约 2.727 V。先不接 ESP32，使用万用表测 R、Vin、Vout，确认后再评估接入 ADC。计算不是实测，也不等于抗瞬态、掉电防回灌或双向电平转换保护。

## 分析仪严禁混淆两块板

[详细引脚图](scope-decision.md)。默认 lmcapacho 固件在分析仪板 A 上把 GPIO22 配成时钟输出、GPIO23 配成时钟输入：只能按其要求连 A22→A23。被测板 T 的 I²C SCL22 应接分析仪 CH1/GPIO19，SDA21 接分析仪 CH0/GPIO21；共地，且检查电压。**不要把 T 的 SCL 接到 A 的 GPIO22，也不要在一块板上同时运行默认扫描器和默认分析仪配置。**

模拟示波器候选使用 GPIO34 输入、GPIO25 测试输出，工具链兼容和幅值限制尚待实测。先按数据手册确认衰减/保护，不把“0–3.3 V”理解成整个量程都准确。Task 9 已有真实静态、PWM 和 I²C NACK 证据；B 类模拟候选尚未实测。

## Task 6：库读取与直接寄存器读取

### 本次真实执行状态（2026-11-02）

本次保持现有电路不变。黑色 ESP32/COM3 已运行 Task 6 诊断程序，三轮均为 GPIO21/SDA、GPIO22/SCL 各 50/50 LOW，输出 BUS_BLOCKED；未初始化 Wire、未探测地址、未发送恢复脉冲。串口 115200 正常。结束后 GPIO 恢复普通输入并等待命令，不会自动重试。不要求本阶段拔线、换线或继续 Task 5。

1. 当前诊断程序：[task06_current_hardware_diagnostic.ino](code/task06_current_hardware_diagnostic/task06_current_hardware_diagnostic.ino)。Arduino IDE 已打开该 Sketch；选择 ESP32 Dev Module / Core 3.3.12 / COM3。已由 CLI 实际编译、上传并验证 Hash。串口发送 `g` 才运行固定三轮，LOW 时只输出故障，不强行扫描；内部弱上拉仅供诊断，不代替正式外部 I²C 上拉。
2. 传感器示例：[task06_library_vs_register.ino](code/task06_library_vs_register/task06_library_vs_register.ino)。已编译，**未上传、未实测**。需要额外的真实 BME280 模块；现有 PCF8574 是 LCD I/O 扩展器，DS1302 是三线 RTC，均不能当作该温度传感器。当前约束下不运行这份程序。
3. 库已安装：Adafruit BME280 Library 2.3.0、Adafruit BusIO 1.17.4、Adafruit Unified Sensor 1.1.15。以后也可在 Library Manager 按名称/作者选择版本；不要将编译通过解释成硬件通信成功。
4. BME280 示例检查候选 0x76/0x77 的芯片 ID 0x60；这不是已扫描到的地址。总线正常但无匹配器件时打印 SENSOR_NOT_FOUND；总线 LOW 则打印 BUS_BLOCKED、设备存在性未测试，两种状态不能混为一谈。
5. 后续如选择进行新硬件实验，应先确认真实 BME280 模块电压/上拉/地址及正常总线。再上传示例，保存芯片 ID、校准系数和真实原始字节。程序比较温度 x1 强制转换同一已完成样本：库 readTemperature 与直接读取 0xFA–0xFC，通过真实 0x88–0x8D 校准系数补偿，输出两种 °C 值与差值。此步骤本次未执行。
6. [英文报告](register-reading.md)记录字节序、补码、公式、编译日志、上传日志和三轮原始串口。表格保持空白，不预填数值或一致结论。库内部初始化存在无整体超时的状态轮询；我们自己的转换轮询 100 ms 上限不等于整个库调用有超时保证，未来仍需实物验证。

原有[库适配模板](code/task06_library_template/README.md)和[直接寄存器通用模板](code/task06_register_template/README.md)保持原样，适用于将来选择其他传感器；不要只打开配置开关而不实现实际驱动。

## Task 7：两只不同地址传感器共用总线

### 当前真实结果与任务边界

ESP32/COM3 的 [Task 7 原路径程序](code/task07_two_devices/README.md)已改为可运行主机，Core 3.3.12 编译、烧录通过。串口 115200 发送 `g` 的三轮真实结果均为 SDA/SCL HIGH、仅 0x27 ACK（112 探测、1 ACK、111 NACK、0 错误），LCD DDRAM 比对三轮通过。0x42 均未 ACK；未启用第二控制器。此结果确认 LCD 当前可通信，不把之前“假定 SDA 已恢复”当作事实来源。DS1302 DAT25/CLK26/CE27 不是 I²C 设备，Task 7 程序不访问它。

### 优先 A：同一 ESP32 的两个独立 I²C 角色

Core 3.3.12 的 Wire/Wire1 主从 API 和经典 ESP32 两控制器能力已核对，相关代码一起编译通过，物理互通尚未验证。Wire1 不是自动接到 Wire，需要实际连两条信号。先断开 USB，保留 LCD 接线，按面包板同一电气节点分支：

| 新连接 | 目的节点 |
|---|---|
| ESP32 GPIO32（Wire1 SDA） | 共享 SDA：GPIO21 与 LCD SDA |
| ESP32 GPIO33（Wire1 SCL） | 共享 SCL：GPIO22 与 LCD SCL |

同一开发板已共地，无需新电源线，也无需蓝色板。保留 DS1302 原接线。GPIO32/33 不与当前 LCD21/22 或 RTC25/26/27 冲突。不得把 SDA 与 SCL 短接，不接 5V 上拉；上拉阻值与上升时间没有实测。复电后由软件发送 `a` 才初始化 Wire1 候选地址 0x42 并跑三轮；目前未执行此命令。经典 ESP32 的从机预装 TX 缓冲可能返回旧帧，主机最多读三次并核对序列号/签名/长度/XOR，不能只看 ACK。

### 备选 B：ESP8266 软件从机

[蓝色板从机程序](code/task07_esp8266_slave/README.md)使用 Core 3.1.2 自带 Wire，不装第三方从机库；已针对 d1_mini 编译通过，未烧录或实测。只有选择 B 才断电增加 D2/GPIO4→共享 SDA21、D1/GPIO5→共享 SCL22、GND→ESP32 GND；两板分别 USB 供电，不连接彼此 3V3/5V 输出。再向 ESP32 发送 `b`。A 与 B 不要同时占用 0x42。软件时序、剩余 IRAM 和 100 kHz 稳定性仍需实测，不能因 API/编译通过就承诺可靠。

### 数据与仿真必须分开

候选 0x42 的协议返回 MCU 实际 uptime/counter，**不是环境传感器读数**。只有真实扫描出现 0x27 和 0x42，LCD DDRAM 比对及 0x42 的有效当前序列包也同时通过，才可宣布该适配方案总线共享验证成功；即使成功，LCD+MCU 也不等于教授要求的两只真实 I²C 传感器。

[Python Simulation](code/task07_simulation/README.md)的八项测试已真实运行通过，但 0x27/0x42 返回值和 uptime=123456 是明确合成夹具，不是串口采集。模型只验证地址分派/协议错误处理，不验证真实电气与时序。[英文报告和全部原始证据](two-devices.md)完整区分已完成与待接线/待实测项目。

Task 5/6 的早期通用模板保留 ESP8266 支持；当前 Task 7 主机程序仅针对经典 ESP32，蓝色板仅作为独立从机候选。不同 sketch 每次需分别烧录。本次只实测了 ESP32 主机/LCD 基线，未烧录蓝色板，也未启用分析仪。若另行恢复 Task 9 独立分析仪实验，按其原文档接线，不把该分析仪时钟跳线混入当前 Task 7 总线。
