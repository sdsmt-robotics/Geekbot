#ifndef __GEEKBOT_CONFIG_H
#define __GEEKBOT_CONFIG_H

#include "geekbot_hardware.h"
#include "geekbot_communication.h"

// Self explanatory
void start_serial()
{
  Serial.begin(BAUDRATE);
  return;
}

// Sets pins to output, attaches the pins to servo objects and centers them
void start_servos()
{
  pinMode(SERVO_IR_PIN, OUTPUT);
  pinMode(SERVO_LEFT_PIN, OUTPUT);
  pinMode(SERVO_RIGHT_PIN, OUTPUT);
  
  leftServo.attach(SERVO_LEFT_PIN);
  rightServo.attach(SERVO_RIGHT_PIN);
  irServo.attach(SERVO_IR_PIN);

  leftServo.write(90);
  rightServo.write(90);
  irServo.write(90);

  return;
}

// Starts all input pins
void start_inputs()
{
  pinMode(BUTTON_PIN, INPUT);
  pinMode(IR_PIN, INPUT);

  start_serial();

  return;
}

// Starts inits all output pins and servos
void start_outputs()
{
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_LEFT_PIN, OUTPUT);
  pinMode(LED_RIGHT_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_LEFT_PIN, LOW);
  digitalWrite(LED_RIGHT_PIN, LOW);

  start_servos();

  return;
}

// Why is this here? Oh yeah, init code should you need it for anything
int start_code()
{
  return 0;
}

// Main call to start the whole shenanigan
void geekbot_init()
{
  start_inputs();
  start_outputs();
  start_code();

  return;
}

#endif
