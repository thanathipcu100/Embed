
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


#define W_SSID "AndroidAP33B6"
#define W_PASS "ofyx4127"
String HOST_NAME = "http://pure-fortress-43600.herokuapp.com/";
//D1
#define OUTPUT_FORCEON 5
//D2
#define OUTPUT_AUTOLIGHT 4
//D5
#define INPUT_ISON 14
//D6
#define INPUT_LIGHT 12

HTTPClient http;

void setup() {
  Serial.begin(115200);
  WiFi.begin(W_SSID, W_PASS);
  pinMode(OUTPUT_FORCEON, OUTPUT);
  pinMode(OUTPUT_AUTOLIGHT, OUTPUT);
  pinMode(INPUT_ISON, INPUT);
  pinMode(INPUT_LIGHT, INPUT);
}

void workWithServer() {
  StaticJsonBuffer<50> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  
  JSONencoder["isOn"] = digitalRead(INPUT_ISON);
  JSONencoder["light"] = digitalRead(INPUT_LIGHT);
  JSONencoder["infrared"] = 0;

  char JSONmessageBuffer[50];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

  http.begin(HOST_NAME + "api/sensor");
  // application/json
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(JSONmessageBuffer);
  //int httpCode = http.GET();
  //if(httpCode>0) Serial.println(httpCode);
  //else Serial.println("error");
  if(httpCode==200) {
    DynamicJsonBuffer jsonBuffer(50);
    JsonObject& resp = jsonBuffer.parseObject(http.getString());
    digitalWrite(OUTPUT_AUTOLIGHT, resp["isAutoIntensitySensor"]);
    digitalWrite(OUTPUT_FORCEON, resp["isForceOn"]);
    Serial.println("AutoLight :"+ String(digitalRead(OUTPUT_AUTOLIGHT)));
   Serial.println("ForceON :" + String(digitalRead(OUTPUT_FORCEON)));
    Serial.println("Light :" + String(digitalRead(INPUT_LIGHT)));
  }
  http.end();
}

void loop() {
  if(WiFi.status()== WL_CONNECTED) {
    workWithServer();
  }
  delay(500);
}
