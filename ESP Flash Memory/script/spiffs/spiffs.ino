#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server;
int pin_led = D1;

void setup()
{
  Serial.begin(115200);
  SPIFFS.begin();
  
  pinMode(pin_led, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.resetSettings();
  bool res;
  res = wm.autoConnect("AutoConnectAP","password");
  if(!res) {
    Serial.println("Failed to connect");
    ESP.restart();  
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected");  
  }

  server.on("/", indexFile);
  server.on("/ledstate", toggleLED);
  server.begin();
}

void loop()
{
  server.handleClient();
}

void indexFile()
{
  File file = SPIFFS.open("/index.html","r");
  server.streamFile(file, "text/html");
  file.close();
}

void toggleLED()
{
  digitalWrite(pin_led,!digitalRead(pin_led));
  server.send(200);
}
