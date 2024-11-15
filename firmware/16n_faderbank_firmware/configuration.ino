/*
 * 16n Faderbank EEPROM-based configuration
 * (c) 2020 by Tom Armitage
 * MIT License
 */

const int defaultUSBCCs[] = {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};
const int defaultTRSCCs[] = {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};

const int defaultUSBChannels[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
const int defaultTRSChannels[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

void checkDefaultSettings() {
  // if byte1 of EEPROM is FF for whatever reason, let's assume the machine needs initializing
  int firstByte = EEPROM.read(0x00);

  if(firstByte > 0x01) {
    D(Serial.println("First Byte is > 0x01, probably needs initialising"));
    initializeFactorySettings();
  } else {
    D(Serial.print("First Byte is set to: "));
    D(printHex(firstByte));
    D(Serial.println());
    byte buffer[80];
    readEEPROMArray(0, buffer, EEPROM_DATA_SIZE);
    D(Serial.println("Config found:"));
    D(printHexArray(buffer, EEPROM_DATA_SIZE));
  }
}

 void initializeFactorySettings() {
  // set default config flags (LED ON, LED DATA, ROTATE, etc)
  // fadermin/max are based on "works for me" for twra2. Your mileage may vary.
  EEPROM.write(0,1);  // LED ON
  EEPROM.write(1,1);  // LED DATA
  EEPROM.write(2,0);  // ROTATE
  EEPROM.write(3,0);  // I2C follower by default
  EEPROM.write(4,15); // fadermin LSB
  EEPROM.write(5,0);  // fadermin MSB
  EEPROM.write(6,71); // fadermax LSB
  EEPROM.write(7,63); // fadermax MSB
  EEPROM.write(8,0);  // Soft midi thru

  for(int i = 9; i < 16; i ++) {
    EEPROM.write(i,0);  // blank remaining config slots.
  }

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

  // set default CC modes
  for(int i = 0; i < 4; i++) {
    int baseAddress = 80;
    int writeAddress = baseAddress + i;
    EEPROM.write(writeAddress, 0x7F);
  }

  // serial dump that config.
  byte buffer[EEPROM_DATA_SIZE];
  readEEPROMArray(0,buffer,EEPROM_DATA_SIZE);
  D(Serial.println("Config Instantiated."));
  D(printHexArray(buffer, EEPROM_DATA_SIZE));
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

  // load USB CC mode flags
  usbCCModes = (EEPROM.read(80) << 8) | EEPROM.read(81);

  D(Serial.println("USB CC modes loaded:"));
  D(printHex(usbCCModes >> 8));
  D(printHex(usbCCModes & 0xFF));

  // load TRS mode flags
  trsCCModes = (EEPROM.read(82) << 8) | EEPROM.read(83);

  D(Serial.println("TRS CC modes loaded:"));
  D(printHex(trsCCModes >> 8));
  D(printHex(trsCCModes & 0xFF));


  // load other config
  ledOn = EEPROM.read(0);
  ledFlash = EEPROM.read(1);
  flip = EEPROM.read(2);
  midiThru = EEPROM.read(8);

  // i2cMaster only read at startup

  int faderminLSB = EEPROM.read(4);
  int faderminMSB = EEPROM.read(5);

  D(Serial.print ("Setting fadermin to "));
  D(Serial.println((faderminMSB << 7) + faderminLSB));
  faderMin = (faderminMSB << 7) + faderminLSB;

  int fadermaxLSB = EEPROM.read(6);
  int fadermaxMSB = EEPROM.read(7);

  D(Serial.print ("Setting fadermax to "));
  D(Serial.println((fadermaxMSB << 7) + fadermaxLSB));
  faderMax = (fadermaxMSB << 7) + fadermaxLSB;
}
