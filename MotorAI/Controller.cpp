#include "Controller.h"

Controller* Controller::s_instance = NULL;

Controller::Controller():
m_lSpdRate(MOTOR_DEFAULT_SPD_RATE),
m_rSpdRate(MOTOR_DEFAULT_SPD_RATE),
m_eBalance(MOTOR_DEFAULT_ERROR_ACCEPTABLE_VALUE)
{ }

Controller::~Controller(){ }

void Controller::Init()
{
//  SPEED->Init(RISING);
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

  m_CurrentDirection = FRONT_SIDE;
  m_TurningBack = false;
  
  m_lSpdRate = MOTOR_MAX_SPD_RATE;
  m_rSpdRate = MOTOR_MAX_SPD_RATE;
  analogWrite(MOTOR_ENA, m_lSpdRate);
  analogWrite(MOTOR_ENB, m_rSpdRate);
}

void Controller::Balance()
{
  float lRPM = SPEED->GetLeftRPM();
  float rRPM = SPEED->GetRightRPM();
  DBG("%s" , "Balance: ");
  DBG("--->lRPM = %f", lRPM);
  DBG("--->rRPM = %f", rRPM);
  if(lRPM != 0 || rRPM != 0)
  {
    if(lRPM > rRPM)
    {
      m_eBalance = m_eBalance * (1 - m_eBalance);
      if(m_eBalance < MOTOR_MIN_ERROR_VALUE)
        m_eBalance = MOTOR_MIN_ERROR_VALUE;
      
      m_rSpdRate = MOTOR_MAX_SPD_RATE;
      m_lSpdRate = (int)(MOTOR_MAX_SPD_RATE * (rRPM / lRPM) * (1 - m_eBalance));
    }
    else if(lRPM < rRPM)
    {
      m_eBalance = m_eBalance * (1 - m_eBalance);
      if(m_eBalance < MOTOR_MIN_ERROR_VALUE)
        m_eBalance = MOTOR_MIN_ERROR_VALUE;
      
      m_lSpdRate = MOTOR_MAX_SPD_RATE;
      m_rSpdRate = (int)(MOTOR_MAX_SPD_RATE * (lRPM / rRPM) * (1 - m_eBalance));
    }
    else
    {
      m_eBalance = m_eBalance * (1  + m_eBalance);
    }
    
    analogWrite(MOTOR_ENA, m_lSpdRate);
    analogWrite(MOTOR_ENB, m_rSpdRate);
    DBG("--->m_lSpdRate = %d", m_lSpdRate);
    DBG("--->m_rSpdRate = %d", m_rSpdRate);
  }
}

void Controller::Stop()
{
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
}

int Controller::GetCurrentDirection()
{
	return m_CurrentDirection;
}

void Controller::SetDirection(int direction)
{
	m_CurrentDirection = direction;
}

void Controller::HandleTurnDirection(int left, int right)
{
	m_lSpdRate = left;
	m_rSpdRate = right;
	
	float distance = LOCATE->GetRange(FRONT_SIDE);
	if ((distance > LOCATE->GetMaxDistance() - DELTA_DISTANCE && 
		distance < LOCATE->GetMaxDistance() + DELTA_DISTANCE) || distance == MAX_DISTANCE)
		m_CurrentDirection = FRONT_SIDE;
}

void Controller::TurnForward()
{
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
  digitalWrite(MOTOR_IN3, LOW);
}

void Controller::TurnLeft()
{
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN4, HIGH);
  digitalWrite(MOTOR_IN3, LOW);
}

void Controller::TurnRight()
{
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
}

void Controller::TurnBack()
{
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
  digitalWrite(MOTOR_IN3, HIGH);
}

void Controller::RunningOnDirection()
{
  if (!m_TurningBack && LOCATE->GetRange(FRONT_SIDE) > MIN_DISTANCE)
  {
    TurnForward();
  }
  else
  {
//    Stop();
    TurnBack();
    unsigned int left = LOCATE->GetRange(LEFT_SIDE);
    unsigned int right = LOCATE->GetRange(RIGHT_SIDE);

    bool leftright = false;
    unsigned int maxdistance = 0;
    if (left > right)
    {
      leftright = true;
      maxdistance = left;
    }
    else
    {
      leftright = false;
      maxdistance = right;
    }

    if (maxdistance > MIN_DISTANCE)
    {
      m_TurningBack = false;
      Stop();
      if (leftright)
      {
        TurnLeft();
      }
      else
      {
        TurnRight();
      }
    }
    else
    {
      m_TurningBack = true;
      TurnBack();
    }
  }
//	analogWrite(MOTOR_ENA, m_lSpdRate);
//	analogWrite(MOTOR_ENB, m_rSpdRate);
}

void Controller::Move(int direct)
{
  switch(direct)
  {
    case LEFT_SIDE:
      SPEED->Stop();
      digitalWrite(MOTOR_IN1, HIGH);
      digitalWrite(MOTOR_IN4, HIGH);
    break;

    case RIGHT_SIDE:
      SPEED->Stop();
      digitalWrite(MOTOR_IN2, HIGH);
      digitalWrite(MOTOR_IN3, HIGH);
    break;
    
    case FRONT_SIDE:
      SPEED->Start();
      digitalWrite(MOTOR_IN2, HIGH);
      digitalWrite(MOTOR_IN4, HIGH);
    break;
    
    case BACK_SIDE:
      SPEED->Stop();
      digitalWrite(MOTOR_IN1, HIGH);
      digitalWrite(MOTOR_IN3, HIGH);
    break;
  }
  analogWrite(MOTOR_ENA, m_lSpdRate);
  analogWrite(MOTOR_ENB, m_rSpdRate);
}

