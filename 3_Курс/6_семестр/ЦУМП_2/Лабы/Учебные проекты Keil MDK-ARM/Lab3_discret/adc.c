/**
  ******************************************************************************
  * ����      adc.c
  * Copyright (C) 2016 �����
  * ������    2.1
  * ��������: ������� ������������� � ������������ ���
  *
  * ������ �������� ������ �� ��� - 12-��������� ��������� ���:
  *   ��� 1111 1111 1111  ������������� U�� = �������� (����������� Vref);
  *   ��� 1000 0000 0000  ������������� U�� = Vref / 2;
  *   ��� 0000 0000 0000  ������������� U�� = ������� (����������� 0); 
  * ��� Vref - ������� ���������� (��� ���������� ����� ����� ���������� �������)
  *
  * ������� ������� ���: ������ �������� TIM1, TIM2, TIM3, TIM4, TIM5, TIM8;
  * ������� ������.
  *
  * ������������� ���������� ������ �� �������� ������ � ����������:
  *   ����     ADC      Channel
  *   PA0      1,2,3    0
  *   PA1      1,2,3    1
  *   PA2      1,2,3    2
  *   PA3      1,2,3    3
  *   PA4      1,2      4
  *   PA5      1,2      5
  *   PA6      1,2      6
  *   PA7      1,2      7
  *   PB0      1,2      8
  *   PB1      1,2      9
  *   PC0      1,2,3    10
  *   PC1      1,2,3    11
  *   PC2      1,2,3    12
  *   PC3      1,2,3    13
  *   PC4      1,2      14
  *   PC5      1,2      15
  *   PF3      3        9
  *   PF4      3        14
  *   PF5      3        15
  *   PF6      3        4
  *   PF7      3        5
  *   PF8      3        6
  *   PF9      3        7
  *   PF10     3        8
  *    -       1        16   (���������� ������������� ������)
  *    -       1        17   (������� ���������� Vref ����������� ���������)
  *    -       1        18   (���������� ������� Vbat)
  *
  * ����������.
  *   1. �������� ����������� � �������������� ������� ����������� �� �������:
  *          ����������� � �C = {(VSENSE � V25) / Avg_Slope} + 25,
  *      ��� VSENSE - ���������� ���������� � �������;
  *          V25 - ���������� � ������� ��� 25�C, ��� STM32F407 ����� 0.76 �;
  *          Avg_Slope - ������������� �����������, ��� STM32F407 ����� 2.5 ��/�C.
  *      ����� �������������� ��� ������ ���� �� ����� 10 ���.
  *   2. �� ����� ������ ��������� ���������� ������� ������� ����������� �������� �� 2.
  *   3. ��� ���������� ��������� �� ���������� ������� ��������� ������������� ��������
  *      ������� ADC_TempSensorVrefintCmd(ENABLE) ��� ADC_VBATCmd(ENABLE).
  *
  ******************************************************************************
  */ 

#include "stm32_p407.h"                         //���� ������������ ���������� ����� STM32-P407
#include "adc.h"                                //������� ��� ������ � ���

