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

char lastKey = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
}

void loop() {
  char customKey = customKeypad.getKey();
  if(customKey && customKey >= '0' && customKey <= '9') {
    lastKey = customKey;
  }
}

void requestEvent(){
  Wire.write(lastKey);
}