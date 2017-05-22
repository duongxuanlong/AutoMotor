

//#include <Wire.h>

#include "Controller.h"

//---------------------------------------------------------------Load library for LCD-------------------------------------------------------------//
#ifdef USE_LCD

#include <Wire.h>
//#include <LCD.h>
//#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_I2C.h>

//#define I2C_ADDR 0x3F //Define I2C Address where the PCF8574A is
//#define BACKLIGHT_PIN 3
//#define En_pin 2
//#define Rw_pin 1
//#define Rs_pin 0
//#define D4_pin 4
//#define D5_pin 5
//#define D6_pin 6
//#define D7_pin 7

LiquidCrystal_I2C lcd(0x27,16,2);
#endif
//---------------------------------------------------------------End for LCD-------------------------------------------------------------//

Controller* Controller::s_instance = NULL;

Controller::Controller()
{ }

Controller::~Controller() 
{ 
  #ifdef USE_LCD
//    if (lcd != NULL)
//      delete lcd;
  #endif
}

void Controller::Init()
{
  DBG("Controller::Init()");
  //SPEED->Init(CHANGE);
  LOCATE->Init();

  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  pinMode(MOTOR_ENB, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);

  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);

//  analogWrite(MOTOR_ENA, MOTOR_MAX_SPD_RATE);
//  analogWrite(MOTOR_ENB, MOTOR_MAX_SPD_RATE);

  analogWrite(MOTOR_ENA, MOTOR_DEFAULT_SPD_RATE);
  //analogWrite(MOTOR_ENB, MOTOR_DEFAULT_SPD_RATE);
  analogWrite(MOTOR_ENB, MOTOR_DEFAULT_SPD_RATE_LEFT);
  //  Move(FRONT_SIDE);

  #ifdef USE_LCD
    //lcd = new LiquidCrystal_I2C (I2C_ADDR, En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
//    lcd.begin (16,2);
//    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
//    lcd.setBacklight(HIGH);

      lcd.init();       //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình
      lcd.backlight();
  #endif

  isMove = false;

  IsStopping = false;
}

void Controller::TestSensorOutput()
{
  //Test front sensor
  int front = LOCATE->GetRange(FRONT_SIDE);
  DBG("Front sensor : %u", front);
  #ifdef USE_LCD
    lcd.setCursor(0,0);
    lcd.print("Front sensor");
    lcd.setCursor(0,1);
    lcd.print(front, DEC);
  #endif
  delay(500);
  #ifdef USE_LCD
    lcd.clear();
  #endif

  int left = LOCATE->GetRange(LEFT_SIDE);
  DBG("Left sensor : %u", left);
  #ifdef USE_LCD
    lcd.setCursor(0,0);
    lcd.print("Left sensor");
    lcd.setCursor(0,1);
    lcd.print(left, DEC);
  #endif
  delay(500);
  #ifdef USE_LCD
    lcd.clear();
  #endif

  int right = LOCATE->GetRange(RIGHT_SIDE);
  DBG("Right sensor : %u", right);
  #ifdef USE_LCD
    lcd.setCursor(0,0);
    lcd.print("Right sensor");
    lcd.setCursor(0,1);
    lcd.print(right, DEC);
  #endif
  delay(500);
  #ifdef USE_LCD
    lcd.clear();
  #endif
}

void Controller::TestWheel()
{
  //Left Wheel ENB
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);

  //Right Wheel ENA
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
}

void Controller::SetSpeed(unsigned int lSpdRate, unsigned int rSpdRate)
{
  analogWrite(MOTOR_ENA, lSpdRate);
  analogWrite(MOTOR_ENB, rSpdRate);
}

void Controller::StopSpeed()
{
  IsStopping = true;
  SetSpeed(0, 0);
}

