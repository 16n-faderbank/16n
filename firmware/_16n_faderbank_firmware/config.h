/*
 * 16n Faderbank Firmware Configuration
 * (c) 2017,2018,2020 by Brian Crabtree, Sean Hellfritsch, Tom Armitage, and Brendon Cassidy
 * MIT License
 */


/*
 * firmware metadata
 */

int MAJOR_VERSION = 0x02;
int MINOR_VERSION = 0x00;
int POINT_VERSION = 0x00;

/*
 * device metadata
 */

int DEVICE_ID = 0x02; // 16n

// restricts output to only channel 1 for development purposes
// #define DEV 1

// activates printing of debug messages
// #define DEBUG 1

// enables legacy compatibility with non-multiplexer boards
// #define V125

// I2C Address for Faderbank. 0x34 unless you ABSOLUTELY know what you are doing.
#define I2C_ADDRESS 0x34

#ifdef DEV

const int channelCount = 1;
const int ports[] = {A0};

#else

const int channelCount = 16;

#ifdef V125
// analog ports on the Teensy for the 1.25 board.
#ifdef REV
const int ports[] = {A15, A14, A13, A12, A11, A10, A9, A8, A7, A6, A5, A4, A3, A2, A1, A0};
#else
const int ports[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};
#endif

#endif

#endif
