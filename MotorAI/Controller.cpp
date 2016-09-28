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
  SPEED->Init(RISING);
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
	if (distance > LOCATE->GetMaxDistance() - DELTA_DISTANCE && 
		distance < LOCATE->GetMaxDistance() + DELTA_DISTANCE)
		m_CurrentDirection = FRONT_SIDE;
}

void Controller::RunningOnDirection()
{
	switch (m_CurrentDirection)
	{
		case FRONT_SIDE:
			digitalWrite(MOTOR_IN2, HIGH);
			digitalWrite(MOTOR_IN4, HIGH);
			m_lSpdRate = MOTOR_MAX_SPD_RATE;
			m_rSpdRate = MOTOR_MAX_SPD_RATE;
		break;
		
		case LEFT_SIDE:
			digitalWrite(MOTOR_IN1, HIGH);
			digitalWrite(MOTOR_IN4, HIGH);
			HandleTurnDirection(0, MOTOR_MAX_SPD_RATE / 3);
			// m_lSpdRate = 0;
			// m_rSpdRate = MOTOR_MAX_SPD_RATE / 3;
			// int new direction 
		break;
		
		case RIGHT_SIDE:
		    digitalWrite(MOTOR_IN2, HIGH);
			digitalWrite(MOTOR_IN3, HIGH);
			HandleTurnDirection(MOTOR_MAX_SPD_RATE / 3, 0);
		break;
		
		default:
			digitalWrite(MOTOR_IN1, HIGH);
			digitalWrite(MOTOR_IN3, HIGH);
			m_lSpdRate = MOTOR_MAX_SPD_RATE / 3;
			m_rSpdRate = MOTOR_MAX_SPD_RATE / 3;
		
	}
	analogWrite(MOTOR_ENA, m_lSpdRate);
	analogWrite(MOTOR_ENB, m_rSpdRate);
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

