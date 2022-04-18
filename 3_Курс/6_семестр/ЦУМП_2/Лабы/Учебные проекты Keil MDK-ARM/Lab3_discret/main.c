//---------------------------------------------------------------------------
//
//  УЧЕБНЫЙ ПРОЕКТ
//  Дискретизация сигналов
//  Copyright (C) 2015 МИРЭА
//
//  В проекте демонстрируются: 
//   - генерация тестового гармонического сигнала;
//   - оцифровка этого сигнала посредством АЦП;
//   - вывод сигнала через ЦАП аудиокодека.
//  Физическая связь выхода генератора и входа АЦП осуществлена
//  благодаря двойной альтернативной функции разряда порта PA4.
//
//---------------------------------------------------------------------------

// Заголовочные файлы
#include "stm32_p407.h"                 //Файл конфигурации отладочной платы STM32-P407
#include "arm_math.h"                   //Определения и функции библиотеки CMSIS DSP Library
#include "adc.h"                        //Функции для работы с АЦП
#include "dac.h"                        //Константы и функции для работы с ЦАП
#include "codec.h"                      //Функции для работы с кодеком

//---------------------------------------------------------------------------
// ОБЪЯВЛЕНИЯ ДАННЫХ
int16_t DataADC1, DataADC2;             //Текущие отсчеты сигнала с АЦП
extern int16_t DataChannel1,            //Данные для кодека в дополнительном коде
               DataChannel2;            // (определены в модуле codec.c)
TIM_TimeBaseInitTypeDef TimeBase;       //Структура конфигурации базового таймера
NVIC_InitTypeDef NVICstruct;            //Структура конфигурации прерываний

//---------------------------------------------------------------------------
// ГЛАВНАЯ ФУНКЦИЯ 
int main()
{
  //Счетчик для реализации периода изменения тестовой индикации
  volatile uint32_t i = 0;

  //Задание 4-х уровней групповых приоритетов и 4-х уровней приоритетов в каждой группе
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  //Инициализация тестовых индикаторов
  STM_LEDInit(LED1); STM_LEDInit(LED2);  STM_LEDInit(LED3); STM_LEDInit(LED4);

  //Инициализация кнопок
  STM_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO);
  STM_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO);

  //Инициализация ЦАП - как генератора входного сигнала для АЦП
  DACInit(192000);
  DACTimerInterruptConfig(ENABLE);

  //Инициализация генератора частоты дискретизации АЦП; для этой цели
  // задействован таймер 2 с собственной частотой тактирования 84 МГц
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TimeBase.TIM_Period = 840000u/44100-1;
  TimeBase.TIM_Prescaler = 100-1;
  TimeBase.TIM_ClockDivision = 0;
  TimeBase.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TimeBase);
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  TIM_Cmd(TIM2, ENABLE);

  //Инициализация двухканального АЦП с запуском от события перезагрузки таймера 2
  ADC_Initialize(ADC1, ADC_Channel_4, ADC_ExternalTrigConv_T2_TRGO);
  ADC_Initialize(ADC2, ADC_Channel_4, ADC_ExternalTrigConv_T2_TRGO);

  //Конфигурирование прерываний от АЦП (по событию окончания преобразования)
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  NVICstruct.NVIC_IRQChannel = ADC_IRQn;
  NVICstruct.NVIC_IRQChannelPreemptionPriority = 2;
  NVICstruct.NVIC_IRQChannelSubPriority = 0;
  NVICstruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVICstruct);

  //Инициализация кодека как устройства вывода сигнала
  SoundCodecConfig(I2S_AudioFreq_192k);

  //Основной цикл
  while (1)
  {
    if (i++ == 0x800) STM_LEDOff(LED1);          //Тестовое управление индикатором
    if (i == 0xB0000) STM_LEDOn(LED1), i = 0;

    if (STM_PBGetState(BUTTON_WAKEUP)) NVIC_SystemReset(); //Проверка на завершение
  }
}

//---------------------------------------------------------------------------
// ОБРАБОТЧИК ПРЕРЫВАНИЯ ОТ ТАЙМЕРА ТАКТИРОВАНИЯ ЦАП
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
// ОБРАБОТЧИК ПРЕРЫВАНИЯ ОТ АЦП
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
// ОБСЛУЖИВАНИЕ ПРЕРЫВАНИЯ ОТ КОДЕКА
// Вызывается из обработчика прерывания ..._IRQHandler(), реализованного в codec.c
void Sample_Handler(void)
{
  //Выводимые значения - в DataChannel1, DataChannel2
}

//---------------------------------------------------------------------------
