// libs
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Thermal.h>
#include <HardwareSerial.h>

// paginas
#include "index.h"
#include "home.h"
#include "saldo.h"
#include "geldKeuze.h"
#include "opmaak.css"
#include "biljetVraag.h"
#include "bonVraag.h"
#include "biljetOptie.h"

// global variables
String noob_api = "http://noob.datalabrotterdam.nl/api/noob/users/";
String local_api = "http://145.24.222.28:9000/api/users/";
bool calculated = false; // Indicates if the biljet options have been calculated

// bon printer
#define RX_PIN 16
#define TX_PIN 17
HardwareSerial mySerial(1);
Adafruit_Thermal printer(&mySerial);

// geld dispenser
#define disp1_1 25
#define disp1_2 26
#define disp2_1 14
#define disp2_2 27 
#define disp3_1 33
#define disp3_2 32

// user variables
String iban = "";
String pasnummer = "";
String pin = "";
float saldo = 0.0;
float amount = 0.0;
String calculatedBiljetOptie1 = "";
String calculatedBiljetOptie2 = "";
String calculatedBiljetOptie3 = "";

// biljet opties
struct BiljetResult {
  int b10;
  int b20;
  int b50;
};

BiljetResult optie1;
BiljetResult optie2;
BiljetResult optie3;

// biljet opties berekenen
String calculateOptie1(int bedrag) {
  optie1.b50 = bedrag / 50;
  bedrag %= 50;

  optie1.b20 = bedrag / 20;
  bedrag %= 20;

  optie1.b10 = bedrag / 10;

  String calculated = "50x" + String(optie1.b50) + "|20x" + String(optie1.b20) + "|10x" + String(optie1.b10);
  Serial.println("Calculated Optie1: " + calculated);

  return calculated;
}

String fallbackGreedy(int remaining, BiljetResult result, int aantalTientjes, int maxTientjes) {
  while (remaining >= 50) {
    result.b50++;
    remaining -= 50;
  }
  while (remaining >= 20) {
    result.b20++;
    remaining -= 20;
  }
  while (remaining >= 10 && aantalTientjes < maxTientjes) {
    result.b10++;
    aantalTientjes++;
    remaining -= 10;
  }

  String calculated = "50x" + String(result.b50) + "|20x" + String(result.b20) + "|10x" + String(result.b10);
  Serial.println("Fallback Greedy Calculated: " + calculated);
  return calculated;
}

String calculateOptie2(int bedrag) {
  int aantalTientjes = 0;
  int remaining = bedrag;
  int maxTientjes = 6;

  while (remaining > 0) {
    std::vector<int> valid;
    if (remaining >= 50) valid.push_back(50);
    if (remaining >= 20) valid.push_back(20);
    if (remaining >= 10 && aantalTientjes < maxTientjes) valid.push_back(10);

    if (valid.empty()) {
      return fallbackGreedy(remaining, optie2, aantalTientjes, maxTientjes);
    }

    int keuze = valid[rand() % valid.size()];

    switch (keuze) {
      case 50: optie2.b50++; break;
      case 20: optie2.b20++; break;
      case 10: optie2.b10++; aantalTientjes++; break;
    }

    remaining -= keuze;
  }
  String calculated = "50x" + String(optie2.b50) + "|20x" + String(optie2.b20) + "|10x" + String(optie2.b10);
  Serial.println("Calculated Optie2: " + calculated);

  return calculated;
}

String calculateOptie3(int bedrag) {
  int remaining = bedrag;

  while (remaining > 0) {
    std::vector<int> valid;
    if (remaining >= 50) valid.push_back(50);
    if (remaining >= 20) valid.push_back(20);
    if (remaining >= 10) valid.push_back(10);

    int keuze = valid[rand() % valid.size()];

    switch (keuze) {
      case 50: optie3.b50++; break;
      case 20: optie3.b20++; break;
      case 10: optie3.b10++; break;
    }

    remaining -= keuze;
  }
  String calculated = "50x" + String(optie3.b50) + "|20x" + String(optie3.b20) + "|10x" + String(optie3.b10);
  Serial.println("Calculated Optie3: " + calculated);

  return calculated;
}

// webserver
const char *ssid = "Samsung Smart Fridge";
const char *password = "qqqqqqqq";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// stuurt een bericht naar de website
void notifyClients(String msg) {
  ws.textAll(msg);
}

