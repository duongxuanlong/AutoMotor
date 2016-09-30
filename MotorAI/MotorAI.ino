#include "stdafx.h"
#include "Controller.h"

//Variables
long lastTimeBalance = 0;

//Initialize
void setup()
{
	Serial.begin(9600);

  CONTROL->Init();
  //LOCATE->UpdateDistance();
  //CONTROL->Move(FRONT_SIDE);
}

//main update
void loop()
{
//  if(millis() - lastTimeBalance >= MOTOR_BALANCE_PERIOD)
//  {
//    lastTimeBalance = millis();
//    CONTROL->Balance();
//  }
//  delay(2000);
//  CONTROL->Move(FRONT_SIDE);
//  delay(2000);
//  CONTROL->Move(BACK_SIDE);
//    if (millis() - lastTimeBalance >= 2000)
//    {
//      lastTimeBalance = millis();
//      LOCATE->UpdateDistance();
//    }

//  if (CONTROL->GetCurrentDirection() == FRONT_SIDE)
//	{
//		LOCATE->UpdateDistance();
//		CONTROL->SetDirection(LOCATE->GetDirection());
//	}
	
	CONTROL->RunningOnDirection();
    //CONTROL->Move(LOCATE->GetDirection());
   // CONTROL->Balance();
}




