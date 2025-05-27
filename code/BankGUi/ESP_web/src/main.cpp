/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-web-server-multiple-pages
 */
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

#include <map>
#include <vector>
#include <cstdlib>  // For rand()

int storedMoneyValue = 0;

const std::vector<int> biljetten = {50, 20, 10};

#define LED_PIN 18  // ESP32 pin GPIO18 connected to LED

const char *ssid = "potatopotatoooooo";     // CHANGE IT
const char *password = "heelsterkwachtwoord";  // CHANGE IT

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

struct BiljetResult {
  int b10;
  int b20;
  int b50;
};

BiljetResult calculateOptie1(int bedrag) {
  BiljetResult result = {0, 0, 0};

  result.b50 = bedrag / 50;
  bedrag %= 50;

  result.b20 = bedrag / 20;
  bedrag %= 20;

  result.b10 = bedrag / 10;

  Serial.print("€50: ");
  Serial.print(result.b50);
  Serial.print(", €20: ");
  Serial.print(result.b20);
  Serial.print(", €10: ");
  Serial.println(result.b10);
  return result;
}

BiljetResult fallbackGreedy(int remaining, BiljetResult result, int aantalTientjes, int maxTientjes) {
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
  Serial.print("€50: ");
  Serial.print(result.b50);
  Serial.print(", €20: ");
  Serial.print(result.b20);
  Serial.print(", €10: ");
  Serial.println(result.b10);
  return result;
}

BiljetResult calculateOptie2(int bedrag, int maxTientjes = 6) {
  BiljetResult result = {0, 0, 0};
  int aantalTientjes = 0;
  int remaining = bedrag;

  while (remaining > 0) {
    std::vector<int> valid;
    if (remaining >= 50) valid.push_back(50);
    if (remaining >= 20) valid.push_back(20);
    if (remaining >= 10 && aantalTientjes < maxTientjes) valid.push_back(10);

    if (valid.empty()) {
      return fallbackGreedy(remaining, result, aantalTientjes, maxTientjes);
    }

    int keuze = valid[rand() % valid.size()];

    switch (keuze) {
      case 50: result.b50++; break;
      case 20: result.b20++; break;
      case 10: result.b10++; aantalTientjes++; break;
    }

    remaining -= keuze;
  }
  Serial.print("€50: ");
  Serial.print(result.b50);
  Serial.print(", €20: ");
  Serial.print(result.b20);
  Serial.print(", €10: ");
  Serial.println(result.b10);
  return result;
}

BiljetResult calculateOptie3(int bedrag) {
  BiljetResult result = {0, 0, 0};
  int remaining = bedrag;

  while (remaining > 0) {
    std::vector<int> valid;
    if (remaining >= 50) valid.push_back(50);
    if (remaining >= 20) valid.push_back(20);
    if (remaining >= 10) valid.push_back(10);

    int keuze = valid[rand() % valid.size()];

    switch (keuze) {
      case 50: result.b50++; break;
      case 20: result.b20++; break;
      case 10: result.b10++; break;
    }

    remaining -= keuze;
  }
  Serial.print("€50: ");
  Serial.print(result.b50);
  Serial.print(", €20: ");
  Serial.print(result.b20);
  Serial.print(", €10: ");
  Serial.println(result.b10);
  return result;
}



void notifyClients(String state) {
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
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



void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);

  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");

  Serial.print("ESP32 Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  initWebSocket();

  server.on("/opmaak.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/css", opmaak_css);
  });

  server.on("/pagina.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/javascript", pagina_js);
  });

  // Serve the specified HTML pages
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Web Server: home page");
    String html = index_html;
    request->send(200, "text/html", html);
  });

  server.on("/home.html", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("Web Server: home page POST");

    const AsyncWebParameter* pincodeParam = request->getParam("pincode", true);
    // String pincode = pincodeParam->value();
    if (true) {
      Serial.println("Received pincode: ");
      // Serial.println(pincode);

      // respond with home page
      request->send(200, "text/html", home_html);
    } 
    else {
      request->send(400, "text/plain", "Missing pincode");
    }
  });

  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Web Server: index page GET");
    String html = index_html;
    request->send(200, "text/html", html);
  });
  
  server.on("/saldo.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Web Server: saldo page GET");
    String html = saldo_html;
    request->send(200, "text/html", html);
  });

  server.on("/geldKeuze.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Web Server: geldkeuze page GET");
    String html = geldKeuze_html;
    request->send(200, "text/html", html);
  });

  server.on("/biljetVraag.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Web Server: biljetvraag page GET");
    String html = biljetVraag_html;
    request->send(200, "text/html", html);
  });

  server.on("/bonVraag.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Web Server: bonVraag page GET");
    String html = bonVraag_html;
    request->send(200, "text/html", html);
  });

  server.on("/biljetOptie.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Web Server: biljetOptie page GET");
    String html = biljetOptie_html;
    request->send(200, "text/html", html);
  });

  server.on("/home.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Web Server: home page GET");
    String html = home_html;
    request->send(200, "text/html", html);
  });

server.on("/saveAmount", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("bedrag", true)) {
        storedMoneyValue = request->getParam("bedrag", true)->value().toInt();
        request->send(200, "text/plain", "Bedrag opgeslagen");
        Serial.println("Received bedrag: " + String(storedMoneyValue));
    } else {
        request->send(400, "text/plain", "Geen bedrag ontvangen");
    }
});

  server.on("/biljettenResult", HTTP_GET, [](AsyncWebServerRequest *request){
    int bedrag = storedMoneyValue; // or from wherever you store the input amount

    BiljetResult optie1 = calculateOptie1(bedrag);
    BiljetResult optie2 = calculateOptie2(bedrag);
    BiljetResult optie3 = calculateOptie3(bedrag);

    String json = "{";
    json += "\"optie1\": {\"10\": " + String(optie1.b10) + ", \"20\": " + String(optie1.b20) + ", \"50\": " + String(optie1.b50) + "},";
    json += "\"optie2\": {\"10\": " + String(optie2.b10) + ", \"20\": " + String(optie2.b20) + ", \"50\": " + String(optie2.b50) + "},";
    json += "\"optie3\": {\"10\": " + String(optie3.b10) + ", \"20\": " + String(optie3.b20) + ", \"50\": " + String(optie3.b50) + "}";
    json += "}";

    request->send(200, "application/json", json);
  });

  server.on("/print", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      String body = "";
      for (size_t i = 0; i < len; i++) {
        body += (char)data[i];
      }

      Serial.println("✅ Received print content:");
      Serial.println(body);
      request->send(200, "text/plain", "Print ontvangen");
  });


  server.begin();
  Serial.println("ESP32 Web server started");
}

void loop() {
  // Your code can go here or be empty; the server is handled asynchronously
}
