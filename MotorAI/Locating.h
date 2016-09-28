#ifndef __LOCATING_H__
#define __LOCATING_H__

#include "stdafx.h"

class Locating
{
  public:
    Locating();
    ~Locating();
    
  public:
    float m_Distances[3];
    void UpdateDistance();
    int GetDirection();
    
  public:
    void Init();
    float GetRange(int direct);
	float GetMaxDistance();
    
  //Singleton
  private:
    static Locating* s_instance;
    int m_LastDirection;
	float m_MaxDistance;
    
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

