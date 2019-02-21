# 16n Firmware

Written for Teensy 3.2.

## Requirements

- Latest Teensyduino install.
- `ResponsiveAnalogRead` library
- `CD74HC4067` library

## Compilation

- You **must** compile this with Tools->USB type set to **MIDI**. (**Serial+MIDI** may have issues with certain devices you connect to).
- Be sure that the board speed is set to 120mhz (overclock) for maximum repsonsiveness.
- If you're having issues compiling related to MIDI libraries or code: make sure your Arduino `libraries` folder doesn't have any old versions of weird MIDI libraries in. The MIDI library should be installed by default via Teensyduino; otherwise, if you have the latest version of the 47effects MIDI library in your Libraries Manager, that'll also behave. It turns out that older versions in the legacy `libraries` folder sometimes lead to conflicts.

## Customisation and configuration

Most configuration you'll want to do is handled in `config.h`, through a set of `define` statements.

```C
#define REV 1
```

will reverse the order of the faders in MIDI and I2C, right-to-left

```C
#define FLIP 1
```

will invert the direction of the faders, top-to-bottom, for MIDI and I2C. Combined with `REV`, this will allow you to use the faderbank upside down - useful if your sole connection is I2C.

```C
#define DEBUG 1
```

will log debug messages to the serial port.

```C
#define DEV 1
```

will restrict the faderbank to its first channel. Designed for breadboard development; almost certainly not of interest.

```C
#define MASTER 1
```

will put the 16n into I2C MASTER mode, broadcasting values from the 16n directly.

MASTER MODE supports up to 4 TXo modules and/or up to 4 Ansible devices and/or 1 ER-301.

If you want to use 16n with an ER-301, you need to turn MASTER MODE on. If you want to use it with a Monome Teletype, leave you want MASTER MODE off - leave it commented out.

### Customising continuous controller numbers and MIDI channels

`config.h` also contains the MIDI configuration.

Midi Continuous Controller numbers are specified by the `usb_ccs` and `trs_ccs` variables; each array has sixteen items, for the faders, left to right. You can alter these should you want specific CCs to come out of the faderbank (if, for instance, you have a tool with fixed mapping, or want to directly control a piece of hardware). It is possible to set the USB MIDI port and the TRS MIDI port to output on different CCs if you wish.

Channels are set per-fader in `usb_channels` and `trs_channels`; again, you can have a different channel per fader if you'd like, and different channels for USB and TRS.

## LICENSING

see `LICENSE`
