#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"

#define RX_PIN 4
#define TX_PIN 3

SoftwareSerial mySerial(TX_PIN, RX_PIN);
Adafruit_Thermal printer(&mySerial);

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  printer.begin();

  printer.setFont('B');
  printer.setFont('A'); 

  String datum = "01-01-2025";
  String tijd = "12:00";
  String iban = "44 12";
  String pasnummer = "1234";
  String transactieId = "1";
  float b = 100.00;
  float tk = 0.00;
  float tb = b + tk;
  String bedrag = String(b);
  String transactieKosten = String(tk);
  String totaalBedrag = String(tb);
  printBon(datum, tijd, iban, pasnummer, transactieId, bedrag, transactieKosten, totaalBedrag);
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

void printBon(String datum, String tijd, String iban, String pasnummer, String transactieId, String bedrag, String transactieKosten, String totaalBedrag){
  printer.feed(2);

  left();
  large();
  printer.boldOn();
  printer.println("SYMPLE");
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
  printer.print(datum);
  printer.print("\t");
  printer.print(tijd);
  printer.println("\t\t1");

  printer.feed(1);

  //Rekeningnummer ten alle tijden deels afgeschermd houden
  printer.println("REKENINGNUMMER\t\t\tPASNR");
  printer.print("NLxx SMPL xxxx xx");
  printer.print(iban);
  printer.print("\t");
  printer.println(pasnummer);

  printer.feed(1);

  printer.print("TRANSACTIE ID\t\t:\t");
  printer.println(transactieId);

  printer.feed(1);

  printer.print("BEDRAG\t\t\t\t:\t");
  printer.println(bedrag);
  printer.print("TRANSACTIE KOSTEN\t:\t");
  printer.println(transactieKosten);

  printer.feed(1);

  printer.print("TOTAAL BEDRAG\t\t:\t");
  printer.println(totaalBedrag);

  printer.feed(2);

  medium();
  center();
  printer.println("TOT ZIENS");

  printer.feed(4);
}

void loop() {
  
}