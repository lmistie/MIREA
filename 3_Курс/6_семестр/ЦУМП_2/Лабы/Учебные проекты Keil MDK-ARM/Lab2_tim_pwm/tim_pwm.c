//---------------------------------------------------------------------------
// 
//  УЧЕБНЫЙ ПРОЕКТ
//  Работа с таймерами. Режим генерации ШИМ-сигнала
//  Copyright (C) 2016 МИРЭА
//
//---------------------------------------------------------------------------

#include "stm32f4xx.h"                  //Константы и структуры данных для процессоров семейства STM32F4xx

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ТАЙМЕРОВ 
/* Таймеры 10,11 подключены к шине APB2, тактируются частотой 168 МГц.
   Предделение: 168 000 кГц / 16800 = 10 кГц.
   Основное деление для TIM10: 10000 Гц / 10000 = 1 Гц    (TIM_Period = 10000-1).
   Основное деление для TIM11: 10000 Гц / 20000 = 0.5 Гц  (TIM_Period = 20000-1).
   Выход TIM10_CH1 подключен к индикатору LED1 (разряд порта PF6).
   Выход TIM11_CH1 подключен к индикатору LED2 (разряд порта PF7).
   ШИМ TIM10 имеет коэффициент заполнения длительности импульса
   (duty cycle) = 10%, загружаемое число для этого
   TIM_CCR = TIM_Period * duty cycle[%] / 100[%] = 10000 * 10 / 100 = 1000.
   Для ШИМ TIM11  (duty cycle) = 75%,  TIM_CCR = 20000 * 75 / 100 = 15000.
   Прерывания от таймеров 10, 11 не используются.
*/ 

void TimerConfig(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;                   //Структура конфигурации портов общего назначения
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;        //Структура конфигурации базового таймера
  TIM_OCInitTypeDef  TIM_OCInitStructure;                //Структура конфигурации таймера с ШИМ
  NVIC_InitTypeDef NVIC_InitStructure;                   //Структура конфигурации прерываний

  //Разрешение тактирования TIM10, TIM11
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10 | RCC_APB2Periph_TIM11, ENABLE);

  //Разрешение тактирования порта PF
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  //Конфигурирование разрядов порта PF6, PF7 как выходных с альтернативной функцией
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //Номер разряда (маска)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //Альтернативная функция
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //Задание быстродействия
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //Установка типа выходного каскада: двухтактный
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //Подтягивающий резистор: к "+" питания 
  GPIO_Init(GPIOF, &GPIO_InitStructure);                 //Функция конфигурирования
  
  //Подключение выхода канала CH1 таймера TIM10 к разряду порта PF6 (как альтернативная функция AF2)
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10); 

  //Подключение выхода канала CH1 таймера TIM11 к разряду порта PF7 (как альтернативная функция AF2)
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_TIM11); 
  
  //Базовая конфигурация таймера
  TIM_TimeBaseStructure.TIM_Period = 10000-1;                   //Основной коэф.деления для TIM10
  TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;                //Предделение
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //Делитель для входного фильтра (не используется)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //Счетчик вверх: от 0 до TIM_Period
  TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);              //Функция конфигурирования 

  TIM_TimeBaseStructure.TIM_Period = 20000-1;                   //Основной коэф.деления для TIM11
  TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure);              //Функция конфигурирования 

  //Конфигурирование ШИМ
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //Режим выхода: ШИМ
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Разрешение аппаратного выхода
  TIM_OCInitStructure.TIM_Pulse = 1000;                         //Загружаемое значение
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //Полярность импульса - положительная
  TIM_OC1Init(TIM10, &TIM_OCInitStructure);                     //Функция конфигурирования

  TIM_OCInitStructure.TIM_Pulse = 15000;
  TIM_OC1Init(TIM11, &TIM_OCInitStructure);

  //Общее разрешение работы таймеров
  TIM_Cmd(TIM10, ENABLE);
  TIM_Cmd(TIM11, ENABLE);


  // КОНФИГУРИРОВАНИЕ ТАЙМЕРА 4
  /* Подключен к шине APB1, тактируется частотой 84 МГц.
     Предделение: 84000 кГц / 42000 = 2 кГц.
     Основное деление: 2000 Гц / 200 = 10 Гц (период 100 мс).
     Аппаратный выход не задействован.
     Таймер используется для генерации прерываний при автоперезагрузке.
  */
  //Разрешение тактирования TIM4
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  //Базовая конфигурация таймера
  TIM_TimeBaseStructure.TIM_Period = 200-1;                     //Основной коэф.деления
  TIM_TimeBaseStructure.TIM_Prescaler = 42000-1;                //Предделение
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //Счетчик вверх: от 0 до TIM_Period
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);               //Функция конфигурирования 

  //Разрешение прерываний при перезагрузке таймера
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  
  //Общее разрешение работы таймера TIM4
  TIM_Cmd(TIM4, ENABLE);

  //Конфигурирование прерывания от таймера
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;               //Номер (линия) прерывания
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     //Приоритет группы
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            //Приоритет внутри группы
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //Разрешение прерывания
  NVIC_Init(&NVIC_InitStructure);                               //Функция конфигурирования
}

//---------------------------------------------------------------------------
