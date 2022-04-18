//---------------------------------------------------------------------------
// 
//  ПОДПРОГРАММЫ РАБОТЫ С USART (RS-232)
//
//  (C) 2016 МИРЭА
//
//  На отладочном модуле STM32-P407 имеются два порта RS-232:
//
//   COM1: розетка RS232_1 (размещена ближе к углу платы)
//     сигнал USART6_TX  - вывод порта PC6  (выход)
//     сигнал USART6_RX  - вывод порта PG9  (вход)
//     сигналы CTS, RTS  - не используются
//
//   COM2: розетка RS232_2
//     сигнал USART3_TX  - вывод порта PD8  (выход)
//     сигнал USART3_RX  - вывод порта PD9  (вход)
//     сигнал USART3_CTS - вывод порта PD11 (вход)
//     сигнал USART3_RTS - вывод порта PD12 (выход)
//
//---------------------------------------------------------------------------

#include "stm32_p407.h"                 //Файл конфигурации отладочной платы

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ USART
void USART_Config(void)
{
  USART_InitTypeDef USART_InitStructure;                 //Структура конфигурации USART
//NVIC_InitTypeDef NVIC_InitStructure;
  
  //Задание основных параметров USART
  USART_InitStructure.USART_BaudRate = 9600;                      //Скорость, бит/с
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //Длина слова данных
  USART_InitStructure.USART_StopBits = USART_StopBits_1;          //Число стоповых бит    
  USART_InitStructure.USART_Parity = USART_Parity_No;             //Контроль четности: нет
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Сигналы RTS, CTS: нет
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Режим приема и передачи

  //Функция конфигурирования (реализована в stm32_p407.c)
  STM_EVAL_COMInit(COM1, &USART_InitStructure);
/*
  //Конфигурирование прерываний от USART
  NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
*/  
}

//---------------------------------------------------------------------------
// ПРОВЕРКА НАЛИЧИЯ ПРИНЯТЫХ ДАННЫХ
// Возвращает true при наличии данных
bool USART_RX_Flag(void)
{
  return (bool)USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE);
}

//---------------------------------------------------------------------------
// ЧТЕНИЕ ПРИНЯТОГО БАЙТА ДАННЫХ
uint16_t USART_RX_Data(void)
{
  return USART_ReceiveData(EVAL_COM1);
}

//---------------------------------------------------------------------------
// ПЕРЕДАЧА ПОСЛЕДОВАТЕЛЬНОСТИ БАЙТ ПО UART
void USART_TX_Data(char* buffer, uint16_t length)
{
  for (int i = 0; i < length; i++)
  { while (!USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE));
    USART_SendData(EVAL_COM1, buffer[i]);
  }  
}

//---------------------------------------------------------------------------
