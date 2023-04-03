/*
 * 16n Faderbank Utility Functions
 * (c) 2020 by Tom Armitage
 * MIT License
 */

void readEEPROMArray(int start, byte buffer[], int length) {
  for (int i = 0; i < length; i++) {
    buffer[i] = EEPROM.read(start+i);
  }
}

void writeEEPROMArray(int start, byte buffer[], int length) {
  for (int i = 0; i < length; i++) {
    EEPROM.write(start+i, buffer[i]);
  }
}

void printHex(uint8_t num) {
  char hexCar[2];

  sprintf(hexCar, "%02X", num);
  Serial.print(hexCar);
}

void printHexArray(byte* array, int size) {
  for(int i=0; i<size; i++){
    printHex(array[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void printIntArray(int* array, int size) {
  for(int i=0; i<size; i++){
    printHex(array[i]);
    Serial.print(" ");
  }
  Serial.println();
}