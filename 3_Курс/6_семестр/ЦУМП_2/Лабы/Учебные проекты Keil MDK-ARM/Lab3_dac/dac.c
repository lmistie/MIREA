/**
  ******************************************************************************
  * Файл      dac.c
  * Copyright (C) 2015 МИРЭА
  * Версия    2.2
  * Описание: Функции инициализации и обслуживания ЦАП
  *
  * ЦАП может быть сконфигурирован для запуска от таймеров
  *   TIM2, TIM4, TIM5, TIM6, TIM7 соответствующей коррекцией в файле dac.h 
  *   (другие источники - EXTI Line9, TIM8 - требуют более сложной модификации)
  *         
  * Формат данных для ЦАП - 12-разрядный смещенный код:
  *   код 1111 1111 1111  соответствует Uвых = максимум (практически Vref);
  *   код 1000 0000 0000  соответствует Uвых = Vref / 2;
  *   код 0000 0000 0000  соответствует Uвых = минимум (практически 0); 
  * где Vref - опорное напряжение (для отладочной платы равно напряжению питания)
  *
  ******************************************************************************
  */ 

#include "stm32_p407.h"                 //Файл конфигурации отладочной платы STM32-P407
#include "dac.h"                        //Константы и функции для работы с ЦАП

//#define USE_DAC2                      //Определение для использования второго канала ЦАП,
                                        //режим возможен, если не используется графический дисплей

void DACTimerConfig(uint32_t SamplingFreq);
void DACConfig(uint32_t DAC_Trigger);

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ЦАП И СИСТЕМЫ ТАКТИРОВАНИЯ
// Входной параметр - частота дискретизации в Гц; если задано нулевое значение,
// то синхронизация не используется, запуск ЦАП производится программно -
// вызовом функции DAC_SetChannel1Data или DAC_SetChannel2Data
void DACInit(uint32_t SamplingFreq)
{
  if (SamplingFreq)
  {
    //Инициализация таймера - источника тактирования ЦАП
    DACTimerConfig(SamplingFreq);

    //Инициализация ЦАП с запуском от таймера
    DACConfig(DAC_Trigger_TD_TRGO);
    
    //Разрешение прерываний от таймера
    //DACTimerInterruptConfig(ENABLE);
  }
  //Если не задана частота дискретизации, инициализация ЦАП
  // с режимом запуска по загрузке данных
  else DACConfig(DAC_Trigger_None);  
}

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ТАЙМЕРА ДЛЯ ТАКТИРОВАНИЯ ЦАП
/* Подпрограмма используется для таймеров TIM2, TIM4, TIM5, TIM6, TIM7,
   подключенных к шине APB1. По умолчанию частота работы шины 42 МГц,
   частота тактирования таймеров этой шины - 84 МГц.
   При отсутствии предделения максимальный коэффициент деления для 16-разрядного
   таймера равен 65536, т.е. минимальная частота дискретизации для этого случая
   84000000 Гц / 65536 = 1282 Гц.
   Для меньших значений частот необходимо предделение или применение 32-разрядного таймера. 
   В настоящей подпрограмме для частоты дискретизации SamplingFreq, заданной в Гц:
     - если предделитель не используется (его коэф. деления = 1),
       число, загружаемое в счетчик: TIM_Period = 84000000[Гц] / SamplingFreq[Гц] - 1;
     - если коэффициент предделителя равен 1000,  
       число, загружаемое в счетчик: TIM_Period = 84000[Гц] / SamplingFreq[Гц] - 1;
   Для еще более низких частот (< 2 Гц), а также повышения точности необходимо
   выбирать 32-разрядный таймер (TIM2, TIM5).
   Прерывания от таймера конфигурируются по событию его автоперезагрузки.
*/ 
void DACTimerConfig(uint32_t SamplingFreq)
{
  //Структура конфигурации базового таймера 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  //Разрешение тактирования таймера
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMD, ENABLE);

  //Базовая конфигурация таймера
  if (SamplingFreq > 1300)
  { TIM_TimeBaseStructure.TIM_Period = 84000000u/SamplingFreq-1;//Основной коэф.деления
    TIM_TimeBaseStructure.TIM_Prescaler = 0;                    //Предделение не используется
  }
  else
  { TIM_TimeBaseStructure.TIM_Period = 84000u/SamplingFreq-1;  //Основной коэф.деления
    TIM_TimeBaseStructure.TIM_Prescaler = 1000-1;              //Предделитель в 1000 раз
  }
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                 //Делитель для входного фильтра
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //Счетчик вверх: от 0 до TIM_Period
  TIM_TimeBaseInit(TIMD, &TIM_TimeBaseStructure);              //Функция конфигурирования 

  //Активизация триггерного выхода TRGO таймера для запуска ЦАП
  TIM_SelectOutputTrigger(TIMD, TIM_TRGOSource_Update);

  //Разрешение прерываний при перезагрузке таймера
  TIM_ITConfig(TIMD, TIM_IT_Update, ENABLE);
  
  //Общее разрешение работы таймера
  TIM_Cmd(TIMD, ENABLE);
}

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ЦАП
// Используются DAC1 с выходом на разряд порта PA4
//            и DAC2 с выходом на разряд порта PA5
// Входной параметр: источник запуска ЦАП - одна из предопределенных констант:
//  DAC_Trigger_None, DAC_Trigger_T2_TRGO, DAC_Trigger_T4_TRGO, DAC_Trigger_T5_TRGO,
//  DAC_Trigger_T6_TRGO, DAC_Trigger_T7_TRGO, DAC_Trigger_T8_TRGO, DAC_Trigger_Ext_IT9,
//  DAC_Trigger_Software
void DACConfig(uint32_t DAC_Trigger)
{
  GPIO_InitTypeDef GPIO_InitStructure;            //Структура конфигурации портов общего назначения
  DAC_InitTypeDef  DAC_InitStructure;             //Структура конфигурации ЦАП

  //Разрешение тактирования порта PA, разряды которого используются как выходы ЦАП
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  //Разрешение тактирования ЦАП
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  //Конфигурирование разрядов PA.4 (PA.5) как аналоговых
#ifdef USE_DAC2  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;          //Номера разрядов (маски) для DAC1, DAC2
#else  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                       //Номер разряда (маска) для DAC1
#endif  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;                    //Альтернативная функция: аналоговый вход-выход
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                //Без подтягивающих резисторов
  GPIO_Init(GPIOA, &GPIO_InitStructure);                          //Функция конфигурирования
   
  //Конфигурирование режимов работы ЦАП
  DAC_InitStructure.DAC_Trigger = DAC_Trigger;                    //Источник запуска
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; //Встроенный генератор: не используется 
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = 0xA00;     //Маска/амплитуда для встроенного генератора
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;   //Разрешение выходного буфера повышенной мощности
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);                    //Функция конфигурирования для DAC1
#ifdef USE_DAC2
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);                    //Функция конфигурирования для DAC2
#endif  

  //Общее разрешение работы ЦАП, загрузка начального значения данных ЦАП (код - смещенный)
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_L, 0x0000);
#ifdef USE_DAC2
  DAC_Cmd(DAC_Channel_2, ENABLE);
  DAC_SetChannel2Data(DAC_Align_12b_L, 0x0000);
