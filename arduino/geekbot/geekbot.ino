#include "geekbot_config.h"
#include "geekbot_motion.h"
#include "geekbot_gimbal.h"
#include "geekbot_communication.h"
#include "geekbot_definitions.h"
#include "geekbot_scanner.h"

bool light = 0; // Stores light state, defaults to off

void setup()
{
  geekbot_init(); // Get things set up and ready to run
}

void loop()
{
  if (Serial.available() > 2) // Wait for a full message
  {
    unsigned char flag = Serial.read();
    unsigned char high = Serial.read();
    unsigned char low  = Serial.read();

    int data = make_int(low, high); // Make those two bytes into a two-byte int. Endian-ness might screw this up
    switch (flag)
    {
      case FLAG_DRIVE_STRAIGHT: // This switch is pre self-explanatory
        if (data > 0)
          straight(data, backward);
        else if (data < 0)
          straight(abs(data), forward);
        else
          halt();
        break;

      case FLAG_DRIVE_TURN:
        if (data > 0)
          turn(data, right);
        else if (data < 0)
          turn(abs(data), left);
        else
          halt();
        break;

      case FLAG_DRIVE_LEFT:
        if (data > 0)
          leftWheelSpeed(data, CLOCKWISE);
        else if (data < 0)
          leftWheelSpeed(abs(data), COUNTERWISE);
        else
          halt();
        break;

      case FLAG_DRIVE_RIGHT:
        if (data > 0)
          rightWheelSpeed(data, COUNTERWISE);
        else if (data < 0)
          rightWheelSpeed(abs(data), CLOCKWISE);
        else
          halt();
        break;

      case FLAG_OUT_LIGHTS:
        if (data == 1)
        {
          light = !light;
          digitalWrite(LED_LEFT_PIN, light);
          digitalWrite(LED_RIGHT_PIN, light);
        }
        break;

      case FLAG_OUT_BUZZER:
        if (data == 1)
          digitalWrite(BUZZER_PIN, HIGH);
        else
          digitalWrite(BUZZER_PIN, LOW);
        break;

      case FLAG_CAM_PAN_HOR:
        hor_pan_to(data);
        break;
        
      case FLAG_CAM_PAN_VERT:
        vert_pan_to(data);
        break;
        
      case FLAG_CAM_ROLL:
        roll_to(data);
        break;

      default:
        halt(); // If things fall apart just stop please
    }
  }
}

