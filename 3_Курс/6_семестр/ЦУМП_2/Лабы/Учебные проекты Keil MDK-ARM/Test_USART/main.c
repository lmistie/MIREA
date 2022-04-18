//---------------------------------------------------------------------------
// 
//  �������� ������
//  ������ � ���������������� ����������� RS-232 (USART)
//  (C) 2016 �����
//
//  ��� �������� ������ ��������� ����������� ������� DB-9M - DB-9F
//  ����������� ���������� ����� RS232_1 � COM-���� ����������.
//  �� ���������� ��������� ������������ ��������� � ����������� �����:
//  9600 ���/�, 8 ��� ������, ��������: ���, 1 ����-���, ���.�������: ���.
//
//---------------------------------------------------------------------------

#include "stm32_p407.h"                 //���� ������������ ���������� �����
#include "lcd.h"                        //������� ��� ������ � ��������
#include "usart.h"                      //������� ��� ������ � �����������
#include <stdio.h>                      //����������� ������� C/C++ (����� - sprintf)

#define FONTSIZE   9                    //������ ������ ��� ������ �� �������
#define SYMPERSTR  20                   //����� �������� � ������ �������

//---------------------------------------------------------------------------

int main()
{
  uint8_t sym, n = 0, m;
  char str[64];
    
  //������������� ������ � ���������
  STM_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);   //������� ���������
  STM_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);    // - " -
  STM_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);      // - " -
  STM_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);    // - " -
  STM_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);     // - " -

  //������������� �������
  LCD_Init(); LCD_Clear(1); LCD_FontSize(FONTSIZE);
  LCD_print("   �������� ������\r\n     U S A R T\r\n");

  //������������� ����������������� ����������
  USART_Config();

  //���� ������������
  while (1) 
  {
    //�������� ������� �������� ������ (�����)
    if (USART_RX_Flag())
    {
      //������ ��������� �����
      sym = USART_RX_Data();
      
      //���� ��� ������������� ������������ ��������, ����� �������, 
      // ����� - ����� ���� � 16-������ �������
      if (' ' <= sym && sym <= '~') LCD_print("%c", sym), n++;
      else LCD_print("\\0x%02X", sym), n += 5;
      if (n >= SYMPERSTR) LCD_print("\r\n"), n = 0;
    }

    //�������� ������� ���������
    if (STM_PBGetState(BUTTON_LEFT))
    { m = sprintf(str, "Left ");                //����� ��������� � �����
      USART_TX_Data(str, m);                    //�������� ������ �� UART
      while (STM_PBGetState(BUTTON_LEFT));      //�������� ����������
    }
    if (STM_PBGetState(BUTTON_RIGHT))
    { m = sprintf(str, "Right ");
      USART_TX_Data(str, m);
      while (STM_PBGetState(BUTTON_RIGHT));
    }
    if (STM_PBGetState(BUTTON_UP))
    { m = sprintf(str, "Up ");
      USART_TX_Data(str, m);
      while (STM_PBGetState(BUTTON_UP));
    }
    if (STM_PBGetState(BUTTON_DOWN))
    { m = sprintf(str, "Down ");
      USART_TX_Data(str, m);
      while (STM_PBGetState(BUTTON_DOWN));
    }
    
  }

}

//---------------------------------------------------------------------------
