//---------------------------------------------------------------------------
// 
//  ������������ ������ � USART (RS-232)
//
//  (C) 2016 �����
//
//  �� ���������� ������ STM32-P407 ������� ��� ����� RS-232:
//
//   COM1: ������� RS232_1 (��������� ����� � ���� �����)
//     ������ USART6_TX  - ����� ����� PC6  (�����)
//     ������ USART6_RX  - ����� ����� PG9  (����)
//     ������� CTS, RTS  - �� ������������
//
//   COM2: ������� RS232_2
//     ������ USART3_TX  - ����� ����� PD8  (�����)
//     ������ USART3_RX  - ����� ����� PD9  (����)
//     ������ USART3_CTS - ����� ����� PD11 (����)
//     ������ USART3_RTS - ����� ����� PD12 (�����)
//
//---------------------------------------------------------------------------

#include "stm32_p407.h"                 //���� ������������ ���������� �����

//---------------------------------------------------------------------------
// ������������� USART
void USART_Config(void)
{
  USART_InitTypeDef USART_InitStructure;                 //��������� ������������ USART
//NVIC_InitTypeDef NVIC_InitStructure;
  
  //������� �������� ���������� USART
  USART_InitStructure.USART_BaudRate = 9600;                      //��������, ���/�
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //����� ����� ������
  USART_InitStructure.USART_StopBits = USART_StopBits_1;          //����� �������� ���    
  USART_InitStructure.USART_Parity = USART_Parity_No;             //�������� ��������: ���
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //������� RTS, CTS: ���
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //����� ������ � ��������

  //������� ���������������� (����������� � stm32_p407.c)
  STM_EVAL_COMInit(COM1, &USART_InitStructure);
/*
  //���������������� ���������� �� USART
  NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/  
}

//---------------------------------------------------------------------------
// �������� ������� �������� ������
// ���������� true ��� ������� ������
bool USART_RX_Flag(void)
{
  return (bool)USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE);
}

//---------------------------------------------------------------------------
// ������ ��������� ����� ������
uint16_t USART_RX_Data(void)
{
  return USART_ReceiveData(EVAL_COM1);
}

//---------------------------------------------------------------------------
// �������� ������������������ ���� �� UART
void USART_TX_Data(char* buffer, uint16_t length)
{
  for (int i = 0; i < length; i++)
  { while (!USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE));
    USART_SendData(EVAL_COM1, buffer[i]);
  }  
}

//---------------------------------------------------------------------------
