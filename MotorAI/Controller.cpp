#include "Controller.h"

Controller* Controller::s_instance = NULL;

Controller::Controller()
{ }

Controller::~Controller() { }

void Controller::Init()
{
  DBG("Controller::Init()");
  SPEED->Init(CHANGE);
  LOCATE->Init();

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

void SetSpeed(unsigned int lSpdRate, unsigned int rSpdRate)
{
  analogWrite(MOTOR_ENA, lSpdRate);
  analogWrite(MOTOR_ENB, rSpdRate);
}

void Controller::Move(int direct, unsigned int angle)
{
  DBG("Controller::Move(direct = %d, angle = %d)", direct, angle);
  switch (direct)
  {
    case FRONT_SIDE:
      SPEED->Start();
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, HIGH);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, HIGH);
      break;

    case LEFT_SIDE:
      SPEED->Stop();
      //SPEED->SetAngle(angle);
      digitalWrite(MOTOR_IN1, HIGH);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, HIGH);
      break;

    case RIGHT_SIDE:
      SPEED->Stop();
      //SPEED->SetAngle(angle);
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, HIGH);
      digitalWrite(MOTOR_IN3, HIGH);
      digitalWrite(MOTOR_IN4, LOW);
      break;

    case BACK_SIDE:
      SPEED->Stop();
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

