/**
  ******************************************************************************
  * ����      dac.c
  * Copyright (C) 2015 �����
  * ������    2.2
  * ��������: ������� ������������� � ������������ ���
  *
  * ��� ����� ���� ��������������� ��� ������� �� ��������
  *   TIM2, TIM4, TIM5, TIM6, TIM7 ��������������� ���������� � ����� dac.h 
  *   (������ ��������� - EXTI Line9, TIM8 - ������� ����� ������� �����������)
  *         
  * ������ ������ ��� ��� - 12-��������� ��������� ���:
  *   ��� 1111 1111 1111  ������������� U��� = �������� (����������� Vref);
  *   ��� 1000 0000 0000  ������������� U��� = Vref / 2;
  *   ��� 0000 0000 0000  ������������� U��� = ������� (����������� 0); 
  * ��� Vref - ������� ���������� (��� ���������� ����� ����� ���������� �������)
  *
  ******************************************************************************
  */ 

#include "stm32_p407.h"                 //���� ������������ ���������� ����� STM32-P407
#include "dac.h"                        //��������� � ������� ��� ������ � ���

//#define USE_DAC2                      //����������� ��� ������������� ������� ������ ���,
                                        //����� ��������, ���� �� ������������ ����������� �������

void DACTimerConfig(uint32_t SamplingFreq);
void DACConfig(uint32_t DAC_Trigger);

//---------------------------------------------------------------------------
// ������������� ��� � ������� ������������
// ������� �������� - ������� ������������� � ��; ���� ������ ������� ��������,
// �� ������������� �� ������������, ������ ��� ������������ ���������� -
// ������� ������� DAC_SetChannel1Data ��� DAC_SetChannel2Data
void DACInit(uint32_t SamplingFreq)
{
  if (SamplingFreq)
  {
    //������������� ������� - ��������� ������������ ���
    DACTimerConfig(SamplingFreq);

    //������������� ��� � �������� �� �������
    DACConfig(DAC_Trigger_TD_TRGO);
    
    //���������� ���������� �� �������
    //DACTimerInterruptConfig(ENABLE);
  }
  //���� �� ������ ������� �������������, ������������� ���
  // � ������� ������� �� �������� ������
  else DACConfig(DAC_Trigger_None);  
}

//---------------------------------------------------------------------------
// ������������� ������� ��� ������������ ���
/* ������������ ������������ ��� �������� TIM2, TIM4, TIM5, TIM6, TIM7,
   ������������ � ���� APB1. �� ��������� ������� ������ ���� 42 ���,
   ������� ������������ �������� ���� ���� - 84 ���.
   ��� ���������� ����������� ������������ ����������� ������� ��� 16-����������
   ������� ����� 65536, �.�. ����������� ������� ������������� ��� ����� ������
   84000000 �� / 65536 = 1282 ��.
   ��� ������� �������� ������ ���������� ����������� ��� ���������� 32-���������� �������. 
   � ��������� ������������ ��� ������� ������������� SamplingFreq, �������� � ��:
     - ���� ������������ �� ������������ (��� ����. ������� = 1),
       �����, ����������� � �������: TIM_Period = 84000000[��] / SamplingFreq[��] - 1;
     - ���� ����������� ������������ ����� 1000,  
       �����, ����������� � �������: TIM_Period = 84000[��] / SamplingFreq[��] - 1;
   ��� ��� ����� ������ ������ (< 2 ��), � ����� ��������� �������� ����������
   �������� 32-��������� ������ (TIM2, TIM5).
   ���������� �� ������� ��������������� �� ������� ��� ����������������.
*/ 
void DACTimerConfig(uint32_t SamplingFreq)
{
  //��������� ������������ �������� ������� 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  //���������� ������������ �������
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMD, ENABLE);

  //������� ������������ �������
  if (SamplingFreq > 1300)
  { TIM_TimeBaseStructure.TIM_Period = 84000000u/SamplingFreq-1;//�������� ����.�������
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                    //����������� �� ������������
  }
  else
  { TIM_TimeBaseStructure.TIM_Period = 84000u/SamplingFreq-1;  //�������� ����.�������
    TIM_TimeBaseStructure.TIM_Prescaler = 1000-1;              //������������ � 1000 ���
  }
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                 //�������� ��� �������� �������
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //������� �����: �� 0 �� TIM_Period
  TIM_TimeBaseInit(TIMD, &TIM_TimeBaseStructure);              //������� ���������������� 

  //����������� ����������� ������ TRGO ������� ��� ������� ���
  TIM_SelectOutputTrigger(TIMD, TIM_TRGOSource_Update);

  //���������� ���������� ��� ������������ �������
  TIM_ITConfig(TIMD, TIM_IT_Update, ENABLE);
  
  //����� ���������� ������ �������
  TIM_Cmd(TIMD, ENABLE);
}

