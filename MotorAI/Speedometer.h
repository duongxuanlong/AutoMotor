#ifndef __SPEEDOMETER_H__
#define __SPEEDOMETER_H__

#include "stdafx.h"

typedef void (*ReachLimitCallback) ();

class Speedometer
{    
  private:
    int m_mode;
    
    static volatile int s_lTrack;
    static volatile int s_rTrack;
    static volatile long s_RecTime;
    static volatile bool s_bRecording;
    static volatile int s_lLimit;
    static volatile int s_rLimit;

    static volatile ReachLimitCallback s_pLeftLimitCB;
    static volatile ReachLimitCallback s_pRightLimitCB;
  public:
    Speedometer();
    ~Speedometer();
    
  public:
    void Init(int mode);
    void Start();
    void Stop();
    
    float GetLeftRPM(); //rpm: round per ms
    float GetRightRPM();
    
    static void SetLeftLimit(unsigned int angle);
    static void SetRightLimit(unsigned int angle);
    static void SetLeftReachLimitCallback(ReachLimitCallback cb);
    static void SetRightReachLimitCallback(ReachLimitCallback cb);

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

