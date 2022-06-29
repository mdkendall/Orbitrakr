---
title: Home
nav_order: 1
---

# Point antennas at satellites.
{: .fs-9 }

Orbitrakr is an antenna rotator controller, for pointing an antenna at satellites in low earth orbit. An ESP32 microcontroller board runs the Orbitrakr firmware, and two stepper motors drive the azimuth and elevation axes of the rotator, which points the antenna.
{: .fs-6 .fw-300 }

This is the user documentation. If you are interested in understanding the source code or contributing to the project, please see the documentation in the [Orbitrakr source code repository](https://github.com/mdkendall/Orbitrakr) on GitHub.

---

## Overview

- Local web interface for configuration and operation.
    - Offers a Wi-Fi access point for initial configuration.
    - Connects to your Wi-Fi network for subsequent operation.
    - Supports Thingpings for easy discoverability.
- Dashboard shows current status.
- Manual control via the web interface.
- Rotctld compatible network interface.
- Built-in tracker can follow any satellite by NORAD catalog number.
- [TinyGS](https://tinygs.com/) follower can track whichever satellite your TinyGS station is listening to.

## Built-in Tracker

A particular feature of Orbitrakr is that it can be configured to track satellites directly by NORAD catalog number. It can fetch Two-Line Element sets from Celestrak and contains an implementation of the Simplified General Perturbations (SGP4) propagator capable of predicting the position of the satellite over time. You do not need to use an external program such as Gpredict to track satellites and thus Orbitrakr can operate standalone, without an external computer.

## TinyGS Follower

Another special feature of Orbitrakr is that it can choose which satellite to track by following a [TinyGS](https://tinygs.com/) station that is using the 'Auto Tune' feature. Orbitrakr can connect to the TinyGS MQTT broker to discover which satellite the station is listening to. As the station is commanded to listen to different satellites Orbitrakr will automatically track them.
