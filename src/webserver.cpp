#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include "html_templates.h"
#include "storage.h"
#include "rtc.h"
#include <RTClib.h>

const char* apSSID = "Lieferdienst-Roulette";
const char* apPassword = "BME4877691";  // mind. 8 Zeichen!
const IPAddress apIP(192, 168, 4, 1);
const byte DNS_PORT = 53;

AsyncWebServer server(80);
DNSServer dnsServer;

void setupWiFiAndWebServer() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  // DNS-Umleitung (Captive Portal)
  dnsServer.start(53, "*", apIP); // Leitet jede Domain auf ESP-IP um
    // Captive Portal für Android/iOS
  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("/");
  });
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("/");
  });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(204);  // No content
  });

  server.onNotFound([](AsyncWebServerRequest *request){
  request->redirect("/");
});

  // Root-Seite anzeigen
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = getMainPageHTML();
    request->send(200, "text/html", html);
  });

  // POST-Handler für Lieferdienstspeicherung
  // JSON-Speicher-POST
server.on("/save_json", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "ok");
}, NULL, [](AsyncWebServerRequest *request, uint8_t* data, size_t len, size_t index, size_t total) {
    String body = "";
    for (size_t i = 0; i < len; i++) {
        body += (char)data[i];
    }
    Serial.println("[save_json] empfangen:");
    Serial.println(body);  // Debug

    saveLieferdienste(body);
});

server.on("/set_time", HTTP_POST, [](AsyncWebServerRequest *request){
  if (request->hasParam("datetime", true)) {
    String datetime = request->getParam("datetime", true)->value();  // Format: 2025-05-27T15:42
    int jahr = datetime.substring(0, 4).toInt();
    int monat = datetime.substring(5, 7).toInt();
    int tag = datetime.substring(8, 10).toInt();
    int stunde = datetime.substring(11, 13).toInt();
    int minute = datetime.substring(14, 16).toInt();
    rtc.adjust(DateTime(jahr, monat, tag, stunde, minute, 0));
    Serial.println("[RTC] Zeit gesetzt auf: " + datetime);
  }
  request->redirect("/");
});


  server.begin();
  Serial.println("[Webserver] Gestartet auf http://" + WiFi.softAPIP().toString());
}

void loopWebServer() {
  dnsServer.processNextRequest();  // Muss im loop() aufgerufen werden
}
