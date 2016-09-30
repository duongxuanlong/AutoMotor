#include "Locating.h"

Locating* Locating::s_instance = NULL;

Locating::Locating(){ }

Locating::~Locating(){
  if (m_CenterSensor != NULL)
    delete m_CenterSensor;

  if (m_LeftSensor != NULL)
    delete m_LeftSensor;

  if (m_RightSensor != NULL)
    delete m_RightSensor;
}

void Locating::Init()
{
  pinMode(L_EYE_TRIG, OUTPUT);
  pinMode(L_EYE_ECHO, INPUT);
  pinMode(F_EYE_TRIG, OUTPUT);
  pinMode(F_EYE_ECHO, INPUT);
  pinMode(R_EYE_TRIG, OUTPUT);
  pinMode(R_EYE_ECHO, INPUT);
//  pinMode(R_EYE_TRIG, OUTPUT);
//  pinMode(R_EYE_ECHO, INPUT);

  m_LastDirection = 0;
  for (int i = 0; i < SENSORS; i++)
    m_Distances[i] = 0;

  m_CenterSensor = new NewPing(F_EYE_TRIG, F_EYE_ECHO, MAX_DISTANCE);
  m_LeftSensor = new NewPing(L_EYE_TRIG, L_EYE_ECHO, MAX_DISTANCE);
  m_RightSensor = new NewPing(R_EYE_TRIG, R_EYE_ECHO, MAX_DISTANCE);
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
  if (m_MaxDistance <= MIN_FRONT_DISTANCE)
    m_LastDirection = BACK_SIDE;
  return m_LastDirection;
}

unsigned int Locating::GetMaxDistance()
{
	return m_MaxDistance;
}

unsigned int Locating::GetRange(int direct)
{
  Serial.print("Run mode: ");
  Serial.println(direct);
  unsigned int result = 0;
  switch (direct)
  {
    case FRONT_SIDE:
      delay(10); 
      result = m_CenterSensor->ping_cm();
      m_CenterSensor->timer_stop();
      Serial.println(result);
    break;
    
    case LEFT_SIDE:
      delay(10);
      result = m_LeftSensor->ping_cm();
      m_LeftSensor->timer_stop();
    break;
    
    case RIGHT_SIDE:
      delay(10);
      result = m_RightSensor->ping_cm();
      m_RightSensor->timer_stop();
    break;
    
    default:
      result = MIN_FRONT_DISTANCE;
  }
  return result;
//  int trig = -1;
//  int echo = -1;
//  
//  switch(direct)
//  {
//    case LEFT_SIDE:
//      trig = L_EYE_TRIG;      
//      echo = L_EYE_ECHO;
//      Serial.print("LEft: ");
//    break;
//    
//    case RIGHT_SIDE:
//      trig = R_EYE_TRIG;      
//      echo = R_EYE_ECHO;
//      Serial.print("Right: ");
//    break;
//    
//    case FRONT_SIDE:
//      trig = F_EYE_TRIG;      
//      echo = F_EYE_ECHO;
//      Serial.print("front: ");
//    break;
//    
//    default:
//      DBG("%s" , "Seriously -_-!");
//      return 0;
//    break;
//  }
//  
//  digitalWrite(trig, LOW);
//  delayMicroseconds(1);
//  digitalWrite(trig, HIGH);
//  delayMicroseconds(5);
//  digitalWrite(trig, LOW);
//
// // Serial.println(pulseIn(echo, HIGH) / 29.1f / 2);
//  return pulseIn(echo, HIGH) / 29.1f / 2;
}


