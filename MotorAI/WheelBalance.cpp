#include "WheelBalance.h"

WheelBalance* WheelBalance::s_instance = NULL;

WheelBalance::WheelBalance():
m_lastUpdate(0),
m_Kp(0),
m_Kd(0),
m_Ki(0),
m_lError(0),
m_rError(0),
m_target(0)
{ }

WheelBalance::~WheelBalance(){
}

void WheelBalance::Init(const int Kp, const int Kd, const int Ki, const float target){
  m_Kp = Kp;
  m_Kd = Kd;
  m_Ki = Ki;
  m_target = target;
  m_lastUpdate = millis();
}

void WheelBalance::Update(){
  if(millis() - m_lastUpdate == WHEEL_UPDATE_INTERNAL)
  {
    DBG("m_lastUpdate");
    float lSpd = SPEED->GetLeftRPM();
    float lError = m_target - lSpd;
    float lBal = m_Kp*lError + m_Kd*(lError - m_lError)*(1000/WHEEL_UPDATE_INTERNAL) + m_Ki*WHEEL_UPDATE_INTERNAL*lError/1000;
    m_lError = lError;
    DBG("lBal: %f", lBal);
    
    float rSpd = SPEED->GetRightRPM();
    float rError = m_target - rSpd;
    float rBal = m_Kp*rError + m_Kd*(rError - m_rError)*(1000/WHEEL_UPDATE_INTERNAL) + m_Ki*WHEEL_UPDATE_INTERNAL*rError/1000;
    m_rError = rError;
    DBG("rBal: %f", rBal);
    
    m_lastUpdate = millis();    
  }
}
