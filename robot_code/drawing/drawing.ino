#include <AccelStepper.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Driving.h"
#define PI 3.1415926535897932384626433832795
#define MAX_ACCELERATION 50
#define MAX_SPEED 200
#define WATER_PIN 40

const char* ssid = "MAKERSPACE";
const char* password = "12345678";

const String apiURL = "http://192.168.0.124:3000/api_coordinates";

// Define motor pin connections
// Update pin numbers based on your motor driver connection
const int motorPin1 = 11;
const int motorPin2 = 10;
const int motorPin3 = 7;
const int motorPin4 = 6;

// Define the motor steps per revolution
const int motorStepsPerRevolution = 200;
long old_id = -1;

Driving driver = Driving(motorPin1, motorPin2, motorPin3, motorPin4, MAX_SPEED, MAX_ACCELERATION, motorStepsPerRevolution);

void setup() {
  // Set max speed and acceleration
  driver.setup();

  // Set up water pump pin
  pinMode(WATER_PIN, OUTPUT);
  Serial.begin(9600);

  // Set up WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi");
  }
  Serial.println("Connected to the WiFi network");
}

void toggle_pump(int on) {
  if (on) {
    digitalWrite(WATER_PIN, HIGH);
  } else {
    digitalWrite(WATER_PIN, LOW);
  }
}


void loop() {
  Serial.println("Refreshing");
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(apiURL);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(8192);
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.println("Deserialization Error");
        return;
      }

      int id = doc["id"];

      if (id != old_id) {
        for (JsonObject instruction : doc["instructions"].as<JsonArray>()) {
          double instruction_angle = instruction["angle"];
          double instruction_distance = instruction["distance"];
          int instruction_pendown = instruction["pendown"];
          toggle_pump(instruction_pendown);
          driver.turn(instruction_angle);
          driver.move_forward(instruction_distance);
        }
        old_id = id;
      }
    }
    http.end();
}
}