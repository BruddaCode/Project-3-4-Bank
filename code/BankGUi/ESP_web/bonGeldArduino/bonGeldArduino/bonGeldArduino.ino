#include <Adafruit_Thermal.h>
#include "HardwareSerialRS485.h"

// bon printer
#define RX_PIN 16
#define TX_PIN 17
HardwareSerial mySerial(1);
Adafruit_Thermal printer(&mySerial);

// geld dispenser
#define disp1_1 26
#define disp1_2 25
#define disp2_1 33
#define disp2_2 32 
#define disp3_1 14
#define disp3_2 27

void setup() {
  Serial.begin(9600);

  // I2C
  Wire.begin(10);
  Wire.onReceive(Filter);

  // bon printer
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  printer.begin();
  printer.setFont('B');
  printer.setFont('A');

  // dispenser
  pinMode(disp1_1, OUTPUT);
  pinMode(disp1_2, OUTPUT);
  pinMode(disp2_1, OUTPUT);
  pinMode(disp2_2, OUTPUT);
  pinMode(disp3_1, OUTPUT);
  pinMode(disp3_2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void Filter(int msg) {
  String fromArduino ="";
  while (Wire.available()) {
    char c = Wire.read();
    fromArduino += c;
  }
  Serial.println(fromArduino);
  // int separatorIndex = fromArduino.indexOf(':');
  // String command = "";
  // String value = "";
  // String subvalue = "";
  // if (separatorIndex != -1) {
  //   command = fromArduino.substring(0, separatorIndex);
  //   value = fromArduino.substring(separatorIndex + 1);
  //   int subSeparatorIndex = value.indexOf(':');
  //   subvalue = "";
  //   if (subSeparatorIndex != -1) {
  //     subvalue = value.substring(subSeparatorIndex + 1);
  //     value = value.substring(0, subSeparatorIndex);
  //   }
  // }
}

// functie voor de bonprinter
void printBon() {
  printer.feed(2); // Feed a few lines
  printer.justify('L');
  printer.setSize('L');
  printer.boldOn();
  printer.println("SYMPLE");
  printer.boldOff();
  printer.justify('C');
  printer.setSize('M');
  printer.println("--------------------------------");
  printer.setSize('M');
  printer.justify('C');
  printer.setLineHeight(10);
  printer.println("OPNAME BILJETTEN");
  printer.setLineHeight();

  // datum
  printer.feed(1);
  printer.justify('L');
  printer.setSize('S');
  printer.println("DATUM\t\tTIJD\t\tAUTOMAAT");
  printer.print("9/11/2001");
  printer.print("\t");
  printer.print("10:00:00");
  printer.println("\t\t1");

  // saldo
  printer.feed(1);
  printer.println("REKENINGNUMMER\t\t\tPASNR");
  printer.print("NLxx SMPL xxxx xx");
  printer.print(iban);
  printer.print("\t");
  printer.println(pasnummer);

  // transactie details
  printer.feed(1);
  printer.print("TRANSACTIE ID\t\t:\t");
  printer.println("1234567890");

  // biljetten
  printer.feed(1);
  printer.print("BEDRAG\t\t\t\t:\t");
  printer.println(String(amount, 2) + " CFA");
  printer.print("TRANSACTIE KOSTEN\t:\t");
  printer.println("€10.000,00");

  printer.feed(2);
  printer.setSize('M');
  printer.justify('C');
  printer.println("TOT ZIENS");
  printer.feed(4);
}

// geld dispenser functies
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

void biljetDispenser(String optie) {
  
  if (optie == "optie1") {
    // Dispenser 1
    if (calculatedBiljetOptie1.indexOf("50x") > -1) {
      int b50 = calculatedBiljetOptie1.substring(calculatedBiljetOptie1.indexOf("50x") + 3, calculatedBiljetOptie1.indexOf("|")).toInt();
      for (int i = 0; i < b50; i++) {
        unsigned long startTime = millis();
        while (millis() - startTime < 3000) { // Push for 3 seconds
          push(1);       
        }      
        while (millis() - startTime < 4000) { // Pull for 1 second
          pull(1);
        }
      }
    }
    if (calculatedBiljetOptie1.indexOf("20x") > -1) {
      int b20 = calculatedBiljetOptie1.substring(calculatedBiljetOptie1.indexOf("20x") + 3, calculatedBiljetOptie1.lastIndexOf("|")).toInt();
      for (int i = 0; i < b20; i++) {
        unsigned long startTime = millis();
        while (millis() - startTime < 3000) { // Push for 3 seconds
          push(2);       
        }      
        while (millis() - startTime < 4000) { // Pull for 1 second
          pull(2);
        }
      }
    }
    if (calculatedBiljetOptie1.indexOf("10x") > -1) {
      int b10 = calculatedBiljetOptie1.substring(calculatedBiljetOptie1.lastIndexOf("10x") + 3).toInt();
      for (int i = 0; i < b10; i++) {
        unsigned long startTime = millis();
        while (millis() - startTime < 3000) { // Push for 3 seconds
          push(3);       
        }      
        while (millis() - startTime < 4000) { // Pull for 1 second
          pull(3);
        }
      }
    }
  }
  if (optie == "optie2") {
    // Dispenser 1
    if (calculatedBiljetOptie2.indexOf("50x") > -1) {
      int b50 = calculatedBiljetOptie2.substring(calculatedBiljetOptie2.indexOf("50x") + 3, calculatedBiljetOptie2.indexOf("|")).toInt();
      for (int i = 0; i < b50; i++) {
        unsigned long startTime = millis();
        while (millis() - startTime < 3000) { // Push for 3 seconds
          push(1);       
        }      
        while (millis() - startTime < 4000) { // Pull for 1 second
          pull(1);
        }
      }
    }
    if (calculatedBiljetOptie2.indexOf("20x") > -1) {
      int b20 = calculatedBiljetOptie2.substring(calculatedBiljetOptie2.indexOf("20x") + 3, calculatedBiljetOptie2.lastIndexOf("|")).toInt();
      for (int i = 0; i < b20; i++) {
        unsigned long startTime = millis();
        while (millis() - startTime < 3000) { // Push for 3 seconds
          push(2);       
        }      
        while (millis() - startTime < 4000) { // Pull for 1 second
          pull(2);
        }
      }
    }
    if (calculatedBiljetOptie2.indexOf("10x") > -1) {
      int b10 = calculatedBiljetOptie2.substring(calculatedBiljetOptie2.lastIndexOf("10x") + 3).toInt();
      for (int i = 0; i < b10; i++) {
        unsigned long startTime = millis();
        while (millis() - startTime < 3000) { // Push for 3 seconds
          push(3);       
        }      
        while (millis() - startTime < 4000) { // Pull for 1 second
          pull(3);
        }
      }
    }
  }
  if (optie == "optie3") {
    // Dispenser 1
    if (calculatedBiljetOptie3.indexOf("50x") > -1) {
      int b50 = calculatedBiljetOptie3.substring(calculatedBiljetOptie3.indexOf("50x") + 3, calculatedBiljetOptie3.indexOf("|")).toInt();
      for (int i = 0; i < b50; i++) {
        unsigned long startTime = millis();
        while (millis() - startTime < 3000) { // Push for 3 seconds
          push(1);       
        }      
        while (millis() - startTime < 4000) { // Pull for 1 second
          pull(1);
        }
      }
    }
    if (calculatedBiljetOptie3.indexOf("20x") > -1) {
      int b20 = calculatedBiljetOptie3.substring(calculatedBiljetOptie3.indexOf("20x") + 3, calculatedBiljetOptie3.lastIndexOf("|")).toInt();
      for (int i = 0; i < b20; i++) {
        unsigned long startTime = millis();
        while (millis() - startTime < 3000) { // Push for 3 seconds
          push(2);       
        }      
        while (millis() - startTime < 4000) { // Pull for 1 second
          pull(2);
        }
      }
    }
    if (calculatedBiljetOptie3.indexOf("10x") > -1) {
      int b10 = calculatedBiljetOptie3.substring(calculatedBiljetOptie3.lastIndexOf("10x") + 3).toInt();
      for (int i = 0; i < b10; i++) {
        unsigned long startTime = millis();
        while (millis() - startTime < 3000) { // Push for 3 seconds
          push(3);       
        }      
        while (millis() - startTime < 4000) { // Pull for 1 second
          pull(3);
        }
      }
    }
  }
}