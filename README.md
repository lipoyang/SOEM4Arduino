# Simple Open EtherCAT Master Library for Arduino (Beta Ver.)

This is SOEM (Simple Open EtherCAT Master) library ported for Arduino.

It's just for educational or experimental purposes. In addition, it has some known issues.

The original SOEM is at https://github.com/OpenEtherCATsociety/SOEM

## Supported hardware

* Arduino Due + Ethernet Shield 2 (W5500)
* GR-SAKURA

## Known issues

Following issues are known on GR-SAKURA.

- EtherCAT must be initialized more than 3 seconds after power-on.
- It takes about 8 msec to receive a packet. It's too much.

## Documentation of original SOEM
See https://openethercatsociety.github.io/doc/soem/

## Article about this library (written in Japanese)
See https://lipoyang.hatenablog.com/entry/2019/08/13/125008