//---------------------------------------------------------------------------
// ������������� ��� ��� ������ � ��������� �������
// ������� ��������� �������:
//   ADCx - ��������� �� ������: ADC1, ADC2, ADC3
//   ADC_Channel - ����� ������: ADC_Channel_0...ADC_Channel_18
//   ADC_ExternalTrigConv - �������� �������, ���� �� �������� ��������:
//     ADC_ExternalTrigConv_T1_CC1
//     ADC_ExternalTrigConv_T1_CC2
//     ADC_ExternalTrigConv_T1_CC3
//     ADC_ExternalTrigConv_T2_CC2
//     ADC_ExternalTrigConv_T2_CC3
//     ADC_ExternalTrigConv_T2_CC4
//     ADC_ExternalTrigConv_T2_TRGO
//     ADC_ExternalTrigConv_T3_CC1
//     ADC_ExternalTrigConv_T3_TRGO
//     ADC_ExternalTrigConv_T5_CC1
//     ADC_ExternalTrigConv_T5_CC2
//     ADC_ExternalTrigConv_T5_CC3
//     ADC_ExternalTrigConv_T8_CC1
//     ADC_ExternalTrigConv_T8_TRGO
//     ADC_ExternalTrigConv_Ext_IT11
//   ���� ��������������� �������� �� ��������������� ��� �������, �������� ����� ���� �����, ��������, 0,
//   � ���� ������ ������ �������������� ���������� - ������� ������� ADC_SoftwareStartConv.
//
void ADC_Initialize(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint32_t ADC_ExternalTrigConv)
{
  ADC_CommonInitTypeDef ADC_CommonInitStruct;   //��������� ����� ������������ ���
  ADC_InitTypeDef ADC_InitStructure;            //��������� ������������ ���
  GPIO_InitTypeDef GPIO_InitStructure;          //��������� ������������ ������ ������ ����������
  GPIO_TypeDef* GPIO_Port;                      //��������� �� ������� ����� �����
  uint32_t RCC_AHB1Periph;                      //�������� ��� ���������� ������������ �����
  uint16_t GPIO_Pin;                            //����� �� ������ �����
  uint8_t flag_port = 1;                        //������� ������������� ������� ����� ��� ��������
  
  //������������� ������� ��� � ������� �������� ������
  switch (ADC_Channel)
  {
    case 0:  RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_0; break;
    case 1:  RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_1; break;
    case 2:  RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_2; break;
    case 3:  RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_3; break;
    case 4:  if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_6; 
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_4;
             break;
    case 5:  if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_7;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_5;
             break;
    case 6:  if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_8;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_6;
             break;
    case 7:  if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_9;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_7;
             break;
    case 8:  if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_10;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOB, GPIO_Port = GPIOB, GPIO_Pin = GPIO_Pin_0;
             break;
    case 9:  RCC_AHB1Periph = RCC_AHB1Periph_GPIOB, GPIO_Port = GPIOB, GPIO_Pin = GPIO_Pin_1; break;
    case 10: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_0; break;
    case 11: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_1; break;
    case 12: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_2; break;
    case 13: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_3; break;
    case 14: if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_4;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_4;
             break;
    case 15: if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_5;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_5;
             break;
    default: flag_port = 0;        
  }
  
  //���������������� ������� ����� ��� ����������� �����
  if (flag_port)
  { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);       //���������� ������������ �����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;               //����� ������� (�����)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;         //�������������� �������: ���������� ����
    GPIO_Init(GPIO_Port, &GPIO_InitStructure);            //������� ����������������
  }

  //���������� ������������ ���
  if (ADCx == ADC1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  if (ADCx == ADC2) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  if (ADCx == ADC3) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  //������� ����� ������������ ��� �� ���������:
  // - ����� ���������� ���;
  // - ��������������� �������� �������� ������� �� 2;
  // - ������ ������ ������� ������� � ������ ��� ���������� ������ ���;
  // - ����� ����� ���������������� ��� ���������� ������ - 5 ������.
  ADC_CommonStructInit(&ADC_CommonInitStruct);
  ADC_CommonInit(&ADC_CommonInitStruct);                         
  
  //���������������� ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //����������� ��������������: 12 (�������� 10, 8, 6)
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;           //�������������� ������������: �� ���������
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;     //����������� ��������������: ���������
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //����� ������� �������
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv;                //�������� �������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;   //������������ ���� � 16-��������� ����: �����
  ADC_InitStructure.ADC_NbrOfConversion = 1;              //����� ������� ��� ��������������: 1
  ADC_Init(ADCx, &ADC_InitStructure);                     //������� ����������������

  //�������������� ��������� ��� ���:
  // - ����� �������� ������,
  // - ����� ������ �������������� - 1 (�������� �� 16 �����),
  // - ����� �������������� - 56 ������ (��������� ��������: 3, 15, 28, 56, 84, 112, 144, 480)
  ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_56Cycles);

  //���������� ������ ���
  ADC_Cmd(ADCx, ENABLE);
}

