#include <SPI.h>
#include <MFRC522.h>

// SDA - pin 10
// SCK - pin 13
// MOSI - pin 11
// MISO - pin 12
// IRQ - none
// RST - pin 9

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

struct BankCard {
  String cardID;
  int Pincode;
  bool blocked;
};

// Define an array of cards with their respective PINs
BankCard validCards[] = {
  {"93DC7CA6", 8008, false},  // Example card 1
  {"F3F1DD95", 4200, false},  // Example card 2
};

const int numCards = sizeof(validCards) / sizeof(validCards[0]);
int card;

#define green_led 7
#define red_led 6

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate SPI bus
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  checkCardIDs(content);
  Serial.println();
}

void checkCardIDs(String content) {
  for (int i = 0; i < numCards; i++) {
    if (content == validCards[i].cardID) {  // Compare scanned ID with stored IDs
      Serial.println("Valid Card Detected!");
      checkCardPincode(i);
      return;
    }
  }
  Serial.println("Geen Geldige Kaart (Invalid Card)");
}

void checkCardPincode(int index) {
  int tries = 3;
  if(!validCards[index].blocked){
    for(int i = 0; i < tries; i++){
      Serial.print("Enter PIN for card ");
      Serial.println(validCards[index].cardID);
      

      // Placeholder for PIN verification logic
      while (Serial.available() == 0) {}
      String input = Serial.readString();
      input.trim();

      if (input == String(validCards[index].Pincode)) {
        Serial.println("Access Granted!");
        return;
      } else {
        Serial.println("Incorrect PIN! Access Denied.");
      }
    }
  }
  validCards[index].blocked = true;
  Serial.println("Card is blocked."); 
}
