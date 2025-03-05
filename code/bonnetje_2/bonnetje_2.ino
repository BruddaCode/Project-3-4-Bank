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

  left();
  large();
  printer.boldOn();
  printer.println("Rabobank");
  printer.boldOff();

  center();
  printLine();

  medium();
  center();
  printer.setLineHeight(10);
  printer.println("OPNAME BILJETTEN");
  printer.setLineHeight();

  printer.feed(1);

  left();
  small();
  printer.println("DATUM\t\tTIJD\t\tAUTOMAAT");
  printer.println("01-01-2025\t12:00\t\t1");

  printer.feed(1);

  printer.println("TRANSACTIE ID\t\t:\t1");
  printer.println("PASNUMMER\t\t\t:\t1234");
  printer.println("REKENIGNUMMER\t\t:\t1234");

  printer.feed(1);

  printer.println("BEDRAG\t\t\t\t:\t100,00");
  printer.println("TRANSACTIE KOSTEN\t:\t0,00");

  printer.feed(1);

  printer.println("TOTAAL BEDRAG\t\t:\t100,00");

  printer.feed(2);

  medium();
  center();
  printer.println("TOT ZIENS");

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
