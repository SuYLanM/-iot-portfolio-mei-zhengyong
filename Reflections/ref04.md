# Reflection 04 — Match the sensing principle to the decision

> I prepared this reflection from attributed records; my personal conclusions and undocumented collaboration remain pending my confirmation.

## Take-away

I would first ask whether I need presence, a threshold alarm, level or volume. My conductive probe separated wet and dry conditions, but that does not make it a calibrated continuous gauge. I need a verified relationship between the measured quantity and the tank decision.

## What was good?

I saved 105 conductive-probe samples across five conditions. They support a point-level alarm more clearly than continuous depth. My StickS3/BMI270 records also include level, tilt, shaking and return-to-rest windows. I can compare those observations in my [IMU report](../Module04/Task03_IMU/experiment-report.md) instead of inferring motion from a single photograph.

## What was difficult?

I obtained zero readings during the initial capacitive tests. My later simulation illustrates an expected trend but does not fix that hardware result. I also received two ADC datasets with different values and timing; their acquisition provenance remains unresolved. I cannot use a setup photograph to verify the calibration behind those numbers.

## Exploration and show stoppers

I explored continuous-level modelling and the effect of motion on orientation estimates. I understand that acceleration-derived roll and pitch become less reliable during shaking. I still lack a verified continuous volume calibration and a synchronized motion-versus-liquid measurement.

## Interaction with peers and instructors

I included LudwigMo's Sections 4–6 as [partner reference material](../Module04/partner_reference/README.md), preserving attribution. I cannot describe those measurements as my own or infer a discussion merely because I copied the reference. I need to confirm any actual exchange or instructor feedback.

## How did you verify AI suggestions?

I used Codex to prepare acquisition, analysis and documentation. I checked the real CSV windows, preserved ADC-zero failures, labelled synthetic curves and separated operator-supplied data from automatically captured logs. I did not accept a visually plausible depth curve as hardware proof.

## Help and extra work

I used the partner's documented sensor-selection reasoning and Codex's organization help. I have no confirmed record of peer teaching or a joint demonstration.

## Self-assessment against learning goals

I can support binary detection and motion observations. I need further evidence for continuous level-to-volume uncertainty, my own hybrid system and a confirmed pod recommendation; my [goal map](../Module04/README.md#learning-goals) makes these gaps visible.
