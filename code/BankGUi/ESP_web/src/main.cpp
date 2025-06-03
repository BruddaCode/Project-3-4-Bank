#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "index.h"
#include "home.h"
#include "saldo.h"
#include "geldKeuze.h"
#include "opmaak.css"
#include "pagina.js"
#include "biljetVraag.h"
#include "bonVraag.h"
#include "biljetOptie.h"

#define button1 27
#define button2 26

int pin = 1;

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

void setup() {
  Serial.begin(9600);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

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
  if (digitalRead(button1) == HIGH) {
    Serial.println("card");
    notifyClients("card:1");
    delay(500); // Debounce delay
  }
  if (digitalRead(button2) == HIGH) {
    Serial.println("pin" + String(pin));
    notifyClients("pin:" + String(pin)); // Example pin, replace with actual logic
    delay(500); // Debounce delay
    pin += pin * 10; // Simulate pin change for next read
  }
}