// functie om de API aan te roepen
int callApi(String type, String iban, String pasnummer, String pin, String amount) {
  HTTPClient http;
  WiFiClient client;
  String url = "";
  String json = "";
  String param = "";
  int urlType = 0; // 0 for noob, 1 for local

  // checkt of de iban van Symple is of niet
  // dit begint ook gelijk de url op te bouwen
  if (iban.indexOf("SYMB") > -1) {
    url = local_api;
    urlType = 1; // Use local API
  } else {
    url = noob_api;
    param = "?target=" + iban;
    urlType = 0; // Use noob API
  }
  
  // kijkt of je geld wilt opnemen of informatie wilt ophalen
  if (type == "getinfo") {
    url += "getinfo" + param;
    json = "{\"iban\": \"" + iban + "\", \"pin\": \"" + pin + "\", \"pasnummer\": \"" + pasnummer + "\"}";
  }
  if (type == "withdraw") {
    url += "withdraw" + param;
    json = "{\"iban\": \"" + iban + "\", \"pin\": \"" + pin + "\", \"pasnummer\": \"" + pasnummer + "\", \"amount\": \"" + amount + "\"}";
  }
  
  // debug print om te zien wat er waar verstuurd wordt
  Serial.println("Calling API: " + url);
  Serial.println("Request JSON:");
  Serial.println(json);
  
  // start de http request
  http.begin(client, url); // Specify the URL
  if (urlType == 0) {
    http.addHeader("NOOB-TOKEN", "7d23088f-5e46-4944-81d4-e99d5c827140"); // shhhhhhhhh you see nothing
  }
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Content-Length", String(json.length()));

  // vangt de response van de API
  int httpCode = http.POST(json);
  Serial.println("HTTP Response code: " + String(httpCode));
  if (httpCode > 0) { // Check for the returning code
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    JsonObject obj = doc.as<JsonObject>();
    iban = obj["iban"].as<String>();
    saldo = obj["saldo"].as<float>();

    Serial.println("HTTP Response code: " + String(httpCode));
    Serial.println("Response payload: " + payload);
    if (httpCode == 200) {
      // ga naar home pagina
      notifyClients("home:");
      
    } else {
      // show error on the page
      // reset variables
      // notifyClients("error:An error occurred while processing your request. Please try again.");
    }
    http.end();
    return httpCode;
  }
}

// functie voor de bonprinter
struct PrintJob {
  String line;
  uint8_t size;
  char justify;
  bool bold;
};

PrintJob printQueue[30];  // adjust for max lines
int queueLength = 0;
int currentLine = 0;
bool printing = false;
unsigned long lastPrintTime = 0;
const unsigned long lineDelay = 50;  // ms between lines

void prepareBon() {
  queueLength = 0;
  currentLine = 0;
  printing = true;

  printQueue[queueLength++] = {"", 'L', 'L', false}; // feed
  printQueue[queueLength++] = {"SYMPLE", 'L', 'L', true};
  printQueue[queueLength++] = {"--------------------------------", 'M', 'C', false};
  printQueue[queueLength++] = {"OPNAME BILJETTEN", 'M', 'C', false};
  printQueue[queueLength++] = {"", 'S', 'L', false};
  printQueue[queueLength++] = {"DATUM    TIJD    AUTOMAAT", 'S', 'L', false};
  printQueue[queueLength++] = {"9/11/2001    10:00:00    1", 'S', 'L', false};
  printQueue[queueLength++] = {"REKENINGNUMMER     PASNR", 'S', 'L', false};
  printQueue[queueLength++] = {"NLxx SMPL xxxx xx" + iban + "\t" + pasnummer, 'S', 'L', false};
  printQueue[queueLength++] = {"TRANSACTIE ID    : 1234567890", 'S', 'L', false};
  printQueue[queueLength++] = {"BEDRAG           : " + String(amount, 2) + " CFA", 'S', 'L', false};
  printQueue[queueLength++] = {"TRANSACTIE KOSTEN: €10.000,00", 'S', 'L', false};
  printQueue[queueLength++] = {"TOT ZIENS", 'M', 'C', false};
}

void handlePrinter() {
  if (!printing || currentLine >= queueLength) return;

  if (millis() - lastPrintTime < lineDelay) return;

  PrintJob& job = printQueue[currentLine];

  printer.setSize(job.size);
  printer.justify(job.justify);
  if (job.bold) printer.boldOn();
  else printer.boldOff();
  printer.println(job.line);

  currentLine++;
  lastPrintTime = millis();

  if (currentLine >= queueLength) {
    printer.feed(4);
    printing = false;
  }
}

