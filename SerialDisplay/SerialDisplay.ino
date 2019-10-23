/*********************************************************************
Программа отображения данных, поступающих по последовательному каналу
Используется библиотека <OLED_I2C.h>
*********************************************************************/

 //========================================================================
 //---------------------------- ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ ------------------------
 //========================================================================
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>
#include <M2M_LM75A.h>

#include <OLED_I2C.h>
#include "config.h"

 //========================================================================
 //-------------------------------- КОНСТАНТЫ -----------------------------
 //========================================================================
#define SERIAL_SPEED 115200 
#define PAUSE 2000

#define DEFAULT_SIZE 1                  // размер текста по умолчанию
// Позиционирование курсора в пикселях
#define HOME_COLUMN 0                   // столбец по умолчанию
#define HOME_LINE 0                     // строка по умолчанию
#define TIME_LINE 8                     // строка вывода текущего времени
#define TEMPERATURE_LINE 16             // строка вывода температуры
#define ACCELEROMETER_LINE 24           // строка вывода показаний акселерометра

#define TITLE "Serial display v1.0.0"   // Начальный текст

 //========================================================================
 //--------------------------- ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ----------------------
 //========================================================================
OLED  display(SDA, SCL, RESET_PIN);
extern uint8_t SmallFont[];
extern uint8_t RusFont[];

String s;

int Count = 0;

String Buffer2 = "";
String Buffer3 = "";
String Buffer4 = "";
String Buffer5 = "";
String Buffer6 = "";
String Buffer7 = "";

bool IsIncoming = false;

double CurrentMillis;
double PreviousMillis;

DS3231 Clock;
bool Century=false;
bool h12;
bool PM;

M2M_LM75A lm75a;


//=====================================================================
// Начальная установка
//=====================================================================
void setup()   
{                
  Serial.begin(SERIAL_SPEED);
  
  Wire.begin();// Старт интерфейса I2C
  
  display.begin();
  display.setFont(SmallFont);
  display.clrScr();
  display.print(TITLE, LEFT, STRING_0);
  display.update();

  CurrentMillis = millis();
  PreviousMillis = CurrentMillis;
  
 }//End of woid install()


//=====================================================================
// Главный цикл
//=====================================================================
void loop() 
{
  CurrentMillis = millis();
  if(CurrentMillis - PreviousMillis > 1000)
  {
    PreviousMillis = CurrentMillis;
    showScreen();
  }
  
  if(Serial.available())
  {
    char x = Serial.read();
    Count ++;
    // Экран не заполнен полностью
    if(Count <= SMALL_SCREEN_WIDTH)
    {
      Buffer2 += x;
    }

    if((Count <= SMALL_SCREEN_WIDTH * 2) && (Count > SMALL_SCREEN_WIDTH * 1))
    {
      Buffer3 += x;
    }

    if((Count <= SMALL_SCREEN_WIDTH * 3) && (Count > SMALL_SCREEN_WIDTH * 2))
    {
      Buffer4 += x;
    }

    if((Count <= SMALL_SCREEN_WIDTH * 4) && (Count > SMALL_SCREEN_WIDTH * 3))
    {
      Buffer5 += x;
    }

    if((Count <= SMALL_SCREEN_WIDTH * 5) && (Count > SMALL_SCREEN_WIDTH * 4))
    {
      Buffer6 += x;
    }

    if((Count <= SMALL_SCREEN_WIDTH * 6) && (Count > SMALL_SCREEN_WIDTH * 5))
    {
      Buffer7 += x;
    }
    // Экран заполнен полностью

    if((Count > SMALL_SCREEN_WIDTH * 6) && (Count % SMALL_SCREEN_WIDTH == 1))
    {
      Buffer2 = Buffer3;
      Buffer3 = Buffer4;
      Buffer4 = Buffer5;
      Buffer5 = Buffer6;
      Buffer6 = Buffer7;
      Buffer7 = x;
    }

    if((Count > SMALL_SCREEN_WIDTH * 6) && (Count % SMALL_SCREEN_WIDTH != 1))
    {
      Buffer7 += x;
    }

    IsIncoming = true;
    
  }//End of if(Serial.available())

  if(IsIncoming)
  {
    showScreen();
    IsIncoming = false;
  }
  
}//End of void loop()


//=====================================================================
// Подпрограммы
//=====================================================================

//
// Отображение состояния датчиков
//
void showScreen()
{
  // Отображение данных
  display.clrScr();
  display.print(TITLE, LEFT, STRING_0);
  
  s = "D:";
  if(Clock.getMonth(Century) < 10)s += "0";
  s += Clock.getMonth(Century);
  s += "-";
  if(Clock.getDate() < 10)s += "0";
  s += Clock.getDate();
  s += " T:";
  if(Clock.getHour(h12, PM) < 10)s += "0";
  s += Clock.getHour(h12, PM);
  s += "-";
  if(Clock.getMinute() < 10)s += "0";
  s += Clock.getMinute();
  s += " t: ";
  s += (int)lm75a.getTemperature();
  
  //s = "Count: ";
  //s += Count;
  display.print(s, LEFT, STRING_1);

  display.print(Buffer2, LEFT, STRING_2);
  display.print(Buffer3, LEFT, STRING_3);
  display.print(Buffer4, LEFT, STRING_4);
  display.print(Buffer5, LEFT, STRING_5);
  display.print(Buffer6, LEFT, STRING_6);
  display.print(Buffer7, LEFT, STRING_7);
  
  display.update();
  
}

