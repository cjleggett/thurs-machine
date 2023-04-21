#include <AccelStepper.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define PI 3.1415926535897932384626433832795
#define WHEEL_RADIUS 34.0
#define TURNING_RADIUS 130.0

const char* ssid = "MAKERSPACE";
const char* password = "12345678";

const String apiURL = "http://192.168.0.124:3000/api";

// Define motor pin connections
// Update pin numbers based on your motor driver connection
const int motorPin1 = 11;
const int motorPin2 = 10;
const int motorPin3 = 7;
const int motorPin4 = 6;

// Define the motor steps per revolution
const int motorStepsPerRevolution = 200;

String method;
long new_time = 1;
long old_time = 1;
float value;


// Define the AccelStepper object
// Connect and configure pins with the motor driver (e.g., A4988 or DRV8825)
AccelStepper stepper(AccelStepper::DRIVER, motorPin1, motorPin2);
AccelStepper stepper1(AccelStepper::DRIVER, motorPin3, motorPin4);

void setup() {
  // Set max speed and acceleration
  stepper.setMaxSpeed(300);
  stepper.setAcceleration(100);
  stepper1.setMaxSpeed(300);
  stepper1.setAcceleration(100);

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi");
  }
  Serial.println("Connected to the WiFi network");
}

// Converts machine turning degrees to motor degrees
float machineToMotor(float machine_degrees) {
    return machine_degrees * TURNING_RADIUS / (WHEEL_RADIUS);
}

turns => 2piR/2pir

// Converts forward distance (in cm) to motor degrees
float forwardToMotor(float forward_cm) {
  return forward_cm * 180 / (PI * WHEEL_RADIUS);
}

// moves machine forward x centimeters
void move_forward(float forward_cm) {
  float degrees = forwardToMotor(forward_cm) * motorStepsPerRevolution/360;
  // stepper.setMaxSpeed(500); // Set the normal speed
  // stepper1.setMaxSpeed(500); // Set the normal speed
  stepper.setCurrentPosition(0);
  stepper1.setCurrentPosition(0);
  delay(100);
  stepper.moveTo(degrees); 
  stepper1.moveTo(degrees); 
  while (stepper.distanceToGo() != 0 && stepper1.distanceToGo() != 0) {
    stepper.run(); 
    stepper1.run(); 
  }
}

void turn(float machine_degrees) {
  float degrees = machineToMotor(machine_degrees) * motorStepsPerRevolution/360;
  // stepper.setMaxSpeed(500); // Set the normal speed
  // stepper1.setMaxSpeed(500); // Set the normal speed
  stepper.setCurrentPosition(0);
  stepper1.setCurrentPosition(0);
  delay(100);
  stepper.moveTo(degrees);
  stepper1.moveTo(-1 * degrees);
  // make this to step(1)
  while (stepper.distanceToGo() != 0 && stepper1.distanceToGo() != 0) {
    stepper.run(); 
    stepper1.run();
  }
}

void circle(float machine_degrees) {
  float degrees = machineToMotor(machine_degrees) * motorStepsPerRevolution/360;
  // stepper.setMaxSpeed(500); // Set the normal speed
  // stepper1.setMaxSpeed(500); // Set the normal speed
  stepper.setCurrentPosition(0);
  stepper1.setCurrentPosition(0);
  delay(100);
  stepper.moveTo(degrees);
  stepper1.moveTo(-1 * degrees);
  // make this to step(1)
  while (stepper.distanceToGo() != 0 && stepper1.distanceToGo() != 0) {
    stepper.run(); 
    stepper1.run();
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
      DynamicJsonDocument doc(256);
      DeserializationError error = deserializeJson(doc, payload);
      // Serial.println(payload);
      if (error) {
        Serial.println("Deserialization Error");
        new_time = 1;
        method = "";
        return;
      }
      method = doc["method"].as<String>();
      value = float(doc["value"]);
      new_time = long(doc["time"]);
      Serial.println(value);
      Serial.println(method);
      Serial.println(new_time);
    }
    http.end();
    // Serial.println(value);
    // Serial.println(method);
    // Serial.println(new_time);
    if (old_time != new_time) {
      if (method == "turn") {
        turn(value);   
      }
      if (method == "move") {
        move_forward(value);
      }
    }

    old_time = new_time;
}
}