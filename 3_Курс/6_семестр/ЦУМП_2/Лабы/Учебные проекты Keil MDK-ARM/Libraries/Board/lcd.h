/**
  ******************************************************************************
  * Файл      lcd.h
  * Автор     МИРЭА
  * Версия    V1.1
  * Дата      01.09.2015
  * Описание  Заголовочный файл с функциями верхнего уровня для работы
  *           с LCD дисплеем на плате STM32-P407
  ******************************************************************************
  */ 

/* Заголовочные файлы */

/* Инициализация дисплея */
void LCD_Init(void);

/* Отображение стандартной заставки (логотипа) */
void LCD_Logo(void);

/* Очистка дисплея */
void LCD_Clear(int back);

/* Установка размера шрифта */
void LCD_FontSize(unsigned int Size);

/* Установка цвета символов */
void LCD_FontColor(unsigned int Color);

/* Установка цвета фона при выводе символов */
void LCD_BackColor(unsigned int Color);

/* Установка окна для вывода */
void LCD_SetWindow(int Left, int Top, int Right, int Bottom);

/* Установка позиции для последующего вывода символов */
void LCD_TextPos(unsigned int X, unsigned int Y);

/* Форматированный вывод строки */
void LCD_print(const char * string, ...);

/* Включение / гашение дисплея */
void LCD_OnOff(unsigned int On);

/* Плавное включение / гашение дисплея */
void LCD_OnOffSmooth(unsigned int On);

/* Задание цвета линий */
void LCD_PenColor(unsigned int Color);

/* Построение закрашенного прямоугольника */
void LCD_Rectangle(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2);

/* Построение растрового изображения */
void LCD_Picture(unsigned short* pData);
