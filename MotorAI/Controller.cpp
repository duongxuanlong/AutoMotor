#include "Controller.h"

Controller* Controller::s_instance = NULL;

volatile bool Controller::s_leftMoveDone = false;
volatile bool Controller::s_rightMoveDone = false;
    
Controller::Controller():
obsSolving(-1)
{ }

Controller::~Controller() { }

void Controller::Init()
{
  DBG("Controller::Init()");
  
  SPEED->Init(CHANGE);
  SPEED->SetLeftReachLimitCallback(Controller::LeftMoveDone);
  SPEED->SetRightReachLimitCallback(Controller::RightMoveDone);
  
  LOCATE->Init();
  WHEEL->Init(8, 10, 1, 8);  

  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  pinMode(MOTOR_ENB, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);

  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);

  analogWrite(MOTOR_ENA, MOTOR_MAX_SPD_RATE);
  analogWrite(MOTOR_ENB, MOTOR_MAX_SPD_RATE);
}

void Controller::LeftMoveDone()
{
  s_leftMoveDone = true;
}

void Controller::RightMoveDone()
{
  s_rightMoveDone = true;
}
    
void Controller::SetSpeed(unsigned int lSpdRate, unsigned int rSpdRate)
{
  analogWrite(MOTOR_ENA, lSpdRate);
  analogWrite(MOTOR_ENB, rSpdRate);
}

void Controller::Move(int direct, unsigned int opt)
{
  DBG("Controller::Move(direct = %d, opt = %d)", direct, opt);
  
  s_leftMoveDone = (opt == 0);
  s_rightMoveDone = (opt == 0);
        
  switch (direct)
  {
    case FRONT_SIDE:
      SPEED->Start();
      SPEED->SetFrontLimit(opt);
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, HIGH);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, HIGH);
      break;

    case LEFT_SIDE:
      SPEED->Stop();
      s_rightMoveDone = true;
      SPEED->SetLeftLimit(opt);
      digitalWrite(MOTOR_IN1, HIGH);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, HIGH);
      break;

    case RIGHT_SIDE:
      SPEED->Stop();
      s_leftMoveDone = true;
      SPEED->SetRightLimit(opt);
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, HIGH);
      digitalWrite(MOTOR_IN3, HIGH);
      digitalWrite(MOTOR_IN4, LOW);
      break;

    case BACK_SIDE:
      SPEED->Stop();
      SPEED->SetBackLimit(opt);
      digitalWrite(MOTOR_IN1, HIGH);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, HIGH);
      digitalWrite(MOTOR_IN4, LOW);
      break;
  }
}

void Controller::Stop()
{
  DBG("Controller::Stop()");
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
}

void Controller::AutoRun()
{
  unsigned int left = LOCATE->GetRange(LEFT_SIDE);
  unsigned int right = LOCATE->GetRange(RIGHT_SIDE);
  unsigned int front = LOCATE->GetRange(FRONT_SIDE);

  if (left == 0 && front == 0 && right == 0)
  {
    Stop();
    return;
  }

  if (left >= MIN_SIDE_DISTANCE && right >= MIN_SIDE_DISTANCE)
  {
    if (left < right)
    {
//      Move(BACK_SIDE);
//      delay(LIMITED_TIME);
      Move(LEFT_SIDE);
      delay(LIMITED_TIME);
      Move(FRONT_SIDE);
      delay(LIMITED_TIME);
      return;
    }
    else
    {
//      Move(BACK_SIDE);
//      delay(LIMITED_TIME);
      Move(RIGHT_SIDE);
      delay(LIMITED_TIME);
      Move(FRONT_SIDE);
      delay(LIMITED_TIME);
      return;
    }
  }

  if (left >= MIN_SIDE_DISTANCE)
  {
//    Move(BACK_SIDE);
//    delay(LIMITED_TIME);
    Move(LEFT_SIDE);
    delay(LIMITED_TIME);
    Move(FRONT_SIDE);
    delay(LIMITED_TIME);
    return;
  }

  if (right >= MIN_SIDE_DISTANCE)
  {
//    Move(BACK_SIDE);
//    delay(LIMITED_TIME);
    Move(RIGHT_SIDE);
    delay(LIMITED_TIME);
    Move(FRONT_SIDE);
    delay(LIMITED_TIME);
    return;
  }

  Move(FRONT_SIDE);
  delay(LIMITED_TIME);
}


