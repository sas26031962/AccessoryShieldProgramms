/*********************************************************************
Программа управления драйвером акселерометром  ADXL345
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
#include <M2M_LM75A.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

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
#define TEMPERATURE_LINE 16             // строка вывода температуры
#define ACCELEROMETER_LINE 24           // строка вывода показаний акселерометра

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

M2M_LM75A lm75a;

//Экземпляр акселерометра
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

//=====================================================================
// Начальная установка
//=====================================================================
void setup()   {                
  Serial.begin(SERIAL_SPEED);
  
  Wire.begin();// Старт интерфейса I2C
  
   // Начальная установка дисплея
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDRESS); 
  // Отображение логотипа, записанного в резидентной памяти
  display.display();
  
  delay(PAUSE);

  // Очистка буфера диспля
  display.clearDisplay();
  // Начальная заставка
  display.setTextSize(DEFAULT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(HOME_COLUMN,HOME_LINE);

  // Начальная установка акселерометра
  if(!accel.begin())
  {
    // Ошибка подключения датчика
    display.println("no ADXL345 detected");
    display.display();
    while(1);
  }
  accel.setRange(ADXL345_RANGE_16_G);
  // Отображение основных параметров датчика
  displaySensorDetails();
  // Отображение дополнительных параметров датчика
  //displayDataRate();
  //displayRange();
  
  //delay(PAUSE);
  
  // Очистка буфера диспля
  display.clearDisplay();
  // Формирование экрана в режиме измерения
  display.setTextSize(DEFAULT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(HOME_COLUMN,HOME_LINE);
  display.println("Accelerometer Test");
  display.display();
  //delay(PAUSE);
}//End of woid install()


//=====================================================================
// Главный цикл
//=====================================================================
void loop() 
{
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();

  // Очистка второй строки
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

  // Очистка третьей строки
  display.fillRect(
    HOME_COLUMN, 
    TEMPERATURE_LINE, 
    128,
    16,
    0
    );
  display.setCursor(HOME_COLUMN,TEMPERATURE_LINE);
  
  display.print("Temperature: ");
  display.print(lm75a.getTemperature());

  // Очистка четвёртой строки
  display.fillRect(
    HOME_COLUMN, 
    ACCELEROMETER_LINE, 
    128,
    24,
    0
    );
  display.setCursor(HOME_COLUMN,ACCELEROMETER_LINE);
  
//------------------------------------------------------------------
 /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  display.print("X:"); display.print(event.acceleration.x); display.print(" ");
  display.print("Y:"); display.print(event.acceleration.y); display.print(" ");
  display.print("Z:"); display.print(event.acceleration.z); 
  //display.print(" ");display.println("m/s^2 ");
//------------------------------------------------------------------  

  display.display();
  
  delay(1000);
  
    
}//End of void loop()


//=====================================================================
// Подпрограммы
//=====================================================================
void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  //Serial.println("------------------------------------");
  display.print  ("Sensor:       "); display.println(sensor.name);
  //display.print  ("Driver Ver:   "); display.println(sensor.version);
  //display.print  ("Unique ID:    "); display.println(sensor.sensor_id);
  display.print  ("Max Value:    "); display.print(sensor.max_value);// display.println(" m/s^2");
  display.print  ("Min Value:    "); display.print(sensor.min_value);// display.println(" m/s^2");
  display.print  ("Resolution:   "); display.print(sensor.resolution);// display.println(" m/s^2");  
  //Serial.println("------------------------------------");
  //Serial.println("");
  display.display();
  delay(2000);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" g");  
}

