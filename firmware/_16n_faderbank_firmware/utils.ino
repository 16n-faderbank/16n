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