// Программа для исследования АЦП в контроллерах Arduino
// Используется Ardino Mega + Accessory shield 
// На Accessory shield используется встроенный резистор и  OLED_I2C дисплей

//=== РЕЗУЛЬТАТЫ ===
// Измеренное значение времени преобразования АЦП равно 114 мкс для ArduinoMega
// При неподвижной ручке встроенного резистора измеренное значение АЦП колеблется 
// около некоего среднего значения


//========================================================================
//---------------------------- КЛАССЫ И СТРУКТУРЫ ------------------------
//========================================================================
struct value{
  int X;
  int Min;
  int Max;

  void store(int x);
  void get(int x);
};

void value::store(int x)
{
  this->X = x;
  if(x > this->Max) this->Max = x;
  if(x < this->Min) this->Min = x;
}

void value::get(int x)
{
  this->X = x;
  this->Max = x;
  this->Min = x;
}


//========================================================================
//-------------------------------- КОНСТАНТЫ -----------------------------
//========================================================================
#define SERIAL_SPEED 115200 
#define VIEW_DWELL 500 

#define COUNT_VALUE 1024
#define ADC_PORT A0

#include <OLED_I2C.h>

//========================================================================
//-------------------------- ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ -----------------------
//========================================================================
  
long CurrentTime;
long PreviousTime;

long Interval;

value A0Value;

//int AnalogSensorValue;
//int AnalogSensorValueMin;
//int AnalogSensorValueMax;

String Sting0;
String Sting1;
String Sting2;
String Sting3;

long SinkCounter;
long SendCounter;

char BufferSend[32];


OLED  myOLED(SDA, SCL, 7);

extern uint8_t SmallFont[];
extern uint8_t RusFont[];

//========================================================================
//---------------------- ПРОГРАММА НАЧАЛЬНОЙ УСТАНОВКИ -------------------
//========================================================================
void setup()
{
  CurrentTime = 0;
  PreviousTime = 0;

  SinkCounter = 0;
  SendCounter = 0;
  
  myOLED.begin();
  myOLED.setFont(SmallFont);
  //
  // Запуск последовательного порта для мониторинга
  //
  Serial.begin(SERIAL_SPEED);

  BufferSend[0] = '0';
  BufferSend[1] = '1';
  BufferSend[2] = '2';
  BufferSend[3] = '3';
  BufferSend[4] = '4';
  BufferSend[5] = '5';
  BufferSend[6] = '6';
  BufferSend[7] = '7';
  BufferSend[8] = '8';
  BufferSend[9] = '9';
  BufferSend[10] = 13;
  BufferSend[10] = 10;

  A0Value.get(analogRead(ADC_PORT));
  
}//End of void setup() 

//========================================================================
//-------------------------- ГЛАВНЫЙ ЦИКЛ ПРОГРАММЫ ----------------------
//========================================================================
void loop()
{
  CurrentTime = millis();
  if(CurrentTime  - PreviousTime > VIEW_DWELL)
  {
    PreviousTime = millis();
    // Сброс статистики по минимуму и максимуму
    A0Value.get(analogRead(ADC_PORT));

    // Измерение в микросекундах
    Interval = micros();   
    for(int i = 0; i < COUNT_VALUE; i++)
    {
      A0Value.store(analogRead(ADC_PORT));
      //delay(1);
    }
    Interval = micros() - Interval;
    
    
    /*
    Serial.write(BufferSend, 12);
    SendCounter += 12;
    */
    
    //Sting0 = "ADC interval view v1.0";
    
    Sting0 = "ADC interval: ";
    Sting0 += Interval;
    
    Sting1 = "A0 value: ";
    Sting1 += A0Value.X;

    Sting2 = "A0 min value: ";
    Sting2 += A0Value.Min;
    
    Sting3 = "A0 max value: ";
    Sting3 += A0Value.Max;
    
    myOLED.clrScr();
    myOLED.print(Sting0, LEFT, 0);
    myOLED.print(Sting1, LEFT, 16);
    myOLED.print(Sting2, LEFT, 32);
    myOLED.print(Sting3, LEFT, 48);
    myOLED.update();
  }
  
}//End of void loop()

//========================================================================
//------------------------------ ПОДПРОГРАММЫ ----------------------------
//========================================================================
//
// Программа обработки событий последовательного порта
// 
void serialEvent()
{
  int x = Serial.read();
  SinkCounter++;
}


