void setup() {
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(9, LOW);
  if(digitalRead(4) == 0){
    Serial.println("1");
  }
  if(digitalRead(5) == 0){
    Serial.println("2");
  }
  if(digitalRead(6) == 0){
    Serial.println("3");
  }
  digitalWrite(9, HIGH);

  digitalWrite(10, LOW);
  if(digitalRead(4) == 0){
    Serial.println("4");
  }
  if(digitalRead(5) == 0){
    Serial.println("5");
  }
  if(digitalRead(6) == 0){
    Serial.println("6");
  }
  digitalWrite(10, HIGH);

  digitalWrite(11, LOW);
  if(digitalRead(4) == 0){
    Serial.println("7");
  }
  if(digitalRead(5) == 0){
    Serial.println("8");
  }
  if(digitalRead(6) == 0){
    Serial.println("9");
  }
  digitalWrite(11, HIGH);

  digitalWrite(12, LOW);
  if(digitalRead(4) == 0){
    Serial.println("X");
  }
  if(digitalRead(5) == 0){
    Serial.println("0");
  }
  if(digitalRead(6) == 0){
    Serial.println("OK");
  }
  digitalWrite(12, HIGH);
}
