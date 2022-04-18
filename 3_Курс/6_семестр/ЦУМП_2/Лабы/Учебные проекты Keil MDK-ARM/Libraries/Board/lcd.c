/**
  ******************************************************************************
  * Файл      lcd.c
  * Автор     МИРЭА
  * Версия    V1.2
  * Дата      01.01.2019
  * Описание  Функции верхнего уровня для работы с LCD дисплеем на плате STM32-P407
  ******************************************************************************
  */ 

/* Заголовочные файлы */
#include "drv_glcd_cnfg.h"
#include "drv_glcd.h"
#include "glcd_ll.h"
#include "lcd.h"

pFontType_t  _Font_Type;                //Текущий тип (размер) шрифта
unsigned int _Font_Color;               //Текущий цвет символов шрифта
unsigned int _Back_Color;               //Текущий цвет фона дисплея и шрифта
unsigned int _Pen_Color;                //Текущий цвет линий
extern FontType_t Terminal_9;           //Параметры имеющихся шрифтов
extern FontType_t Terminal_11;
extern FontType_t Terminal_14;
extern FontType_t Terminal_18;

/*******************************************************************************
* Функция:   LCD_Init
*            Инициализация дисплея
* Параметры: Нет
* Возврат:   Нет
*/
void LCD_Init(void)
{
  GLCD_PowerUpInit(NULL);
  GLCD_Backlight(BACKLIGHT_ON);
  _Font_Type = &Terminal_14;
  _Font_Color = DEF_FONT_COLOUR;
  _Back_Color = DEF_BACKGND_COLOUR;
  GLCD_SetFont(_Font_Type,_Font_Color,_Back_Color);
  GLCD_TextSetPos(0,0);
}

/*******************************************************************************
* Функция:   LCD_Logo
*            Отображение стандартной заставки (логотипа)
* Параметры: Нет
* Возврат:   Нет
*/
void LCD_Logo(void)
{
  GLCD_DisplayClear(0);
  GLCD_SetFont(&Terminal_9,0x000F00,0x00FFF);
  GLCD_TextSetPos(0,0);
  GLCD_print("   ===============");

  GLCD_SetFont(&Terminal_14,0x0000F0,0x00FFF);
  GLCD_SetWindow(4, 14, 131, 129);
  GLCD_TextSetPos(0,0);
  GLCD_print("   МИРЭА  ");

  GLCD_SetFont(&Terminal_9,0x00000F,0x00FFF);
  GLCD_TextSetPos(0,2);
  GLCD_print(" Кафедра радиоэлектр.\r\n систем и комплексов\r\n");

  GLCD_SetFont(&Terminal_9,0x000F00,0x00FFF);
  GLCD_print(" ...................");
  
  GLCD_SetFont(&Terminal_9,0x000000,0x00FFF);
  GLCD_TextSetPos(0,5);
  GLCD_print("  Приветствуем вас\r\n    в лаборатории\r\n  микропроцессорной\r\n      техники!");
  
}

/*******************************************************************************
* Функция:   LCD_Clear
*            Очистка дисплея
* Параметры: back - 0 (по умолчанию) светлый фон, 1 - темный фон
* Возврат:   Нет
*/
void LCD_Clear(int back)
{
  GLCD_SetWindow(3, 0, GLCD_HORIZONTAL_SIZE - 1, GLCD_VERTICAL_SIZE - 1);
  GLCD_DisplayClear(back);
  if (back)
  { _Back_Color = 0x000;
    if (_Font_Color == 0x000) _Font_Color = 0xFFF;
  }
  else  
  { _Back_Color = DEF_BACKGND_COLOUR; 
    if (_Font_Color == 0xFFF) _Font_Color = 0x000;
  }
  GLCD_SetFont(_Font_Type,_Font_Color,_Back_Color);
  GLCD_TextSetPos(0,0);
}

/*******************************************************************************
* Функция:   LCD_FontSize
*            Установка размера шрифта
* Параметры: Size - размер символов, точек
* Возврат:   Нет
*/
void LCD_FontSize(unsigned int Size)
{
  if (Size <= 10) _Font_Type = &Terminal_9;
  else if (Size <= 12) _Font_Type = &Terminal_11;
       else if (Size <= 16) _Font_Type = &Terminal_14;
            else _Font_Type = &Terminal_18;
  GLCD_SetFont(_Font_Type, _Font_Color, _Back_Color);
}

/*******************************************************************************
* Функция:   LCD_FontColor
*            Установка цвета символов
* Параметры: Color - цвет (0x000...0xFFF) в формате 0xBGR (R,G,B - 0...F)
* Возврат:   Нет
*/
void LCD_FontColor(unsigned int Color)
{
  if (Color > 0xFFF) Color = 0xFFF;
  _Font_Color = Color;
  //if (_Font_Color == 0xFFF) _Back_Color = 0x000;
  GLCD_SetFont(_Font_Type, _Font_Color, _Back_Color);
}

