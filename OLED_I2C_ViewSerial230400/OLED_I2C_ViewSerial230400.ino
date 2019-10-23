// OLED_I2C_ViewSerialStatistics

//========================================================================
//-------------------------------- КОНСТАНТЫ -----------------------------
//========================================================================
#define SERIAL_SPEED 230400 
#define VIEW_DWELL 500 

#include <OLED_I2C.h>

//========================================================================
//-------------------------- ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ -----------------------
//========================================================================
  
long CurrentTime;
long PreviousTime;

String SinkString;
String SendString;
String InfoString;

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
  BufferSend[11] = 10;

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

    Serial.write(BufferSend, 12);
    SendCounter += 12;

    SinkString = "Sink: ";
    SinkString += SinkCounter;
    
    SendString = "Send: ";
    SendString += SendCounter;

    InfoString = "Speed: ";
    InfoString += SERIAL_SPEED;
    InfoString += " baud";
    
    myOLED.clrScr();
    myOLED.print("Serial port view v1.0", LEFT, 0);
    myOLED.print(SinkString, LEFT, 16);
    myOLED.print(SendString, LEFT, 32);
    myOLED.print(InfoString, LEFT, 48);
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


