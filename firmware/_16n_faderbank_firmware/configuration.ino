const int defaultUSBCCs[] = {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};
const int defaultTRSCCs[] = {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};

const int defaultUSBChannels[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const int defaultTRSChannels[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

void checkDefaultSettings() {
  // if byte1 of EEPROM is FF for whatever reason, let's assume the machine needs initializing
  int firstByte = EEPROM.read(0x00);

  if(firstByte == 0xff) {
    D(Serial.println("First Byte is 0xFF, probably needs initialising"));
    initializeFactorySettings();
  } else {
    D(Serial.print("First Byte is set to: "));
    D(printHex(firstByte));
    D(Serial.println());
    byte buffer[80];
    readEEPROMArray(0, buffer, 80);
    D(Serial.println("Config found:"));
    D(printHexArray(buffer, 80));
  }
}

 void initializeFactorySettings() {
  // set default config flags (LED ON, LED DATA, ROTATE, etc)
  EEPROM.write(0,1); // LED ON
  EEPROM.write(1,1); // LED DATA
  EEPROM.write(2,0); // ROTATE

  // set default USB channels
  for(int i = 0; i < channelCount; i++) {
    int baseAddress = 16;
    int writeAddress = baseAddress + i;
    EEPROM.write(writeAddress, defaultUSBChannels[i]);
  }

  // set default TRS channels
  for(int i = 0; i < channelCount; i++) {
    int baseAddress = 32;
    int writeAddress = baseAddress + i;
    EEPROM.write(writeAddress, defaultTRSChannels[i]);
  }

  // set default USB channel ccs
  for(int i = 0; i < channelCount; i++) {
    int baseAddress = 48;
    int writeAddress = baseAddress + i;
    EEPROM.write(writeAddress, defaultUSBCCs[i]);
  }

  // set default TRS channel ccs
  for(int i = 0; i < channelCount; i++) {
    int baseAddress = 64;
    int writeAddress = baseAddress + i;
    EEPROM.write(writeAddress, defaultTRSCCs[i]);
  }

  // serial dump that config.
  byte buffer[80];
  readEEPROMArray(0,buffer,80);
  D(Serial.println("Config Instantiated."));
  D(printHexArray(buffer, 80));
}

void loadSettingsFromEEPROM() {
  // load usb channels
  for(int i = 0; i < channelCount; i++) {
    int baseAddress = 16;
    int readAddress = baseAddress + i;
    usbChannels[i] = EEPROM.read(readAddress);
  }

  D(Serial.println("USB Channels loaded:"));
  D(printIntArray(usbChannels,channelCount));

  // load TRS channels
  for(int i = 0; i < channelCount; i++) {
    int baseAddress = 32;
    int readAddress = baseAddress + i;
    trsChannels[i] = EEPROM.read(readAddress);
  }

  D(Serial.println("TRS Channels loaded:"));
  D(printIntArray(trsChannels,channelCount));

  // load USB ccs
  for(int i = 0; i < channelCount; i++) {
    int baseAddress = 48;
    int readAddress = baseAddress + i;
    usbCCs[i] = EEPROM.read(readAddress);
  }

  D(Serial.println("USB CCs loaded:"));
  D(printIntArray(usbCCs,channelCount));


  // load TRS ccs
  for(int i = 0; i < channelCount; i++) {
    int baseAddress = 64;
    int readAddress = baseAddress + i;
    trsCCs[i] = EEPROM.read(readAddress);
  }

  D(Serial.println("TRS CCs loaded:"));
  D(printIntArray(trsCCs,channelCount));

  // load other config
  ledOn = EEPROM.read(0);
  ledFlash = EEPROM.read(1);
  flip = EEPROM.read(2);
}
