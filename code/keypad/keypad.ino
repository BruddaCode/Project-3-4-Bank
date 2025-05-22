#define ADDRESS 20

#include <Keypad.h>
#include <Wire.h>

const byte ROWS = 4; 
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'X', '0', 'K'}
};
byte rowPins[ROWS] = {9, 10, 11, 12};   
byte colPins[COLS] = {4, 5, 6};   
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

#define BUFFER_SIZE 10
char keyBuffer[BUFFER_SIZE];
int bufferStart = 0;
int bufferEnd = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
}

void loop() {
  char key = customKeypad.getKey();
  if ((key && key >= '0' && key <= '9') || (key && key == 'X') || (key && key == 'K')) {
    // Add to buffer if space is available
    int nextPos = (bufferEnd + 1) % BUFFER_SIZE;
    if (nextPos != bufferStart) { // Buffer not full
      keyBuffer[bufferEnd] = key;
      bufferEnd = nextPos;
    }
  }
}

void requestEvent() {
  if (bufferStart != bufferEnd) {
    Wire.write(keyBuffer[bufferStart]);
    bufferStart = (bufferStart + 1) % BUFFER_SIZE;
  } else {
    Wire.write('\0');  // No key available
  }
}