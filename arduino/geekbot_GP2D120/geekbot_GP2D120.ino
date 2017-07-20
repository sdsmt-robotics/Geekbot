#include "geekbot_config.h"
#include "geekbot_motion.h"
#include "geekbot_communication.h"
#include "geekbot_definitions.h"
#include "geekbot_scanner.h"

bool light = 0; // Stores light state, defaults to off
bool debug = 0; // debug switch
int dist;

void beep(int count)
{
  for(int i = 0; i < count; i++)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
  delay(200);
}

void setup()
{
  geekbot_init(); // Get things set up and ready to run
  unsigned char shake = 0;
  //beep(1);
  while(shake != 0x77)
    if(Serial.available() > 0)
      shake = Serial.read();
  //beep(1);
  while(Serial.available() > 0)
    Serial.read();
  beep(2);
  Serial.write(0x77);
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
          straight(data, forward);
        else if (data < 0)
          straight(abs(data), backward);
        else
          halt();
        if(debug) beep(1);
        break;

      case FLAG_DRIVE_TURN:
        if (data > 0)
          turn(data, right);
        else if (data < 0)
          turn(abs(data), left);
        else
          halt();
        if(debug) beep(2);
        break;

      case FLAG_DRIVE_LEFT:
        if (data > 0)
          leftWheelSpeed(data, CLOCKWISE);
        else if (data < 0)
          leftWheelSpeed(abs(data), COUNTERWISE);
        else
          halt();
        if(debug) beep(3);
        break;

      case FLAG_DRIVE_RIGHT:
        if (data > 0)
          rightWheelSpeed(data, COUNTERWISE);
        else if (data < 0)
          rightWheelSpeed(abs(data), CLOCKWISE);
        else
          halt();
        if(debug) beep(4);
        break;

      case FLAG_IR_PAN:
        ir_pan_to(data);
        if(debug) beep(5);
        break;

      case FLAG_IR_GET:
        dist = ir_get_mm();
        send_value(dist);
        if(debug) beep(6);
        //Serial.write(lowByte(dist));
        //Serial.write(highByte(dist));
        //Serial.println(dist);
        break;

      case FLAG_OUT_LIGHTS:
        if (data > 1)
        {
          digitalWrite(LED_LEFT_PIN, HIGH);
          digitalWrite(LED_RIGHT_PIN, HIGH);
        }
        else
        {
          digitalWrite(LED_LEFT_PIN, LOW);
          digitalWrite(LED_RIGHT_PIN, LOW);
        }
        if(debug) beep(7);
        break;

      case FLAG_OUT_BUZZER:
        if (data > 1)
          digitalWrite(BUZZER_PIN, HIGH);
        else
          digitalWrite(BUZZER_PIN, LOW);
        if(debug) beep(8);
        break;

      default:
        halt(); // If things fall apart just stop please
        beep(1);
        Serial.read();
    }
  }
}

