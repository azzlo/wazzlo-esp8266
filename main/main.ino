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
// Libreria para mejorar la medición y uso del HC-SR04 y otros
#include <NewPing.h>


// ############################
// #  CONFIGURACION!!!!!!!!!  #
// ############################

// Variables para sensor de ultrasonido
#define USE_SERIAL Serial
#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     0  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 450 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

const int DEVICE_ID = 1; // ID del dispositivo en la API
const char URL_API[] = "http://wazzlo.herokuapp.com/containers"; // URL para actuazar con metodo PUT los valores del contenedor en la API
const char WIFI_SSID[] = "o";
const char WIFI_PASS[] = "SomosBonitos_";

// ############################
// #  FIN CONFIGURACION!!!!!  #
// ############################

ESP8266WiFiMulti WiFiMulti;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  //pinMode(Trigger, OUTPUT);
  //pinMode(Echo, INPUT);
  //digitalWrite(Trigger, LOW);

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);
}

void loop() {
  long distance; // Distancia en CM
  
  delay(50);
  distance = sonar.ping_cm();

  USE_SERIAL.printf("ULTRASONIC SENSOR values is %ld", distance);
  
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    USE_SERIAL.print("[HTTP] begin...\n");
    // configure traged server and url
    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    char url[1024];
    sprintf(url, "%s/%d", URL_API, DEVICE_ID);
    http.begin(url); //HTTP

    USE_SERIAL.printf("[HTTP] PUT to %s\n", url);
    // start connection and send HTTP header

    //###########################
    //    START OF My changes   #
    //###########################
    
    //int httpCode = http.GET();
    //char data[] = "{\"water_devise\": {\"current_value\": \"\"}}";
    char data[1024];
    snprintf(data, sizeof(data), "{\"container\": {\"last_sensor\": \"%ld\"}}", distance);
    
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.PUT(data);

    //###########################
    //     END OF My changes    #
    //###########################

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] PUT... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);
      }
    } else {
      USE_SERIAL.printf("[HTTP] PUT... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);
}

