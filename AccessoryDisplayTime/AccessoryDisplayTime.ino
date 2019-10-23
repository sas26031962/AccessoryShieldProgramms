/*********************************************************************
Программа управления драйвером реального времени DS3231
из состава платы Accessory Shield
Подключение по I2C
*********************************************************************/

 //========================================================================
 //---------------------------- ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ ------------------------
 //========================================================================
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS3231.h>

 //========================================================================
 //-------------------------------- КОНСТАНТЫ -----------------------------
 //========================================================================
#define SERIAL_SPEED 115200 
#define PAUSE 2000

#define OLED_RESET 7                    // линия аппаратного сброса микросхемы контроллера дисплея
#define DISPLAY_I2C_ADDRESS 0x3C        // адрес контроллера дисплея на шине I2C

#define DEFAULT_SIZE 1                  // размер текста по умолчанию
// Позиционирование курсора в пикселях
#define HOME_COLUMN 0                   // столбец по умолчанию
#define HOME_LINE 0                     // строка по умолчанию
#define TIME_LINE 8                     // строка вывода текущего времени

#define TITLE "Accessory shield v1.0"   // Начальный текст

 //========================================================================
 //--------------------------- ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ----------------------
 //========================================================================
Adafruit_SSD1306 display(OLED_RESET);
DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte year, month, date, DoW, hour, minute, second;

//=====================================================================
// Начальная установка
//=====================================================================
void setup()   {                
  Serial.begin(SERIAL_SPEED);
  
  Wire.begin();//Старт интерфейса I2C
  
  // Начальная установка дисплея
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS); 
  // Отображение логотипа, записанного в резидентной памяти
  display.display();
  
  delay(PAUSE);

  // Очистка буфера диспля
  display.clearDisplay();
  //delay(PAUSE);
  // Начальная заставка
  display.setTextSize(DEFAULT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(HOME_COLUMN,HOME_LINE);
  display.println(TITLE);
  display.display();
  delay(PAUSE);

}//End of woid install()


//=====================================================================
// Главный цикл
//=====================================================================
void loop() 
{
  int second,minute,hour,date,month,year,temperature;
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();

  /*
  display.clearDisplay();
  display.setTextSize(DEFAULT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(HOME_COLUMN,HOME_LINE);
  display.println(TITLE);
  */
  display.fillRect(
    HOME_COLUMN, 
    TIME_LINE, 
    128,
    8,
    0
    );
  display.setCursor(HOME_COLUMN,TIME_LINE);
  
  display.print("20");
  if(year < 10)display.print(0);
  display.print(year,DEC);
  display.print('-');
  if(month < 10)display.print(0);
  display.print(month,DEC);
  display.print('-');
  if(date < 10)display.print(0);
  display.print(date,DEC);
  display.print(' ');
  if(hour < 10)display.print(0);
  display.print(hour,DEC);
  display.print(':');
  if(minute < 10)display.print(0);
  display.print(minute,DEC);
  display.print(':');
  if(second < 10)display.print(0);
  display.print(second,DEC);

  display.display();
  

  
  delay(1000);
  
  //display.display();
    
}//End of void loop()


//=====================================================================
// Подпрограммы
//=====================================================================

