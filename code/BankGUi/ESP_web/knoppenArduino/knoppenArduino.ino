#include <Keypad.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

// keypad
const byte ROWS = 4; 
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'K', '0', 'X'}
};
byte rowPins[ROWS] = {2, 3, 8, 7};   
byte colPins[COLS] = {5, 4, 6};   
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// RFID
#define SS_PIN 10
#define RST_PIN 8
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);

  Wire.begin();

  // RFID
  SPI.begin();
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  char key = customKeypad.getKey();
  if ((key && key >= '0' && key <= '9') || (key && key == 'X') || (key && key == 'K')) {
    Serial.println("pas:" + key);
    Wire.beginTransmission(4);
    Wire.write(("pas:" + key).c_str);
    Wire.endTransmission();
  }
  RFID();
}

// RFID
//TODO laten werken met api
void RFID () {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  String pasData = "pas:";

  pasData += readBlock(1, 3); // block 1, trailer block 3 (sector 0)
  pasData += readBlock(2, 3); // block 2, trailer block 3 (sector 0)
  pasData += ":";
  pasData += readBlock(4, 7); // block 4, trailer block 7 (sector 1)
  Serial.println(String(pasData));
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  Wire.beginTransmission(4);
  Wire.write(pasData.c_str());
  Wire.endTransmission();
}

String readBlock(byte blockAddr, byte trailerBlock) {
  byte buffer[18];
  byte size = sizeof(buffer);
  int dataSize;
  if (blockAddr == 1) { dataSize = 8; }
  if (blockAddr == 2) { dataSize = 10; }
  if (blockAddr == 4) { dataSize = 3; }

  // Authenticate
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(
    MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Auth failed for block ")); Serial.print(blockAddr); Serial.print(": ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Read block
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Read failed for block ")); Serial.print(blockAddr); Serial.print(": ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Output hex and string
  Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.print(F(" (hex): "));
  dump_byte_array(buffer, 16); Serial.println();

  Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.print(F(" (string): "));
  String data = "";
  for (byte i = 0; i < dataSize; i++) {
    data += (char)buffer[i];
  }
  return data;
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}