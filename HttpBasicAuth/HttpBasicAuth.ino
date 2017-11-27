#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#import "config.h"

#define DEBUGPRINT
#ifdef DEBUGPRINT
#define DEBUG_PRINT(x) Serial.println (x)
#else
#define DEBUG_PRINT(x)
#endif

//const char* ssid = "Funknetzwerk";
//const char* password = "#C3106_T1906#$";

ESP8266WebServer server(80);

const char* host = "emoncms.org";
const char* nodeId = "21";

//const char* www_username = "admin";
//const char* www_password = "esp8266";
long startMills = 0;

void setup() {
 /* Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if(WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }*/
  startMills = millis();

  Serial.begin(115200);

  DEBUG_PRINT();
  
  WiFiManager wifi;
  
  wifi.setTimeout(120);

  if(!wifi.autoConnect("Funknetzwerk")){
    DEBUG_PRINT("timeout - going to sleep");
    DEBUG_PRINT(millis() - startMills);

    delay(1000);

    ESP.restart();
  }
  
  ArduinoOTA.begin();

  server.on("/", [](){
    if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();
    server.send(200, "text/plain", "Login OK");
  });
  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
