/**
  ******************************************************************************
  * ����      dac.h
  * Copyright (C) 2015 ���� �����
  * ������    2.1
  * ��������: ������� ������������� � ������������ ���
  ******************************************************************************
  */ 

#ifndef _DAC_H
#define _DAC_H

/* � ��������� ������������ ������ TIM7 ����� ���� ������������ TIM2, TIM4, TIM5, TIM7 */
#define  TIMD                 TIM7
#define  DAC_Trigger_TD_TRGO  DAC_Trigger_T7_TRGO
#define  RCC_APB1Periph_TIMD  RCC_APB1Periph_TIM7
#define  TIMD_IRQn            TIM7_IRQn
#define  TIMD_IRQHandler      TIM7_IRQHandler

/* ����������� ��� ������������� TIM6: 
#define  TIMD                 TIM6
#define  DAC_Trigger_TD_TRGO  DAC_Trigger_T6_TRGO
#define  RCC_APB1Periph_TIMD  RCC_APB1Periph_TIM6
#define  TIMD_IRQn            TIM6_DAC_IRQn
#define  TIMD_IRQHandler      TIM6_DAC_IRQHandler
*/

/* ���������� ������� ��� �� ������������� � ������ ����������� ������� */
void DACInit(uint32_t SamplingFreq);
void DACTimerInterruptConfig(FunctionalState NewState);

//---------------------------------------------------------------------------
#endif /* _DAC_H */
