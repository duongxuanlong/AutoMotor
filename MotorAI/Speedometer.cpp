#include "Speedometer.h"

volatile int Speedometer::s_lTrack = 0;
volatile int Speedometer::s_rTrack = 0;
long Speedometer::s_RecTime = -1;
bool Speedometer::s_bRecording = false;

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

void Speedometer::LeftWheelTrack()
{
  if(s_bRecording)
  {
    s_lTrack++;
  }
}

void Speedometer::RightWheelTrack()
{
  if(s_bRecording)
  {
    s_rTrack++;
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

