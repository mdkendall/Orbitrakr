---
title: Home
nav_order: 1
---

Orbitrakr is an antenna rotator controller, intended for pointing an antenna at satellites in low earth orbit. The controller consists of an ESP32 microcontroller board running the Orbitrakr firmware, and two motor drivers. The motors drive the azimuth and elevation axes of the rotator, to which the antenna is mounted.

This is the user documentation. If you are interested in understanding the source code or contributing to the project, please see the documentation in the [Orbitrakr source code repository](https://github.com/mdkendall/Orbitrakr) on GitHub.

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


