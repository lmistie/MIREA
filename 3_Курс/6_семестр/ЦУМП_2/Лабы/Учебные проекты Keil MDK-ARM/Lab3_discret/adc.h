/**
  ******************************************************************************
  * ����      adc.h
  * Copyright (C) 2016 �����
  * ������    2.1
  * ��������: ������� ������������� � ������������ ���
  ******************************************************************************
  */ 

#ifndef _ADC_H
#define _ADC_H

/* ���������� ������� ��� �� ������������� � ������ ����������� ������� */
void ADC_Initialize(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint32_t ADC_ExternalTrigConv);
void ADC_Init_MultiChannels(ADC_TypeDef* ADCx, uint8_t ADC_NbrOfConversion, uint32_t ADC_ExternalTrigConv);
void ADC_ChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime,
                       GPIO_TypeDef* GPIO_Port, uint32_t GPIO_Pin);

//---------------------------------------------------------------------------
#endif /* _ADC_H */
