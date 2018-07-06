# 16n Firmware

Written for Teensy 3.2.

## Requirements

Latest Teensyduino install.

## Compilation

- You **must** compile this with Tools->USB type set to MIDI or MIDI/Serial (for debugging)
- Be sure that the board speed is set to 120mhz (overclock) for maximum snappiness.

## Customisation and configuration

Most configuration you'll want to do is handled in `config.h`, through a set of `define` statements.

```C
#define DEV 1
```

will restrict everything to a single channel. Designed for breadboard development.

```C
define REV 1
```

will reverse the order of the analogue ports.

```C
#define DEBUG 1
```

will log debug messages to the serial port.

```C
#define MASTER 1
```

MASTER MODE allows you to broadcast values from the 16n directly. This supports up to 4 TXo modules and/or up to 4 Ansible devices and/or 1 ER-301. In MASTER MODE the 16n will not respond to a monome Teletype.

### Customising continuous controller numbers and MIDI channels

`config.h` also contains the MIDI configuration.

Midi Continuous Controller numbers are specified by the `usb_ccs` and `trs_ccs` variables; each array has sixteen items, for the faders, left to right. You can alter these should you want specific CCs to come out of the faderbank (if, for instance, you have a tool with fixed mapping, or want to directly control a piece of hardware). It is possible to set the USB MIDI port and the TRS MIDI port to output on different CCs if you wish. 

Channels are set per-fader in `usb_channels` and `trs_channels`; again, you can have a different channel per fader if you'd like, and different channels for USB and TRS.

## LICENSING

see `LICENSE`