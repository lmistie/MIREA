/**
  ******************************************************************************
  * ����      codec.c
  * Copyright (C) 2016 �����
  * ������    2.2
  * ��������: ������� ������������� � ������������ �����������
  ******************************************************************************
  */ 

#include "stm32_p407.h"                 //���� ������������ ���������� ����� STM32-P407
#include "codec.h"                      //������� ��� ������ � �������

//---------------------------------------------------------------------------
// ������������� ��������� ������
/* 
   ������ ������ ��� ����������� - 16-��������� �������������� ���,
   � ������ ����������� ����� ����������� ���������:
     ��� 0111 1111 1111 1111  ������������� U��� = +Um;
     ��� 0000 0000 0000 0000  ������������� U��� = 0;
     ��� 1000 0000 0000 0000  ������������� U��� = -Um;
   ��� Um �������� ����� 1 �

   ������������� ��������� PLL I2S ��� I2S_Mode_Master
   �������   PLLI2SN PLLI2SR I2SDIV
    8000 ��    256      5      12
   16000 ��    213      2      13
   22050 ��    429      4       9
   32000 ��    213      2       6
   44100 ��    271      2       6
   48000 ��    258      3       3
   �������� ������� �������������: 1 ��� * PLLI2SN / PLLI2SR / I2SDIV / 512
   PLLI2SN PLLI2SR - �������� � RCC_PLLI2SConfig(PLLI2SN, PLLI2SR)
   I2SDIV ����������� � ������������ I2S_Init()

   ��� ������� ������� ������������� � stm32f4xx_spi.h ��������� ��������� ���������
    #define I2S_AudioFreq_192k               ((uint32_t)192000)
    #define I2S_AudioFreq_96k                ((uint32_t)96000)
    #define I2S_AudioFreq_48k                ((uint32_t)48000)
    #define I2S_AudioFreq_44k                ((uint32_t)44100)
    #define I2S_AudioFreq_32k                ((uint32_t)32000)
    #define I2S_AudioFreq_22k                ((uint32_t)22050)
    #define I2S_AudioFreq_16k                ((uint32_t)16000)
    #define I2S_AudioFreq_11k                ((uint32_t)11025)
    #define I2S_AudioFreq_8k                 ((uint32_t)8000)
*/
void SoundCodecConfig(uint32_t AudioFreq)
{
  //��������� ������������
  GPIO_InitTypeDef GPIO_InitStructure;
  I2S_InitTypeDef I2S_InitStructure;

  //���������������� ����������� ���� ��� ���������� I2S (PLL I2S)
  RCC_PLLI2SConfig(213,2);
  //���������� ������ PLL I2S
  RCC_PLLI2SCmd(ENABLE);
  //�������� ���������� PLL I2S
  while(RESET == RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY));
  //������� PLL I2S ��� ��������� ������������ I2S
  RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
  //���������� ������������ ������ SPI3/I2S3 �� ���� APB1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  //��������������� (�����) ���������� SPI3/I2S3
  SPI_I2S_DeInit(SPI3); 

  //���������������� ���������� I2S3
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;           //�����: _SlaveTx _SlaveRx _MasterTx _MasterRx
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;   //��������: _Phillips _MSB   _LSB  _PCMShort _PCMLong
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;    //������: _16b _16bextended _24b _32b
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable; //���������� ������ �� ����� ��������� �������
  I2S_InitStructure.I2S_AudioFreq = AudioFreq;              //������� �������������: _8k ... _192k
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;                //�������� ������� ��������� �������: _Low _High
  I2S_Init(SPI3, &I2S_InitStructure);                       //������� ����������������

  //������ ���������� I2S3 TXE 
  SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, DISABLE);
 
  //���������������� ������� ����������
  //���������� ������������ ������ A, B, C
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  //���������������� PA15 (������� ������ I2S3_WS)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_SPI3);  //����������� ������ � I2S

  //���������������� PB3 (������� ������ I2S3_CK) � PB5 (������� ������ I2S3_SD)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);

  //���������������� PC7 (������� ������ I2S3_MCK)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);

  //���������� ������������� ���� ����������: ��� ������ ���������� ������
  SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE); 

  //���������� ���������� ����������
  SoundCodecInterruptConfig(ENABLE);

  //���������� ������ ���������� SPI3/I2S3
  I2S_Cmd(SPI3, ENABLE);
}  

//---------------------------------------------------------------------------
// ����������/������ ���������� ���������� �� SPI3/I2S3 (������)
void SoundCodecInterruptConfig(FunctionalState NewState)
{
  NVIC_InitTypeDef NVIC_InitStructure;                      //��������� ������������ ����������

  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;           //����� (�����) ����������
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��������� ������
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��������� ������ ������
  NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;         //����������/������ ����������
  NVIC_Init(&NVIC_InitStructure);                           //������� ����������������
}  

//---------------------------------------------------------------------------
// ���������� ���������� �� I2S3 (������)
// ���������� � ������� �������� �������������.
// �� ���� ������������ ���������� 16-������ ������ ������ ������.
// ����� ���������� ����������� Sample_Handler(), ������������ �������
// ����� ���� �������, ���������� � 2 ���� ����. 
// ������ ������ ��� ������ - ��������������.

int16_t DataChannel1, DataChannel2;            //�������� �������

void SPI3_IRQHandler(void)
{
  static uint16_t select_chan;                 //���� ������������� �������
  if (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) != RESET)  //�������� ����� �������
  { 
    SPI_I2S_ClearFlag(SPI3, SPI_I2S_FLAG_TXE); //����� ����� ������� ����������
    if (!select_chan)
    {
      Sample_Handler();                        //��������� ����������
      SPI_I2S_SendData(SPI3, DataChannel1);    //�������� ������ ������� ������� ������
    }
    else SPI_I2S_SendData(SPI3, DataChannel2); //�������� ������ ������� ������� ������
    select_chan ^= 1;                          //������������ �������
  }
}
//---------------------------------------------------------------------------
// ��������� ���������� �� ������ �� ���������
// ���������� � �������� �������������
__weak void Sample_Handler(void)
{
}
//---------------------------------------------------------------------------

