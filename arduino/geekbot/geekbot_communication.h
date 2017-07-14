#ifndef __GEEKBOT_COMMUNICATION_H
#define __GEEKBOT_COMMUNICATION_H

#define BAUDRATE      		57600  // Default baudrate
#define RIGHT_MASK        0xFF
#define LEFT_MASK         0xFF00
#define HANDSHAKE         0x77
#define FLAG_FINISHED     0x33

#define FLAG_DRIVE_STRAIGHT 0x45    // These are the flag values, must match on the linux side
#define FLAG_DRIVE_TURN     0x37
#define FLAG_DRIVE_LEFT     0x36
#define FLAG_DRIVE_RIGHT    0x35

#define FLAG_OUT_LIGHTS     0x30
#define FLAG_OUT_BUZZER     0x29

#define FLAG_IR_PAN         0x28
#define FLAG_IR_GET         0x27



// This just makes an int from two bytes
int make_int(unsigned char highbyte, unsigned char lowbyte)
{
  int val = 0;
  val |= highbyte;
  val <<= 8;
  val |= lowbyte;

  return val;
}

short bound_short(short num)
{
  if(num > 32767)
    return 32767;
  if(num < -32768)
    return -32768;
  else
    return num;
}
bool send_value(int input)
{
    unsigned int left_byte = 0;
    unsigned int right_byte = 0;
    input = bound_short(input);
    //Get and write each individual unsigned char of sensor value
    //bit things with leftValue
    left_byte = input & LEFT_MASK;
    left_byte = left_byte >> 8;
    right_byte = input & RIGHT_MASK;

    Serial.write( (unsigned char) left_byte);
    Serial.write( (unsigned char) right_byte);
}

#endif

