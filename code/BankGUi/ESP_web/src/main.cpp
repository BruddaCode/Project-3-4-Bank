// libs
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <HTTPClient.h>

// paginas
#include "index.h"
#include "home.h"
#include "saldo.h"
#include "geldKeuze.h"
#include "opmaak.css"
#include "pagina.js"
#include "biljetVraag.h"
#include "bonVraag.h"
#include "biljetOptie.h"

// global variables
String noob_api = "noob.datalabrotterdam.nl/api/noob/users/";
String local_api = "http://145.24.222.28:9000/api/noob/users/";

// user variables
String iban;
String pasnummer;
String pin;
float saldo;
float amount;

// webserver
const char *ssid = "potatopotatoooooo";     // CHANGE IT
const char *password = "heelsterkwachtwoord";  // CHANGE IT

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    String msg = "";
    for (size_t i = 0; i < len; i++) {
      msg += (char) data[i];
    }
    Serial.println(msg);
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

void notifyClients(String msg) {
  ws.textAll(msg);
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void callApi(String type, String iban, String pasnummer, String pin, String amount) {
  HTTPClient http;
  String url = "";
  String param = "?target=" + iban;
  String json = "";
  if (iban.indexOf("SYMB") > -1) {
    url += local_api;
  } else {
    url += noob_api;
  }

  http.addHeader("NOOB-TOKEN", "7d23088f-5e46-4944-81d4-e99d5c827140"); // shhhhhhhhh you see nothing

  if (type == "getinfo") {
    url += "getinfo" + param;
    json = "{\"iban\": "+ iban +",  \"pin\": "+ pin +", \"pasnummer\": "+ pasnummer +"}";
  }

  if (type == "withdraw") {
    url += "withdraw" + param;
    json = "{\"iban\": "+ iban +",  \"pin\": "+ pin +", \"pasnummer\": "+ pasnummer +", \"amount\": "+ amount +"}";
  }

  Serial.println("Calling API: " + url);
  
  int httpCode = http.POST(json);
  if (httpCode > 0) { // Check for the returning code
    String payload = http.getString();
    Serial.println("HTTP Response code: " + String(httpCode));
    Serial.println("Response payload: " + payload);
    if (httpCode == 200) {
      // go to next page
      // fill saldo variable
    } else {
      // show error on the page
      // reset variables
    }
  }
}


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
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.print("ESP32 Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
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

  server.on("/pagina.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/javascript", pagina_js);
  });

  server.begin();
}

void loop()
{
  
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