//---------------------------------------------------------------------------
// ������������� ���
// ������������ DAC1 � ������� �� ������ ����� PA4
//            � DAC2 � ������� �� ������ ����� PA5
// ������� ��������: �������� ������� ��� - ���� �� ���������������� ��������:
//  DAC_Trigger_None, DAC_Trigger_T2_TRGO, DAC_Trigger_T4_TRGO, DAC_Trigger_T5_TRGO,
//  DAC_Trigger_T6_TRGO, DAC_Trigger_T7_TRGO, DAC_Trigger_T8_TRGO, DAC_Trigger_Ext_IT9,
//  DAC_Trigger_Software
void DACConfig(uint32_t DAC_Trigger)
{
  GPIO_InitTypeDef GPIO_InitStructure;            //��������� ������������ ������ ������ ����������
  DAC_InitTypeDef  DAC_InitStructure;             //��������� ������������ ���

  //���������� ������������ ����� PA, ������� �������� ������������ ��� ������ ���
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  //���������� ������������ ���
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  //���������������� �������� PA.4 (PA.5) ��� ����������
#ifdef USE_DAC2  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;          //������ �������� (�����) ��� DAC1, DAC2
#else  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                       //����� ������� (�����) ��� DAC1
#endif  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;                    //�������������� �������: ���������� ����-�����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                //��� ������������� ����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                          //������� ����������������
   
  //���������������� ������� ������ ���
  DAC_InitStructure.DAC_Trigger = DAC_Trigger;                    //�������� �������
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; //���������� ���������: �� ������������ 
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = 0xA00;     //�����/��������� ��� ����������� ����������
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;   //���������� ��������� ������ ���������� ��������
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);                    //������� ���������������� ��� DAC1
#ifdef USE_DAC2
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);                    //������� ���������������� ��� DAC2
#endif  

  //����� ���������� ������ ���, �������� ���������� �������� ������ ��� (��� - ���������)
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_L, 0x0000);
#ifdef USE_DAC2
  DAC_Cmd(DAC_Channel_2, ENABLE);
  DAC_SetChannel2Data(DAC_Align_12b_L, 0x0000);
#endif  

}

//---------------------------------------------------------------------------
// ����������/������ ���������� ���������� �� �������
void DACTimerInterruptConfig(FunctionalState NewState)
{
  NVIC_InitTypeDef NVIC_InitStructure;                      //��������� ������������ ����������

  NVIC_InitStructure.NVIC_IRQChannel = TIMD_IRQn;           //����� (�����) ����������
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��������� ������
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��������� ������ ������
  NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;         //����������/������ ����������
  NVIC_Init(&NVIC_InitStructure);                           //������� ����������������
}

//---------------------------------------------------------------------------
/* � � � � � �
// ���������� ���������� �� �������
// ���������� � �������� �������� ������������� ���.
// ��� ��� �������� � ��� - ���������, ����������� �����.
uint16_t DACData1, DACData2;                                //������ ��� ��� � ��������� ����
void TIMD_IRQHandler(void)
{
  if (TIM_GetITStatus(TIMD, TIM_IT_Update) != RESET)        //�������� ����� ���� ����������
  {                                                         //(�������������, ���� ������ ����� ���)
    TIM_ClearITPendingBit(TIMD, TIM_IT_Update);             //������� �����
    DAC_SetChannel1Data(DAC_Align_12b_L, DACData1);         //�������� ������ � ��� 1
    DAC_SetChannel2Data(DAC_Align_12b_L, DACData2);         //�������� ������ � ��� 2
  }
}
*/
//---------------------------------------------------------------------------
