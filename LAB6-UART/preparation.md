# Lab 6: Universal Asynchronous Receiver-Transmitter (UART)

## Pre-Lab preparation

1. Use schematic of [Arduino Uno](https://oshwlab.com/tomas.fryza/arduino-shields) and find out on which Arduino Uno pins the UART transmitter (Tx) and receiver (Rx) are located.

| Pin | Function |
|---|---|
| PD0 | Rx       |
| PD1 | Tx       |

2. Remind yourself, what an [ASCII table](http://www.asciitable.com/) is. What codes are defined for control characters `Esc`, `Space`, `Tab`, and `Enter`?


|          Char           | Dec | Hex  |
|---|---|---|
| Tab                     |   9 | 0x9  |
| Enter (carriage return) |  13 | 0xD  |
| Esc                     |  27 | 0x1B |
| Space                   |  32 | 0x20 |
