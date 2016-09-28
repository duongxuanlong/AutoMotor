#include "Locating.h"

Locating* Locating::s_instance = NULL;

Locating::Locating(){ }

Locating::~Locating(){ }

void Locating::Init()
{
  pinMode(L_EYE_TRIG, OUTPUT);
  pinMode(L_EYE_ECHO, INPUT);
  pinMode(F_EYE_TRIG, OUTPUT);
  pinMode(F_EYE_ECHO, INPUT);
  pinMode(R_EYE_TRIG, OUTPUT);
  pinMode(R_EYE_ECHO, INPUT);
  pinMode(R_EYE_TRIG, OUTPUT);
  pinMode(R_EYE_ECHO, INPUT);

  m_LastDirection = 0;
  for (int i = 0; i < SENSORS; i++)
    m_Distances[i] = 0;
}

void Locating::UpdateDistance()
{
  for (int i = 0; i < SENSORS; i++)
  {
    m_Distances[i] = GetRange(i);
    Serial.print("Update Distance: Direction: ");
    Serial.println(i);
    Serial.print("Update Distance: distance: ");
    Serial.println(m_Distances[i]);
  }
}

int Locating::GetDirection()
{
  // if (m_LastDirection != 0)
  // {
    // m_LastDirection = 0;
    // return m_LastDirection;
  // }
  // int direction = 0;
  // float max = m_Distances[0];
  m_LastDirection = 0;
  m_MaxDistance = m_Distances[0];
  for (int i = 1; i < SENSORS; i++)
    if (m_Distances[i] > m_MaxDistance)
    {
      m_MaxDistance = m_Distances[i];
      m_LastDirection = i;
    }
  Serial.print("Direction: ");
  Serial.println(m_LastDirection);
  Serial.print("Distance: ");
  Serial.println(m_MaxDistance);
  return m_LastDirection;
}

float Locating::GetMaxDistance()
{
	return m_MaxDistance;
}

float Locating::GetRange(int direct)
{
  int trig = -1;
  int echo = -1;
  
  switch(direct)
  {
    case LEFT_SIDE:
      trig = L_EYE_TRIG;      
      echo = L_EYE_ECHO;
      Serial.print("LEft: ");
    break;
    
    case RIGHT_SIDE:
      trig = R_EYE_TRIG;      
      echo = R_EYE_ECHO;
      Serial.print("Right: ");
    break;
    
    case FRONT_SIDE:
      trig = F_EYE_TRIG;      
      echo = F_EYE_ECHO;
      Serial.print("front: ");
    break;
    
    default:
      DBG("%s" , "Seriously -_-!");
      return 0;
    break;
  }
  
  digitalWrite(trig, LOW);
  delayMicroseconds(1);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);

 // Serial.println(pulseIn(echo, HIGH) / 29.1f / 2);
  return pulseIn(echo, HIGH) / 29.1f / 2;
}