#endif  

}

//---------------------------------------------------------------------------
// РАЗРЕШЕНИЕ/ЗАПРЕТ ГЛОБАЛЬНЫХ ПРЕРЫВАНИЙ ОТ ТАЙМЕРА
void DACTimerInterruptConfig(FunctionalState NewState)
{
  NVIC_InitTypeDef NVIC_InitStructure;                      //Структура конфигурации прерываний

  NVIC_InitStructure.NVIC_IRQChannel = TIMD_IRQn;           //Номер (линия) прерывания
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //Приоритет группы
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //Приоритет внутри группы
  NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;         //Разрешение/запрет прерывания
  NVIC_Init(&NVIC_InitStructure);                           //Функция конфигурирования
}

//---------------------------------------------------------------------------
/* Ш А Б Л О Н
// ОБРАБОТЧИК ПРЕРЫВАНИЯ ОТ ТАЙМЕРА
// Вызывается с заданной частотой дискретизации ЦАП.
// Код для передачи в ЦАП - смещенный, выравненный влево.
uint16_t DACData1, DACData2;                                //Данные для ЦАП в смещенном коде
void TIMD_IRQHandler(void)
{
  if (TIM_GetITStatus(TIMD, TIM_IT_Update) != RESET)        //Проверка флага типа прерывания
  {                                                         //(необязательна, если других типов нет)
    TIM_ClearITPendingBit(TIMD, TIM_IT_Update);             //Очистка флага
    DAC_SetChannel1Data(DAC_Align_12b_L, DACData1);         //Загрузка данных в ЦАП 1
    DAC_SetChannel2Data(DAC_Align_12b_L, DACData2);         //Загрузка данных в ЦАП 2
  }
}
*/
//---------------------------------------------------------------------------
