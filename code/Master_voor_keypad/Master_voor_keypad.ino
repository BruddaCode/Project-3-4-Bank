#define keypad 20

#include <Wire.h>

char pin[5];

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);

  requestPin();
  String pincode = pin;
  pincode = pincode.substring(0, 4);
  Serial.println(pincode);
  for(int i = 0; i < 4; i++){
    pin[i] = '\0';
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
    return Wire.read();
  }
  return '\0';
}

void requestPin() {
  for (int i = 0; i < 5; i++) {
    char c = '\0';

    // Wait for a non-zero key (user pressed something)
    while ((c = requestNumber()) == '\0') {
      delay(50);
    }

    pin[i] = c;
    Serial.print("Pressed: ");
    Serial.println(c);

    if(c == 'K' || c == 'X'){
      break;
    }

    // Wait for key release before continuing (avoid reading same key again)
    while (requestNumber() == c) {
      delay(50);
    }
  }
}