#define keypad 20

#include <Wire.h>

char pin[4];

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);

  requestPin();
  String pincode = pin;
  pincode = pincode.substring(0, 4);
  Serial.println(pincode);
  for(int i = 0; i < 4; i++){
    pin[i] = NULL;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

void Transmission(){
  Wire.beginTransmission(keypad);
  Wire.write(1);
  Wire.endTransmission();
}

char requestNumber(){
  Wire.requestFrom(keypad, 1);
  if(Wire.available()){
    char c = Wire.read();
    return c;
  }
}

void requestPin(){
  for(int i = 0; i < 4; i++){
    pin[i] = requestNumber();
  }
}