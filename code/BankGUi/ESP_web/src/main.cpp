// libs
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

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
String noob_api = "noob.datalabrotterdam.nl/api/noob/users/";
String local_api = "http://145.24.222.28:9000/api/noob/users/";

// user variables
String iban = "";
String pasnummer = "";
String pin = "";
float saldo = 0.0;
float amount = 0.0;

// webserver
const char *ssid = "Samsung Smart Fridge";
const char *password = "qqqqqqqq";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// stuurt een bericht naar de website
void notifyClients(String msg) {
  ws.textAll(msg);
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
    }
    if (msg == "pin20") {
      amount = 20.0;
    }
    if (msg == "pin50") {
      amount = 50.0;
    }
    if (msg == "pin70") {
      amount = 70.0;
    }
    if (msg == "pin100") {
      amount = 100.0;
    }
    if (msg == "getAmount") {
      notifyClients("amount:" + String(amount, 2)); // Notify clients with the current amount
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

void callApi(String type, String iban, String pasnummer, String pin, String amount) {
  HTTPClient http;
  WiFiClient client;
  String url = "";
  String json = "";
  String param = "";
  int urlType = 0; // 0 for noob, 1 for local

  // checkt of de iban van Symple is of niet
  // dit begint ook gelijk de url op te bouwen
  if (iban.indexOf("SYMB") > -1) {
    url += local_api;
    urlType = 1; // Use local API
  } else {
    url += noob_api;
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
  }
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
  
  // I2C
  Wire.begin(4);
  Wire.onReceive(Arduino);

  // pinMode(right1, INPUT);
  // pinMode(right2, INPUT);
  // pinMode(right3, INPUT);

  // webserver
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
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

void loop()
{

  notifyClients("amount:" + String(amount, 2)); // Notify clients with the current amount
  notifyClients("saldo:" + String(saldo, 2)); // Notify clients with the current saldo
  delay(1000); // Adjust the delay as needed
  
  // if (digitalRead(right1) == HIGH) {
  //   Serial.println("right1");
  //   delay(500); // Debounce delay
  // }
  // if (digitalRead(right2) == HIGH) {
  //   Serial.println("right2");
  //   delay(500); // Debounce delay
  // }
  // if (digitalRead(right3) == HIGH) {
  //   Serial.println("right3");
  //   delay(500); // Debounce delay
  // }


  // if (digitalRead(button1) == HIGH) {
  //   Serial.println("card");
  //   notifyClients("card:1");
  //   delay(500); // Debounce delay
  // }
  // if (digitalRead(button2) == HIGH) {
  //   Serial.println("pin" + String(pin));
  //   notifyClients("pin:" + String(pin)); // Example pin, replace with actual logic
  //   delay(500); // Debounce delay
  //   pin += pin * 10; // Simulate pin change for next read
  // }
}