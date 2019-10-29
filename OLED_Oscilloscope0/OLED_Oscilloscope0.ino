/*********************************************************************

Тестирование OLED - дисплея в графическом режиме
Монохромный дисплей
Крайний левый бит в строке - младший бит младшего байта
Крайний првый бит в строке - старший бит старшего байта

Отображение одного отсчёта на графике 
Вертикальная связь между отсчётами при больших перепадах
Использование массива данных при отображении
Порядок следования битов в строке слева-направо: 7654 3210
Программная генерация осей координат
*********************************************************************/
#include <OLED_I2C.h>


//=========================================================================
// КОНСТАНТЫ
//=========================================================================
#define COUNTER_DWELL 100

//=========================================================================
// КЛАССЫ И СТРУКТУРЫ
//=========================================================================

struct sScreen{
  const int WIDTH = 128;
  const int HEIGHT = 64;
  const int GRID = 10;
  const int LEFT_MARGIN_0 = 4;
  const int BOTTOM_MARGIN_0 = 3;

  int LeftMargin;
};

struct sControl{
  long MillisCurrent;
  long MillisPrevious;
  int Resistor;
};

//=========================================================================
// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
//=========================================================================

OLED  myOLED(SDA, SCL, 7);

sScreen Screen;
sControl Control; 

// Область данных экрана для процедуры drawBitmap(int x, int y, uint8_t* bitmap, int sx, int sy)
  
 unsigned char screen_data[128] = {
  
 };

//=====================================================================================
// Программа начальной установки
//=====================================================================================
void setup()
{                
  for(int i = 0; i < 128; i++)
  {
    screen_data[i] = 15;//Линия посередине экрана
    /*
    // меандр
    if(i % 8 == 0) bottom = !bottom;
    if(bottom)
      screen_data[i] = 12;
    else  
       screen_data[i] = 4;
    */ 
    
    /*  
    // иголки
    int stepX = 13;
    
    bottom = (i % stepX == 0);
    if(bottom)
    {
      screen_data[i] = 12;
      //Serial.print("i = ");
      //Serial.println(i);
    }  
    else
    {  
       screen_data[i] = 4;
    }
    */
    // пила
    //screen_data[i] = i % 20 + 3;

    // синус
    //screen_data[i] = 2 + 10 * (1 - sin(i / 4));
    
  }

    
  /* Инициализация дисплея с помощью процедуры drawBitmap(int x, int y, uint8_t* bitmap, int sx, int sy)
   * Описание параметров процедуры: 
   * x - смещение по оси абсцисс
   * y - смещение по оси ординат
   * bitmap - указатель на массив, содержащий графическую информацию
   * sx - размер изображения по оси абсцисс
   * sy - размер изображения по оси ординат
   * 
   * Алгоритм работы процедуры
   * 
   */



  Screen.LeftMargin = 5;
   
  myOLED.begin();

  myOLED.clrScr();
  //Рисование оси абсцисс
  drawAxisX();

  //Рисование оси ординат
  drawAxisY();

  /*
  // Рисование горизонтальной линии (y = const)
  for(int i = 4; i < 128; i++)
  {
    for(int j = 0; j < 61; j++)
    {
      if(j == 3)
      {
        myOLED.setPixel(i, j);  
      }
    }
    
    //screen_data[i] = 2 + 10 * (1 - sin(i / 4));
  }
  */
  // Рисование графика
  for(int i = Screen.LEFT_MARGIN_0 + Screen.LeftMargin; i < Screen.WIDTH; i++)
  {
    for(int j = 0; j < Screen.HEIGHT - Screen.BOTTOM_MARGIN_0; j++)
    {
      if(j == (2 + 20 * ((int)(1 - sin(i / 4)))))
      {
        myOLED.setPixel(i, j);  
      }
    }
  }
  
  myOLED.update();
  
}//End of void setup()


//=====================================================================================
// Главный цикл программы
//=====================================================================================
void loop() {
  exec(COUNTER_DWELL);
  /*
  MillisCurrent = millis();
  if(MillisCurrent - MillisPrevious > COUNTER_DWELL)
  {
    MillisPrevious = MillisCurrent;
    //---
    //Измерение напряжения
    float fX = 1.23456;
    int iX = analogRead(0);
    fX = (5.0 / 1024) * iX;
    //---
    //Очистка дисплея
    display.clearDisplay();
    //Установка параметров дисплея
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    //Формирование текста
    display.println("Voltmeter ver. 1.0.0");
    display.print("V = ");//
    //display.println("1.23456");// 
    display.println(fX);// 
    //Вывод на дисплей
    display.display();
   //--- 
  }
  */
}//End of void loop()

//=====================================================================================
// Подпрограммы
//=====================================================================================
void exec(long dwell)
{
  Control.MillisCurrent = millis();
  if(Control.MillisCurrent - Control.MillisPrevious > dwell)
  {
    Control.MillisPrevious = Control.MillisCurrent;
    Control.Resistor = (analogRead(A0) >> 4);
    //---
    myOLED.clrScr();
    //Рисование оси абсцисс
    drawAxisX();

    //Рисование оси ординат
    drawAxisY();
    // Рисование графика
    for(int i = Screen.LEFT_MARGIN_0 + Control.Resistor; i < Screen.WIDTH; i++)
    {
      for(int j = 0; j < Screen.HEIGHT - Screen.BOTTOM_MARGIN_0; j++)
      {
        if(j == (2 + 20 * ((int)(1 - sin(i / 4)))))
        {
          myOLED.setPixel(i, j);  
        }
      }
    }
  
    myOLED.update();
    //---
  }  
}//End of void exec(long dwell)  

//
// Рисование оси абсцисс
//
void drawAxisX()
{
  for(int i = 0; i < Screen.WIDTH; i++) myOLED.setPixel(i, Screen.HEIGHT - 3);
  
  //Рисование меток абсцисс
  for(int i = 0; i < Screen.WIDTH; i++)
  {
    if(i % Screen.GRID == 0)
    {
      myOLED.setPixel(i, Screen.HEIGHT - 2);
      myOLED.setPixel(i, Screen.HEIGHT - 1);
    }
  }
}//End of void drawAxisX()

//
// Рисование оси ординат
//
void drawAxisY()
{
  for(int i = 0; i < Screen.HEIGHT - 1; i++) myOLED.setPixel(2, i);
  //Рисование меток ординат
  for(int i = 0; i < Screen.HEIGHT - 1; i++)
  {
    if(i % Screen.GRID == 0)
    {
      myOLED.setPixel(0, i);
      myOLED.setPixel(1, i);
    }
  }
}//End of void drawAxisX()


