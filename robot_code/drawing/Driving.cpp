#include "Driving.h"
#include <Arduino.h>
#include <AccelStepper.h>
#include <Arduino.h>

#define PI 3.1415926535897932384626433832795
#define WHEEL_RADIUS 34.0
#define TURNING_RADIUS 142.0

Driving :: Driving(int pin1, int pin2, int pin3, int pin4, int maxSp, int maxAc, int motorSPR) {
  stepper1 = AccelStepper(AccelStepper::DRIVER, pin1, pin2);
  stepper1 = AccelStepper(AccelStepper::DRIVER, pin3, pin4);
  maxSpeed = maxSp;
  maxAccel = maxAc;
  motorStepsPerRevolution = motorSPR;
}

void Driving :: setup() {
  stepper1.setMaxSpeed(maxSpeed);
  stepper1.setAcceleration(maxAccel);
  stepper2.setMaxSpeed(maxSpeed);
  stepper2.setAcceleration(maxAccel);
}

// Converts machine turning degrees to motor degrees
float Driving :: machineToMotor(float machine_degrees) {
    return machine_degrees * TURNING_RADIUS / (WHEEL_RADIUS);
}

// Converts forward distance (in cm) to motor degrees
float Driving :: forwardToMotor(float forward_cm) {
  return forward_cm * 180 / (PI * WHEEL_RADIUS);
}

// Loops through to execute whatever commands are sent to the two motors
void Driving :: execute(float deg1, float deg2) {
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  delay(100);
  stepper1.moveTo(deg1); 
  stepper2.moveTo(deg2);
  while (stepper1.distanceToGo() != 0 && stepper2.distanceToGo() != 0) {
    stepper1.run(); 
    stepper2.run(); 
  }
}

// moves machine forward x centimeters
void Driving :: move_forward(float forward_cm) {
  float degrees = forwardToMotor(forward_cm) * motorStepsPerRevolution/360;
  execute(degrees, degrees);
}

// Turns machine X degrees
void Driving :: turn(float machine_degrees) {
  float degrees = machineToMotor(machine_degrees) * motorStepsPerRevolution/360;
  execute(degrees, -1 * degrees);
}