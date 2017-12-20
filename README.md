# 16n

Once upon a time, Sean Hellfritsch and Brian Crabtree [made a faderbank][linespost].

**16n** is the revised version of that object.

* 16 faders
* sixteen CCs over USB-midi
* sixten CCs over minijack midi (with switch to swap between standards)
* sixteen 0-5V CV output jacks, one per channel.
* II (monome-style I2C protocol) over TRS: tip is SDA, ring is SCL).
* Same width and similar height to monome 128.

Power from 5V micro-usb into Teensy on left-hand side, or from your computer.

Toggle switch allows you to swap between 'Arturia/Novation' (tip is current source) and 'Korg/Makenoise' (ring is current source) standards; board is labelled such.

## Repository Contents

Source files for Autodesk Eagle. Firmware for Teensy.

## Current status

Work in progress.


## BOM

TBD

## Credits

Based on work by [Brian Crabtree][tehn] and Sean Hellfritsch. 
Minijack MIDI, I2C circuitry and CV outputs by [Tom Armitage][infovore].
I2C code by [Brendon Cassidy][bpcmusic].

[linespost]: https://llllllll.co/t/sixteen-n-faderbank/3643
[tehn]: https://github.com/tehn
[bpcmusic]: https://github.com/bpcmusic
[infovore]: https://github.com/infovore
