//---------------------------------------------------------------------------
// 
//  ������������ ������ � ���������
//
//  (C) 2016 �����
//
//  ������������ ������������ ������� ��������� � �������� ������:
//   PE9  - ����� ������� TIM1_CH1
//   PE11 - ����� ������� TIM1_CH2
//   PE10 - ������ ������
//
//  ��� ������ ������ ����� ��������� �������� �������� �������� ��� ����������:
//  ��� ��������� TIM_EncoderMode_TI1 - �� 2, ��� ��������� TIM_EncoderMode_TI12 - �� 4.
//  ��� ����������� ������ �� ������� ������ � ������ ������� ��������� ������
//  ����� 1 � ��������� ��������� � 0 - ��� �������.
//
//---------------------------------------------------------------------------

#include "stm32f4xx.h"          //��������� � ��������� ������ ��� ����������� ��������� STM32F4xx

//---------------------------------------------------------------------------
// ������������� ��������
void EncoderConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                   //��������� ������������ ������ ������ ����������
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;        //��������� ������������ �������� �������
  TIM_ICInitTypeDef  TIM_ICInitStructure;                //��������� ������������ ������� � �������� ���������
  
  //���������� ������������ ����� PE
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  //���������������� ������� PE10 ��� ����������� ����� ������ ����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //����� ������� (�����)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //�������: ����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //������� �������������� (����� �� ���.)
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //��������� ���� ��������� ������� (����� �� ���.)
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //������������� ��������: � "+" ������� 
  GPIO_Init(GPIOE, &GPIO_InitStructure);                 //������� ����������������
  
  //���������������� �������� PE9, PE11 ��� �������������� �������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;//������ �������� (�����)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //�������������� �������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //������� �������������� (����� �� ���.)
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //��������� ���� ��������� ������� (����� �� ���.)
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //������������� ��������: � "+" ������� 
  GPIO_Init(GPIOE, &GPIO_InitStructure);                 //������� ����������������
  
  //����� ������� CH1, CH2 ������� � ��������� ����� (�������������� ������� AF1)
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 

  //���������� ������������ �������
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  //������� ������������ �������
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                    //������������ �������� ��� ����������������
  TIM_TimeBaseStructure.TIM_Prescaler = 0;                      //����������� �� ������������
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //������������ ��� �������� ������� (�� �����.)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //������� �����: �� 0 �� TIM_Period
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;              //����� ���������� ������������ (����� �� ���.)
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);               //������� ���������������� 

  //���������������� ������� ������� �������
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;              //����� ������
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;  //���������� ������ ������� (�� �����)
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//������������ ������� ������� ��������� �������
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;         //������������ ������ ������ �������� ������� 
  TIM_ICInitStructure.TIM_ICFilter = 0xF;                       //��������� �������� �������
  TIM_ICInit(TIM1, &TIM_ICInitStructure);                       //������� ���������������� ��� ������� ������
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;              //����� ������
  TIM_ICInit(TIM1, &TIM_ICInitStructure);                       //������� ���������������� ��� ������� ������
  
  //���������������� ������� � ������ ���������� ��������
  TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);

  //���������� ������ �������
  TIM_Cmd(TIM1, ENABLE);
}

//---------------------------------------------------------------------------
// ��������� ��������� �������� - �������� �������� � ������ ������
// �������� ������������ ����� ���������
void GetEncoderValue(unsigned short * counter_value, unsigned char * button_set)
{
  if (counter_value) *counter_value = TIM_GetCounter(TIM1);
  if (button_set) *button_set = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10);
}

//---------------------------------------------------------------------------
