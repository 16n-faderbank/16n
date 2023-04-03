void midiNoteOn(byte channel, byte note, byte velocity){
  MIDI.sendNoteOn(note, velocity, channel);
}
void midiNoteOff(byte channel, byte note, byte velocity){
  MIDI.sendNoteOff(note, velocity, channel);
}
void midiAfterTouchPoly(byte channel, byte note, byte velocity){
  MIDI.sendAfterTouch(note, velocity, channel);
}
void midiAfterTouch(byte channel, byte pressure){
  MIDI.sendAfterTouch(pressure, channel);
}
void midiControlChange(byte channel, byte control, byte value){
  MIDI.sendControlChange(control, value, channel);  
}
void midiProgramChange(byte channel, byte program){
  MIDI.sendProgramChange(program, channel);
}
void midiTimeCodeQuarterFrame(byte data){
  MIDI.sendTimeCodeQuarterFrame(data);
}
void midiSongPosition(uint16_t beats){
  MIDI.sendSongPosition(beats);
}
void midiSongSelect(byte songNumber){
  MIDI.sendSongSelect(songNumber);
}
void midiTuneRequest(){
  MIDI.sendTuneRequest();
}
