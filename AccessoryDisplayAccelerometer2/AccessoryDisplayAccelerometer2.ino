/*********************************************************************
Программа управления драйвером акселерометром  ADXL345
из состава платы Accessory Shield
Подключение по I2C
Используется библиотека <OLED_I2C.h>
*********************************************************************/

 //========================================================================
 //---------------------------- ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ ------------------------
 //========================================================================
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>
#include "M2M_LM75A.h"

#include <OLED_I2C.h>
#include "config.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

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

#define TITLE "Accessory shield test"   // Начальный текст

#define ACCELEROMETER_ID 123456
 //========================================================================
 //--------------------------- ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ----------------------
 //========================================================================
OLED  display(SDA, SCL, RESET_PIN);
extern uint8_t SmallFont[];
extern uint8_t RusFont[];
String s;

DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte DoW;

//byte year, month, date, DoW, hour, minute, second;

M2M_LM75A lm75a;

//Экземпляр акселерометра
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(ACCELEROMETER_ID);



//=====================================================================
// Начальная установка
//=====================================================================
void setup()   {                
  Wire.begin();// Старт интерфейса I2C
  
  display.begin();
  display.setFont(SmallFont);
  display.clrScr();
  display.print(TITLE, LEFT, STRING_0);

  // Начальная установка акселерометра
  if(!accel.begin())
  {
    display.print("no ADXL345 detected", LEFT, STRING_1);
    while(1);
  }
  accel.setRange(ADXL345_RANGE_16_G);
  // Отображение основных параметров датчика
  displaySensorDetails();
  // Отображение дополнительных параметров датчика
  displayDataRate();
  displayRange();
  
  display.update();

  delay(PAUSE);

}//End of woid install()


//=====================================================================
// Главный цикл
//=====================================================================
void loop() 
{
  // Чтение данных
  sensors_event_t event; 
  accel.getEvent(&event);
  
  // Отображение данных
  display.clrScr();
  display.print(TITLE, LEFT, STRING_0);
  s = "T:20";
  if(Clock.getYear() < 10)s += "0";
  s += Clock.getYear();
  s += "-";
  if(Clock.getMonth(Century) < 10)s += "0";
  s += Clock.getMonth(Century);
  s += "-";
  if(Clock.getDate() < 10)s += "0";
  s += Clock.getDate();
  s += " ";
  if(Clock.getHour(h12, PM) < 10)s += "0";
  s += Clock.getHour(h12, PM);
  s += ":";
  if(Clock.getMinute() < 10)s += "0";
  s += Clock.getMinute();
  s += ":";
  if(Clock.getSecond() < 10)s += "0";
  s += Clock.getSecond();
  display.print(s, LEFT, STRING_2);

  s = "Temperature: ";
  s += lm75a.getTemperature();
  display.print(s, LEFT, STRING_3);
  
  s = "X:";
  s += event.acceleration.x;
  s += " Y:";
  s += event.acceleration.y;
  s += " Z:";
  s += event.acceleration.z;
  display.print(s, LEFT, STRING_5);
/*  
  s = "A1:";
  s += analogRead(A1);
  s += " A2:";
  s += analogRead(A2);
  s += " A3:";
  s += analogRead(A3);
  s += " A4:";
  s += analogRead(A4);
  s += " A5:";
  s += analogRead(A5);
  display.print(s, LEFT, STRING_5);
*/  
  display.update();
  
}//End of void loop()


//=====================================================================
// Подпрограммы
//=====================================================================
void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);

  s = "Sensor: ";
  s += sensor.name;
  display.print(s, LEFT, STRING_1);
  s = "Driver Ver: ";
  s += sensor.version;
  display.print(s, LEFT, STRING_2);
  //s = "Unique ID: ";
  //s += sensor.sensor_id;
  //display.print(s, LEFT, STRING_3);
  s = "Max Value: ";
  s += sensor.max_value;
  display.print(s, LEFT, STRING_3);
  s = "Min Value: ";
  s += sensor.min_value;
  display.print(s, LEFT, STRING_4);
  s = "Resolution: ";
  s += sensor.resolution;
  display.print(s, LEFT, STRING_5);

}//End of void displaySensorDetails(void)

void displayDataRate(void)
{
  s = "Data Rate:";
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      s += "3200 "; 
      break;
    case ADXL345_DATARATE_1600_HZ:
      s += "1600 "; 
      break;
    case ADXL345_DATARATE_800_HZ:
      s += "800 "; 
      break;
    case ADXL345_DATARATE_400_HZ:
      s += "400 "; 
      break;
    case ADXL345_DATARATE_200_HZ:
      s += "200 "; 
      break;
    case ADXL345_DATARATE_100_HZ:
      s += "100 "; 
      break;
    case ADXL345_DATARATE_50_HZ:
      s += "50 "; 
      break;
    case ADXL345_DATARATE_25_HZ:
      s += "25 "; 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      s += "12.5 "; 
      break;
    case ADXL345_DATARATE_6_25HZ:
      s += "6.25 "; 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      s += "3.13 "; 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      s += "1.56 "; 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      s += "0.78 "; 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      s += "0.39 "; 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      s += "0.20 "; 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      s += "0.10 "; 
      break;
    default:
      s += "???? "; 
      break;
  } 
  s += "Hz"; 
  display.print(s, LEFT, STRING_6);
  
}//End of void displayDataRate(void)

void displayRange(void)
{
  s = "Data Rate:";
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      s += "16 "; 
      break;
    case ADXL345_RANGE_8_G:
      s += "8 "; 
      break;
    case ADXL345_RANGE_4_G:
      s += "4 "; 
      break;
    case ADXL345_RANGE_2_G:
      s += "2 "; 
      break;
    default:
      s += "?? "; 
      break;
  }  
  s += "g"; 
  display.print(s, LEFT, STRING_7);

}//End of void displayRange(void)

