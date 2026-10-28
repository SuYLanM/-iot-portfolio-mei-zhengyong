# Task 9 A 类：ESP32 Logic Analyzer + PulseView 中文操作手册

**Software Prepared / Hardware Validation Pending。** 本手册没有实测波形、成功烧录或课堂决定。[英文报告](scope-decision.md) · [构建核验](task09-analyzer-build.md) · [空白实验记录](evidence/task09-logic-analyzer-record.md)。

## 1. 准备两块板，分别运行固件

| 角色 | 烧录的程序 | 串口用途 |
|---|---|---|
| 分析仪 A | 上游 `ESP32_LogicAnalyzer/ESP32_LogicAnalyzer.ino`，连同全部同目录源码及本项目时钟补丁 | UART0，**921600 / 8N1**，由 PulseView 独占 |
| 信号源 T：先测 PWM | [task09_pwm_reference.ino](code/task09_pwm_reference/task09_pwm_reference.ino) | Serial Monitor **115200**，打印配置参数 |
| 信号源 T：之后改测 I2C | [task09_i2c_capture_source.ino](code/task09_i2c_capture_source/task09_i2c_capture_source.ino) | Serial Monitor **115200**，打印扫描结果，接收 `s` |

T 每次只能运行一种固件。给 T 改烧 I2C 程序时，A 保持分析仪固件。不要合并三份程序，也不能仅凭一块板烧录成功就声称完成双板捕获。

准备两条 USB 数据线、短杜邦线。I2C 阶段还需要到 **T 的 3.3 V** 的合适上拉；要观察真实 ACK，需要已确认电压和地址的 I2C 设备。只有上拉而没有设备时可以研究 NACK，不能声称发现设备。

## 2. 下载、版本和 Arduino IDE 编译

