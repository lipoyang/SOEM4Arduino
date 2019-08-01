# Simple Open EtherCAT Master Library for Arduino (Beta Ver.)

This is SOEM (Simple Open EtherCAT Master) library ported for Arduino.

It's just a experimental implementation with some problems.

The original SOEM is at https://github.com/OpenEtherCATsociety/SOEM

## Supported hardware

* Arduino Due + Ethernet Shield 2 (W5500)
* GR-SAKURA

## Known issues

Following issues are known on GR-SAKURA.

- EtherCAT must be initialized after more than 3 seconds of power-on.
- It takes about 8 msec to receive a packet. It's too much.

## Documentation of original SOEM
See https://openethercatsociety.github.io/doc/soem/
