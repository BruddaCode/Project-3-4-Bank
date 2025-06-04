#define disp1_1 26
#define disp1_2 25
#define disp2_1 24
#define disp2_2 23 
#define disp3_1 22
#define disp3_2 21

void setup() {
  pinMode(disp1_1, OUTPUT);
  pinMode(disp1_2, OUTPUT);
  pinMode(disp2_1, OUTPUT);
  pinMode(disp2_2, OUTPUT);
  pinMode(disp3_1, OUTPUT);
  pinMode(disp3_2, OUTPUT);

  stop(4);  
}


void loop() {
  push(1);
  push(2);
  push(3);
  delay(6000);
  pull(1);
  pull(2);
  pull(3);
  delay(1000);
  stop(4);
  delay(2000);
}

void push(int dispenser){
  switch(dispenser) {
    case 1:
      digitalWrite(disp1_1, LOW);
      digitalWrite(disp1_2, HIGH);
      break;
    case 2: 
      digitalWrite(disp2_1, LOW);
      digitalWrite(disp2_2, HIGH);
      break;
    case 3:
      digitalWrite(disp3_1, LOW);
      digitalWrite(disp3_2, HIGH);
      break;
  }
}

void pull(int dispenser){
  switch(dispenser) {
    case 1:
      digitalWrite(disp1_1, HIGH);
      digitalWrite(disp1_2, LOW);
      break;
    case 2: 
      digitalWrite(disp2_1, HIGH);
      digitalWrite(disp2_2, LOW);
      break;
    case 3:
      digitalWrite(disp3_1, HIGH);
      digitalWrite(disp3_2, LOW);
      break;
  }
}

void stop(int dispenser){
  switch(dispenser) {
    case 1:
      digitalWrite(disp1_1, LOW);
      digitalWrite(disp1_2, LOW);
      break;
    case 2: 
      digitalWrite(disp2_1, LOW);
      digitalWrite(disp2_2, LOW);
      break;
    case 3:
      digitalWrite(disp3_1, LOW);
      digitalWrite(disp3_2, LOW);
      break;
    case 4:
      digitalWrite(disp1_1, LOW);
      digitalWrite(disp1_2, LOW);
      digitalWrite(disp2_1, LOW);
      digitalWrite(disp2_2, LOW);
      digitalWrite(disp3_1, LOW);
      digitalWrite(disp3_2, LOW);
      break;
  }
}