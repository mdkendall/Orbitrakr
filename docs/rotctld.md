---
title: Rotctld
nav_order: 5
---

# Rotctld Interface

Orbitrakr presents an interface on TCP port 4533 compatible with the [Hamlib](https://github.com/Hamlib/Hamlib) rotctld protocol.

This means Orbitrakr is compatible with any software that will talk to rotctld, for example [Gpredict](http://gpredict.oz9aec.net/) and the [SatNOGS](https://wiki.satnogs.org/) client.

Programs like this, run on a separate computer, can be used to manually control the rotator, or to track satellites and command the rotator to point the antenna as they pass overhead.

## Commands Supported

Orbitrakr supports the following commands in both the short and long forms:

- `P` or `set_pos` Set target position
- `p` or `get_pos` Get current position
- `S` or `stop` Stop
- `R` or `reset` Reset (i.e. home)
- `_` or `get_info` Get product information