//---------------------------------------------------------------------------
// ������������� ��� ��� ������ � ����������� ��������
// ������� ��������� �������:
//   ADCx - ��������� �� ������: ADC1, ADC2, ADC3
//   ADC_NbrOfConversion  - ����� ������� (�� 1 �� 16)
//   ADC_ExternalTrigConv - �������� �������, ���� �� �������� (��. ������� ����)
//
void ADC_Init_MultiChannels(ADC_TypeDef* ADCx, uint8_t ADC_NbrOfConversion, uint32_t ADC_ExternalTrigConv)
{
  ADC_CommonInitTypeDef ADC_CommonInitStruct;             //��������� ����� ������������ ���
  ADC_InitTypeDef ADC_InitStructure;                      //��������� ������������ ���

  //���������� ������������ ���
  if (ADCx == ADC1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  if (ADCx == ADC2) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  if (ADCx == ADC3) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  //������� ����� ������������ ��� �� ���������:
  // - ����� ���������� ���;
  // - ��������������� �������� �������� ������� �� 2;
  // - ������ ������ ������� ������� � ������ ��� ���������� ������ ���;
  // - ����� ����� ���������������� ��� ���������� ������ - 5 ������.
  ADC_CommonStructInit(&ADC_CommonInitStruct);
  ADC_CommonInit(&ADC_CommonInitStruct);                         
  
  //���������������� ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //����������� ��������������: 12 (�������� 10, 8, 6)
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;            //�������������� ������������: ���������
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;     //����������� ��������������: ���������
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //����� ������� �������
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv;                //�������� �������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;   //������������ ���� � 16-��������� ����: �����
  ADC_InitStructure.ADC_NbrOfConversion = ADC_NbrOfConversion;                  //����� �������
  ADC_Init(ADCx, &ADC_InitStructure);                     //������� ����������������
}

//---------------------------------------------------------------------------
// ���������������� ������ ��� �������������� ������ ������ ���
// ������� ��������� �������:
//   ADCx - ��������� �� ������: ADC1, ADC2, ADC3
//   ADC_Channel - ����� ������: ADC_Channel_0...ADC_Channel_18
//   Rank  - ����� ������ � ������������������ ������
//   ADC_SampleTime - ����� �������������� � ������, ���� �� ��������:
//     ADC_SampleTime_3Cycles, ADC_SampleTime_15Cycles, ADC_SampleTime_28Cycles, 
//     ADC_SampleTime_56Cycles, ADC_SampleTime_84Cycles, ADC_SampleTime_112Cycles,
//     ADC_SampleTime_144Cycles, ADC_SampleTime_480Cycles
//   GPIO_Port - ������� ����� �����: GPIOA, GPIOB, GPIOC GPIOF
//   GPIO_Pin - ����� ������� ����� � ����� �����: GPIO_Pin_0...GPIO_Pin_15 
void ADC_ChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime,
                       GPIO_TypeDef* GPIO_Port, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;          //��������� ������������ ������ ������ ����������
  uint32_t RCC_AHB1Periph;                      //�������� ��� ���������� ������������ �����

  //���������������� ������� �����
  if (GPIO_Port == GPIOA) RCC_AHB1Periph = RCC_AHB1Periph_GPIOA;
  else if (GPIO_Port == GPIOB) RCC_AHB1Periph = RCC_AHB1Periph_GPIOB;
       else if (GPIO_Port == GPIOC) RCC_AHB1Periph = RCC_AHB1Periph_GPIOC;
            else if (GPIO_Port == GPIOF) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF;
                 else RCC_AHB1Periph = 0;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);       //���������� ������������ �����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;               //����� ������� (�����)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;         //�������������� �������: ���������� ����
  GPIO_Init(GPIO_Port, &GPIO_InitStructure);            //������� ����������������

  ADC_RegularChannelConfig(ADCx, ADC_Channel, Rank, ADC_SampleTime);
}
