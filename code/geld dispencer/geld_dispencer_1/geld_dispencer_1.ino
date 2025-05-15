#define mp1 8
#define mp2 7

void setup() {
  pinMode(mp1, OUTPUT);
  pinMode(mp2, OUTPUT);
}

void loop() {
  push();
  delay(2000);
  pull();
  delay(1000);
}

void push(){
  digitalWrite(mp1, HIGH);
  digitalWrite(mp2, LOW);
}

void pull(){
  digitalWrite(mp1, LOW);
  digitalWrite(mp2, HIGH);
}