#include "stdafx.h"
#include "Controller.h"

//Variables
long lastTimeBalance = 0;

//Initialize
void setup()
{
	Serial.begin(9600);

  CONTROL->Init();
  CONTROL->Move(FRONT_SIDE);
}

//main update
void loop()
{
  if(millis() - lastTimeBalance >= MOTOR_BALANCE_PERIOD)
  {
    lastTimeBalance = millis();
    CONTROL->Balance();
  }
}



