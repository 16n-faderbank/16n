#include <Wire.h>
#include <MIDI.h>
// Hardware MIDI is on pin 1
// You **must** also compile this with Tools->USB type set to MIDI.

const int channel = 1;
const int channelCount = 1; // will be 16 in production; this must match the length of `ports`
//const int ports[] = {A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16};
const int ports[] = {A0};
int v[2][channelCount];
int last;


void setup() {
  // we're using the alternate I2C pins on a Teensy 3.2
  Wire.setSDA(30);
  Wire.setSCL(29);

  Wire.begin();
  MIDI.begin();
}

void loop() {
  for(int i=0;i<channelCount;i++) {
    last = v[0][i];

    // because of voltage division, inbound range is not quite 1023 TODO: confirm that
    int rawReading = map(analogRead(ports[i]), 0, 1020, 0, 1023);

    // smooth by averaging previous reading
    v[0][i] = (rawReading / 4 + v[1][i]) / 2;
    
    
    if(v[0][i] != last) {
      usbMIDI.sendControlChange(i, v[0][i] / 2, channel);
      MIDI.sendControlChange(i, v[0][i] / 2, channel);

      // TODO: send I2C message.
    }
      
    v[1][i] = v[0][i];
  }
  delay(4);
}
