#ifndef __LOCATING_H__
#define __LOCATING_H__

#include "stdafx.h"

class Locating
{
  typedef struct locateInfo
  {
    NewPing* ping;
    unsigned int distance;
  } LOCATE_INFO;

  public:
    Locating();
    ~Locating();
    
  private:
    LOCATE_INFO m_info[TOTAL_SENSORS];
    
  public:
    void Init();    
    unsigned int GetRange(int direct);

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