上游仓库：[lmcapacho/ESP32_LogicAnalyzer](https://github.com/lmcapacho/ESP32_LogicAnalyzer)。本次核对的默认分支 HEAD：

`5c4f2b9fd85ac09aca3e42596e6e7e0b640cdab3`

可以下载[此提交的源码 ZIP](https://github.com/lmcapacho/ESP32_LogicAnalyzer/archive/5c4f2b9fd85ac09aca3e42596e6e7e0b640cdab3.zip)，解压到独立工具目录，保留整个 sketch 文件夹；不要只拷贝 `.ino`。无需下载 KiCad、PCB 工具或额外 Arduino 第三方库。Arduino IDE 内安装 `esp32 by Espressif Systems`。上游写明测试过 Core **3.3.7**，本机实际安装 **3.3.12**，本次已对后者做真实编译检查；这不是硬件兼容性认证。

**Core 3 必须处理时钟重设问题：** 上游 `LogicAnalyzer.cpp` 的 `enable_out_clock()` 每次请求采样率都调用 `ledcAttach()`。Core 3.3.12 已绑定引脚会拒绝重复 attach，不能据此认为时钟已改变。应用本项目的 [Core 3 补丁](patches/task09-core3-clock-retune.patch)，它首次 attach、随后调用 `ledcChangeFrequency()`。补丁的源码依据和编译记录见[构建核验](task09-analyzer-build.md)。其运行、频率量化及误差仍 Pending。

如果使用 Git，在**上游仓库根目录**执行（把补丁路径替换为你的实际绝对路径）：

```powershell
git checkout 5c4f2b9fd85ac09aca3e42596e6e7e0b640cdab3
git apply --check "<portfolio>\Module02\patches\task09-core3-clock-retune.patch"
git apply "<portfolio>\Module02\patches\task09-core3-clock-retune.patch"
```

若下载 ZIP 而不用 Git，可以按补丁中的 `+` 行修改 `LogicAnalyzer.cpp` 的对应 Core 3 分支，保留 `#else` 的旧 Core 分支。不要改引脚、UART 波特率或启用 `_DEBUG_MODE_`。应用后打开：

`ESP32_LogicAnalyzer/ESP32_LogicAnalyzer.ino`

Arduino IDE 设置：**ESP32 Dev Module → Core 3.3.12 → Core Debug Level: None**，选择 A 对应的实际 COM 口，先 Verify，再由你检查硬件后 Upload。无需为了本方案降级全局 Core。烧录时可能需按实物板的 BOOT/EN 操作；不能把尚未执行的烧录写成通过。

## 3. 分析仪 A 的完整引脚表

来自固定提交的 [LogicAnalyzerConfig.h](https://github.com/lmcapacho/ESP32_LogicAnalyzer/blob/5c4f2b9fd85ac09aca3e42596e6e7e0b640cdab3/ESP32_LogicAnalyzer/LogicAnalyzerConfig.h)。CH 编号不是 GPIO 编号。

| 通道 | A GPIO | 通道 | A GPIO |
|---|---|---|---|
| CH0 | 21 | CH8 | 14 |
| CH1 | 19 | CH9 | 27 |
| CH2 | 18 | CH10 | 26 |
| CH3 | 5 | CH11 | 25 |
| CH4 | 4 | CH12 | 33 |
| CH5 | 15 | CH13 | 32 |
| CH6 | 13 | CH14 | 35 |
| CH7 | 12 | CH15 | 34 |

| A 上占用 / 保留资源 | 用途与限制 |
|---|---|
| GPIO22，LEDC 输出 | CLK_OUT；必须只跳接到 A GPIO23，不是探头输入 |
| GPIO23，I2S0 输入时钟 | CLK_IN；接 A GPIO22，不接目标 SCL |
| 上表 16 路输入，I2S0 并行 DMA | 捕获资源；不要同时运行其他 I2S/信号源固件 |
| GPIO2 | 捕获状态 LED；也是启动配置相关引脚 |
| GPIO1 TX0 / GPIO3 RX0 | 默认 UART0，通过板载 USB-UART 连接电脑；不上探头 |
| GPIO6–11 | 经典模块通常由 SPI Flash 占用，不可接探头 |
| GPIO16/17 | 默认非探头；调试模式可能用 UART2，部分带 PSRAM 模块也占用，勿自行复用 |
| GPIO0、2、5、12、15 | 启动配置相关；初次测试避免外部信号影响复位，特别不要把 GPIO12 拉成不合适电平 |
| GPIO34/35 | 输入专用、无内部上拉；不是输出信号源 |

默认 `_DEBUG_MODE_` 关闭。启用后上游会改用 Serial2、3 Mbaud 等另一套配置，本手册不支持这种未经核对的混用。源码注释中对 UART0 RX/TX 有颠倒描述；此表按经典 ESP32 的 **TX0=GPIO1、RX0=GPIO3** 标注。

## 4. 第一次先接 PWM：只接这三根线

先断电，分别确认板 A/T。接好后再上电：

| 起点 | 终点 | 用途 |
|---|---|---|
| **A GPIO22** | **A GPIO23** | 分析仪自身采样时钟跳线 |
| T GND | A GND | 共地 |
| **T GPIO25** | **A GPIO21 / CH0** | 1 kHz PWM 探测 |

两块板各自 USB 供电，不连接两块板的 3V3/5V 输出。所有被测输入只能是兼容 **3.3 V 逻辑**；裸 ESP32 没有 5 V 容忍，也不能测负电压或市电。上游有电平转换 PCB 设计，不代表你的裸板已有保护。

在 T 的 Arduino IDE 中打开 PWM `.ino`、选 T 的 COM 口烧录，Serial Monitor 115200 确认实际输出的配置状态。目标是 GPIO25、1 kHz、50%，配置打印不是频率测量。A 的 Serial Monitor/Plotter 必须关闭，T 的 Monitor 可以使用另一个 COM 口。

## 5. PulseView 连接与初次捕获

本机已找到 64-bit PulseView，命令行自报 **0.5.0-git-e2fe9df**，支持 `ols`。这只确认程序能运行及驱动被列出，尚未连接 A。无需仅为本实验再次安装 PulseView。若另一台电脑缺失，使用 [sigrok 官方下载入口](https://sigrok.org/wiki/Downloads) 获取 Windows 64-bit 版本，并核对安装来源。

1. 在 Windows 设备管理器中分别插拔 A/T，识别实际 COM 口，记下 USB-UART 芯片和端口；不假定 A 是 COM3。缺驱动时按实物桥接芯片使用相应厂商驱动。**不要用 Zadig 随意替换 USB-UART 的 COM 驱动**。
2. A 烧录完成并退出 Serial Monitor 后，打开 PulseView 的设备选择/Connect to Device 对话框。选择 **Openbench Logic Sniffer & SUMP compatibles (`ols`)**，不是 Demo、fx2lafw 或普通 UART 解码器。
3. 选择 A 的串口，明确设置 **921600、8 data bits、no parity、1 stop bit**。ols 驱动默认可能是 115200，不能照默认直接扫描。上游 README 的 912600 笔误不能用于连接。
4. Scan/连接；应该以实际发现的 SUMP 设备为准。若 GUI 不提供对应串口配置，可使用已核对支持 `-D`/`-d` 的启动方式：

```powershell
& "D:\PulseView\pulseview.exe" -D -d "ols:conn=COM7:serialcomm=921600/8n1"
```

这里安装路径对应本机，**COM7 只是占位示例**，必须替换为 A 的实际端口。正常连接不需要给 A 打开文字串口监视器。

5. 初次选 CH0，采样率请求 **1 MS/s**、采样数 **16k**（若 UI 显示 16.384k，记录实际选项）、**RLE 关闭**、触发 None/无触发。先用短记录，避免超过设备实际接受的样本数。不要开启自测 pattern、外部时钟选项或把 20 MS/s 当作已验证值。A 的硬件时钟跳线仍然必须存在，PulseView 的 External Clock 选项与这个本地跳线不是一回事。
6. Run 单次捕获，应等待采集和串口回传完成。1 MS/s × 16k 样本约为 16 ms 的理论捕获窗口；回传耗时不等于采样时间。无波形时先查接线和 GPIO，不要伪造图像。
7. 放大 CH0 两个相邻同方向边沿，用双游标读取周期 T，计算 `f=1/T`；测量高电平持续时间 `tH`，计算 `duty=tH/T×100%`。1 ms / 50% 是配置目标，实际数值由你填写。换数个周期和采样率重复比较，记录稳定性与漏边沿。
8. 如需触发，先用 CH0=LOW 或 HIGH 的**单阶段电平触发**。已核对 ols 源码只列出 0/1 匹配，固件也只处理 stage 0；不要假定它支持硬件上升沿、多阶段、脉宽触发或精确预触发位置。若触发一直不结束，取消触发并重新捕获。

## 6. 换成 I2C 扫描与解码

断电后，将 T 改烧独立 I2C 捕获源程序。移除 PWM 探测线，保留 A 的时钟跳线，按下表接线：

| 起点 | 终点 |
|---|---|
| T GPIO21 SDA | A GPIO21 / CH0 |
| **T GPIO22 SCL** | **A GPIO19 / CH1** |
| T GND | A GND |
| A GPIO22 | A GPIO23，仅 A 本地 |
| T SDA / SCL | 各自通过合适上拉到 T 3V3；先检查传感器板是否已有上拉 |
| 可选已核对传感器 SDA/SCL/GND/供电 | 连接 T 的总线及匹配电源，禁止 5 V 上拉 |

不要将 T SCL 接到 A GPIO22，这会与分析仪时钟输出冲突。4.7 kΩ/线可作为短线低速设计的起点，但须依据模块已有上拉、总线电容和允许灌电流核对，不是已验证阻值。

PulseView 开 CH0/CH1，请求 1 MS/s 或以上，先 16k 样本、RLE 关。I2C 配置速率 100 kHz，教授建议至少 1 MS/s；不是本项目实测采样率。扫描间有约 3 秒空闲，短自由采集可能只得到 HIGH：可设 CH1/SCL=LOW 电平触发等下一次扫描，或先 Arm、再向 **T 的 115200 串口**发送 `s`。不要向 A 的 SUMP 串口发送文字。

添加 **I2C** 协议解码器，将 **SDA=CH0、SCL=CH1**，尽量选择 7-bit 地址显示。先显示原始两条数字线，再看解码注释。逐项检查：

- START：SCL 为 HIGH 时 SDA 下降。
- 七位地址与 R/W=0（扫描器发出空写探测）。8-bit 线缆地址字节等于 `7-bit地址<<1`，不是另一个设备。
- 第九个 SCL 脉冲：SDA LOW 是 ACK，HIGH 是 NACK；应对照该地址的实际串口返回值。
- STOP：SCL 为 HIGH 时 SDA 上升。这个扫描程序不发送寄存器地址或传感器数据；不要把探测帧当成读取测量值。

无传感器时在正确上拉和实际波形下可记录 NACK；要证明 ACK，应加入真实兼容设备、核对其数据手册，并保存实际地址截图。调整采样率后检查测得 PWM 周期/时基是否保持合理，不能只相信 UI 标签或补丁编译通过。

## 7. 保存证据

每次保存原始 **sigrok `.sr`** 采样（Save/Save As，按实际文件类型选项）；若当前版本另存会话配置，则同时保存它。仅会话设置或截图不能替代原始采样，CSV/解码文本导出只作补充。重新打开 `.sr` 确认波形存在，必要时重新指定解码器映射。

需要真实保存：双板接线照、A 固件/Core/补丁编译记录、PulseView 版本及 ols/COM/波特率、PWM 波形与周期/占空比游标、I2C 两条线及 START/地址/RW/ACK/NACK/STOP、T 串口扫描日志。记录采样率/样本数/通道/触发/RLE，使用的上游 commit、补丁和目标程序 commit，以及从安装到首个可用捕获的真实时间。可参考[空白记录](evidence/task09-logic-analyzer-record.md)中的建议文件名；文件尚未存在时不要添加图片链接。

## 8. 排查表

| 症状 | 先核对 |
|---|---|
| 找不到设备 / COM 被占用 | 是否选 A 端口、921600/8N1、ols；关闭 A Monitor/Plotter、其他 PulseView 会话；等待复位完成后重试 |
| 采集不结束 | A22→A23 时钟跳线、触发条件、目标是否有活动、样本数；先关触发/RLE，减少采样数 |
| 切换采样率后频率明显改变 | 是否正确应用 Core 3 retune 补丁；记录实际时基并重新连接；不能把未经验证的显示当正确结果 |
| PWM 平线 | T 是否仍运行 PWM 固件、目标25是否接 A21/CH0、是否共地、配置是否报错 |
| I2C 只有 HIGH / 无解码 | 是否捕获到扫描窗口；触发/发送 s；上拉、电源、SDA/SCL 解码映射、两块板身份 |
| 全部 NACK | 可能没有设备、地址/供电/接线不符；先观察真实第九时钟，不自动判分析仪故障或传感器成功 |
| 地址翻倍 / 奇怪地址 | 7-bit 与 8-bit 显示、R/W 位、交换的 SDA/SCL、实际采样时基 |
| 启动失败 / 串口乱码 | 启动相关引脚外部电平、错误固件/板型、UART0 调试打印污染二进制流；不要改错 A/T 固件 |
| 高采样率漏边沿或异常 | 降低请求速率、短线、接地；记录失败。项目“最高20 MS/s”不是本项目保证值 |

## 9. 尚待真实完成

两板烧录、设备识别、PWM 捕获/测量、I2C ACK/NACK 解码、有效采样率与误差、截图/原始文件、课堂 demo 和最终 A/B 工具决定均 Pending。报告中的候选选择是准备阶段建议，不能替代全班决定。
