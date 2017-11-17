# Sixteen-N Faderbank, minijack-MIDI version

Derived from [this post on lines][linespost], but adding a minijack connection to facilitate MIDI out. 

Toggle switch allows you to swap between 'Arturia/Novation' (tip is current source) and 'Korg/Makenoise' (ring is current source) standards; board is labelled such.

Implementation of hardware MIDI out and code is left to the reader; the two resistors and jack should be all you need - optocouplers are only relevant for MIDI inputs.

## Repository Contents

Source files for Autodesk EAGLE. Make your own Gerbers if you want.

## Current status

Completely untested.

## TODO

* Tidy frontpanel silking.
* Tidy legending silking
* Credits

## Credits

Based on work by [Brian Crabtree][tehn] and Sean Hellfritsch. 
Minijack MIDI, I2C circuitry and CV outputs by [Tom Armitage][infovore].
I2C code by TK

[linespost]: https://llllllll.co/t/sixteen-n-faderbank/3643
[tehn]: https://github.com/tehn
[infovore]: https://github.com/infovore

