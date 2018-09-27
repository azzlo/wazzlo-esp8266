/**
 * From:
   BasicHTTPClient.ino

    Created on: 24.05.2015

   Este codigo es modificado para enviar valores
   de un sensor a una api especificada en el codigo
   a travez del metodo PUT

*/

#include <Arduino.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

// Variables para sensor de ultrasonido
const int Trigger = 2;
const int Echo = 0;

void setup() {
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  digitalWrite(Trigger, LOW);

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("o", "SomosBonitos_");
}

void loop() {
  long t; // Demora del eco
  long d; // Distancia en CM
  
  // Vamos por el valor del sensor
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); // se obtiene el ancho del pulso
  d = t/59; //escalamos el tiempo a una distancia en cm
  
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    http.begin("http://192.168.6.121/water_devises/1"); //HTTP

    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header

    //###########################
    //    START OF My changes   #
    //###########################
    
    //int httpCode = http.GET();
    //char data[] = "{\"water_devise\": {\"current_value\": \"\"}}";
    char data[1024];
    snprintf(data, sizeof(data), "{\"water_devise\": {\"current_value\": \"%ld\"}}", d);
    
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.PUT(data);

    //###########################
    //     END OF My changes    #
    //###########################

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);
}

