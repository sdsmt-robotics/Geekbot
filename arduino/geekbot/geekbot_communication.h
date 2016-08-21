#ifndef __GEEKBOT_COMMUNICATION_H
#define __GEEKBOT_COMMUNICATION_H

#define BAUDRATE      		115200  // Default baudrate

#define FLAG_DRIVE_STRAIGHT 0x45    // These are the flag values, must match on the linux side
#define FLAG_DRIVE_TURN     0x37
#define FLAG_DRIVE_LEFT     0x36
#define FLAG_DRIVE_RIGHT    0x35

#define FLAG_OUT_LIGHTS     0x30
#define FLAG_OUT_BUZZER     0x29

#define FLAG_CAM_PAN_HOR    0x28
#define FLAG_CAM_PAN_VERT   0x27
#define FLAG_CAM_ROLL       0x26

// This just makes an int from two bytes
int make_int(unsigned char highbyte, unsigned char lowbyte)
{
  int val = 0;
  val |= highbyte;
  val <<= 8;
  val |= lowbyte;

  return val;
}

#endif

