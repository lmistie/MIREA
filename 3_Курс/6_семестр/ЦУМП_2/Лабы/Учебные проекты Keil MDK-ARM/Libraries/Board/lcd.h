/**
  ******************************************************************************
  * ����      lcd.h
  * �����     �����
  * ������    V1.1
  * ����      01.09.2015
  * ��������  ������������ ���� � ��������� �������� ������ ��� ������
  *           � LCD �������� �� ����� STM32-P407
  ******************************************************************************
  */ 

/* ������������ ����� */

/* ������������� ������� */
void LCD_Init(void);

/* ����������� ����������� �������� (��������) */
void LCD_Logo(void);

/* ������� ������� */
void LCD_Clear(int back);

/* ��������� ������� ������ */
void LCD_FontSize(unsigned int Size);

/* ��������� ����� �������� */
void LCD_FontColor(unsigned int Color);

/* ��������� ����� ���� ��� ������ �������� */
void LCD_BackColor(unsigned int Color);

/* ��������� ���� ��� ������ */
void LCD_SetWindow(int Left, int Top, int Right, int Bottom);

/* ��������� ������� ��� ������������ ������ �������� */
void LCD_TextPos(unsigned int X, unsigned int Y);

/* ��������������� ����� ������ */
void LCD_print(const char * string, ...);

/* ��������� / ������� ������� */
void LCD_OnOff(unsigned int On);

/* ������� ��������� / ������� ������� */
void LCD_OnOffSmooth(unsigned int On);

/* ������� ����� ����� */
void LCD_PenColor(unsigned int Color);

/* ���������� ������������ �������������� */
void LCD_Rectangle(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2);

/* ���������� ���������� ����������� */
void LCD_Picture(unsigned short* pData);
