/**
  ******************************************************************************
  * ����      lcd.c
  * �����     �����
  * ������    V1.2
  * ����      01.01.2019
  * ��������  ������� �������� ������ ��� ������ � LCD �������� �� ����� STM32-P407
  ******************************************************************************
  */ 

/* ������������ ����� */
#include "drv_glcd_cnfg.h"
#include "drv_glcd.h"
#include "glcd_ll.h"
#include "lcd.h"

pFontType_t  _Font_Type;                //������� ��� (������) ������
unsigned int _Font_Color;               //������� ���� �������� ������
unsigned int _Back_Color;               //������� ���� ���� ������� � ������
unsigned int _Pen_Color;                //������� ���� �����
extern FontType_t Terminal_9;           //��������� ��������� �������
extern FontType_t Terminal_11;
extern FontType_t Terminal_14;
extern FontType_t Terminal_18;

/*******************************************************************************
* �������:   LCD_Init
*            ������������� �������
* ���������: ���
* �������:   ���
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
* �������:   LCD_Logo
*            ����������� ����������� �������� (��������)
* ���������: ���
* �������:   ���
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
  GLCD_print("   �����  ");

  GLCD_SetFont(&Terminal_9,0x00000F,0x00FFF);
  GLCD_TextSetPos(0,2);
  GLCD_print(" ������� �����������.\r\n ������ � ����������\r\n");

  GLCD_SetFont(&Terminal_9,0x000F00,0x00FFF);
  GLCD_print(" ...................");
  
  GLCD_SetFont(&Terminal_9,0x000000,0x00FFF);
  GLCD_TextSetPos(0,5);
  GLCD_print("  ������������ ���\r\n    � �����������\r\n  �����������������\r\n      �������!");
  
}

/*******************************************************************************
* �������:   LCD_Clear
*            ������� �������
* ���������: back - 0 (�� ���������) ������� ���, 1 - ������ ���
* �������:   ���
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
* �������:   LCD_FontSize
*            ��������� ������� ������
* ���������: Size - ������ ��������, �����
* �������:   ���
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
* �������:   LCD_FontColor
*            ��������� ����� ��������
* ���������: Color - ���� (0x000...0xFFF) � ������� 0xBGR (R,G,B - 0...F)
* �������:   ���
*/
void LCD_FontColor(unsigned int Color)
{
  if (Color > 0xFFF) Color = 0xFFF;
  _Font_Color = Color;
  //if (_Font_Color == 0xFFF) _Back_Color = 0x000;
  GLCD_SetFont(_Font_Type, _Font_Color, _Back_Color);
}

/*******************************************************************************
* �������:   LCD_BackColor
*            ��������� ����� ���� ��� ������ ��������
* ���������: Color - ���� (0x000...0xFFF) � ������� 0xBGR (R,G,B - 0...F)
* �������:   ���
*/
void LCD_BackColor(unsigned int Color)
{
  if (Color > 0xFFF) Color = 0xFFF;
  _Back_Color = Color;
}

/*******************************************************************************
* �������:   LCD_SetWindow
*            ��������� ���� ��� ������
* ���������: Left, Top, Right, Bottom - ���������� � �������� (0...)
* �������:   ���
*/
void LCD_SetWindow(int Left, int Top, int Right, int Bottom)
{
  GLCD_SetWindow(Left, Top, Right, Bottom);
}

/*******************************************************************************
* �������:   LCD_TextPos
*            ��������� ������� ��� ������������ ������ ��������
* ���������: X, Y - ���������� � ���������� �������� (0...)
* �������:   ���
*/
void LCD_TextPos(unsigned int X, unsigned int Y)
{
  GLCD_TextSetPos(X, Y);
}

/*******************************************************************************
* �������:   LCD_print
*            ��������������� ����� ������
* ���������: string
* �������:   ���
*/
//������� ����������� � drv_glcd.c
//void LCD_print(const char * string, ...)

/*******************************************************************************
* �������:   LCD_OnOff
*            ��������� / ������� �������
* ���������: On - 1 ��������, 0 - ���������
* �������:   ���
*/
void LCD_OnOff(unsigned int On)
{
  GLCD_SendCmd(On ? DISPON : DISPOFF, NULL, 0);
  GLCD_Backlight(On ? BACKLIGHT_ON : 0);
}

/*******************************************************************************
* �������:   LCD_OnOffSmooth
*            ������� ��������� / ������� �������
* ���������: On - 1 ��������, 0 - ���������
* �������:   ���
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
* �������:   LCD_PenColor
*            ������� ����� �����
* ���������: Color - ���� (0x000...0xFFF) � ������� 0xBGR (R,G,B - 0...F)
* �������:   ���
*/
void LCD_PenColor(unsigned int Color)
{
  _Pen_Color = Color;  
}

/*******************************************************************************
* �������:   LCD_Rectangle
*            ���������� ������������ ��������������
* ���������: x1, x2, y1, y2 - ���������� � �������� (0...), ������������
* �������:   ���
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
* �������:   LCD_Picture
*            ���������� ���������� �����������
* ���������: pData - ��������� �� ���������� ������ � ������� 0x0BGR, �������
*                    ���������� ������: ����� �������, ������ ����.
* �������:   ���
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
