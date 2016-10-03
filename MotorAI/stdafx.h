#ifndef __STDAFX_H__
#define __STDAFX_H__
#include <Arduino.h>
#include <stdarg.h>
#include <NewPing.h>

#define ARDBUFFER 16
#define DEBUG_MODE
void DBG(String str, ...);

/******************** PIN USAGE ********************/
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
#define MOTOR_RPM_TARGET 7

//Speedometer
#define DPR_RISING_FALLING_MODE 20
#define DPR_CHANGE_MODE (int)(DPR_RISING_FALLING_MODE * 2)

//Timing
#define WHEEL_UPDATE_INTERNAL 5000 //ms
#define LOCATING_PING_INTERVAL 33 //ms
#define LIMITED_TIME 10 //ms

//Direction Enum
#define FRONT_SIDE 0
#define LEFT_SIDE 1
#define RIGHT_SIDE 2
#define BACK_SIDE 3

#define DELTA_DISTANCE 5

//Number of sensors
#define TOTAL_SENSORS 3

//Define Distances
#define MAX_DISTANCE 400
#define MIN_FRONT_DISTANCE 50
#define LIMITED_SIDES 15
#define MIN_SIDE_DISTANCE 10
#endif //__STDAFX_H__

