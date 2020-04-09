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
int POINT_VERSION = 0x01;

/*
 * device metadata
 */

#if defined(__MKL26Z64__) || defined(__MK20DX128__) || defined(_LC_DEBUG)
const int DEVICE_ID = 0x03; // 16nLC, do not change, needed by editor
#else
const int DEVICE_ID = 0x02; // 16n, do not change, needed by editor
#endif

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

#endif
