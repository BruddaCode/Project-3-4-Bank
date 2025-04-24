#define mp1 8
#define mp2 9
#define enable 7
#define ir 10


void setup() {
  pinMode(mp1, OUTPUT);
  pinMode(mp2, OUTPUT);
  digitalWrite(mp2, LOW);
  pinMode(ir, INPUT);
}

void loop() {

  bool value = digitalRead(ir);


  if(value == 1){
    digitalWrite(mp1, LOW);
  }
  
  if(value == 0){
    digitalWrite(mp1, HIGH);
  }


}