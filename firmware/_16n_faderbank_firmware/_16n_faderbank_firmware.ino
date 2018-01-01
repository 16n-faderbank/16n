/*
 * 16n Faderbank Firmware
 * (c) 2017,2018 by Brian Crabtree, Sean Hellfritsch, Tom Armitage, and Brendon Cassidy
 * MIT License
 */

/*
 * NOTES:
 * - Hardware MIDI is on pin 1
 * - You **must** also compile this with Tools->USB type set to MIDI or MIDI/Serial (for debugging)
 * - You also should overclock to 120MHz to make it as snappy as possible
 */

// restricts to only 1 channel
// #define DEV 1

// activates printing of debug messages
// #define DEBUG 1
 
#include <i2c_t3.h>
#include <MIDI.h>
#include <ResponsiveAnalogRead.h>

#include "TxHelper.h"

MIDI_CREATE_DEFAULT_INSTANCE();

// the midi channel for the device to output
const int channel = 1;

#ifdef DEV
const int channelCount = 1; 
const int ports[] = { A0 };
const int usb_ccs[]= { 32 };
const int trs_ccs[]= { 32 };
#else
const int channelCount = 16;
const int ports[] = { A15, A14, A13, A12, A11, A10, A9, A8, A7, A6, A5, A4, A3, A2, A1, A0 };

// set up CCs.
// if you wish to have different CCs for TRS and USB, specify them here.

const int usb_ccs[]= { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48 };
const int trs_ccs[]= { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48 };

#endif

// loop helpers
int i, temp;

// midi write helpers
int q, shiftyTemp;

// the storage of the values; current is in the main loop; last value is for midi output
int volatile currentValue[channelCount];
int lastValue[channelCount];

// the thing that smartly smooths the input
ResponsiveAnalogRead *analog[channelCount];

// the MIDI write timer
IntervalTimer midiWriteTimer;
int midiInterval = 5000; // 5ms

// helper values for i2c reading and future expansion
int activeInput = 0;
int activeMode = 0;

/*
 * The function that sets up the application
 */
void setup() {

  #ifdef DEBUG
  while(!Serial);
  Serial.print("16n Firmware Debug Mode\n");
  #endif

  // initialize the TX Helper
  TxHelper::UseWire1(true);
  TxHelper::SetPorts(16);
  TxHelper::SetModes(4);

  // set read resolution to 16 bit (13 is actually useable)
  analogReadResolution(16);  

  // initialize the value storage
  for (i=0; i<channelCount; i++){
    // analog[i] = new ResponsiveAnalogRead(0, false);
    analog[i] = new ResponsiveAnalogRead(0, true, .0001);
    analog[i]->setAnalogResolution(1<<16); 
    currentValue[i] = 0;
    lastValue[i] = 0;
  }

  // i2c using the alternate I2C pins on a Teensy 3.2
  Wire1.begin(I2C_SLAVE, 0x80, I2C_PINS_29_30, I2C_PULLUP_EXT, 400000); 
  Wire1.onReceive(i2cWrite);  
  Wire1.onRequest(i2cReadRequest);

  // turn on the MIDI party
  MIDI.begin();
  midiWriteTimer.begin(writeMidi, midiInterval);
  
}

/*
 * The main read loop that goes through all of the sliders
 */
void loop() {
  
  // read loop using the i counter
  for(i=0; i<channelCount; i++) {
    
    // read the value
    temp = analogRead(ports[i]);
    
    // put the value into the smoother
    analog[i]->update(temp);
    
    // update the value 
    noInterrupts();
    currentValue[i] = analog[i]->getValue();
    interrupts();
  }
  
}

/*
 * The function that writes changes in slider positions out the midi ports
 */
void writeMidi(){
  
  // write loop using the q counter (
  // (can't use i or temp cuz this might interrupt the reads)
  for(q=0; q<channelCount; q++) {
    
    // shift for MIDI precision (0-127)
    noInterrupts();
    shiftyTemp = currentValue[q] >> 9;
    interrupts();    
    
    // if there was a change in the midi value
    if(shiftyTemp != lastValue[q]) {
      
      // send the message over USB and physical MIDI
      usbMIDI.sendControlChange(usb_ccs[q], shiftyTemp, channel);
      MIDI.sendControlChange(trs_ccs[q], shiftyTemp, channel);
      
      // store the shifted value for future comparison
      lastValue[q] = shiftyTemp;

      #ifdef DEBUG
      Serial.printf("MIDI[%d]: %d\n", q, shiftyTemp);
      #endif
    }
  }
}

/*
 * The function that responds to a command from i2c.
 * In the first version, this simply sets the port to be read from.
 */
void i2cWrite(size_t len){
    
  #ifdef DEBUG
  Serial.printf("i2c Write (%d)\n", len);
  #endif

  // parse the response
  TxResponse response = TxHelper::Parse(len);

  // true command our setting of the input for a read?
  if (len == 1) {

    // use a helper to decode the command
    TxIO io = TxHelper::DecodeIO(response.Command);
    
    #ifdef DEBUG
    Serial.printf("Port: %d; Mode: %d [%d]\n", io.Port, io.Mode, response.Command);
    #endif
    
    // this is the single byte that sets the active input
    activeInput = io.Port;
    activeMode = io.Mode;
    
  } else {
    // act on the command
    actOnCommand(response.Command, response.Output, response.Value);
  }
  
}

/*
 * The function that responds to read requests over i2c.
 * This uses the port from the write request to determine which slider to send.
 */
void i2cReadRequest(){
    
  #ifdef DEBUG
  Serial.print("i2c Read\n");
  #endif
  
  // disable interrupts. get and cast the value
  uint16_t shiftReady = 0;
  switch(activeMode){
    case 1:
      noInterrupts();
      shiftReady = (uint16_t)currentValue[activeInput];
      interrupts();
      break;
    case 2:
      noInterrupts();
      shiftReady = (uint16_t)currentValue[activeInput];
      interrupts();
      break;
    default:
      noInterrupts();
      shiftReady = (uint16_t)currentValue[activeInput];
      interrupts();
      break;
  }
    
  // take from 16 bits and reduce to the teensy's 13 useable 
  // and then up to the teletype's 14 bits
  shiftReady = shiftReady >> 3 << 1;
  
  #ifdef DEBUG
  Serial.printf("delivering: %d; value: %d [%d]\n", activeInput, currentValue[activeInput], shiftReady);
  #endif

  // send the puppy as a pair of bytes
  Wire1.write(shiftReady >> 8);
  Wire1.write(shiftReady & 255);

}

/*
 * Future function if we add more i2c capabilities beyond reading values.
 */
void actOnCommand(byte cmd, byte out, int value){}

