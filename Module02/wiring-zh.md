# Module 02 中文接线与实测前检查

状态：本次只补充文档和程序，未执行新硬件操作。已收到 [Task 1 真实照片](evidence/task01-voltage-divider/README.md)；原有 [Task 9 NACK 扫描](evidence/task09-esp8266-i2c.md)可复用作空总线基线。Task 5 的从设备 ACK/拔插、Task 6/7 的传感器读取仍未验证。先确认开发板丝印、传感器型号、模块原理图和引脚定义。未知模块不能仅凭排针顺序接线。

## Task 5：ESP32、DS3231 与 PCF8574 LCD1602

### 当前方案与尚待核验项

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

1. 先提供真实传感器的**型号、模块正反面丝印、供电/上拉条件**；仅凭一次地址 ACK 不足以确认型号。至少先完成 Task 5 的设备扫描和数据手册地址映射。
2. 打开 [库模板](code/task06_library_template/task06_library_template.ino)，根据真实库的文档选择准确版本，添加 include/实例，并实现 beginSensorLibrary()、readSensorLibrary()。填写 DEVICE_ADDRESS、VALUE_UNIT 和实际模式，核对后才将 SENSOR_CONFIGURATION_VERIFIED 改为 true。当前函数返回 false；仅改开关不会让它变成真实驱动。
3. 打开 [寄存器模板](code/task06_register_template/task06_register_template.ino)，按数据手册设置 -1 占位的设备地址/寄存器指针，以及指针宽度、读取字节数、Repeated START/STOP。传感器复位、量程、等待转换完成等初始化也要按该型号补充。不要假设换程序后前一个程序的配置仍然有效。
4. 先保存真实原始字节。只有核对字节序、有效位、补码位宽、缩放和单位后才启用 DECODING_VERIFIED。线性解码器并不适合所有传感器；需要补偿系数、校验或命令流程的型号应单独实现。
5. 在相同稳定条件和相同模式下分别运行库与直接读取，记录独立时间戳、实际字节、转换公式、单位和差值。失败不得填零；不要预填一致或成功。默认模板只会打印 PENDING_CONFIG。
6. 填写 [英文报告原有对照表](register-reading.md)，附真实日志、照片、数据手册页码和错误分析。编译后未来修改了库/配置，还需重新编译和真实验证。

## Task 7：两只不同地址传感器共用总线

1. 准备两只实际 I²C 传感器，核对模块电压、上拉和地址设置；同型号若允许设置不同地址也可使用。只有 OLED 加一只传感器不能证明读取两只传感器的原任务已完成。
2. 断电后分别接 A、接 B、再接两者，运行同一个 Task 5 扫描程序，保存三份真实结果。两模块 SDA 都接控制器同一个 SDA，SCL 也相同，且共地。检查并联上拉总阻值与负载，不盲目叠加电阻。
3. 在 [双设备框架](code/task07_two_devices/task07_two_devices.ino)中填写实扫地址 ADDRESS_A/B 和实际单位 UNIT_A/B，实现 initializeDevices/readDeviceA/readDeviceB 后再启用 TWO_DEVICE_CONFIGURATION_VERIFIED。两个地址不同只是必要条件；ACK 不等于初始化或测量成功。
4. 分别记录 A/B 的有效标志、真实数值/单位和错误。框架默认地址 -1、开关 false、适配函数 false，不会产生伪数据。故障设备可能把整个总线拉低，软件有效标志不能隔离电气故障。
5. 填写 [双设备英文报告](two-devices.md)原有表格，说明为什么无需增加 SDA/SCL，以及电容、上拉、速率、地址冲突和 stuck-low 的限制。没有真实设备时保持 Pending。

以上三个任务可复用蓝色 ESP8266，但不同 sketch 每次需分别烧录。若继续接黑色分析仪：蓝色 D2→黑色 GPIO21/CH0，蓝色 D1→黑色 GPIO19/CH1，共地并保留黑色 GPIO22→GPIO23。本次没有执行这些烧录或新实验。
