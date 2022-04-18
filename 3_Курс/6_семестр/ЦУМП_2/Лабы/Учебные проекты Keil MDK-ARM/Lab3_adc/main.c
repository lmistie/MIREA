//---------------------------------------------------------------------------
// 
//  ������� ������
//  ��������� � ������� �������-��������� ���������������
//  Copyright (C) 2016 �����
//
//  ��� ������������ ������ ��� ������������ ������������� �� ����� ������������
//  (�������), ����������� �������� �� ������� ����� PC0 ����� �������� ����������
//  � �������� 0...3.3 �. ������ PC0 ����� �������������� �������: ���������� �����
//  ADC1_IN10, ADC2_IN10, ADC3_IN10.
//  ������ ������������ ��������� �������� ���������� ������� �������, ��������
//  �� ������� �������� �� �������� �������.
//
//---------------------------------------------------------------------------

#include "stm32_p407.h"                         //���� ������������ ���������� ����� STM32-P407
#include "adc.h"                                //������� ��� ������ � ���
#include "lcd.h"                                //������� ��� ������ � ��������

#define  AVERAGE  200                           //����� ���������� ������ � ���

//---------------------------------------------------------------------------
// ������� �������
int main()
{
  long AdcData;                                 //������������� ������ � ���
  unsigned short UData16;                       //����������� ��������� (�����������) ��� 
  short Data16;                                 //����������� �������������� (��������) ���
  int Average = 1;                              //������� ����� ����������
  int i;                                        //������� ���������� �����
  NVIC_InitTypeDef NVIC_InitStructure;          //��������� ������������ ����������
  
  //������������� ������
  STM_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO);  //������ WAKEUP ������ �� ���������
  STM_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);   //������� ��������� ��� ��������� ������ ����������
  STM_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);    //������� ��������� ��� ���������� ������ ����������
  STM_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);    //������� ��������� ��� ��������� ��������� ���������

  //������������� �����������
  STM_LEDInit(LED3); STM_LEDOff(LED3);
  
  //������������� �������
  LCD_Init(); LCD_Clear(1);
  LCD_FontSize(11);
  LCD_FontColor(0x0F0);
  LCD_TextPos(1,1); LCD_print("��������.�����");
  LCD_TextPos(1,4); LCD_print("��������.������");
  LCD_FontColor(0x08F);
  LCD_TextPos(1,2); LCD_print("����.  ������.");
  LCD_TextPos(1,5); LCD_print("����.  ������.");
  LCD_FontColor(0xFFF);

  //������������� � ��������� ������ ���
  ADC_Initialize(ADC2, ADC_Channel_10, 0);
  ADC_SoftwareStartConv(ADC2);

  //���������������� ���������� �� ���
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;                   //����� (�����) ����������
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;        //��������� ������
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;               //��������� ������ ������
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //����������/������ ����������
  NVIC_Init(&NVIC_InitStructure);                                  //������� ����������������

  //���������������� ��� � ������ "����������� �������"
  ADC_AnalogWatchdogSingleChannelConfig(ADC2, ADC_Channel_10);     //����� ������
  ADC_AnalogWatchdogThresholdsConfig(ADC2, 3500, 500);             //������� � ������ ������
  ADC_ITConfig(ADC2, ADC_IT_AWD, ENABLE);                          //���������� ���������� �� "�������"
  ADC_AnalogWatchdogCmd(ADC2, ADC_AnalogWatchdog_SingleRegEnable); //���������� "����������� �������"

  //���� � �������� ���������
  while (1)
  { 
    //���������/���������� ������ ����������
    if (STM_PBGetState(BUTTON_RIGHT))
    { Average = AVERAGE;
      LCD_TextPos(1,0); LCD_print("- ���������� -");
    }
    if (STM_PBGetState(BUTTON_LEFT))
    { Average = 1;
      LCD_TextPos(1,0); LCD_print("              ");
    }
    
    //���������������� ��������� ��������� ��� ������ ���������
    while (STM_PBGetState(BUTTON_DOWN));
    
    //���� �� ����������
    for (i = Average, AdcData = 0; i > 0; i--)
    {
      while (!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)); //�������� ��������� ��������������
      AdcData += ADC_GetConversionValue(ADC2);        //���� � ������������ ���������
      ADC_SoftwareStartConv(ADC2);                    //����� ������ ��������������
      Delay_ms(1);
    }
    
    //�������������� ���� � ���
    UData16 = AdcData / Average;                      //���������� ��������
    Data16 = UData16 ^ 0x8000;                        //�������������� ���������� ���� � ��������������

    //����� ����������� �� �������
    LCD_TextPos(1,3); LCD_print("%5d", UData16);      //��������� � ������������� �����
    LCD_TextPos(8,3); LCD_print("%6d", Data16);       //�������������� � ������������� �����
    Data16 >>= 4; UData16 >>= 4;                      //�������� ������������ ������
    LCD_TextPos(1,6); LCD_print("%5d", UData16);      //��������� � ������������� ������
    LCD_TextPos(8,6); LCD_print("%6d", Data16);       //�������������� � ������������� ������
    
    if (Average < AVERAGE) Delay_ms(300);             //������ ���������� ��������� 
    STM_LEDOff(LED3);                                 //������� ���������� "����������� �������"
    
    //�������� ������� ������ WAKEUP ���������� ������ (������ ����������)
    if (STM_PBGetState(BUTTON_WAKEUP)) NVIC_SystemReset();
  }
}

//---------------------------------------------------------------------------
// ���������� ���������� �� ���
void ADC_IRQHandler(void)
{
  if (ADC_GetFlagStatus(ADC2, ADC_FLAG_AWD) != RESET) //�������� ����� "����������� �������"
  {
    ADC_ClearFlag(ADC2, ADC_FLAG_AWD);                //����� ����� 
    STM_LEDOn(LED3);                                  //������������ � ������ ������ �� �������
  }
}

//---------------------------------------------------------------------------
