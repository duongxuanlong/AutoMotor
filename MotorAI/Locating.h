#ifndef __LOCATING_H__
#define __LOCATING_H__

#include "stdafx.h"
#include <NewPing.h>

class Locating
{
  public:
    Locating();
    ~Locating();
    
  public:
    unsigned int m_Distances[3];
    void UpdateDistance();
    int GetDirection();
    
  public:
    void Init();
    unsigned int GetRange(int direct);
	  unsigned int GetMaxDistance();
    
  //Singleton
  private:
    static Locating* s_instance;
    int m_LastDirection;
	  unsigned int m_MaxDistance;
    NewPing* m_CenterSensor;
    NewPing* m_LeftSensor;
    NewPing* m_RightSensor;
    
  public:
    static Locating* GetInstance()
    {
        if (!s_instance)
          s_instance = new Locating();
        return s_instance;
    }
};
#define LOCATE Locating::GetInstance()

#endif //__LOCATING_H__


