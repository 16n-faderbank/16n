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

/*
 * ALL configuration should take place in config.h.
 * You can disable/enable flags, and configure  MIDI channels in there.
 */

#include "config.h"
#include <i2c_t3.h>
#include <MIDI.h>
#include <ResponsiveAnalogRead.h>
#include <CD74HC4067.h>

#include "TxHelper.h"

MIDI_CREATE_DEFAULT_INSTANCE();

// loop helpers
int i, temp;

// midi write helpers
int q, shiftyTemp, notShiftyTemp;

// the storage of the values; current is in the main loop; last value is for midi output
int volatile currentValue[channelCount];
int lastMidiValue[channelCount];

#ifdef MASTER

// memory of the last unshifted value
int lastValue[channelCount];

// the i2c message buffer we are sending
uint8_t messageBuffer[4];

// temporary values
uint16_t valueTemp;
uint8_t device = 0;
uint8_t port = 0;

#endif

// the thing that smartly smooths the input
ResponsiveAnalogRead *analog[channelCount];

// mux config
CD74HC4067 mux(8,7,6,5);
#ifdef REV
const int muxMapping[16] =  { 8, 9, 10, 11, 12, 13, 14, 15, 7, 6, 5, 4, 3, 2, 1, 0 };
#else
const int muxMapping[16] = { 0,1,2,3,4,5,6,7,15,14,13,12,11,10,9,8 };
#endif

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

  // set read resolution to teensy's 13 usable bits
  analogReadResolution(13);  

  // initialize the value storage
  for (i=0; i<channelCount; i++){
    // analog[i] = new ResponsiveAnalogRead(0, false);

    analog[i] = new ResponsiveAnalogRead(0, true, .0001);
    analog[i]->setAnalogResolution(1<<13); 
    currentValue[i] = 0;
    lastMidiValue[i] = 0;
    #ifdef MASTER
    lastValue[i] = 0;
    #endif
  }

  // i2c using the alternate I2C pins on a Teensy 3.2
  #ifdef MASTER

  #ifdef DEBUG
  Serial.println("Enabling i2c in MASTER mode");
  #endif
  
  Wire1.begin(I2C_MASTER, 0x80, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000); 
  #else
  
  #ifdef DEBUG
  Serial.println("Enabling i2c enabled in SLAVE mode");
  #endif
  
  Wire1.begin(I2C_SLAVE, 0x80, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000); 
  Wire1.onReceive(i2cWrite);  
  Wire1.onRequest(i2cReadRequest);
  
  #endif

  // turn on the MIDI party
  MIDI.begin();
  midiWriteTimer.begin(writeMidi, midiInterval);

  #ifdef LED
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  #endif
}


/*
 * The main read loop that goes through all of the sliders
 */
void loop() {
  // read loop using the i counter
  for(i=0; i<channelCount; i++) {
    // set mux to channelCount
    mux.channel(muxMapping[i]);
    // read the value
    temp = analogRead(0); // mux goes into A0
    
    // put the value into the smoother
    analog[i]->update(temp);

    // read from the smoother, constrain (to account for tolerances), and map it
    temp = analog[i]->getValue();
    temp = constrain(temp, MINFADER, MAXFADER);
    temp = map(temp, MINFADER, MAXFADER, 0, 16383);
   
    
    // map and update the value 
    noInterrupts();
    currentValue[i] = temp;
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

    noInterrupts();
    notShiftyTemp = currentValue[q];
    interrupts();   
    // shift for MIDI precision (0-127)
    shiftyTemp = notShiftyTemp >> 7; 
    
    // if there was a change in the midi value
    if(shiftyTemp != lastMidiValue[q]) {
      // send the message over USB and physical MIDI
      usbMIDI.sendControlChange(usb_ccs[q], shiftyTemp, usb_channels[q]);
      MIDI.sendControlChange(trs_ccs[q], shiftyTemp, trs_channels[q]);
      
      // store the shifted value for future comparison
      lastMidiValue[q] = shiftyTemp;

      #ifdef DEBUG
      Serial.printf("MIDI[%d]: %d\n", q, shiftyTemp);
      #endif
    }
    
    #ifdef MASTER

    // we send out to all three supported i2c slave devices
    // keeps the firmware simple :)
    
    if (notShiftyTemp != lastValue[q]){
      
      #ifdef DEBUG
      Serial.printf("i2c Master[%d]: %d\n", q, notShiftyTemp);
      #endif

      // for 4 output devices
      port = q % 4;
      device = q / 4;

      // TXo
      sendi2c(0x60, device, 0x11, port, notShiftyTemp);

      // ER-301
      sendi2c(0xB0, 0, 0x11, q, notShiftyTemp);

      // ANSIBLE
      sendi2c(0xA0, device << 1, 0x06, port, notShiftyTemp);
    
      lastValue[q] = notShiftyTemp;
    }

    #endif
  }
}

#ifdef MASTER

/*
 * Sends an i2c command out to a slave when running in master mode
 */
void sendi2c(uint8_t model, uint8_t deviceIndex, uint8_t cmd, uint8_t devicePort, int value){
      
      valueTemp = (uint16_t)value;
      messageBuffer[2] = valueTemp >> 8;
      messageBuffer[3] = valueTemp & 0xff;

      Wire1.beginTransmission(model + deviceIndex);
      messageBuffer[0] = cmd; 
      messageBuffer[1] = (uint8_t)devicePort;
      Wire1.write(messageBuffer, 4);
      Wire1.endTransmission();
}

#else

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

#endif

