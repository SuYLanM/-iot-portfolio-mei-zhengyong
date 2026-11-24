# Reflection 05 — Verify the light, the units and the source version

> I assembled this retrospective from source and photographs; personal reactions and undocumented exchanges remain pending my confirmation.

## Take-away

I learned that an LED demonstration needs an electrical explanation as well as a visible result. PWM duty, a resistor's voltage drop, perceived colour and supply current describe different things. I should check units and operating conditions before using any of them in a power claim.

## What was good?

I have separate PlatformIO projects for a fade, an RGB serial interface and an eight-pixel strip. My [strip photographs](../Module05/task3-strip/experiment-report.md) show selected positions retaining different colours, then one and all pixels illuminated. I can explain why one serial data line can control several integrated RGB pixels.

## What was difficult?

I found discrepancies between documentation and evidence. My fade constant selects GPIO13 despite an old GPIO16 comment. The strip photos show luminance 96, whereas the archived source uses 80. My RGB green readings support about 4.5 mA through a nominal 220 ohm resistor, but the blue-node assignments remain inconsistent.

## Exploration and show stoppers

I used NeoPixelBus with GRB order and an ESP32 RMT method rather than software-timed output. I have no archived Wi-Fi stress or gamma comparison. I can compute larger-strip budgets, but I cannot claim measured strip current from photographs displaying millivolts without a known measurement circuit.

## Interaction with peers and instructors

I still need to confirm any actual review, feedback or jointly agreed learning. I have a repository link prepared for sharing, but I have no evidence that I posted it in Discord.

## How did you verify AI suggestions?

I used Codex for drafts, serial-input troubleshooting and calculations. I compared the generated claims with GPIO constants, original photographs and meter units. I kept theoretical budgets labelled computed and retained missing current values as Not measured in my [module report](../Module05/README.md).

## Help and extra work

I used Codex to organize the projects, check links and add development rules. I cannot claim a classroom contribution or human feedback without confirming it.

## Self-assessment against learning goals

I have code and photographic evidence for PWM/RGB/addressable-light work. I still need current measurements, a dedicated fade recording and stronger timing/power verification, as my [goal map](../Module05/README.md#learning-goals) records.
