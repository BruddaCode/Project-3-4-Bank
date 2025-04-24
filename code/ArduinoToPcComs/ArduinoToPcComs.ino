void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  if (Serial.available()) {
    // leest shit van de pc af
    String message = Serial.readStringUntil('\n');
    
    if (message == "0") {
      digitalWrite(LED_BUILTIN, HIGH);

      // stuurt zooi naar de pc toe
      Serial.println(0);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println(1);
    }
  }
}
