#ifndef DRIVING_H
#define DRIVING_H
#include <AccelStepper.h>
#include <Arduino.h>

class Driving
{
  private:
    AccelStepper stepper1;
    AccelStepper stepper2;
    int maxSpeed;
    int maxAccel;
    int motorStepsPerRevolution;
    float machineToMotor(float machine_degrees);
    float forwardToMotor(float forward_mm);
    void execute(float deg1, float deg2);

  public:
    Driving(int pin1, int pin2, int pin3, int pin4, int maxSpeed, int maxAccel, int motorStepsPerRevolution);
    void move_forward(float forward_mm);
    void turn(float machine_degrees);
    void setup();
};

#endif