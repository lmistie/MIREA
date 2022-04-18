//---------------------------------------------------------------------------
//
//  ������� ������
//  ������������� ��������
//  Copyright (C) 2015 �����
//
//  � ������� ���������������: 
//   - ��������� ��������� �������������� �������;
//   - ��������� ����� ������� ����������� ���;
//   - ����� ������� ����� ��� �����������.
//  ���������� ����� ������ ���������� � ����� ��� ������������
//  ��������� ������� �������������� ������� ������� ����� PA4.
//
//---------------------------------------------------------------------------

// ������������ �����
#include "stm32_p407.h"                 //���� ������������ ���������� ����� STM32-P407
#include "arm_math.h"                   //����������� � ������� ���������� CMSIS DSP Library
#include "adc.h"                        //������� ��� ������ � ���
#include "dac.h"                        //��������� � ������� ��� ������ � ���
#include "codec.h"                      //������� ��� ������ � �������

//---------------------------------------------------------------------------
// ���������� ������
int16_t DataADC1, DataADC2;             //������� ������� ������� � ���
extern int16_t DataChannel1,            //������ ��� ������ � �������������� ����
               DataChannel2;            // (���������� � ������ codec.c)
TIM_TimeBaseInitTypeDef TimeBase;       //��������� ������������ �������� �������
NVIC_InitTypeDef NVICstruct;            //��������� ������������ ����������

//---------------------------------------------------------------------------
// ������� ������� 
int main()
{
  //������� ��� ���������� ������� ��������� �������� ���������
  volatile uint32_t i = 0;

  //������� 4-� ������� ��������� ����������� � 4-� ������� ����������� � ������ ������
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  //������������� �������� �����������
  STM_LEDInit(LED1); STM_LEDInit(LED2);  STM_LEDInit(LED3); STM_LEDInit(LED4);

  //������������� ������
  STM_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO);
  STM_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO);

  //������������� ��� - ��� ���������� �������� ������� ��� ���
  DACInit(192000);
  DACTimerInterruptConfig(ENABLE);

  //������������� ���������� ������� ������������� ���; ��� ���� ����
  // ������������ ������ 2 � ����������� �������� ������������ 84 ���
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TimeBase.TIM_Period = 840000u/44100-1;
  TimeBase.TIM_Prescaler = 100-1;
  TimeBase.TIM_ClockDivision = 0;
  TimeBase.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TimeBase);
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  TIM_Cmd(TIM2, ENABLE);

  //������������� �������������� ��� � �������� �� ������� ������������ ������� 2
  ADC_Initialize(ADC1, ADC_Channel_4, ADC_ExternalTrigConv_T2_TRGO);
  ADC_Initialize(ADC2, ADC_Channel_4, ADC_ExternalTrigConv_T2_TRGO);

  //���������������� ���������� �� ��� (�� ������� ��������� ��������������)
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  NVICstruct.NVIC_IRQChannel = ADC_IRQn;
  NVICstruct.NVIC_IRQChannelPreemptionPriority = 2;
  NVICstruct.NVIC_IRQChannelSubPriority = 0;
  NVICstruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVICstruct);

  //������������� ������ ��� ���������� ������ �������
  SoundCodecConfig(I2S_AudioFreq_192k);

  //�������� ����
  while (1)
  {
    if (i++ == 0x800) STM_LEDOff(LED1);          //�������� ���������� �����������
    if (i == 0xB0000) STM_LEDOn(LED1), i = 0;

    if (STM_PBGetState(BUTTON_WAKEUP)) NVIC_SystemReset(); //�������� �� ����������
  }
}

//---------------------------------------------------------------------------
// ���������� ���������� �� ������� ������������ ���
void TIMD_IRQHandler(void)
{
  static int16_t arg_sin;
  STM_LEDOn(LED2);
  TIM_ClearITPendingBit(TIMD, TIM_IT_Update);
  DAC_SetChannel1Data(DAC_Align_12b_L, arm_sin_q15(arg_sin)>>1^0x8000);
  arg_sin = (arg_sin + 0x200) & 0x7FFF;
  STM_LEDOff(LED2);
}

//---------------------------------------------------------------------------
// ���������� ���������� �� ���
void ADC_IRQHandler(void)
{
  STM_LEDOn(LED4);
  ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
  DataADC1 = ADC_GetConversionValue(ADC1);
  DataChannel1 = DataADC1 ^ 0x8000;
  DataADC2 = ADC_GetConversionValue(ADC2);
  DataChannel2 = DataADC2 ^ 0x8000;
  STM_LEDOff(LED4);
}

//---------------------------------------------------------------------------
// ������������ ���������� �� ������
// ���������� �� ����������� ���������� ..._IRQHandler(), �������������� � codec.c
void Sample_Handler(void)
{
  //��������� �������� - � DataChannel1, DataChannel2
}

//---------------------------------------------------------------------------
