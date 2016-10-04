#include "stdafx.h"
#include "Controller.h"

/* This AI is used for the Gameloft AI Contest 2016
 * Created by:
 *    > Team: =z=OcDzit=z=
 *    > Division: Game Evolution
 *    > Members:
 *      1. Huynh Le Trung Hau
 *      2. Duong Xuan Long
 *      3. Nguyen Dang Khanh
 */

//Initialize
void setup()
{
	Serial.begin(9600);

  CONTROL->Init();
  WHEEL->Init(8,10,1,7);
}

//main update
void loop()
{
//  WHEEL->Update();
//  CONTROL->RunForward();
  CONTROL->AutoRun();
}

