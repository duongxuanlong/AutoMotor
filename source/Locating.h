#ifndef __LOCATING_H__
#define __LOCATING_H__

#include "stdafx.h"

class Locating
{
  public:
    Locating();
    ~Locating();
    
  public:
    void Init();
    float GetRange(int direct);
    
  //Singleton
  private:
    static Locating* s_instance;
    
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

