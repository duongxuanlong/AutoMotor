#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "stdafx.h"
#include "Locating.h"
#include "Speedometer.h"

class Controller
{
  private:
    int m_lSpdRate;
    int m_rSpdRate;
    float m_eBalance;
    
  public:
    Controller();
    ~Controller();
    
  public:
    void Init();
    void Stop();
    void Move(int direct);
    
    void Balance();

  public:
    int GetCurrentDirection();
	void SetDirection(int direction);
	void RunningOnDirection();
  private:
    int m_CurrentDirection;
    bool m_TurningBack;
    void TurnForward();
    void TurnLeft();
    void TurnRight();
    void TurnBack();
	void HandleTurnDirection(int left, int right);
    
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

