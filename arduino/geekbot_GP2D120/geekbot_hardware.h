#ifndef GEEKBOT_HARDWARE_H
#define GEEKBOT_HARDWARE_H

#include <Servo.h>

#define BUZZER_PIN      2 // Which pin goes where!
#define LED_LEFT_PIN    7
#define LED_RIGHT_PIN   4
#define SERVO_IR_PIN    3
#define SERVO_LEFT_PIN  11
#define SERVO_RIGHT_PIN 9
#define SERVO_CAM_HORIZONTAL_PIN  6
#define SERVO_CAM_VERTICAL_PIN    5
#define SERVO_CAM_ROLL_PIN        10

#define BUTTON_PIN      8
#define IR_PIN          A0

Servo leftServo, rightServo, irServo; // Servo objects used elsewhere
Servo horServo, vertServo, rollServo;

#endif