void Controller::RecoverSpeed()
{
  IsStopping = false;
  //SetSpeed(MOTOR_DEFAULT_SPD_RATE, MOTOR_DEFAULT_SPD_RATE);
  SetSpeed(MOTOR_DEFAULT_SPD_RATE, MOTOR_DEFAULT_SPD_RATE_LEFT);
}

void Controller::Move(int direct, unsigned int angle)
{
//  StopSpeed(); //for debug
  DBG("Controller::Move(direct = %d, angle = %d)", direct, angle);
  switch (direct)
  {
    case FRONT_SIDE:
      SPEED->Start();
      //Right wheel
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, HIGH);
      //Left wheel
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, HIGH);
      break;

    case LEFT_SIDE:
      SPEED->Stop();
      //SPEED->SetAngle(angle);
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, HIGH);
      digitalWrite(MOTOR_IN3, HIGH);
      digitalWrite(MOTOR_IN4, LOW);
      
//      digitalWrite(MOTOR_IN1, HIGH);
//      digitalWrite(MOTOR_IN2, LOW);
//      digitalWrite(MOTOR_IN3, LOW);
//      digitalWrite(MOTOR_IN4, HIGH);
      break;

    case RIGHT_SIDE:
      SPEED->Stop();
      //SPEED->SetAngle(angle);
      digitalWrite(MOTOR_IN1, HIGH);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, HIGH);
      
//      digitalWrite(MOTOR_IN1, LOW);
//      digitalWrite(MOTOR_IN2, HIGH);
//      digitalWrite(MOTOR_IN3, HIGH);
//      digitalWrite(MOTOR_IN4, LOW);
      break;

    case BACK_SIDE:
      SPEED->Stop();
      digitalWrite(MOTOR_IN1, HIGH);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, HIGH);
      digitalWrite(MOTOR_IN4, LOW);
      break;
  }
}

void Controller::Stop()
{
  DBG("Controller::Stop()");
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
}

void Controller::ShouldMoveFront(int delaytime = LIMITED_TIME)
{
//  int count = 0;
//  unsigned int front = 0;
//  while (front < LIMITED_SIDES && count < 3)
//    front = LOCATE->GetRange(FRONT_SIDE);
//  if (front >= LIMITED_SIDES)
//  {
//    RecoverSpeed();
//    Move(FRONT_SIDE);
//    delay(LIMITED_TIME + LIMITED_TIME + LIMITED_TIME);
//  }

  RecoverSpeed();
  Move(FRONT_SIDE);
  delay(delaytime);
}

