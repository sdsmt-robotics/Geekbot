#ifndef GEEKBOT_MOTION_H
#define GEEKBOT_MOTION_H

#include "geekbot_config.h"
#include "geekbot_definitions.h"
#include <Servo.h>


#define CLOCKWISE   0
#define COUNTERWISE 1

#define INPUT_SPEED_MIN  -INT_MAX
#define INPUT_SPEED_MAX   INT_MAX

#define PULSE_OFFSET      -12 // Geekbot cont-rotation servos DO NOT run the same speed forward and 
                              // backward with default Arduino ppm timings. This is a tuned value to give teh same
                              // speed roatation forward and backward 

#define MIN_SERVO_PULSE   1250 + PULSE_OFFSET
#define MAX_SERVO_PULSE   1750 + PULSE_OFFSET
#define DFT_SERVO_PULSE   (MAX_SERVO_PULSE + MIN_SERVO_PULSE)/2

enum Direction // enum for simple direction passing in function calls. Note this is from the whole robot's perspective
{
  forward,
  backward,
  left,
  right,
  stop
};

// Set wheel speed and direction
void leftWheelSpeed(int speed, unsigned char dir)
{
  int val = map(speed, INPUT_SPEED_MIN, INPUT_SPEED_MAX, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  if(dir == COUNTERWISE) 
    leftServo.writeMicroseconds(val);
  else if(dir == CLOCKWISE)
    leftServo.writeMicroseconds(DFT_SERVO_PULSE - (val - DFT_SERVO_PULSE));
  else
    leftServo.writeMicroseconds(DFT_SERVO_PULSE);
  return;
}

void rightWheelSpeed(int speed, unsigned char dir)
{
  int val = map(speed, INPUT_SPEED_MIN, INPUT_SPEED_MAX, MIN_SERVO_PULSE, MAX_SERVO_PULSE);
  if(dir == COUNTERWISE) 
    rightServo.writeMicroseconds(val);
  else if(dir == CLOCKWISE)
    rightServo.writeMicroseconds(DFT_SERVO_PULSE - (val - DFT_SERVO_PULSE));
  else
    rightServo.writeMicroseconds(DFT_SERVO_PULSE);
  return;
}

void halt()
{
  leftServo.write(90);
  rightServo.write(90);
}

// Turn in place
void turn(int inputSpeed, Direction dir)
{
  if(dir == left)
  {
    leftWheelSpeed(inputSpeed, CLOCKWISE);
    rightWheelSpeed(inputSpeed, CLOCKWISE);
  }
  else if(dir == right)
  {
    leftWheelSpeed(inputSpeed, COUNTERWISE);
    rightWheelSpeed(inputSpeed, COUNTERWISE);
  }
  else
    halt();

  return;
}

// Drive straight
void straight(int inputSpeed, Direction dir)
{
  if(dir == forward)
  {
    leftWheelSpeed(inputSpeed, COUNTERWISE);
    rightWheelSpeed(inputSpeed, CLOCKWISE);
  }
  else if(dir == backward)
  {
    leftWheelSpeed(inputSpeed, CLOCKWISE);
    rightWheelSpeed(inputSpeed, COUNTERWISE);
  }
  else
    halt();

  return;
}

#endif
