/*
 Аппаратная конфигурация OLED дисплея на плате Accessory Shield
 */
#ifndef C_CONFIG
#define C_CONFIG

 //========================================================================
 //------------------------- АППАРАТНАЯ КОНФИГУРАЦИЯ ----------------------
 //========================================================================

#define RESET_PIN 7 			// Вывод, используемый для аппаратного сброса

#define SMALL_STRING_HEIGHT 8 	// Высота строки для шрифта SmallFont
#define SMALL_SCREEN_WIDTH 21 	// Ширина строки в символах для шрифта SmallFont
#define SMALL_SCREEN_HEIGHT 8 	// Высота строки в символах для шрифта SmallFont

#define SCREEN_HEIGHT 64        // Высота экрана в пикселях
#define SCREEN_WIDTH  128       // Ширина экрана в пикселях

 //========================================================================
 //-------------------------------- КОНСТАНТЫ -----------------------------
 //========================================================================
 
 #define TONE_FREQUENCY 1000    //Частота тонального сигнала
 #define TONE_DWELL 100         //Длительность тонального сигнала

 #define UDP_BUFFER_SIZE 128    //Размер буфера для приёма и передачи данных по UDP

 #define BASE_HEADER_LENGTH 11  //Длина заголовка базового протокола

 #define STRING_0 0
 #define STRING_1 8
 #define STRING_2 16
 #define STRING_3 24
 #define STRING_4 32
 #define STRING_5 40
 #define STRING_6 48
 #define STRING_7 56
 
 //--------------------------------------------------------------------------------
#endif
