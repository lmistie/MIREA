//---------------------------------------------------------------------------
// 
//  ���������������� ������
//
//  ������ � ���������
//
//  Copyright (C) 2013 ���� �����
//
//---------------------------------------------------------------------------

#include "stm32f4xx.h"                  //��������� � ��������� ������ ��� ����������� ��������� STM32F4xx

#define  TIM_GEN_PRESCALER  84          //����������� �������� ��������� ��� �������-����������
#define  TIM_GEN_PERIOD     1000
#define  TIM_DUR_PRESCALER  33600       //����������� �������� ��������� ��� �������-������������� ������������
#define  TIM_DUR_PERIOD     10000

//---------------------------------------------------------------------------
// ������������� �������-���������� �������
/* ������ 3 ��������� � ���� APB1, ������� ������������ ������� � 4 ���� ���� ���������:
   168 ��� / 4 = 42 ���. ��� ��� ����������� ������� ������� ��� ������ ���� �� 1, 
   �� �� ������� ���� ���� ��������� ��������� �������� �������, �.�. 84 ���.
   ��� ��������� �� ������ ������� 1 ��� ����������� ������� ������ ���� ����� 84000.
   ��� ��� ������ 16-���������, �� ���������� ������������ �����������.
   � ������� ���� ����������� ���������������� ������� (TIM_Prescaler) - 84,
   ����������� ������� �������� ������� (TIM_Period) - 1000.
   ����� ������� - ����� CH4 - ������� �� ������ ����� PB1.
   �� ������ CH4 ������������ ������������� � ���, ����������� ���������� ������������
   �������� ������ (duty cycle) = 40%, ����������� ����� ��� �����
   TIM_Pulse = TIM_Period * duty cycle[%] / 100[%] = TIM_Period * 4 / 10
*/   
void TimerGenerationConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                  //��������� ������������ ������ ������ ����������
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //��������� ������������ �������� �������
  TIM_OCInitTypeDef  TIM_OCInitStructure;               //��������� ������������ ������� � ���

  //���������� ������������ TIM3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  //���������� ������������ ����� PB
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  //���������������� ������� ����� PB1 ��� ��������� � �������������� ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;             //����� ������� (�����)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          //�������������� �������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //������� ��������������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //��������� ���� ��������� �������: �����������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //������������� ��������: � "+" ������� 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  //����������� ������ ������ CH4 ������� TIM3 � ������� ����� PB1
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); 

  //������� ������������ �������
  TIM_TimeBaseStructure.TIM_Period = TIM_GEN_PERIOD-1;          //�������� ����. �������
  TIM_TimeBaseStructure.TIM_Prescaler = TIM_GEN_PRESCALER-1;    //����������� �����������
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //�������� ��� �������� ������� (�� ������������)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //����� ����� �����: �� 0 �� TIM_Period
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);               //������� ���������������� 

  //���������������� ���
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //����� ������: ��� ���� 1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //���������� ����������� ������
  TIM_OCInitStructure.TIM_Pulse = TIM_GEN_PERIOD * 4 / 10;      //������������ ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //���������� �������� - �������������
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);                      //������� ����������������
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);             //���������� �������� �������� ���

  //����� ���������� ���� ��������: ����� TIM1 -> ���� TIM3 ����� ITR0
  TIM_SelectInputTrigger(TIM3, TIM_TS_ITR0);

  //������� ������ ��������, ������ ����� 
  TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Gated);

  //����� ���������� ������ �������, ��� ���� �������� ���� ������������ �������� �� �������� �������
  TIM_Cmd(TIM3, ENABLE);
}

//---------------------------------------------------------------------------
// ������������� �������-������������� ������������
/* ������ 1 ��������� � ���� APB2, ������� ������������ ���� - 84 ���, 
   ������� ������������ �������� �� ���� ���� - 168 ���.
   ��� ���������� ��������� �������� 2 � ��������� ����� ����������� �������
   168e6 / (1/2) = 336 000 000, ������� ����� �����������, ��������, ��� 33 600 * 10 000
   ������ ��������������� � �������:
     - Master - ����� �������� ��� ������ ��������, � ������ ������ - ��� ������� 3;
     - ��������� ���������� �������� (� �������������� �� ������������);
     - ������ ����� ���������� ������ ��� ��������� �������.
    ���������� ������ ������� ������ � �������� ���������. 
    ��� ���� ������� �������� �������, ����������� ������ �������� �������,
    ��������������� � �������� ��������� ��� ������ ������� �������. �� ����������
    ���c��������� ��������� �������� (������������) ������� ��������������� (����������),
    ������ ���������������, �������� ������� ��������� ���������� �������.
*/
void TimerDurationConfig(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //��������� ������������ �������� �������

  //���������� ������������ �������
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  //������� ������������ �������
  TIM_TimeBaseStructure.TIM_Period = TIM_DUR_PERIOD-1;          //�������� ����. �������
  TIM_TimeBaseStructure.TIM_Prescaler = TIM_DUR_PRESCALER-1;    //����������� �����������
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //�������� ��� �������� ������� (�� ������������)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //����� ����� �����: �� 0 �� TIM_Period
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);               //������� ���������������� 

  //������� ������ ��������� ���������� ��������
  TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Single);
  
  //���������� ������ �������� (Master)
  TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);

  //����� ����� ���������� ������ � �������� ��������� ������� ������� ��� �������� ������� 
  TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Enable);
}

//---------- ����� ����� timer2.c -------------------------------------------
