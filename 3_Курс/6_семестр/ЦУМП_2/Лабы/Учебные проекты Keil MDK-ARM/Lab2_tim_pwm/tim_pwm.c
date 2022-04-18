//---------------------------------------------------------------------------
// 
//  ������� ������
//  ������ � ���������. ����� ��������� ���-�������
//  Copyright (C) 2016 �����
//
//---------------------------------------------------------------------------

#include "stm32f4xx.h"                  //��������� � ��������� ������ ��� ����������� ��������� STM32F4xx

//---------------------------------------------------------------------------
// ������������� �������� 
/* ������� 10,11 ���������� � ���� APB2, ����������� �������� 168 ���.
   �����������: 168 000 ��� / 16800 = 10 ���.
   �������� ������� ��� TIM10: 10000 �� / 10000 = 1 ��    (TIM_Period = 10000-1).
   �������� ������� ��� TIM11: 10000 �� / 20000 = 0.5 ��  (TIM_Period = 20000-1).
   ����� TIM10_CH1 ��������� � ���������� LED1 (������ ����� PF6).
   ����� TIM11_CH1 ��������� � ���������� LED2 (������ ����� PF7).
   ��� TIM10 ����� ����������� ���������� ������������ ��������
   (duty cycle) = 10%, ����������� ����� ��� �����
   TIM_CCR = TIM_Period * duty cycle[%] / 100[%] = 10000 * 10 / 100 = 1000.
   ��� ��� TIM11  (duty cycle) = 75%,  TIM_CCR = 20000 * 75 / 100 = 15000.
   ���������� �� �������� 10, 11 �� ������������.
*/ 

void TimerConfig(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;                   //��������� ������������ ������ ������ ����������
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;        //��������� ������������ �������� �������
  TIM_OCInitTypeDef  TIM_OCInitStructure;                //��������� ������������ ������� � ���
  NVIC_InitTypeDef NVIC_InitStructure;                   //��������� ������������ ����������

  //���������� ������������ TIM10, TIM11
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10 | RCC_APB2Periph_TIM11, ENABLE);

  //���������� ������������ ����� PF
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  //���������������� �������� ����� PF6, PF7 ��� �������� � �������������� ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //����� ������� (�����)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //�������������� �������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //������� ��������������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //��������� ���� ��������� �������: �����������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //������������� ��������: � "+" ������� 
  GPIO_Init(GPIOF, &GPIO_InitStructure);                 //������� ����������������
  
  //����������� ������ ������ CH1 ������� TIM10 � ������� ����� PF6 (��� �������������� ������� AF2)
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10); 

  //����������� ������ ������ CH1 ������� TIM11 � ������� ����� PF7 (��� �������������� ������� AF2)
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11); 
  
  //������� ������������ �������
  TIM_TimeBaseStructure.TIM_Period = 10000-1;                   //�������� ����.������� ��� TIM10
  TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;                //�����������
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //�������� ��� �������� ������� (�� ������������)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //������� �����: �� 0 �� TIM_Period
  TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);              //������� ���������������� 

  TIM_TimeBaseStructure.TIM_Period = 20000-1;                   //�������� ����.������� ��� TIM11
  TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure);              //������� ���������������� 

  //���������������� ���
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //����� ������: ���
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //���������� ����������� ������
  TIM_OCInitStructure.TIM_Pulse = 1000;                         //����������� ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //���������� �������� - �������������
  TIM_OC1Init(TIM10, &TIM_OCInitStructure);                     //������� ����������������

  TIM_OCInitStructure.TIM_Pulse = 15000;
  TIM_OC1Init(TIM11, &TIM_OCInitStructure);

  //����� ���������� ������ ��������
  TIM_Cmd(TIM10, ENABLE);
  TIM_Cmd(TIM11, ENABLE);


  // ���������������� ������� 4
  /* ��������� � ���� APB1, ����������� �������� 84 ���.
     �����������: 84000 ��� / 42000 = 2 ���.
     �������� �������: 2000 �� / 200 = 10 �� (������ 100 ��).
     ���������� ����� �� ������������.
     ������ ������������ ��� ��������� ���������� ��� ����������������.
  */
  //���������� ������������ TIM4
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  //������� ������������ �������
  TIM_TimeBaseStructure.TIM_Period = 200-1;                     //�������� ����.�������
  TIM_TimeBaseStructure.TIM_Prescaler = 42000-1;                //�����������
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //������� �����: �� 0 �� TIM_Period
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);               //������� ���������������� 

  //���������� ���������� ��� ������������ �������
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  
  //����� ���������� ������ ������� TIM4
  TIM_Cmd(TIM4, ENABLE);

  //���������������� ���������� �� �������
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;               //����� (�����) ����������
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     //��������� ������
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            //��������� ������ ������
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //���������� ����������
  NVIC_Init(&NVIC_InitStructure);                               //������� ����������������
}

//---------------------------------------------------------------------------
