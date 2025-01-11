void midiClock(byte realtimebyte){
  MIDI.sendRealTime((midi::MidiType)realtimebyte);
}
