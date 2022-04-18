/**
  ******************************************************************************
  * Файл      codec.c
  * Copyright (C) 2016 МИРЭА
  * Версия    2.2
  * Описание: Функции инициализации и обслуживания аудиокодека
  ******************************************************************************
  */ 

#include "stm32_p407.h"                 //Файл конфигурации отладочной платы STM32-P407
#include "codec.h"                      //Функции для работы с кодеком

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ЗВУКОВОГО КОДЕКА
/* 
   Формат данных для аудиокодека - 16-разрядный дополнительный код,
   с учетом включенного после аудиокодека усилителя:
     код 0111 1111 1111 1111  соответствует Uвых = +Um;
     код 0000 0000 0000 0000  соответствует Uвых = 0;
     код 1000 0000 0000 0000  соответствует Uвых = -Um;
   где Um примерно равно 1 В

   Рекомендуемые настройки PLL I2S для I2S_Mode_Master
   Частота   PLLI2SN PLLI2SR I2SDIV
    8000 Гц    256      5      12
   16000 Гц    213      2      13
   22050 Гц    429      4       9
   32000 Гц    213      2       6
   44100 Гц    271      2       6
   48000 Гц    258      3       3
   Реальная частота дискретизации: 1 МГц * PLLI2SN / PLLI2SR / I2SDIV / 512
   PLLI2SN PLLI2SR - задаются в RCC_PLLI2SConfig(PLLI2SN, PLLI2SR)
   I2SDIV вычисляется в библиотечной I2S_Init()

   Для задания частоты дискретизации в stm32f4xx_spi.h объявлены следующие константы
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
  //Структуры конфигурации
  GPIO_InitTypeDef GPIO_InitStructure;
  I2S_InitTypeDef I2S_InitStructure;

  //Конфигурирование синтезатора ФАПЧ для интерфейса I2S (PLL I2S)
  RCC_PLLI2SConfig(213,2);
  //Разрешение работы PLL I2S
  RCC_PLLI2SCmd(ENABLE);
  //Ожидание готовности PLL I2S
  while(RESET == RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY));
  //Задание PLL I2S как источника тактирования I2S
  RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
  //Разрешение тактирования модуля SPI3/I2S3 по шине APB1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  //Деинициализация (сброс) интерфейса SPI3/I2S3
  SPI_I2S_DeInit(SPI3); 

  //Конфигурирование интерфейса I2S3
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;           //Режим: _SlaveTx _SlaveRx _MasterTx _MasterRx
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;   //Стандарт: _Phillips _MSB   _LSB  _PCMShort _PCMLong
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;    //Формат: _16b _16bextended _24b _32b
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable; //Разрешение выдачи на выход тактового сигнала
  I2S_InitStructure.I2S_AudioFreq = AudioFreq;              //Частота дискретизации: _8k ... _192k
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;                //Исходный уровень тактового сигнала: _Low _High
  I2S_Init(SPI3, &I2S_InitStructure);                       //Функция конфигурирования

  //Запрет прерывания I2S3 TXE 
  SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, DISABLE);
 
  //Конфигурирование выводов процессора
  //Разрешение тактирования портов A, B, C
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  //Конфигурирование PA15 (внешний сигнал I2S3_WS)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_SPI3);  //Подключение вывода к I2S

  //Конфигурирование PB3 (внешний сигнал I2S3_CK) и PB5 (внешний сигнал I2S3_SD)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);

  //Конфигурирование PC7 (внешний сигнал I2S3_MCK)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);

  //Разрешение определенного типа прерываний: при пустом передающем буфере
  SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE); 

  //Разрешение глобальных прерываний
  SoundCodecInterruptConfig(ENABLE);

  //Разрешение работы интерфейса SPI3/I2S3
  I2S_Cmd(SPI3, ENABLE);
}  

//---------------------------------------------------------------------------
// РАЗРЕШЕНИЕ/ЗАПРЕТ ГЛОБАЛЬНЫХ ПРЕРЫВАНИЙ ОТ SPI3/I2S3 (КОДЕКА)
void SoundCodecInterruptConfig(FunctionalState NewState)
{
  NVIC_InitTypeDef NVIC_InitStructure;                      //Структура конфигурации прерываний

  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;           //Номер (линия) прерывания
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //Приоритет группы
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //Приоритет внутри группы
  NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;         //Разрешение/запрет прерывания
  NVIC_Init(&NVIC_InitStructure);                           //Функция конфигурирования
}  

//---------------------------------------------------------------------------
// ОБРАБОТЧИК ПРЕРЫВАНИЯ ОТ I2S3 (КОДЕКА)
// Вызывается с двойной частотой дискретизации.
// За одно обслуживание передается 16-битный отсчет одного канала.
// Вызов вторичного обработчика Sample_Handler(), формирующего отсчеты
// сразу двух каналов, происходит в 2 раза реже. 
// Формат данных для кодека - дополнительный.

int16_t DataChannel1, DataChannel2;            //Выходные отсчеты

void SPI3_IRQHandler(void)
{
  static uint16_t select_chan;                 //Флаг переключателя каналов
  if (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) != RESET)  //Проверка флага запроса
  { 
    SPI_I2S_ClearFlag(SPI3, SPI_I2S_FLAG_TXE); //Сброс флага запроса прерывания
    if (!select_chan)
    {
      Sample_Handler();                        //Вторичный обработчик
      SPI_I2S_SendData(SPI3, DataChannel1);    //Передача кодеку отсчета первого канала
    }
    else SPI_I2S_SendData(SPI3, DataChannel2); //Передача кодеку отсчета второго канала
    select_chan ^= 1;                          //Переключение каналов
  }
}
//---------------------------------------------------------------------------
// ВТОРИЧНЫЙ ОБРАБОТЧИК ОТ КОДЕКА ПО УМОЛЧАНИЮ
// Вызывается с частотой дискретизации
__weak void Sample_Handler(void)
{
}
//---------------------------------------------------------------------------

