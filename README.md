# 16n

Once upon a time, Sean Hellfritsch and Brian Crabtree [made a faderbank][linespost].

**16n** is the revised version of that object: it is open-source and ready for you to make, modify, or hack. 

It is currently at version **1.31**

# Repository contents

* `firmware` is the Teensy source code.
* `electronics` is schematics and gerber files for the PCB.
* `panel` is SVG/DXF files for top and bottom panels.
* `build` is build documentation.

## 16n Details

* 16 60mm faders
* sixteen CCs over USB-midi
* sixten CCs over minijack midi (with switch to swap between standards)
* sixteen 0-5V CV output jacks, one per channel.
* I2C: monome-style I2C protocol over TRS (tip is SDA, ring is SCL), works with Monome Teletype; also, I2C MASTER mode, works with monome Ansible, ER-301, TXo, etc (may require firmware patching or modification to connected device).
* Same width and similar height to monome 128 grid.

Power from 5V micro-usb into Teensy on left-hand side, or from your computer.

Toggle switch allows you to swap between 'Arturia/Novation' (tip is current source) and 'Korg/Makenoise' (ring is current source) standards; board is labelled such.

## Give us a picture, then

![](faderbank.jpg)

Here's a prototype board with the case off.

## Current status

Work in progress.

## BOM

[Available on Octopart][octobom]; a CSV version is in `electronics/bom-csv.csv`

Most parts you can get from eg. Mouser. I recommend buying PJ-302 jacks from [Thonk](https://thonk.co.uk) as an alternative to the more expensive CUI parts.

## Credits

Based on original work by [Brian Crabtree][tehn] and Sean Hellfritsch.  
Minijack MIDI, I2C circuitry and CV outputs by [Tom Armitage][infovore].  
Firmware by [Brian Crabtree][tehn], [Tom Armitage][infovore], and [Brendon Cassidy][bpcmusic].  

## Licensing

Panels and electronic schematics/layouts/gerber files are licensed under
[Creative Commons Attribution Share-Alike 4.0][ccbysa].

Firmware is licensed under the [MIT License][mitlicense].

[linespost]: https://llllllll.co/t/sixteen-n-faderbank/3643
[tehn]: https://github.com/tehn
[bpcmusic]: https://github.com/bpcmusic
[infovore]: https://github.com/infovore
[octobom]: https://octopart.com/bom-tool/nlacEI7z
[ccbysa]: https://creativecommons.org/licenses/by-sa/4.0/
[mitlicense]: https://opensource.org/licenses/MIT
