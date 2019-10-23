/* Тестовая программа для Arduino по работе с драйвером трёхцветного светодиода P9813 
 * Яркость светодиода задаётся с помощью потенциометра, подключённого к линии А0 
 * 
 *
 */

//==============================================================================
// Определения
//==============================================================================
#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long int

//данный параметр используется при управлении яркостью,
//чем меньше значение, тем больше яркость
 
#define BRIGHTNESS 30 
//==============================================================================
// Использование аппаратной части
//==============================================================================
int Clkpin = 6;
int Datapin = 5;


//==============================================================================
// Подпрограммы
//==============================================================================
//
// Генерация тактового импульса
//
void ClkProduce(void)
{
  digitalWrite(Clkpin, LOW);
  delayMicroseconds(20);
  digitalWrite(Clkpin, HIGH);
  delayMicroseconds(20);
}
//-----------------------------------------------------------------------------
//
// Посылка 32 нулей 
//
void Send32Zero(void)
{
  unsigned char i;

  for (i=0; i<32; i++) 
  {
    digitalWrite(Datapin, LOW);
    ClkProduce();
  }
}
//-----------------------------------------------------------------------------
//
// Преобразование данных
// Если во входном байте сброшен бит 7, устанавливается бит 1
// Если во входном байте сброшен бит 6, устанавливается бит 0
//
uint8 TakeAntiCode(uint8 dat)
{
  uint8 tmp = 0;

  if ((dat & 0x80) == 0) {
    tmp |= 0x02;
  }

  if ((dat & 0x40) == 0) {
    tmp |= 0x01;
  }

  return tmp;
}
//-----------------------------------------------------------------------------
//
// Передача 32-х битного слова 
//
void DatSend(uint32 dx)
{
  uint8 i;

  for (i = 0; i < 32; i++) 
  {
    if ((dx & 0x80000000) != 0) 
    {
      digitalWrite(Datapin, HIGH);
    } 
    else 
    {
      digitalWrite(Datapin, LOW);
    }
    dx <<= 1;
    ClkProduce();
  }
}
//-----------------------------------------------------------------------------
//
// Кодирование данных
// Входные данные: данные цветов
//
void DataDealWithAndSend(uint8 r, uint8 g, uint8 b)
{
  // Сброс 32-х разрядного результата
  
  uint32 dx = 0;

  dx |= (uint32)0x03 << 30;             // Установка старших битов - битов флагов
  dx |= (uint32)TakeAntiCode(b) << 28;  // Инверсия старших битов синего цвета
  dx |= (uint32)TakeAntiCode(g) << 26;  // Инверсия старших битов зелёного цвета 
  dx |= (uint32)TakeAntiCode(r) << 24;  // Инверсия старших битов красного цвета

  dx |= (uint32)b << 16;                // Кодирование синего цвета
  dx |= (uint32)g << 8;                 // Кодирование зелёного цвета
  dx |= r;                              // Кодирование красного цвета

  DatSend(dx);                          // Предача данных
}

//==============================================================================
// Глобальные переменные
//==============================================================================
int t = 0;     // Loop counter
int incomingByte[37];

//==============================================================================
// Программа начальной установки
//==============================================================================
void setup()
{
  //Serial.begin(115200);
  pinMode(Datapin, OUTPUT);
  pinMode(Clkpin, OUTPUT);
  Send32Zero(); // blank all outputs in string
  //Feel free to leave this next section out until ^^^^ if you don't want to see when the Arduino inits
  for(int i = 0; i < 12; i++)
  {
    //DataDealWithAndSend(255, 255, 255);
    //DataDealWithAndSend(255, 0, 0);
    //DataDealWithAndSend(0,255, 0);
    DataDealWithAndSend(0, 0, 255);
  }

  Send32Zero();
  delay(50);
  Send32Zero(); // blank all outputs in string
  
  for(int i = 0; i < 12; i++)
  {
    DataDealWithAndSend(0, 0, 0);
  }

  Send32Zero();

  delay(3000);
}


//==============================================================================
// Главный цикл программы
//==============================================================================
void loop()
{
  int brightness = (int)(analogRead(A0)/BRIGHTNESS);
  if(brightness < 2) brightness = 2;
  //Serial.println(brightness);
  
  for(int i = 0; i < 12; i++)
  {
    DataDealWithAndSend(
      (rand() % brightness), 
      (rand() % brightness), 
      (rand() % brightness)
      );
  }

  Send32Zero();
  delay(50);
  Send32Zero(); // blank all outputs in string
  
  for(int i = 0; i < 12; i++)
  {
    DataDealWithAndSend(0, 0, 0);
  }

  Send32Zero();
  
}//End of void loop()
