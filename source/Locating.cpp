#include "Locating.h"

Locating* Locating::s_instance = NULL;

Locating::Locating(){ }

Locating::~Locating(){ }

void Locating::Init()
{
  pinMode(L_EYE_TRIG, OUTPUT);
  pinMode(L_EYE_ECHO, INPUT);
  pinMode(F_EYE_TRIG, OUTPUT);
  pinMode(F_EYE_ECHO, INPUT);
  pinMode(R_EYE_TRIG, OUTPUT);
  pinMode(R_EYE_ECHO, INPUT);
  pinMode(R_EYE_TRIG, OUTPUT);
  pinMode(R_EYE_ECHO, INPUT);
}

float Locating::GetRange(int direct)
{
  int trig = -1;
  int echo = -1;
  
  switch(direct)
  {
    case LEFT_SIDE:
      trig = L_EYE_TRIG;      
      echo = L_EYE_ECHO;
    break;
    
    case RIGHT_SIDE:
      trig = R_EYE_TRIG;      
      echo = R_EYE_ECHO;
    break;
    
    case FRONT_SIDE:
      trig = F_EYE_TRIG;      
      echo = F_EYE_ECHO;
    break;
    
    default:
      DBG("%s" , "Seriously -_-!");
      return 0;
    break;
  }
  
  digitalWrite(trig, LOW);
  delayMicroseconds(1);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);
  
  return pulseIn(echo, HIGH) / 29.1f / 2;
}

