# Module 4 – Task 1: Conductive Water Level Detection

使用 ESP32 DevKit、双金属探针和 LM393 四引脚模块。断电接线，只让探针接触水，保持模块和 ESP32 干燥。

| LM393 模块 | ESP32 DevKit |
|---|---|
| `+` | `3V3` |
| `-` | `GND` |
| `AO` | `GPIO34` |
| `DO` | `GPIO18` |

使用 3.3 V 给模块供电，并确保 AO、DO 电平与 ESP32 的 3.3 V 输入兼容。实验结束后断电并擦干探针，以减少长期通电浸水造成的腐蚀。

## 打开程序和校准

1. 在 Arduino IDE 中打开本目录的 `Module4_Task1_WaterLevel.ino`。选择对应的 ESP32 DevKit 板型，经典 ESP32 可选 **ESP32 Dev Module**。选择实际串口，点击验证，再手动上传。
2. 打开串口监视器，设为 **115200 baud**。每 500 ms 输出 `time_ms,AO,DO,state`。AO 是 12 位原始读数（0–4095），DO 为 0 或 1。校准前状态为 `UNCALIBRATED`。
3. 把干燥探针放在空气中，等 DO 稳定后发送 `d`。再将两根探针浸入水中至清楚可重复的深度，等 DO 稳定后发送 `w`。两次 DO 必须不同，程序才会显示 `DRY` 或 `WET`。
4. 若两次 DO 相同，先检查接线和 AO 是否随浸水变化。如果模块有阈值电位器，在干湿条件之间调整并复查 DO。调整阈值后发送 `r`，重新记录两个参考。重启也会清空校准。

## 记录读数

按空气中、刚接触水、浅浸、较深浸入的顺序测试。尽量保持同一水样和探针角度，每档记录至少 5 行，另记条件、实际浸入深度和阈值设置。复制串口输出时保留表头与数据行，移除以 `#` 开头的提示行即可另存为 CSV。可比较每档 AO 的范围，以及 DO 在什么条件下切换。

`WET` 和 `DRY` 表示校准后的比较器阈值状态。探针刚触水时，DO 可能仍显示 `DRY`，此时也应记录 AO 的变化。AO 原始值需要校准才能换算成精确电压，测量也可能在量程端点饱和。

[Espressif Arduino-ESP32 ADC 文档](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html)
