# Simple Open EtherCAT Master Library for Arduino (Beta Ver.)

This is SOEM (Simple Open EtherCAT Master) library ported for Arduino.

It's just for educational or experimental purposes.

The original SOEM is at https://github.com/OpenEtherCATsociety/SOEM

## Supported hardware

* Arduino Due + Ethernet Shield 2 (W5500)
* ESP32 + WIZ850io (W5500)
* GR-SAKURA (Renesas RX63N)
* GR-ROSE (Renesas RX65N)

## Notes for ESP32

Connect ESP32 and W5500 as shown below.

| ESP32-DevKitC | WIZ850io (W5500) |
| ---- | ---- |
| GND  | GND  |
| 3V3  | 3.3V |
| IO23 (VSPI MOSI) | MOSI |
| IO19 (VSPI MISO) | MISO |
| IO18 (VSPI SCK)  | SCLK |
| IO5  (VSPI SS)   | SCSn |  

## Notes for GR-ROSE

You must create a user task to use SOEM.

GR-ROSE's main task (setup & loop) has only 512 byte stack.
On the other hand, SOEM uses more than 3k byte local variables.
Therefore, the memory corruption will occur when you call SOEM's API on the main task.
You must creat a task with more than 4k byte stack and then call SOEM's API on the task.
Please see the sample sketch (slaveinfo.ino).

## Documentation of original SOEM
See https://openethercatsociety.github.io/doc/soem/

## Article about this library (written in Japanese)
See https://lipoyang.hatenablog.com/entry/2019/08/13/125008
