/*
 * TELEX Eurorack Modules
 * (c) 2016,2017 Brendon Cassidy
 * MIT License
 */
 
#ifndef TxHelper_h
#define TxHelper_h

// i2c
#include <i2c_t3.h>

#include "Arduino.h"

struct TxResponse {
  byte Command;
  byte Output;
  int Value;
};

struct TxIO {
  short Port;
  short Mode;
};

class TxHelper
{
  public:

    static TxResponse Parse(size_t len);
    static TxIO DecodeIO(int io);

    static void SetPorts(int ports);
    static void SetModes(int modes);
    static void UseWire1(bool use);

  protected:
    
    static int Ports;
    static int Modes;
    static bool W0;

  private:
 

};

#endif