/*******************************************************************************
* Функция:   LCD_BackColor
*            Установка цвета фона при выводе символов
* Параметры: Color - цвет (0x000...0xFFF) в формате 0xBGR (R,G,B - 0...F)
* Возврат:   Нет
*/
void LCD_BackColor(unsigned int Color)
{
  if (Color > 0xFFF) Color = 0xFFF;
  _Back_Color = Color;
}

/*******************************************************************************
* Функция:   LCD_SetWindow
*            Установка окна для вывода
* Параметры: Left, Top, Right, Bottom - координаты в пикселах (0...)
* Возврат:   Нет
*/
void LCD_SetWindow(int Left, int Top, int Right, int Bottom)
{
  GLCD_SetWindow(Left, Top, Right, Bottom);
}

/*******************************************************************************
* Функция:   LCD_TextPos
*            Установка позиции для последующего вывода символов
* Параметры: X, Y - координаты в символьных позициях (0...)
* Возврат:   Нет
*/
void LCD_TextPos(unsigned int X, unsigned int Y)
{
  GLCD_TextSetPos(X, Y);
}

/*******************************************************************************
* Функция:   LCD_print
*            Форматированный вывод строки
* Параметры: string
* Возврат:   Нет
*/
//Функция реализована в drv_glcd.c
//void LCD_print(const char * string, ...)

/*******************************************************************************
* Функция:   LCD_OnOff
*            Включение / гашение дисплея
* Параметры: On - 1 включить, 0 - отключить
* Возврат:   Нет
*/
void LCD_OnOff(unsigned int On)
{
  GLCD_SendCmd(On ? DISPON : DISPOFF, NULL, 0);
  GLCD_Backlight(On ? BACKLIGHT_ON : 0);
}

/*******************************************************************************
* Функция:   LCD_OnOffSmooth
*            Плавное включение / гашение дисплея
* Параметры: On - 1 включить, 0 - отключить
* Возврат:   Нет
*/
void LCD_OnOffSmooth(unsigned int On)
{
  int i, j;
  if (On)
  { GLCD_SendCmd(DISPON, NULL, 0); 
    for (j = BACKLIGHT_ON/2; j <= BACKLIGHT_ON; j += BACKLIGHT_ON/32)
    { 
      GLCD_Backlight(j);
      i = 1000000; while (i-- != 0);
    }
    GLCD_Backlight(BACKLIGHT_ON);
  }
  else
  {
    for (j = BACKLIGHT_ON-32; j >= BACKLIGHT_ON/4; j -= BACKLIGHT_ON/64)
    { 
      GLCD_Backlight(j);
      i = 1000000; while (i-- != 0);
    }
    GLCD_Backlight(0);
    GLCD_SendCmd(DISPOFF, NULL, 0);
  }
}

/*******************************************************************************
* Функция:   LCD_PenColor
*            Задание цвета линий
* Параметры: Color - цвет (0x000...0xFFF) в формате 0xBGR (R,G,B - 0...F)
* Возврат:   Нет
*/
void LCD_PenColor(unsigned int Color)
{
  _Pen_Color = Color;  
}

/*******************************************************************************
* Функция:   LCD_Rectangle
*            Построение закрашенного прямоугольника
* Параметры: x1, x2, y1, y2 - координаты в пикселах (0...), включительно
* Возврат:   Нет
*/
void LCD_Rectangle(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2)
{
  unsigned int i, j;
  LCD_SET_WINDOW(x1,x2,y1,y2);
  j = (y2 - y1 + 1) * (x2 - x1 + 1);
  for (i = 0; i < j; i++) LCD_WRITE_PIXEL(_Pen_Color);
  LCD_FLUSH_PIXELS();
}

/*******************************************************************************
* Функция:   LCD_Picture
*            Построение растрового изображения
* Параметры: pData - указатель на пиксельные данные в формате 0x0BGR, порядок
*                    следования данных: слева направо, сверху вниз.
* Возврат:   Нет
*/
void LCD_Picture(unsigned short* pData) 
{
  Int32U Data;
  Data = 0 | ((GLCD_HORIZONTAL_SIZE - 1) << 8);
  GLCD_SendCmd(CASET, (pInt8U)&Data, 0);\
  Data = 0 | ((GLCD_VERTICAL_SIZE - 1) << 8);
  GLCD_SendCmd(RASET, (pInt8U)&Data, 0);
  GLCD_SendCmd(RAMWR, (pInt8U)pData, GLCD_HORIZONTAL_SIZE * GLCD_VERTICAL_SIZE * 2);
}