/*******************************************************************/
/*******************************************************************/
/*******************************************************************/
void Controller::RunQuickTest()
{
  /*unsigned int front = LOCATE->GetRange(FRONT_SIDE, 3);
  unsigned int left = LOCATE->GetRange(LEFT_SIDE, 3);
  unsigned int right = LOCATE->GetRange(RIGHT_SIDE, 3);
  Serial.print("front sensor: ");
  Serial.print(front);
  Serial.println(" cm");
  Serial.print("left sensor: ");
  Serial.print(left);
  Serial.println(" cm");
  Serial.print("right sensor: ");
  Serial.print(right);
  Serial.println(" cm");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  delay(1000);

  Move(FRONT_SIDE, 1);
  while(!s_leftMoveDone || !s_rightMoveDone); //Wait task done
  Stop();
  delay(3000);*/
  Move(LEFT_SIDE, 20);
  while(!s_leftMoveDone || !s_rightMoveDone); //Wait task done
  Stop();
  delay(3000);
  Move(RIGHT_SIDE, 20);
  while(!s_leftMoveDone || !s_rightMoveDone); //Wait task done
  Stop();
  delay(3000);
  /*Move(BACK_SIDE, 1);
  while(!s_leftMoveDone || !s_rightMoveDone); //Wait task done
  Stop();
  delay(3000);*/
  
}

/*******************************************************************/
/*******************************************************************/
/*******************************************************************/

void Controller::AutoRun2()
{
  unsigned int front = LOCATE->GetRange(FRONT_SIDE, 3);
  unsigned int left = LOCATE->GetRange(LEFT_SIDE, 3);
  unsigned int right = LOCATE->GetRange(RIGHT_SIDE, 3);
  
  int direct[50];
  int opt[50];
  int count = 0;

  //Planning

  if(left == 0)
  {
    direct[count] = RIGHT_SIDE;
    opt[count++] = 10;
  }
  else if (right == 0)
  {
    direct[count] = LEFT_SIDE;
    opt[count++] = 10;
  }
  else if (front < 25) //Decision for new direction need to be made here
  {
    if (front < 6) //stuck front, should back then choose new direction
    {
      direct[count] = BACK_SIDE;
      opt[count++] = (int)(3 - front/2 - 0.1f);
    }
    else {} //This is perfect time to change direction
    
    if (obsSolving > 0 && left >= 50 && right >= 50)
    {
      int testLeft = abs(obsSolving - 90);
      int testRight = abs(obsSolving + 90);

      direct[count] = testLeft < testRight? LEFT_SIDE : RIGHT_SIDE ;
      obsSolving = testLeft < testRight? testLeft : testRight;
      opt[count++] = 90;
      
    }
    else if (left >= 50 && right > left)
    {
      direct[count] = LEFT_SIDE;
      if(obsSolving == 0)
      {
        obsSolving = -90;
      }
      else
      {
        obsSolving -= 90;
      }
      opt[count++] = 90;
    }
    else if (right >= 50 && left > right)
    {
      direct[count] = RIGHT_SIDE;
      if(obsSolving == 0)
      {
        obsSolving = -90;
      }
      else
      {
        obsSolving += 90;
      }
      opt[count++] = 90;
    }
    else
    {
      direct[count] = left > right? LEFT_SIDE : RIGHT_SIDE;
      if(obsSolving != -1)
      {
        obsSolving += direct[count] == LEFT_SIDE? -90 : 90;
      }
      opt[count++] = 90;
    }
  }
  else
  {
    if (obsSolving > 0 && (left >= 50 || right >= 50))
    {
      int testLeft = left >= 50? abs(obsSolving - 90) : 360;
      int testRight = right >= 50? abs(obsSolving + 90) : 360;

      if(testLeft < obsSolving && testLeft < testRight)
      {
        direct[count] = LEFT_SIDE;
        obsSolving -= 90;
        opt[count++] = 90;
      }
      else if(testRight < obsSolving && testRight < testLeft)
      {
        direct[count] = RIGHT_SIDE;
        obsSolving += 90;
        opt[count++] = 90;
      }
      else
      {
        direct[count] = FRONT_SIDE;
        opt[count++] = 0;
      }
    }
    else if (left <= 3 && right >= 3) //It run too close to the left, need move to right for awhile
    {
      direct[count] = RIGHT_SIDE;
      opt[count++] = 10;
  
      direct[count] = FRONT_SIDE;
      opt[count++] = 1;
  
      direct[count] = LEFT_SIDE;
      opt[count++] = 10;
    }
    else if (right <= 3 && left >= 3) //It run too close to the right, need move to left for awhile
    {
      direct[count] = LEFT_SIDE;
      opt[count++] = 10;
  
      direct[count] = FRONT_SIDE;
      opt[count++] = 1;
  
      direct[count] = RIGHT_SIDE;
      opt[count++] = 10;
    }
    else
    {
      direct[count] = FRONT_SIDE;    
      opt[count++] = 0;
    }
  }

   //Process plan
  for (int i = 0; i < count; i++)
  {
    Move(direct[i], opt[i]);
    while(!s_leftMoveDone || !s_rightMoveDone); //Wait task done
  }
}
