#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "stdafx.h"
#include "Locating.h"
#include "Speedometer.h"
#include "WheelBalance.h"

class Controller
{
  public:
    Controller();
    ~Controller();
    
  public:
    void Init();
    void Stop();
    void Move(int direct, unsigned int angle = 0);
    void SetSpeed(unsigned int lSpdRate, unsigned int rSpdRate);
    
    void AutoRun();
    
  //Singleton
  private:
    static Controller* s_instance;
    
  public:
    static Controller* GetInstance()
    {
        if (!s_instance)
          s_instance = new Controller();
        return s_instance;
    }
};
#define CONTROL Controller::GetInstance()

#endif //__CONTROLLER_H__

