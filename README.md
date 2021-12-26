# ![Orbitrakr](assets/logo-320px-opt.png)

This project is a work-in-progress. Not all features described below are fully implemented yet.

## Description

Orbitrakr is an antenna rotator controller intended for pointing an antenna at satellites in low earth orbit.

The controller consists of an ESP32 processor board and two motor drivers. The motors drive the azimuth and elevation axes of the rotator, to which the antenna is mounted.

This repository contains the controller firmware.

The reference implementation uses an ESP32 DevKit v1 board, two ULN2003-based motor drivers, driving a rotator with two 28BYJ-48 geared stepper motors, to form a small desktop unit suitable for pointing lightweight antennas. However, the firmware will run on most ESP32-based boards, and the motor driver parameters are customisable enough that most driver and motor combinations could be used. You can use this same firmware to drive large, outdoor antenna rotators.

## Configuration

On initial startup Orbitrakr creates a Wi-Fi access point. Connecting to this (e.g. from a phone) provides access to a web interface where operating parameters can be configured. The most useful thing to configure initially is the SSID and password for your regular Wi-Fi network. After this is configured Orbitrakr will join your network and you can then use the web interface (e.g. from a desktop or laptop computer) to configure the remaining parameters, and view a dashboard where you can control and monitor operation.

Orbitrakr supports the Thingpings service. To find the device on your local network you can visit [www.thingpings.com](https://www.thingpings.com) and click on the Things tab.

## Operation

Orbitrakr provides several ways to control where the antenna is to be pointed:

### Manual Control via Web Interface

Orbitrakr can be manually commanded to point in a given direction directly via the web interface.

### Rotctld Interface

Orbitrakr presents an interface on port 4533 compatible with the [Hamlib](https://github.com/Hamlib/Hamlib) rotctld protocol. This means Orbitrakr is compatible with any software that will talk to rotctld, for example [Gpredict](http://gpredict.oz9aec.net/) and the [SatNOGS](https://wiki.satnogs.org/) client.

Programs like this, run on a separate computer, can be used to track many satellites and command the rotator to point the antenna as they pass overhead.

### Built-in Tracker

Orbitrakr can be commanded to track a specific satellite by NORAD catalog number. It can fetch Two-Line Element sets directly from Celestrak and contains an implementation of the Simplified General Perturbations (SGP4) propagator capable of predicting the position of the satellite over time.

### TinyGS Follower

Orbitrakr can choose which satellite to track by following a [TinyGS](https://tinygs.com/) station that is using the 'Auto Tune' feature. Orbitrakr can connect to the TinyGS MQTT broker to discover which satellite the station is listening to. As the station is commanded to listen to different satellites Orbitrakr will automatically track them.

## References

Vallado, David A., Paul Crawford, Richard Hujsak, and T.S. Kelso, "Revisiting Spacetrack Report #3" presented at the AIAA/AAS Astrodynamics Specialist Conference, Keystone, CO, 2006 August 21–24.

- https://celestrak.com/publications/AIAA/2006-6753/
- https://celestrak.com/publications/AIAA/2006-6753/AIAA-2006-6753.zip

David Vallado, "Fundamentals of Astrodynamics and Applications, Fourth Edition", Microcosm Press 2013

- https://celestrak.com/software/vallado-sw.php
- https://celestrak.com/software/vallado/cpp.zip


