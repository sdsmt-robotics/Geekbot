#ifndef GEEKBOT_GIMBAL_H
#define GEEKBOT_GIMBAL_H
#include "geekbot_hardware.h"
#include "geekbot_definitions.h"

void hor_pan_to(int data)
{
  int pos = map(data, INT_MAX, INT_MIN, 0, 180); // Maps the value of a Short int to something a servo can use
  horServo.write(pos);
  return;
}


void vert_pan_to(int data)
{
  int pos = map(data, INT_MIN, INT_MAX, 0, 180);
  vertServo.write(pos);
  return;
}


void roll_to(int data)
{
  int pos = map(data, INT_MIN, INT_MAX, 0, 180);
  rollServo.write(pos);
  return;
}



#endif
