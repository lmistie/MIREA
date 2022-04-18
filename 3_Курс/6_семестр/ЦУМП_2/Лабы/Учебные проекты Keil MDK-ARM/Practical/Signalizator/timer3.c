//---------------------------------------------------------------------------
// 
//  ���������������� ������
//
//  ������ � ��������� � ���
//
//  Copyright (C) 2013 ���� �����
//
//---------------------------------------------------------------------------

#include "stm32f4xx.h"                  //��������� � ��������� ������ ��� ����������� ��������� STM32F4xx

#define  F_DISCRET          16000                    //������� �������������, ��
#define  TIM_DISCRET_PERIOD (84000000/F_DISCRET-1)   //�������� ��� �������� � ������

//---------------------------------------------------------------------------
// ������������� ������� - ��������� ������� �������������
/* ������ 4 ��������� � ���� APB1, ������� ������������ ������� � 4 ���� ���� ���������:
   168 ��� / 4 = 42 ���. ��� ��� ����������� ������� ������� ��� ������ ���� �� 1, 
   �� �� ������� ���� ���� ��������� ��������� �������� �������, �.�. 84 ���.
   ��� ������� ������������� 16 ���:
    - ������������ �� ������������ (����. ������� = 1)
    - �����, ����������� � �������: TIM_Period = 84 ��� / 16 ��� - 1 = 5249
   ���������� �� ������� ��������������� �� ������� ��� ������������.
*/ 
void TimerDiscretConfig(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //��������� ������������ �������� �������

  //���������� ������������ �������
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  //������� ������������ �������
  TIM_TimeBaseStructure.TIM_Period = TIM_DISCRET_PERIOD;        //�������� ����.�������
  TIM_TimeBaseStructure.TIM_Prescaler = 0;                      //����������� �� ������������
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //�������� ��� �������� ������� (�� ������������)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //������� �����: �� 0 �� TIM_Period
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);               //������� ���������������� 

  //����������� ����������� ������ ������� TRGO, ������� ����� �������������� ��� ������� ���
  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);

  //����� ������� ������������ ������� ��� ��������� ����������
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  
  //����� ���������� ������ �������
  TIM_Cmd(TIM4, ENABLE);
}

//---------------------------------------------------------------------------
// ����������/������ ���������� �� ������� ������� �������������
// ������� ��������: NewState - true (���������), false (���������)
void TimerDiscretInterrupt(bool NewState)
{
  NVIC_InitTypeDef NVIC_InitStructure;                      //��������� ������������ ����������

  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //����� ������/��������� (��������� � stm32f4xx.h)
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //������� ���������� (0 - ������, 15 - ������)
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //��������� ������ ������
  NVIC_InitStructure.NVIC_IRQChannelCmd = (FunctionalState)NewState; //����������/������
  NVIC_Init(&NVIC_InitStructure);                           //������� ����������������
}

//---------------------------------------------------------------------------
// ������������� ���
// ������������ DAC1 � ������� �� ������ ����� PA4
void DACConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                  //��������� ������������ ������ ������ ����������
  DAC_InitTypeDef  DAC_InitStructure;                   //��������� ������������ ���

  //���������� ������������ ����� PA, ������� �������� ������������ ��� ������ ���
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  //���������� ������������ ���
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  //���������������� ������� PA.4 ��� �����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                        //����� ������� (�����) ��� DAC1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;                     //�������������� �������: ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                 //��� ������������� ����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                           //������� ����������������
   
  //���������������� ������� ������ ���
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;             //������ �� ������� TIM4
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;  //��� ����������� ���������
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095; //�������� �����
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;    //���������� ��������� ������
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);                     //������� ����������������

  //����� ���������� ������ ���, �������� ���������� �������� ������ ���
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_L, 0x0000);
}

//---------------------------------------------------------------------------
// ������������� ������� ���������� ����������
/* ������ 10 ��������� � ���� APB2, ������� ������������ ���� - 84 ���, 
   ������� ������������ �������� �� ���� ���� - 168 ���.
   �������� ������ ������� TIM10_CH1 ����� ���� ������� �� ������ ����� PF6 (� �����������),
   � ���� ������ �� ������ ����������������� � �������� ��� �������� - ����������� ������ ��.
   � �������� 2 �������. ����� ������ ����� ����������� ������ ��� ������� ���������
   ��������, ������� � ��������� 1 � 2 ������� ������������� ���������� (��������� blink_counter).
   ������� ��������� ��������� �����������:
   � ������ ��������: ������ ������� 1000 ��, ������������ ����������� ��������� 10 ��.
   � ������ ���������: ������ ������� 100 ��, ������������ ����������� ��������� 60 ��.
   ��� �������� ���������� � main3.c � ������� � �������������. ��������������� � ������� ������� 
   ��� �� ����� ���� ���������, ��� ��� ��������������� ����������� ������� ��� ���� ������ ����
   ����� 168 000 000 �� / (1/1e-3 �) = 168 000, ��� ������ ������������� 16-���������� ��������.
   ������� ��������������� ����������� ������� �������� � 10 ��� (�� 16 800), � ����������� ��������
   � ������������� � �������� �������� ������������� � 10 ���.
   �� ��������� ��������� �������� ������������ ���������� �� ������� ���������������� ��������.
*/
void TimerIndicatorConfig(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;               //��������� ������������ �������� �������
  NVIC_InitTypeDef NVIC_InitStructure;                          //��������� ������������ ����������

  //���������� ������������ �������
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
  
  //������� ������������ �������
  TIM_TimeBaseStructure.TIM_Period = 1000;                      //�������� ����. ������� (������ �� �����)
  TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;                //����������� �����������
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //�������� ��� �������� ������� (�� ������������)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //����� ����� �����: �� 0 �� TIM_Period
  TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);              //������� ���������������� 

  //����� ������� ������������ ������� ��� ��������� ����������
  TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);

  //���������������� ���������� �� �������
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;      //����� ������/��������� ����������
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //������� ���������� (0 - ������, 15 - ������)
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            //��������� ������ ������
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //���������� ����������
  NVIC_Init(&NVIC_InitStructure);                               //������� ����������������

  //����� ���������� ������ �������
  TIM_Cmd(TIM10, ENABLE);
}

//---------- ����� ����� timer3.c -------------------------------------------