void Controller::AutoRun()
{
  ////////////////////////////////////////////////////////////////////////Test Move/////////////////////////////////////////////////////////////////////////////////
  #ifdef USE_LCD
    lcd.setCursor(0,0);
    lcd.print("isMove: ");
    lcd.setCursor(0, 1);
    lcd.print(isMove, DEC);
    delay(1000);
    lcd.clear();
  #endif
//    isMove = true;
//  Move(LEFT_SIDE);
//  delay(50 + 50);
//  Move(FRONT_SIDE);
//  delay(50);
//  Move(RIGHT_SIDE);
//  delay(50);
//  Move(FRONT_SIDE);
//  delay(50);
//  Move(RIGHT_SIDE);
//  delay(50);
//  Move(FRONT_SIDE);
//  delay(50);
//  Move(LEFT_SIDE);
//  delay(250);
//  //Stop();
//  SetSpeed(0, 0);
//  Move(RIGHT_SIDE);
//  Move(BACK_SIDE);
//  Move(LEFT_SIDE);
//  delay(LIMITED_TIME);
//  StopSpeed();
  //ShouldMoveFront();
//  Move(FRONT_SIDE);
//  delay(1000);
//  SetSpeed(0, 0);
//  Move(FRONT_SIDE);
//  RecoverSpeed();
//  delay(500);
//  StopSpeed();
//  if (isMove)
//      return;
  ////////////////////////////////////////////////////////////////////////End Test/////////////////////////////////////////////////////////////////////////////////

   ////////////////////////////////////////////////////////////////////////Test Sensor/////////////////////////////////////////////////////////////////////////////////
//     TestSensorOutput();
//     return;
  ////////////////////////////////////////////////////////////////////////End Test/////////////////////////////////////////////////////////////////////////////////

  //delay(1000);
  unsigned int front = 0;
  unsigned int left = 0; 
  unsigned int right = 0;

  int count = 0;
//  while ((front < LIMITED_SIDES && left < LIMITED_SIDES && right < LIMITED_SIDES) && (count < 3))
  while (count < 3)
  {
    front = LOCATE->GetRange(FRONT_SIDE);
    left = LOCATE->GetRange(LEFT_SIDE);
    right = LOCATE->GetRange(RIGHT_SIDE);
    count++;
  }

  #ifdef USE_LCD
    lcd.setCursor(0,0);
    lcd.print("Front distance ");
    lcd.setCursor(0,1);
    lcd.print(front, DEC);
    delay(1000);
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Left distance ");
    lcd.setCursor(0,1);
    lcd.print(left, DEC);
    delay(1000);
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("right distance ");
    lcd.setCursor(0,1);
    lcd.print(right, DEC);
    delay(1000);
    lcd.clear();

//    return;
  #endif

  /*******************************************************New idea**************************************************/

  ////////////////////////////////////////////////////Handle 3 options//////////////////////////////////////////////
  if ((front > MIN_SIDE_DISTANCE || front == 0) && (left > MIN_SIDE_DISTANCE || left == 0) && (right > MIN_SIDE_DISTANCE || right == 0))
  {
    #ifdef USE_LCD
      lcd.setCursor(0, 0);
      lcd.print("Front, Left, Right");
      lcd.setCursor(0, 1);
      lcd.print("MIN_SIDE_DISTANCE");
      delay(1000);
      lcd.clear();
    #endif
    
    //Handle both left and right
    if ((left >= LIMITED_SIDES || left == 0) && (right >= LIMITED_SIDES || right == 0))
    {
      #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("Left, Right");
        lcd.setCursor(0, 1);
        lcd.print("LIMITED_SIDES");
        delay(1000);
        lcd.clear();
      #endif
      
      if ((left <= right && left != 0) || left == 0)
      {
        #ifdef USE_LCD
          lcd.setCursor(0, 0);
          lcd.print("Left, Right");
          lcd.setCursor(0, 1);
          lcd.print("left <= right || 0");
          delay(1000);
          lcd.clear();
        #endif
      
        Move(LEFT_SIDE);
        delay(LIMITED_TIME);
//        delay(LIMITED_TIME_SMALL);
//        StopSpeed();
//        int subcount = 0;
//        while (subcount < 3)
//        {
//          front = LOCATE->GetRange(FRONT_SIDE);
//          subcount++;
//        }
//
//        if (front >= LIMITED_SIDES_DOUBLE || front == 0)
//        {
//          RecoverSpeed();
//          ShouldMoveFront();
//        }
//        else
//        {
//          RecoverSpeed();
//          Move(LEFT_SIDE);
//          delay(LIMITED_TIME_SMALL);
//          ShouldMoveFront();
//        }

        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      }
      else
      {
        
       #ifdef USE_LCD
          lcd.setCursor(0, 0);
          lcd.print("Left, Right");
          lcd.setCursor(0, 1);
          lcd.print("left > right || 0");
          delay(1000);
          lcd.clear();
        #endif
        
        Move(RIGHT_SIDE);
        delay(LIMITED_TIME);
//        delay(LIMITED_TIME_SMALL);
//        StopSpeed();
//        int subcount = 0;
//        while (subcount < 3)
//        {
//          front = LOCATE->GetRange(FRONT_SIDE);
//          subcount++;
//        }
//
//        if (front >= LIMITED_SIDES_DOUBLE || front == 0)
//        {
//          RecoverSpeed();
//          ShouldMoveFront();
//        }
//        else
//        {
//          RecoverSpeed();
//          Move(RIGHT_SIDE);
//          delay(LIMITED_TIME_SMALL);
//          ShouldMoveFront();
//        }
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      }
      //ShouldMoveFront();
      return;
    }
    //End handle both left and right

    //Handle only left
    if (left >= LIMITED_SIDES || left == 0)
    {
       #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("Left");
        lcd.setCursor(0, 1);
        lcd.print("LIMITED_SIDES || 0");
        delay(1000);
        lcd.clear();
      #endif
      
      Move(LEFT_SIDE);
      delay(LIMITED_TIME);
//      delay(LIMITED_TIME_SMALL);
//      StopSpeed();
//      int subcount = 0;
//      while (subcount < 3)
//      {
//        front = LOCATE->GetRange(FRONT_SIDE);
//        subcount++;
//      }
//
//      if (front >= LIMITED_SIDES_DOUBLE || front == 0)
//      {
//        RecoverSpeed();
//        ShouldMoveFront();
//      }
//      else
//      {
//        RecoverSpeed();
//        Move(LEFT_SIDE);
//        delay(LIMITED_TIME_SMALL);
//        ShouldMoveFront();
//      }
      ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      return;
    }
    //End handle only left

    //Handle only right
    if (right >= LIMITED_SIDES || right == 0)
    {
      #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("right");
        lcd.setCursor(0, 1);
        lcd.print("LIMITED_SIDES || 0");
        delay(1000);
        lcd.clear();
      #endif
      
      Move(RIGHT_SIDE);
      delay(LIMITED_TIME);
//      delay(LIMITED_TIME_SMALL);
//      StopSpeed();
//      int subcount = 0;
//      while (subcount < 3)
//      {
//        front = LOCATE->GetRange(FRONT_SIDE);
//        subcount++;
//      }
//
//      if (front >= LIMITED_SIDES_DOUBLE || front == 0)
//      {
//        RecoverSpeed();
//        ShouldMoveFront();
//      }
//      else
//      {
//        RecoverSpeed();
//        Move(RIGHT_SIDE);
//        delay(LIMITED_TIME_SMALL);
//        ShouldMoveFront();
//      }
      ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      return;
    }
    //End hanlde only right
    
    //Hanndle front
    #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("Front");
        delay(1000);
        lcd.clear();
      #endif
      
    ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
    //End handle front
    return;
  }
  //////////////////////////////////////////////////////////////////////////////////////End handle 3 options///////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////Hanlde 2 options//////////////////////////////////////////////////////////////////////////////

  //Hanlde option left, front
  if ((front > MIN_SIDE_DISTANCE || front == 0) && (left > MIN_SIDE_DISTANCE || left == 0))
  {
    #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("front, left");
        lcd.setCursor(0, 1);
        lcd.print("MIN || 0");
        delay(1000);
        lcd.clear();
      #endif
    if (left >= LIMITED_SIDES || left == 0)
    {
      #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("left");
        lcd.setCursor(0, 1);
        lcd.print("LIMITED_SIDES || 0");
        delay(1000);
        lcd.clear();
      #endif
      Move(LEFT_SIDE);
      delay(LIMITED_TIME);
//      delay(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
//      StopSpeed();
//      int subcount = 0;
//      while (subcount < 3)
//      {
//        front = LOCATE->GetRange(FRONT_SIDE);
//        subcount++;
//      }
//      if (front >= LIMITED_SIDES_DOUBLE || front == 0)
//      {
//        RecoverSpeed();
//        ShouldMoveFront();
//      }
//      else
//      {
//        RecoverSpeed();
//        Move(LEFT_SIDE);
//        delay(LIMITED_TIME_SMALL);
//        ShouldMoveFront();
//      }
      ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      return;
    }
    else
    {
      #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("left");
        lcd.setCursor(0, 1);
        lcd.print(" < LIMITED_SIDES");
        delay(1000);
        lcd.clear();
      #endif
//      Move(LEFT_SIDE);
//      delay(LIMITED_TIME_SMALL);
      Move(LEFT_SIDE);
      delay(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      ShouldMoveFront(LIMITED_TIME_SMALL);
      Move(RIGHT_SIDE);
      delay(LIMITED_TIME_SMALL);
      ShouldMoveFront(LIMITED_TIME_SMALL);
      return;
    }

//    Move(LEFT_SIDE);
//    delay(LIMITED_TIME_SMALL);
//    ShouldMoveFront();
//    return;
  }
  //End Hanlde option left, front

  //Handle option right, front
  if ((front > MIN_SIDE_DISTANCE || front == 0) && (right > MIN_SIDE_DISTANCE || right == 0))
  {
    #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("front, right");
        lcd.setCursor(0, 1);
        lcd.print("MIN || 0");
        delay(1000);
        lcd.clear();
      #endif
    if (right >= LIMITED_SIDES || right == 0)
    {
      #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("right");
        lcd.setCursor(0, 1);
        lcd.print("LIMITED_SIDES || 0");
        delay(1000);
        lcd.clear();
      #endif
      Move(RIGHT_SIDE);
      delay(LIMITED_TIME);
//      StopSpeed();
//      int subcount = 0;
//      while (subcount < 3)
//      {
//        front = LOCATE->GetRange(FRONT_SIDE);
//        subcount++;
//      }
//
//      if (front >= LIMITED_SIDES_DOUBLE || front == 0)
//      {
//        RecoverSpeed();
//        ShouldMoveFront();
//      }
//      else
//      {
//        RecoverSpeed();
//        Move(RIGHT_SIDE);
//        delay(LIMITED_TIME_SMALL);
//        ShouldMoveFront();
//      }
      ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      return;
    }
    else
    {
      #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("right");
        lcd.setCursor(0, 1);
        lcd.print("< LIMITED_SIDES");
        delay(1000);
        lcd.clear();
      #endif
      Move(RIGHT_SIDE);
      delay(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      ShouldMoveFront(LIMITED_TIME_SMALL);
      Move(LEFT_SIDE);
      delay(LIMITED_TIME_SMALL);
      ShouldMoveFront(LIMITED_TIME_SMALL);
      return;
    }

      //Should I use this
//    Move(RIGHT_SIDE);
//    delay(LIMITED_TIME_SMALL);
//    ShouldMoveFront();
//    return;
  }
  //End Handle option right, front

  //Hanlde option left, right
  if ((left > MIN_SIDE_DISTANCE || left == 0) && (right > MIN_SIDE_DISTANCE || right == 0))
  {
    #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("left, right");
        lcd.setCursor(0, 1);
        lcd.print("MIN || 0");
        delay(1000);
        lcd.clear();
      #endif
    Move(BACK_SIDE);
    delay(LIMITED_TIME);
    if ((left >= LIMITED_SIDES || left == 0) && (right >= LIMITED_SIDES || right == 0))
    {
      #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("left, right");
        lcd.setCursor(0, 1);
        lcd.print("LIMITED_SIDES || 0");
        delay(1000);
        lcd.clear();
      #endif
      if ((left <= right && left != 0) || left == 0)
      {
        #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("left, right");
        lcd.setCursor(0, 1);
        lcd.print("left < right || 0");
        delay(1000);
        lcd.clear();
      #endif
        Move(LEFT_SIDE);
        delay(LIMITED_TIME);
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      }
      else
      {
        #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("left, right");
        lcd.setCursor(0, 1);
        lcd.print("right < left || 0");
        delay(1000);
        lcd.clear();
      #endif
        Move(RIGHT_SIDE);
        delay(LIMITED_TIME);
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      }
      return;
    }

    if (left >= LIMITED_SIDES || left == 0)
    {
       #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("left");
        lcd.setCursor(0, 1);
        lcd.print("LIMITED_SIDES || 0");
        delay(1000);
        lcd.clear();
      #endif
        Move(LEFT_SIDE);
        delay(LIMITED_TIME);
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
        return;
    }

    if (right >= LIMITED_SIDES || right == 0)
    {
      #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("right");
        lcd.setCursor(0, 1);
        lcd.print("LIMITED_SIDES || 0");
        delay(1000);
        lcd.clear();
      #endif
        Move(RIGHT_SIDE);
        delay(LIMITED_TIME);
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
        return;
    }

    if (left >= right)
    {
      #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("left, right");
        lcd.setCursor(0, 1);
        lcd.print("left >= right");
        delay(1000);
        lcd.clear();
      #endif
        Move(LEFT_SIDE);
        delay(LIMITED_TIME);
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
        return;
    }
    else
    {
       #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("left, right");
        lcd.setCursor(0, 1);
        lcd.print("left < right");
        delay(1000);
        lcd.clear();
      #endif
        Move(RIGHT_SIDE);
        delay(LIMITED_TIME);
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
        return;
    }
  }
  //End hanlde option left, right
  
  ///////////////////////////////////////////////////////////////////////////////////End Handle 2 options////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////Handle only options///////////////////////////////////////////////////////////////////////////
  //Handle option left
  if (left > MIN_SIDE_DISTANCE)
  {
    if (left >= LIMITED_SIDES || left == 0)
    {
        Move(LEFT_SIDE);
        delay(LIMITED_TIME);
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
        return;
    }
    else
    {
      Move(BACK_SIDE);
      delay(LIMITED_TIME);
      Move(LEFT_SIDE);
      delay(LIMITED_TIME);
      ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      StopSpeed();
      int subcount = 0;
      while (subcount < 3)
      {
        front = LOCATE->GetRange(FRONT_SIDE);
        right = LOCATE->GetRange(RIGHT_SIDE);
        subcount++;
      }

      if (right >= LIMITED_SIDES || right == 0)
      {
        Move(RIGHT_SIDE);
        delay(LIMITED_TIME);
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      }
      else
      {
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      }
    }
    return;
  }
  //End handle option left

  //Handle option right
  if (right > MIN_SIDE_DISTANCE)
  {
    if (right >= LIMITED_SIDES || right == 0)
    {
        Move(RIGHT_SIDE);
        delay(LIMITED_TIME);
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
        return;
    }
    else
    {
      Move(BACK_SIDE);
      delay(LIMITED_TIME);
      Move(RIGHT_SIDE);
      delay(LIMITED_TIME);
      ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      StopSpeed();
      int subcount = 0;
      while (subcount < 3)
      {
        front = LOCATE->GetRange(FRONT_SIDE);
        left = LOCATE->GetRange(LEFT_SIDE);
        subcount++;
      }

      if (left >= LIMITED_SIDES || left == 0)
      {
        Move(LEFT_SIDE);
        delay(LIMITED_TIME);
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      }
      else
      {
        ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
      }
      return;
    }
   }
  //End handle option right

  //Handle option front
  if (front > MIN_SIDE_DISTANCE || front == 0)
  {
    #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("front");
        lcd.setCursor(0, 1);
        lcd.print("MIN || 0");
        delay(1000);
        lcd.clear();
      #endif
    ShouldMoveFront();
    return;
  }
  //End handle option front
  //////////////////////////////////////////////////////////////////////////////End Hanlde only option///////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////Handle No options///////////////////////////////////////////////////////////////////////////
  ShouldMoveFront(LIMITED_TIME_SMALL + LIMITED_TIME_SMALL);
  #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("THE END");
        delay(1000);
        lcd.clear();
      #endif
  return;
  #ifdef USE_LCD
        lcd.setCursor(0, 0);
        lcd.print("NOT THE END");
        delay(1000);
        lcd.clear();
      #endif
  //////////////////////////////////////////////////////////////////////////////End Hanlde No option///////////////////////////////////////////////////////////////////////////

  
  /*******************************************************End New idea**************************************************/

//  if (IsStopping)
//    RecoverSpeed();
  
  //handle when being stuck
  if (left < LIMITED_SIDES && right < LIMITED_SIDES && front < LIMITED_SIDES)
  {
    //SetSpeed(0, 0);
    Move(BACK_SIDE);
    delay(LIMITED_TIME);
    if (left >= right)
    {
      //delay(LIMITED_TIME);
      Move(LEFT_SIDE);
      delay(LIMITED_TIME);
      Move(FRONT_SIDE);
      delay(LIMITED_TIME / 2);
      Move(RIGHT_SIDE);
      delay(LIMITED_TIME);
    }
    else
    {
      //delay(LIMITED_TIME);
      Move(RIGHT_SIDE);
      delay(LIMITED_TIME);
      Move(FRONT_SIDE);
      delay(LIMITED_TIME / 2);
      Move(LEFT_SIDE);
      delay(LIMITED_TIME);
    }
    Move(FRONT_SIDE);
    delay(LIMITED_TIME + LIMITED_TIME);
    return;  
  }

  //Handle near wall
  if (left < MIN_SIDE_DISTANCE && front < MIN_SIDE_DISTANCE)
  {
    return;
  }
  if (left < MIN_SIDE_DISTANCE)
  {
    Move(RIGHT_SIDE);
    delay(LIMITED_TIME / 2);
    Move(FRONT_SIDE);
    delay(LIMITED_TIME / 2);
    return;
  }
  if (right < MIN_SIDE_DISTANCE)
  {
    Move(LEFT_SIDE);
    delay(LIMITED_TIME / 2);
    Move(FRONT_SIDE);
    delay(LIMITED_TIME / 2);
    return;
  }
  
  //Nornmal running
  if (left >= LIMITED_SIDES && right >= LIMITED_SIDES)
  {
    if (left <= right)
    {
      Move(LEFT_SIDE);
      delay(LIMITED_TIME);
    }
    else
    {
      Move(RIGHT_SIDE);
      delay(LIMITED_TIME);
    }
    //StopSpeed();
    ShouldMoveFront();
    
  }
  else if (left >= LIMITED_SIDES)
  {
    Move(LEFT_SIDE);
    delay(LIMITED_TIME);
    //StopSpeed();
    ShouldMoveFront();
  }
  else if (right >= LIMITED_SIDES)
  {
    Move(RIGHT_SIDE);
    delay(LIMITED_TIME);
    //StopSpeed();
    ShouldMoveFront();
  }
  else
  {
    Move(FRONT_SIDE);
    delay(LIMITED_TIME + LIMITED_TIME + LIMITED_TIME);
  }
//  if (front >= MIN_FRONT_DISTANCE)
//  {
//    Move(FRONT_SIDE);
//    //    delay(100);
//  }
//  else
//  {
//    unsigned int left = LOCATE->GetRange(LEFT_SIDE);
//    unsigned int right = LOCATE->GetRange(RIGHT_SIDE);
//    if (front <= LIMITED_SIDES && left <= LIMITED_SIDES && right <= LIMITED_SIDES)
//    {
//      Move(BACK_SIDE);
//      return;
//    }
//    //Move(BACK_SIDE);
//    //    Stop();
//    if (front > left && front > right)
//    {
//      Move(FRONT_SIDE);
//      return;
//    }
//
//    if (left > right)
//    {
//      //      unsigned int current_time = millis();
//      //      while (millis() - current_time < LIMITED_TIME)
//      Move(LEFT_SIDE);
//      delay(LIMITED_TIME);
////      Move(FRONT_SIDE);
//    }
//    else
//    {
//      //      unsigned int current_time = millis();
//      //      while (millis() - current_time < LIMITED_TIME)
//      Move(RIGHT_SIDE);
//      delay(LIMITED_TIME);
////      Move(FRONT_SIDE);
//    }
//    Move(FRONT_SIDE);
//  }
}

