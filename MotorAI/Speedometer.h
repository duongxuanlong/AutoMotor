#ifndef __SPEEDOMETER_H__
#define __SPEEDOMETER_H__

#include "stdafx.h"

#define DPR_RISING_FALLING_MODE 20
#define DPR_CHANGE_MODE (int)(DPR_RISING_FALLING_MODE * 2)

class Speedometer
{    
  private:
    int m_mode;
    
    static volatile int s_lTrack;
    static volatile int s_rTrack;
    static long s_RecTime;
    static bool s_bRecording;
    
  public:
    Speedometer();
    ~Speedometer();
    
  public:
    void Init(int mode);
    int GetCurrentDPR(); //dpr: drain per round
    
    void Start();
    void Stop();
    
    float GetLeftRPM();
    float GetRightRPM();

  private:
    static void LeftWheelTrack();
    static void RightWheelTrack();

  //Singleton
  private:
    static Speedometer* s_instance;
    
  public:
    static Speedometer* GetInstance()
    {
        if (!s_instance)
          s_instance = new Speedometer();
        return s_instance;
    }
};
#define SPEED Speedometer::GetInstance()

#endif //__SPEEDOMETER_H__


