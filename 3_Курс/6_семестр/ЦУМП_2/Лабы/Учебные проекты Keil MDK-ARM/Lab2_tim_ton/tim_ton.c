//---------------------------------------------------------------------------
// 
//  ������� ������
//  ������ � ���������. ����� ������� �������
//  Copyright (C) 2016 �����
//
//---------------------------------------------------------------------------

#include "stm32f4xx.h"                  //��������� � ��������� ������ ��� ����������� ��������� STM32F4xx

//---------------------------------------------------------------------------
// ������������� ������� 
/* ������ 3 ��������� � ���� APB1, ����������� �������� 84 ���.
   ������������ � ������������� ������� 42 ������� ������� �� 2 ���.
   �������� ����������� ������� ������������ �������� ������������ ����,
   ��� ���� ������� ������, �������� ������� ���������� ��� � 2 ���� ������,
   ��� ��� ��� ������ ������������ �������� �������� ����������� ���������� �������.
   ����� ������� - ����� CH4 - ������� �� ������ ����� PB1.
*/
void TimerConfig(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;                   //��������� ������������ ������ ������ ����������
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;        //��������� ������������ �������� �������
  TIM_OCInitTypeDef  TIM_OCInitStructure;                //��������� ������������ ������� � ���

  //���������� ������������ �������
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  //���������� ������������ ����� PB
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  //���������������� ������� ����� PB1 ��� ��������� � �������������� ��������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;              //����� ������� (�����)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //�������������� �������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //������� ��������������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //��������� ���� ��������� �������: �����������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //������������� ��������: � "+" ������� 
  GPIO_Init(GPIOB, &GPIO_InitStructure);                 //������� ����������������
  
  //����������� ������ ������ CH4 ������� TIM3 � ������� ����� PB1
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); 

  //������� ������������ �������
  TIM_TimeBaseStructure.TIM_Period = 1000-1;                    //�������� ����.������� �� ���������
  TIM_TimeBaseStructure.TIM_Prescaler = 42-1;                   //����������� �� 2 ���
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //������� �����: �� 0 �� TIM_Period
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);               //������� ���������������� 

  //���������������� ���
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;           //����� ������: ������������ ������
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //���������� ����������� ������
  TIM_OCInitStructure.TIM_Pulse = 0;                            //��������� �������� ��� ������������ ������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //���������� �������� - �������������
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);                      //������� ����������������

  //������ ������� ���� �� ���������
  TIM_Cmd(TIM3, DISABLE);
}

//---------------------------------------------------------------------------
