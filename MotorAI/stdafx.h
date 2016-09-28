#ifndef __STDAFX_H__
#define __STDAFX_H__
#include <Arduino.h>
#include <stdarg.h>

#define ARDBUFFER 16
int DBG(String str, ...);

/******************** PIN USAGE ********************/
//Number of sensors
#define SENSORS 3

//LED light
#define LED 13

//SRF04 Ultrasonic Ranger
#define L_EYE_TRIG A5
#define L_EYE_ECHO A4 
#define F_EYE_TRIG A3
#define F_EYE_ECHO A2
#define R_EYE_TRIG A1
#define R_EYE_ECHO A0

//LM393 IR Spd Sensor
#define L_SPD_TRACK 2
#define R_SPD_TRACK 3

//L289 UNO Motor Controller
#define MOTOR_ENA 10
#define MOTOR_IN1 9
#define MOTOR_IN2 8

#define MOTOR_ENB 5
#define MOTOR_IN3 7
#define MOTOR_IN4 6

/******************** INGAME VALUE ********************/
//Speed rate of Motor: 0 ~ 255
#define MOTOR_DEFAULT_SPD_RATE 127
#define MOTOR_MAX_SPD_RATE 255
#define MOTOR_DEFAULT_ERROR_ACCEPTABLE_VALUE 0.1
#define MOTOR_MIN_ERROR_VALUE 0.03

//Timing
#define MOTOR_BALANCE_PERIOD 5000 //ms

#define FRONT_SIDE 0
#define LEFT_SIDE 1
#define RIGHT_SIDE 2
#define BACK_SIDE 3

#define DELTA_DISTANCE 5
//Direction Enum
//#define LEFT_SIDE 4
//#define RIGHT_SIDE 6
//#define FRONT_SIDE 8
//#define BACK_SIDE 2
#endif //__STDAFX_H__
