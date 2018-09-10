/*
 * 16n Faderbank Configuration
 * (c) 2017,2018 by Brian Crabtree, Sean Hellfritsch, Tom Armitage, and Brendon Cassidy
 * MIT License
 */

// restricts output to only channel 1 for development purposes
// #define DEV 1

// reverses faders left-to-right
// #define REV 1

// activates printing of debug messages
// #define DEBUG 1

// turn on power LED
// #define LED 1

// MASTER MODE allows you to broadcast values from the 16n
// this supports up to 4 TXo modules and/or up to 4 Ansible devices and/or 1 ER-301
// uncomment this #define and compile the firmware
//
// NOTE: in MASTER MODE the 16n will not respond to the Teletype
//
//#define MASTER 1
 
// minimum and maximum values for faders (to deal with tolerances)
#define MINFADER 15
#define MAXFADER 8135

#ifdef DEV

const int channelCount = 1; 
const int ports[] = { A0 };
const int usb_ccs[]= { 32 };
const int trs_ccs[]= { 32 };

#else

const int channelCount = 16;

#ifdef REV
const int ports[] = { A15, A14, A13, A12, A11, A10, A9, A8, A7, A6, A5, A4, A3, A2, A1, A0 };
#else
const int ports[] = { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15 };
#endif

// set up CCs.
// if you wish to have different CCs for TRS and USB, specify them here.

const int usb_ccs[]= { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48 };
const int trs_ccs[]= { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48 };

// set up MIDI channels for each fader
// if you wish to have different channels for TRS and USB - or for each channel - specify them here.

const int usb_channels[]= {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const int trs_channels[]= {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

#endif

