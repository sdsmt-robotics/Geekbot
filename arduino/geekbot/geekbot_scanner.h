#ifndef GEEKBOT_SCANNER_H
#define GEEKBOT_SCANNER_H

#include "geekbot_hardware.h"
#include "geekbot_definitions.h"

#define MIN_DISTANCE  5   //in MM
#define SWEEP_DEGREES 180
#define SWEEP_STEPS   30  //one way
#define INCREMENT_DEGREES (SWEEP_DEGREES / SWEEP_STEPS)
#define SWEEP_PERIOD  5.0    //in seconds
#define STEP_MICROS   (SWEEP_PERIOD * 1000000) / (SWEEP_STEPS) //micros per step to get one full sweep 

int sweep_distances[SWEEP_STEPS] = {0};
int distances_temp[SWEEP_STEPS] = {0};

unsigned int ir_get_mm(void)
{
  int val = analogRead(IR_PIN);
  int dist = MIN_DISTANCE;
  if( val > MIN_DISTANCE)
    dist = (6787/(val-3)) - 4; //distance in mm  pow((val / 1893.9), -1.087);
  return dist; 
}

void update_scanner(void) // This is broken, please ignore.
{
  
}

void ir_pan_to(int data)
{
  int pos = map(data, INT_MAX, INT_MIN, 0, 180); // Maps the value of a Short int to something a servo can use
  irServo.write(pos);
  return;
}


#endif
