#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"

#define RX_PIN 4
#define TX_PIN 3

SoftwareSerial mySerial(TX_PIN, RX_PIN);
Adafruit_Thermal printer(&mySerial);

void setup() {
  mySerial.begin(9600);
  printer.begin();

  printer.setFont('B');
  printer.setFont('A');

  printer.feed(2);

  center();
  
  printLine();

  large();
  printer.println("Rabobank");

  printLine();

  printer.feed(1);

  small();
  left();
  printer.println("Automaat nummer");
  right();
  printer.println("1");
  left();
  printer.println("Transactie nummer");
  right();
  printer.println("1");
  left();
  printer.println("Datum");
  right();
  printer.println("12:00 01-01-2025");

  printer.feed(2);

  left();
  printer.println("Rekeningnummer");
  right();
  printer.println("NL XX RABO 0XXX XXXX XX");
  left();
  printer.println("Pasnummer");
  right();
  printer.println("1234");

  printer.feed(2);

  left();
  printer.println("Gepind bedrag");
  right();
  printer.println("100,00");
  left();
  printer.println("Transactie kosten");
  right();
  printer.println("0,00");

  printer.feed(2);

  left();
  printer.println("Totale afschrijving");
  right();
  printer.println("100.00");

  printer.feed(1);

  printLine();

  printer.feed(2);
}

void right(){
  printer.justify('R');
}

void left(){
  printer.justify('L');
}

void center(){
  printer.justify('C');
}

void printLine(){
  medium();
  printer.println("--------------------------------");
}

void small(){
  printer.setSize('S');
}

void medium(){
  printer.setSize('M');
}

void large(){
  printer.setSize('L');
}

void loop() {
  // put your main code here, to run repeatedly:

}
