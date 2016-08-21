#ifndef GEEKBOT_SCANNER_H
#define GEEKBOT_SCANNER_H

#include "geekbot_hardware.h"

#define MIN_DISTANCE  50   //in MM
#define SWEEP_DEGREES 180
#define SWEEP_STEPS   30  //one way
#define INCREMENT_DEGREES (SWEEP_DEGREES / SWEEP_STEPS)
#define SWEEP_PERIOD  5.0    //in seconds
#define STEP_MICROS   (SWEEP_PERIOD * 1000000) / (SWEEP_STEPS) //micros per step to get one full sweep 

int sweep_distances[SWEEP_STEPS] = {0};
int distances_temp[SWEEP_STEPS] = {0};

double get_distance_mm(void)
{
  int val = analogRead(IR_PIN);
  double dist = MIN_DISTANCE;
  if( val > 3)
    double dist = (67870/(val-3)) - 4; //distance in mm
  return dist; 
}

void update_scanner(void) // This is broken, please ignore.
{
  static bool reverse = 0;
  static int step = 0, pos = 0;
  static unsigned long micros_last_sweep = 0;
  if(micros_last_sweep == 0)
    micros_last_sweep = micros();

  if(micros() > micros_last_sweep + 1)
  {
    if(step < 0 || step > SWEEP_STEPS)
    {
      memcpy(sweep_distances, distances_temp, sizeof(distances_temp));
      micros_last_sweep = micros();
    }
  }
}


#endif
