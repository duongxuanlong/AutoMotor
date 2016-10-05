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
  if(opt > 0)
  {
      s_leftMoveDone = false;
      s_rightMoveDone = false;
  }
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
    unsigned int front = LOCATE->GetRange(FRONT_SIDE);
    if (front > MIN_FRONT_DISTANCE)
    {
      Move(FRONT_SIDE);
    }
    else
    {
      unsigned int left = LOCATE->GetRange(LEFT_SIDE);
      unsigned int right = LOCATE->GetRange(RIGHT_SIDE);
      if (front <= LIMITED_SIDES && left <= LIMITED_SIDES && right <= LIMITED_SIDES)
      {
        Move(BACK_SIDE);
        return;
      }
  
      if (front > left && front > right)
      {
        Move(FRONT_SIDE);
        return;
      }
      //Move(BACK_SIDE);
      if (front <= LIMITED_SIDES || left <= LIMITED_SIDES || right <= LIMITED_SIDES)
      {
        Move(BACK_SIDE);
        delay(LIMITED_MINOR_TIME);
      }
      if (left > right)
      {
        //      unsigned int current_time = millis();
        //      while (millis() - current_time < LIMITED_TIME)
        Move(LEFT_SIDE);
        delay(LIMITED_TIME);
        Move(FRONT_SIDE);
      }
      else
      {
        //      unsigned int current_time = millis();
        //      while (millis() - current_time < LIMITED_TIME)
        Move(RIGHT_SIDE);
        delay(LIMITED_TIME);
        Move(FRONT_SIDE);
      }
    }
}

/*******************************************************************/
/*******************************************************************/
/*******************************************************************/

void Controller::AutoRun2()
{
  unsigned int front = LOCATE->GetRange(FRONT_SIDE);
  unsigned int left = LOCATE->GetRange(LEFT_SIDE);
  unsigned int right = LOCATE->GetRange(RIGHT_SIDE);

  int direct[50];
  int opt[50];
  int count = 0;

  //Planning
  if (front < 25) //Decision for new direction need to be made here
  {
    if (front < 10) //stuck front, should back then choose new direction
    {
      direct[count] = BACK_SIDE;
      opt[count++] = 10 - front;
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
    else if (left <= 5 && right >= 5) //It run too close to the left, need move to right for awhile
    {
      direct[count] = RIGHT_SIDE;
      opt[count++] = 20;
  
      direct[count] = FRONT_SIDE;
      opt[count++] = 6 - left;
  
      direct[count] = LEFT_SIDE;
      opt[count++] = 10;
    }
    else if (right <= 5 && left >= 5) //It run too close to the right, need move to left for awhile
    {
      direct[count] = LEFT_SIDE;
      opt[count++] = 20;
  
      direct[count] = FRONT_SIDE;
      opt[count++] = 6 - left;
  
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
