/*
 * TELEX Eurorack Modules
 * (c) 2016,2017 Brendon Cassidy
 * MIT License
 */
 
#include "TxHelper.h"
#include "Arduino.h"

// i2c
#include <i2c_t3.h>

// initialize the basic values for the TXi
int TxHelper::Ports = 8;
int TxHelper::Modes = 3;
bool TxHelper::W0 = true;

/**
 * Set the number of ports the device has (TXi is 8; FADER is 16)
 */
void TxHelper::SetPorts(int ports){
  TxHelper::Ports = ports;
}

/**
 * Determine how the modes shift (TXi shifts 3; FADER shifts 4)
 */
void TxHelper::SetModes(int modes){
  TxHelper::Modes = modes;
}

/**
 * Determine if the Wire or Wire1 interface should be used
 */
void TxHelper::UseWire1(bool use){
  TxHelper::W0 = !use;
}

/**
 * Parse the response coming down the wire
 */
TxResponse TxHelper::Parse(size_t len){

  TxResponse response;

  int buffer[4] = { 0, 0, 0, 0 };

  // zero out the read buffer
  int counterPal = 0;
  memset(buffer, 0, sizeof(buffer));

  // read the data
  while (1 < (W0 ? Wire.available() : Wire1.available())) {
    if (counterPal < 4) {
      buffer[counterPal++] = (W0 ? Wire.read() : Wire1.read());
    }
  }
  // get the last byte
  buffer[counterPal] = (W0 ? Wire.read() : Wire1.read());

  // Serial.printf("Buffers: %d, %d, %d, %d\n", buffer[0], buffer[1], buffer[2], buffer[3]);

  uint16_t temp = (uint16_t)((buffer[2] << 8) + (buffer[3]));
  int16_t temp2 = (int16_t)temp;

  response.Command = buffer[0];
  response.Output = buffer[1];
  response.Value = (int)temp2;
  
  // Serial.printf("temp: %d; temp2: %d; helper: %d\n", temp, temp2, response.Value);

  return response;
  
}

/**
 * Decode the IO from the value coming down the wire
 */
TxIO TxHelper::DecodeIO(int io) {
  
  TxIO decoded;
  
  // turn it into 0-7 for the individual device's port (TXi)
  decoded.Port = io % Ports;
  
  // output mode (0-7 = normal; 8-15 = Quantized; 16-23 = Note Number) (TXi)
  decoded.Mode = io >> Modes;

  return decoded;
}

