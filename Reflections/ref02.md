# Reflection 02 — An address is something I must observe

> I drafted this retrospective from my saved logs; my personal account and undocumented exchanges remain pending my confirmation.

## Take-away

I learned to distinguish a configured address from a responding device. My LCD answered at 0x27, and its address disappeared after I disconnected SDA. That before/after comparison is more convincing than assuming a common LCD address. I also need to distinguish a bus protocol from the labels on a module.

## What was good?

I obtained five connected sweeps and four disconnected sweeps in the [scanner records](../Module02/evidence/README.md). My later dual-target experiment checked both 0x27 and 0x42 and transferred data. I can use the [real log](../Module02/evidence/task07-two-devices/dual-wired-serial-20261102.txt) to support that claim rather than relying on an ACK alone.

## What was difficult?

I initially had both bus lines LOW. I preserved that failure instead of treating a successful upload as successful communication. I also had to separate DS1302 three-wire RTC operation from I2C. Its time display does not satisfy the missing sensor-library versus direct-register comparison.

## Exploration and show stoppers

I explored a second ESP32 controller as a wired target when I lacked a second sensor. I understand this as a useful adaptation, not two physical I2C sensors. I retained LOW/HIGH, PWM and NACK-only analyzer captures, but I cannot claim a calibrated timebase or decoded sensor data.

## Interaction with peers and instructors

I have a prepared CAN contribution and an existing group listing with Felix. I need to confirm actual presentation, comparison discussions and the class instrument decision; I have no recorded agreement to quote.

## How did you verify AI suggestions?

I used Codex to prepare diagnostics and interpret logs. I checked actual addresses and data, including rejection of stale replies. I did not accept an example BME280 sketch as evidence that a sensor had been measured.

## Help and extra work

I used Codex for firmware preparation, acquisition support and evidence organization. I still need to confirm any human help, feedback or sharing beyond these records.

## Self-assessment against learning goals

I have practical scanning and adapted multi-target evidence. I need further evidence for real sensor registers, address-conflict resolution and classroom agreement, as my [goal map](../Module02/README.md#learning-goals) shows.
