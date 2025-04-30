#define mp1 8
#define mp2 9
#define button 2
int value;

void setup() {
  pinMode(mp1, OUTPUT);
  pinMode(mp2, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(digitalRead(button));

  if (digitalRead(button) == 1){
    digitalWrite(mp1, LOW);
    delay(1800);
  }
  else if (value == 0){
    digitalWrite(mp1, HIGH);
  }

}
