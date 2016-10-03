#ifndef __WHEEL_BALANCE_H__
#define __WHEEL_BALANCE_H__

#include "stdafx.h"
#include "Speedometer.h"

class WheelBalance
{
  public:
    WheelBalance();
    ~WheelBalance();
    
  private:
    long int m_lastUpdate;
    int m_Kp;
    int m_Kd;
    int m_Ki;

    float m_target;
    float m_lError;
    float m_rError;    
    
  public:
    void Init(const int Kp, const int Kd, const int Ki, const float spdTarget);
    void Update();

  //Singleton
  private:
    static WheelBalance* s_instance;
    
  public:
    static WheelBalance* GetInstance()
    {
        if (!s_instance)
          s_instance = new WheelBalance();
        return s_instance;
    }
};
#define WHEEL WheelBalance::GetInstance()

#endif //__PID_BALANCE_H__

