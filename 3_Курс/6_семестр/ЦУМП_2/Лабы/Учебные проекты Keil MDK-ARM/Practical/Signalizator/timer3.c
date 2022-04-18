//---------------------------------------------------------------------------
// 
//  ДЕМОНСТРАЦИОННЫЙ ПРОЕКТ
//
//  Работа с таймерами и ЦАП
//
//  Copyright (C) 2013 МГТУ МИРЭА
//
//---------------------------------------------------------------------------

#include "stm32f4xx.h"                  //Константы и структуры данных для процессоров семейства STM32F4xx

#define  F_DISCRET          16000                    //Частота дискретизации, Гц
#define  TIM_DISCRET_PERIOD (84000000/F_DISCRET-1)   //Значение для загрузки в таймер

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ТАЙМЕРА - ИСТОЧНИКА ЧАСТОТЫ ДИСКРЕТИЗАЦИИ
/* Таймер 4 подключен к шине APB1, частота тактирования которой в 4 раза ниже системной:
   168 МГц / 4 = 42 МГц. Так как коэффициент деления частоты для данной шины не 1, 
   то на таймеры этой шины поступает удвоенная тактовая частота, т.е. 84 МГц.
   Для частоты дискретизации 16 кГц:
    - предделитель не используется (коэф. деления = 1)
    - число, загружаемое в счетчик: TIM_Period = 84 МГц / 16 кГц - 1 = 5249
   Прерывания от таймера конфигурируются по событию его автозагрузки.
*/ 
void TimerDiscretConfig(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //Структура конфигурации базового таймера

  //Разрешение тактирования таймера
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  //Базовая конфигурация таймера
  TIM_TimeBaseStructure.TIM_Period = TIM_DISCRET_PERIOD;        //Основной коэф.деления
  TIM_TimeBaseStructure.TIM_Prescaler = 0;                      //Предделение не используется
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //Делитель для входного фильтра (не используется)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //Счетчик вверх: от 0 до TIM_Period
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);               //Функция конфигурирования 

  //Активизация триггерного выхода таймера TRGO, который будет использоваться для запуска ЦАП
  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);

  //Выбор события перезагрузки таймера как источника прерываний
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  
  //Общее разрешение работы таймера
  TIM_Cmd(TIM4, ENABLE);
}

//---------------------------------------------------------------------------
// РАЗРЕШЕНИЕ/ЗАПРЕТ ПРЕРЫВАНИЙ ОТ ТАЙМЕРА ПЕРИОДА ДИСКРЕТИЗАЦИИ
// Входной параметр: NewState - true (разрешить), false (запретить)
void TimerDiscretInterrupt(bool NewState)
{
  NVIC_InitTypeDef NVIC_InitStructure;                      //Структура конфигурации прерываний

  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;           //Номер канала/источника (определен в stm32f4xx.h)
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //Уровень приоритета (0 - высший, 15 - низший)
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //Приоритет внутри группы
  NVIC_InitStructure.NVIC_IRQChannelCmd = (FunctionalState)NewState; //Разрешение/запрет
  NVIC_Init(&NVIC_InitStructure);                           //Функция конфигурирования
}

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ЦАП
// Используется DAC1 с выходом на разряд порта PA4
void DACConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                  //Структура конфигурации портов общего назначения
  DAC_InitTypeDef  DAC_InitStructure;                   //Структура конфигурации ЦАП

  //Разрешение тактирования порта PA, разряды которого используются как выходы ЦАП
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  //Разрешение тактирования ЦАП
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  //Конфигурирование разряда PA.4 как аналогового
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                        //Номер разряда (маска) для DAC1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;                     //Альтернативная функция: аналог
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                 //Без подтягивающих резисторов
  GPIO_Init(GPIOA, &GPIO_InitStructure);                           //Функция конфигурирования
   
  //Конфигурирование режимов работы ЦАП
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;             //Запуск от таймера TIM4
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;  //Нет собственной генерации
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095; //Диапазон кодов
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;    //Разрешение выходного буфера
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);                     //Функция конфигурирования

  //Общее разрешение работы ЦАП, загрузка начального значения данных ЦАП
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_L, 0x0000);
}

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ТАЙМЕРА УПРАВЛЕНИЯ ИНДИКАЦИЕЙ
/* Таймер 10 подключен к шине APB2, частота тактирования шины - 84 МГц, 
   частота тактирования таймеров на этой шине - 168 МГц.
   Выходной сигнал таймера TIM10_CH1 может быть выведен на разряд порта PF6 (с индикатором),
   в этом случае он должен конфигурироваться с выходным ШИМ сигналом - аналогичный пример см.
   в варианте 2 проекта. Здесь таймер будет использован только для задания временных
   задержек, которые в вариантах 1 и 2 проекта формировались программно (счетчиком blink_counter).
   Приняты следующие временные соотношения:
   В режиме ожидания: период мигания 1000 мс, длительность включенного состояния 10 мс.
   В режиме генерации: период мигания 100 мс, длительность включенного состояния 60 мс.
   Эти значения определены в main3.c и указаны в миллисекундах. Непосредственно в счетчик таймера 
   они не могут быть загружены, так как предварительный коэффициент деления при этом должен быть
   равен 168 000 000 Гц / (1/1e-3 с) = 168 000, что больше максимального 16-разрядного значения.
   Поэтому предварительный коэффициент деления уменьшен в 10 раз (до 16 800), а загружаемые величины
   в миллисекундах в процессе загрузки увеличиваются в 10 раз.
   По окончании отработки задержки генерируется прерывание по событию автоперезагрузки счетчика.
*/
void TimerIndicatorConfig(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;               //Структура конфигурации базового таймера
  NVIC_InitTypeDef NVIC_InitStructure;                          //Структура конфигурации прерываний

  //Разрешение тактирования таймера
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
  
  //Базовая конфигурация таймера
  TIM_TimeBaseStructure.TIM_Period = 1000;                      //Основной коэф. деления (сейчас не важен)
  TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;                //Коэффициент предделения
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //Делитель для входного фильтра (не используется)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //Режим счета вверх: от 0 до TIM_Period
  TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);              //Функция конфигурирования 

  //Выбор события перезагрузки таймера как источника прерываний
  TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);

  //Конфигурирование прерывания от таймера
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;      //Номер канала/источника прерывания
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //Уровень приоритета (0 - высший, 15 - низший)
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            //Приоритет внутри группы
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //Разрешение прерывания
  NVIC_Init(&NVIC_InitStructure);                               //Функция конфигурирования

  //Общее разрешение работы таймера
  TIM_Cmd(TIM10, ENABLE);
}

//---------- Конец файла timer3.c -------------------------------------------
