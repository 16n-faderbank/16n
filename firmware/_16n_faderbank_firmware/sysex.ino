/*
 * 16n Faderbank Configuration Sysex Processing
 * (c) 2020 by Tom Armitage
 * MIT License
 */

void processIncomingSysex(byte* sysexData, unsigned size) {
  D(Serial.println("Ooh, sysex"));
  D(printHexArray(sysexData, size));
  D(Serial.println());

  if(size < 3) {
    D(Serial.println("That's an empty sysex, bored now"));
    return;
  }

  // if(!(sysexData[1] == 0x00 && sysexData[2] == 0x58 && sysexData[3] == 0x49)) {
  if(!(sysexData[1] == 0x7d && sysexData[2] == 0x00 && sysexData[3] == 0x00)) {
    D(Serial.println("That's not a sysex message for us"));
    return;
  }

  switch(sysexData[4]) {
    case 0x1f:
      // 1F = "1nFo" - please send me your current config
      D(Serial.println("Got an 1nFo request"));
      sendCurrentState();
      break;
    case 0x0e:
      // 0E - c0nfig Edit - here is a new config
      D(Serial.println("Incoming c0nfig Edit"));
      updateSettingsAndStoreInEEPROM(sysexData, size);
      break;
  }
}

void updateSettingsAndStoreInEEPROM(byte* newConfig, unsigned size) {
  // store the settings from sysex in flash
  // also update all our settings.
  D(Serial.print("Received a new config with size "));
  D(Serial.println(size));
  // D(printHexArray(newConfig,size));

  // walk the config
  // ignore the top, tail, and firmware version
  int startIndex = 9; // after the start signal + 
  int dataLength = 80; // five chunks of 16

  byte dataToWrite[dataLength];

  for(int i = 0; i < (dataLength); i++) {
    int configIndex = i + startIndex;
    dataToWrite[i] = newConfig[configIndex];
  }

  // write new Data
  writeEEPROMArray(0, dataToWrite, dataLength);

  // now load that.
  loadSettingsFromEEPROM();
}

void sendCurrentState() {
  //   0F - "c0nFig" - outputs its config:
  byte sysexData[88];

  sysexData[0] = 0x7d; // manufacturer
  sysexData[1] = 0x00;
  sysexData[2] = 0x00;
  
  sysexData[3] = 0x0F; // ConFig;

  sysexData[4] = DEVICE_ID; // Device 01, ie, dev board
  sysexData[5] = MAJOR_VERSION; // major version
  sysexData[6] = MINOR_VERSION; // minor version
  sysexData[7] = POINT_VERSION; // point version
  

  // 	16 bytes of config flags, notably:
  // 	LED PERMANENT
  // 	LED DATA XFER
  // 	ROTATE (flip+reverse)
  //  i2c MASTER/FOLLOEWR
  //  fadermin LSB
  //  fadermin MSB
  //  fadermax LSB
  //  fadermax MSB

  // 	16x USBccs
  // 	16x TRSccs
  // 	16x USBchannel
  // 	16x TRS channel
    
  // So that's 3 for the mfg + 1 for the message + 80 bytes
  // can be done with a simple "read eighty bytes and send them."

  byte buffer[80];
  readEEPROMArray(0, buffer, 80);

  int offset = 8;
  for(int i = 0; i < 80; i++) {
    byte data = buffer[i];
    if(data == 0xff) {
      data = 0x7f;
    }
    sysexData[i+offset] = data;
  }

  D(Serial.println("Sending this data"));
  D(printHexArray(sysexData,88));

  usbMIDI.sendSysEx(88, sysexData, false);
  forceMidiWrite = true;
}