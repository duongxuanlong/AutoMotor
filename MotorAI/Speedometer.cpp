#include "Speedometer.h"

volatile int Speedometer::s_lTrack = 0;
volatile int Speedometer::s_rTrack = 0;
volatile long Speedometer::s_RecTime = -1;
volatile bool Speedometer::s_bRecording = false;
volatile int Speedometer::s_lLimit = 0;
volatile int Speedometer::s_rLimit = 0;

volatile ReachLimitCallback Speedometer::s_pLeftLimitCB = NULL;
volatile ReachLimitCallback Speedometer::s_pRightLimitCB = NULL;
    
Speedometer* Speedometer::s_instance = NULL;

Speedometer::Speedometer():
m_mode(RISING)
{ }

Speedometer::~Speedometer(){ }

void Speedometer::Init(int mode)
{
  DBG("Speedometer::Init(mode = %d)", mode);
  m_mode = mode;
  
  pinMode(L_SPD_TRACK, INPUT_PULLUP);
  pinMode(R_SPD_TRACK, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(L_SPD_TRACK), Speedometer::LeftWheelTrack, m_mode);
  attachInterrupt(digitalPinToInterrupt(R_SPD_TRACK), Speedometer::RightWheelTrack, m_mode);
}

void Speedometer::Start()
{
  DBG("Speedometer::Start()");
  if (!s_bRecording)
  {
    s_lTrack = 0;
    s_rTrack = 0;
    s_RecTime = millis();
    s_bRecording = true;
  }
}

void Speedometer::Stop()
{
  DBG("Speedometer::Stop()");
  if (s_bRecording)
  {
    s_RecTime = -1;
    s_bRecording = false;
    s_lTrack = 0;
    s_rTrack = 0;
  }
}

void Speedometer::SetLeftLimit(unsigned int angle)
{
  if(angle <= 0 || angle >= 360) return; //meaningless
  s_lLimit = (int)((angle * SPEED->GetCurrentDPR()) / LEFT_LIMIT_WHEEL_ANGLE_PER_ROUND);
}

void Speedometer::SetRightLimit(unsigned int angle)
{
  if(angle <= 0 || angle >= 360) return; //meaningless
  s_rLimit = (int)((angle * SPEED->GetCurrentDPR()) / RIGHT_LIMIT_WHEEL_ANGLE_PER_ROUND);
}

void Speedometer::SetBackLimit(unsigned int duration)
{
  if(duration <= 0) return; //meaningless
  s_lLimit = duration * SPEED->GetCurrentDPR();
  s_rLimit = duration * SPEED->GetCurrentDPR();
}

void Speedometer::SetFrontLimit(unsigned int duration)
{
  if(duration <= 0) return; //meaningless
  s_lLimit = duration * SPEED->GetCurrentDPR();
  s_rLimit = duration * SPEED->GetCurrentDPR();
}

void Speedometer::SetLeftReachLimitCallback(ReachLimitCallback cb)
{
  if(s_pLeftLimitCB == NULL)
    s_pLeftLimitCB = cb;
}

void Speedometer::SetRightReachLimitCallback(ReachLimitCallback cb)
{
  if(s_pRightLimitCB == NULL)
    s_pRightLimitCB = cb;
}

void Speedometer::LeftWheelTrack()
{
  if(s_bRecording)
  {
    s_lTrack++;
  }

  if(s_lLimit >= 0)
  {
    s_lLimit--;
    if(s_lLimit < 0)
    {
      if(s_pLeftLimitCB != NULL)
      {
        s_pLeftLimitCB();
      }
    }
  }
}

void Speedometer::RightWheelTrack()
{
  if(s_bRecording)
  {
    s_rTrack++;
  }
  
  if(s_rLimit >= 0)
  {
    s_rLimit--;
    if(s_rLimit < 0)
    {
      if(s_pRightLimitCB != NULL)
      {
        s_pRightLimitCB();
      }
    }
  }
}

float Speedometer::GetLeftRPM()
{
  if(s_RecTime < 0)
    return 0;
  float dpr = GetCurrentDPR();
  float timez = (millis() - s_RecTime) / 1000;
  return (s_lTrack / dpr) / timez;
}

float Speedometer::GetRightRPM()
{
  if(s_RecTime < 0)
    return 0;
  float dpr = GetCurrentDPR();
  float timez = (millis() - s_RecTime) / 1000;
  return (s_rTrack / dpr) / timez;
}

int Speedometer::GetCurrentDPR()
{
  switch(m_mode)
  {
    case RISING:
    case FALLING:
      return DPR_RISING_FALLING_MODE;
    case CHANGE:
      return DPR_CHANGE_MODE;
    default:
      return 0;
  }  
}

