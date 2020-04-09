# 16n Firmware

16n Firmware is designed to run on a Teensy 3.2; it is also compatible with a Teensy LC, although the official BOM recommends a 3.2.

This README serves as a guide for **developing** and **compiling** your own versions of the firwmare. Since firmware v2.0.0, the recommended method for putting firmware onto a 16n is to use Teensy Loader directly. [Find out more on the wiki][load-firmware]

If you are interested in compiling your own firmware, or hacking on it, read on!

## Requirements

- Latest Teensyduino install.
- `ResponsiveAnalogRead` library
- `CD74HC4067` library

## Compilation

- You **must** compile this with Tools->USB type set to **MIDI**. (**Serial+MIDI** may have issues with certain devices you connect to).
- Be sure that the board speed is set to 120mhz (overclock) for maximum repsonsiveness.
- If you're having issues compiling related to MIDI libraries or code: make sure your Arduino `libraries` folder doesn't have any old versions of weird MIDI libraries in. The MIDI library should be installed by default via Teensyduino; otherwise, if you have the latest version of the 47effects MIDI library in your Libraries Manager, that'll also behave. It turns out that older versions in the legacy `libraries` folder sometimes lead to conflicts.

## Customisation and configuration

As of 16n firmware 2.0.0, you no longer should do ANY configuration through the Arduino IDE. All configuration is conducted from a web browser, using the [16n editor][editor]

When you upgrade to 2.0.0 you will LOSE ANY CONFIGURATION YOU HAVE from v1.3x. This is a one-time thing; apologies. You will not lose future configurations (and also, they are easier to back up).

The 16n will be initialised to a set of default settings (All outputs for TRS and USB set to MIDI channel 1, CCs 32-47, I2C set to follower). Once this is done, connect over USB, and go to the [editor][editor] in Google Chrome; you will be able to see the current configuration, edit the configuration, and transmit the new config to your device. You will likely need to customise the maximum fader value calibrations.

Note that if you do change any config related to I2C, you should power-cycle the 16n before it will be picked up.

Some options _do_ remain in `config.h`; they are for developers to specify options that are likely to need setting once, or adjusting during the development process:

In `config.h`

```C
#define DEBUG 1
```

will log debug messages to the serial port.

```C
#define DEV 1
```

will restrict the faderbank to its first channel. Designed for breadboard development; almost certainly not of interest.

## Memory Map

Configuration is stored in the first 80 bytes of the on-board EEPROM. It looks like this:

Addresses 0-15 are reserved for configuration flags/data.

FADERMAX and FADERMIN are 14-bit numbers; as such, they are stored in two bytes as MSB and LSB; the actual number is calculated by `(MSB << 8) + LSB`

+---------+--------+------------------------------------+
| Address | Format |            Description             |
+---------+--------+------------------------------------+
| 0       | 0/1    | LED on when powered                |
| 1       | 0/1    | LED blink on MIDI data             |
| 2       | 0/1    | Rotate controller outputs via 180º |
| 3       | 0/1    | I2C Master/Follower                |
| 4,5     | 0-127  | FADERMIN lsb/msb                   |
| 6,7     | 0-127  | FADERMAX lsb/msb                   |
| 8-15    |        | Currently vacant                   |
+---------+--------+------------------------------------+
| 16-31   | 0-15   | Channel for each control (USB)     |
| 32-47   | 0-15   | Channel for each control (TRS)     |
| 48-63   | 0-127  | CC for each control (USB)          |
| 64-79   | 0-127  | CC for each control (TRS)          |
+---------+--------+------------------------------------+

## LICENSING

see `LICENSE`

[load-firmware]: https://github.com/16n-faderbank/16n/wiki/Firmware:-installation-instructions
[editor]: https://16n-faderbank.github.io/editor
