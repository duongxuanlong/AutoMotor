#include "Locating.h"

Locating* Locating::s_instance = NULL;

Locating::Locating()
{ }

Locating::~Locating(){  
  for (int i = 0; i < TOTAL_SENSORS; i++)
  {
    delete[] m_info[i].ping;
  }
}

void Locating::Init()
{
  DBG("Locating::Init()");
  pinMode(L_EYE_TRIG, OUTPUT);
  pinMode(L_EYE_ECHO, INPUT);
  pinMode(F_EYE_TRIG, OUTPUT);
  pinMode(F_EYE_ECHO, INPUT);
  pinMode(R_EYE_TRIG, OUTPUT);
  pinMode(R_EYE_ECHO, INPUT);

  for (int i = 0; i < TOTAL_SENSORS; i++)
  {
    switch(i)
	  {
  		case FRONT_SIDE:
  			m_info[i].ping = new NewPing(F_EYE_TRIG, F_EYE_ECHO, MAX_DISTANCE);
          break;
  		case LEFT_SIDE:	
  			m_info[i].ping = new NewPing(L_EYE_TRIG, L_EYE_ECHO, MAX_DISTANCE);
          break;
  		case RIGHT_SIDE:
  			m_info[i].ping = new NewPing(R_EYE_TRIG, R_EYE_ECHO, MAX_DISTANCE);
          break;
    }
	  m_info[i].distance = 0;
  }
}

unsigned int Locating::GetRange(int direct, unsigned int check)
{
  unsigned int range = 0;
  unsigned int count = check;
  do
  {
    range = GetRange(direct);
    count--;
  } while (range == 0 && count > 0);

  return range;
}

unsigned int Locating::GetRange(int direct)
{
  DBG("Locating::GetRange(direct = %d)", direct);
  unsigned int result = 0;
  switch (direct)
  {
    case FRONT_SIDE:
    case LEFT_SIDE:
    case RIGHT_SIDE:
      delay(LOCATING_PING_INTERVAL); 
      result = m_info[direct].ping->ping_cm();
      m_info[direct].ping->timer_stop();
    break;
    
    default:
      result = MIN_FRONT_DISTANCE;
  }
  return result;
}