// geld dispenser functies
void push(int dispenser){
  Serial.println("Pushing dispenser " + String(dispenser));
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
  Serial.println("Pulling dispenser " + String(dispenser));
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
  Serial.println("Stopping dispenser " + String(dispenser));
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

enum DispenserPhase { IDLE, PUSHING, PULLING };

struct Dispenser {
  int b10;
  int b20;
  int b50;
  bool isActive;

  DispenserPhase phase = IDLE;
  unsigned long phaseStart = 0;
  int currentChannel = 0;  // 1 = b50, 2 = b20, 3 = b10
};

Dispenser dispenser;

void startDispenser(const String& optie) {
  Serial.println("optie1 | b10: " + String(optie1.b10) + ", b20: " + String(optie1.b20) + ", b50: " + String(optie1.b50));
  Serial.println("optie2 | b10: " + String(optie2.b10) + ", b20: " + String(optie2.b20) + ", b50: " + String(optie2.b50));
  Serial.println("optie3 | b10: " + String(optie3.b10) + ", b20: " + String(optie3.b20) + ", b50: " + String(optie3.b50));
  if (optie == "optie1") {
    dispenser.b10 = optie1.b10;
    dispenser.b20 = optie1.b20;
    dispenser.b50 = optie1.b50;
  } else if (optie == "optie2") {
    dispenser.b10 = optie2.b10;
    dispenser.b20 = optie2.b20;
    dispenser.b50 = optie2.b50;
  } else if (optie == "optie3") {
    dispenser.b10 = optie3.b10;
    dispenser.b20 = optie3.b20;
    dispenser.b50 = optie3.b50;
  } else {
    Serial.println("Invalid option: " + optie);
    return;
  }
  dispenser.isActive = true;
  dispenser.phase = IDLE;
  dispenser.currentChannel = 0;
}

void rammelGeldUitAutomaat() {
  if (dispenser.isActive == false) return;
  unsigned long now = millis();
  
  if (dispenser.phase == IDLE) {
    // Pick next available bill
    if (dispenser.b50 > 0) {
      dispenser.currentChannel = 1;
      dispenser.b50--;
    } else if (dispenser.b20 > 0) {
      dispenser.currentChannel = 2;
      dispenser.b20--;
    } else if (dispenser.b10 > 0) {
      dispenser.currentChannel = 3;
      dispenser.b10--;
    } else {
      // Done
      stop(4);
      dispenser.isActive = false;
      dispenser.currentChannel = 0;
      Serial.println("All money dispensed, stopping dispensers.");
      return;
    }

    // Start push phase
    dispenser.phaseStart = now;
    dispenser.phase = PUSHING;
    push(dispenser.currentChannel);

  } else if (dispenser.phase == PUSHING) {
    if (now - dispenser.phaseStart >= 3000) {
      dispenser.phaseStart = now;
      dispenser.phase = PULLING;
      pull(dispenser.currentChannel);
    } else {
      push(dispenser.currentChannel); // Optional repeated call if needed
    }

  } else if (dispenser.phase == PULLING) {
    if (now - dispenser.phaseStart >= 1000) {
      dispenser.phase = IDLE; // Ready for next bill
      stop(dispenser.currentChannel);  // ✅ Stop motor after pull
    } else {
      pull(dispenser.currentChannel); // Optional repeated call
    }
  }
}

// ontvangt berichten die verstuurd zijn vanuit de website
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    String msg = "";
    for (size_t i = 0; i < len; i++) {
      msg += (char) data[i];
    }
    if (msg == "getSaldo") {
      notifyClients("saldo:" + String(saldo, 2)); // Notify clients with the current saldo
    }
    if (msg == "break") {
      iban = "";
      pasnummer = "";
      pin = "";
      saldo = 0.0;
      amount = 0.0;
      calculatedBiljetOptie1 = "";
      calculatedBiljetOptie2 = "";
      calculatedBiljetOptie3 = "";
      optie1 = {0, 0, 0};
      optie2 = {0, 0, 0};
      optie3 = {0, 0, 0};
    }
    if (msg == "pin20") {
      amount = 20.0;
      calculatedBiljetOptie1 = calculateOptie1(amount);
      calculatedBiljetOptie2 = calculateOptie2(amount);
      calculatedBiljetOptie3 = calculateOptie3(amount);
    }
    if (msg == "pin50") {
      amount = 50.0;
      calculatedBiljetOptie1 = calculateOptie1(amount);
      calculatedBiljetOptie2 = calculateOptie2(amount);
      calculatedBiljetOptie3 = calculateOptie3(amount);
    }
    if (msg == "pin70") {
      amount = 70.0;
      calculatedBiljetOptie1 = calculateOptie1(amount);
      calculatedBiljetOptie2 = calculateOptie2(amount);
      calculatedBiljetOptie3 = calculateOptie3(amount);
    }
    if (msg == "pin100") {
      amount = 100.0;
      calculatedBiljetOptie1 = calculateOptie1(amount);
      calculatedBiljetOptie2 = calculateOptie2(amount);
      calculatedBiljetOptie3 = calculateOptie3(amount);
    }
    if (msg == "getAmount") {
      notifyClients("amount:" + String(amount, 2)); // Notify clients with the current amount
    }
    if (msg == "optie1") {
      int code = callApi("withdraw", iban, pasnummer, pin, String(amount));
      if (code != 409) {
        Serial.println("Starting dispenser for optie1 with amount: " + String(amount));
        startDispenser("optie1");
      }
    }
    if (msg == "optie2") {
      int code = callApi("withdraw", iban, pasnummer, pin, String(amount));
      if (code != 409) {
        Serial.println("Starting dispenser for optie2 with amount: " + String(amount));
        startDispenser("optie2");
      }
    }
    if (msg == "optie3") {
      int code = callApi("withdraw", iban, pasnummer, pin, String(amount));
      if (code != 409) {
        Serial.println("Starting dispenser for optie3 with amount: " + String(amount));
        startDispenser("optie3");
      }
    }
    if (msg == "bon") {
      prepareBon();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("Client connected: %u\n", client->id());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("Client disconnected: %u\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    default:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

// leest de data van de arduino via I2C
void Arduino(int msg) {
  String fromArduino ="";
  while (Wire.available()) {
    char c = Wire.read();
    fromArduino += c;
  }
  int separatorIndex = fromArduino.indexOf(':');
  String command = "";
  String value = "";
  String subvalue = "";
  if (separatorIndex != -1) {
    command = fromArduino.substring(0, separatorIndex);
    value = fromArduino.substring(separatorIndex + 1);
    int subSeparatorIndex = value.indexOf(':');
    subvalue = "";
    if (subSeparatorIndex != -1) {
      subvalue = value.substring(subSeparatorIndex + 1);
      value = value.substring(0, subSeparatorIndex);
    }
  }

  // filtert de ontvangen data en stopt de data in de juiste variabelen
  if (command == "pas") {
    iban = value;
    pasnummer = subvalue;
    Serial.println("Received IBAN: " + iban + ", Pasnummer: " + pasnummer);
    pin = ""; // Reset pin after receiving new pas
    notifyClients("card:"); // Notify clients with the new PIN
  }
  if (command == "pin" && pin.length() != 4) {
    pin += value.toInt();
    Serial.println("Received PIN: " + String(pin));
    notifyClients("pin:" + pin); // Notify clients with the new PIN
  }
  if (command == "pin" && pin.length() == 4 && value == "K") {
    Serial.println("PIN already set: " + String(pin));
    callApi("getinfo", iban, pasnummer, pin, "0"); // Call API to get user info
  }
  if (command == "pin" && value == "X") {
    Serial.println("PIN reset");
    pin = "";
    notifyClients("pin:" + pin); // Notify clients with the new PIN
  }
}

// arduino code
void setup() {
  Serial.begin(9600);

  // dispenser
  pinMode(disp1_1, OUTPUT);
  pinMode(disp1_2, OUTPUT);
  pinMode(disp2_1, OUTPUT);
  pinMode(disp2_2, OUTPUT);
  pinMode(disp3_1, OUTPUT);
  pinMode(disp3_2, OUTPUT);
  
  // bon printer
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  printer.begin();
  printer.setFont('B');
  printer.setFont('A');

  // I2C
  Wire.begin(4);
  Wire.onReceive(Arduino);

  // pinMode(right1, INPUT);
  // pinMode(right2, INPUT);
  // pinMode(right3, INPUT);

  // webserver
  WiFi.config(INADDR_NONE, IPAddress(8,8,8,8), INADDR_NONE, INADDR_NONE);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  Serial.println(WiFi.dnsIP());
  initWebSocket();
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  const char *paths[7] = {"/", "/home", "/saldo", "/geldKeuze", "/bonVraag", "/biljetVraag", "/biljetOptie"};
  const char *varPages[7] = {index_html, home_html, saldo_html, geldKeuze_html, bonVraag_html, biljetVraag_html, biljetOptie_html};

  for (int i = 0; i < 7; i++) {
    server.on(paths[i], HTTP_GET, [i, varPages](AsyncWebServerRequest *request) {
      request->send(200, "text/html", varPages[i]);
    });
  }

  server.on("/opmaak.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/css", opmaak_css);
  });

  server.begin();
}

unsigned long lastMillis = 0; // Variable to store the last time the loop was executed

void loop()
{
  if (millis() - lastMillis >= 500) {
    lastMillis = millis(); // Update the lastMillis to the current time
    notifyClients("saldo:" + String(saldo, 2) + ":" +
                  "amount:" + String(amount, 2) + ":" +
                  "optie1:" + calculatedBiljetOptie1 + ":" +
                  "optie2:" + calculatedBiljetOptie2 + ":" +
                  "optie3:" + calculatedBiljetOptie3);
  }

  // bon
  handlePrinter();

  // geld
  rammelGeldUitAutomaat();
}