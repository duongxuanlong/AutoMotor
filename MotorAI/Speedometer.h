#ifndef __SPEEDOMETER_H__
#define __SPEEDOMETER_H__

#include "stdafx.h"

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
    void Start();
    void Stop();
    
    float GetLeftRPM(); //rpm: round per ms
    float GetRightRPM();

  private:
    int GetCurrentDPR(); //dpr: drain per round
    
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

