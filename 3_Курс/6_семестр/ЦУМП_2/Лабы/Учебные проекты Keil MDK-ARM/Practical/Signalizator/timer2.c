//---------------------------------------------------------------------------
// 
//  ДЕМОНСТРАЦИОННЫЙ ПРОЕКТ
//
//  Работа с таймерами
//
//  Copyright (C) 2013 МГТУ МИРЭА
//
//---------------------------------------------------------------------------

#include "stm32f4xx.h"                  //Константы и структуры данных для процессоров семейства STM32F4xx

#define  TIM_GEN_PRESCALER  84          //Загружаемые значения делителей для таймера-генератора
#define  TIM_GEN_PERIOD     1000
#define  TIM_DUR_PRESCALER  33600       //Загружаемые значения делителей для таймера-формирователя длительности
#define  TIM_DUR_PERIOD     10000

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ТАЙМЕРА-ГЕНЕРАТОРА СИГНАЛА
/* Таймер 3 подключен к шине APB1, частота тактирования которой в 4 раза ниже системной:
   168 МГц / 4 = 42 МГц. Так как коэффициент деления частоты для данной шины не 1, 
   то на таймеры этой шины поступает удвоенная тактовая частота, т.е. 84 МГц.
   Для получения на выходе частоты 1 кГц коэффициент деления должен быть равен 84000.
   Так как таймер 16-разрядный, то необходимо использовать предделение.
   В примере ниже коэффициент предварительного деления (TIM_Prescaler) - 84,
   коэффициент деления счетчика таймера (TIM_Period) - 1000.
   Выход таймера - канал CH4 - выведен на разряд порта PB1.
   На выходе CH4 используется формирователь с ШИМ, коэффициент заполнения длительности
   импульса принят (duty cycle) = 40%, загружаемое число для этого
   TIM_Pulse = TIM_Period * duty cycle[%] / 100[%] = TIM_Period * 4 / 10
*/   
void TimerGenerationConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                  //Структура конфигурации портов общего назначения
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //Структура конфигурации базового таймера
  TIM_OCInitTypeDef  TIM_OCInitStructure;               //Структура конфигурации таймера с ШИМ

  //Разрешение тактирования TIM3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  //Разрешение тактирования порта PB
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  //Конфигурирование разряда порта PB1 как выходного с альтернативной функцией
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;             //Номер разряда (маска)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          //Альтернативная функция
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //Задание быстродействия
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //Установка типа выходного каскада: двухтактный
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //Подтягивающий резистор: к "+" питания 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  //Подключение выхода канала CH4 таймера TIM3 к разряду порта PB1
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); 

  //Базовая конфигурация таймера
  TIM_TimeBaseStructure.TIM_Period = TIM_GEN_PERIOD-1;          //Основной коэф. деления
  TIM_TimeBaseStructure.TIM_Prescaler = TIM_GEN_PRESCALER-1;    //Коэффициент предделения
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //Делитель для входного фильтра (не используется)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //Режим счета вверх: от 0 до TIM_Period
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);               //Функция конфигурирования 

  //Конфигурирование ШИМ
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //Режим выхода: ШИМ типа 1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Разрешение аппаратного выхода
  TIM_OCInitStructure.TIM_Pulse = TIM_GEN_PERIOD * 4 / 10;      //Длительность импульса
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //Полярность импульса - положительная
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);                      //Функция конфигурирования
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);             //Разрешение загрузки регистра ШИМ

  //Выбор соединения двух таймеров: выход TIM1 -> вход TIM3 через ITR0
  TIM_SelectInputTrigger(TIM3, TIM_TS_ITR0);

  //Задание режима ведомого, режима ворот 
  TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Gated);

  //Общее разрешение работы таймера, при этом реальный счет инициируется запуском от ведущего таймера
  TIM_Cmd(TIM3, ENABLE);
}

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ТАЙМЕРА-ФОРМИРОВАТЕЛЯ ДЛИТЕЛЬНОСТИ
/* Таймер 1 подключен к шине APB2, частота тактирования шины - 84 МГц, 
   частота тактирования таймеров на этой шине - 168 МГц.
   Для реализации временной задержки 2 с требуется общий коэффициент деления
   168e6 / (1/2) = 336 000 000, который можно реализовать, например, как 33 600 * 10 000
   Таймер конфигурируется в режимах:
     - Master - режим ведущего для других таймеров, в данной задаче - для таймера 3;
     - генерации одиночного импульса (с автоостановкой по перезагрузке);
     - выбора флага разрешения работы как выходного сигнала.
    Разрешение работы таймера дается в основной программе. 
    При этих режимах выходной импульс, управляющий счетом ведомого таймера,
    устанавливается в активное состояние при работе данного таймера. По достижении
    макcимального заданного значения (переполнения) счетчик перезагружается (обнуляется),
    таймер останавливается, выходной импульс принимает неактивный уровень.
*/
void TimerDurationConfig(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //Структура конфигурации базового таймера

  //Разрешение тактирования таймера
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
  //Базовая конфигурация таймера
  TIM_TimeBaseStructure.TIM_Period = TIM_DUR_PERIOD-1;          //Основной коэф. деления
  TIM_TimeBaseStructure.TIM_Prescaler = TIM_DUR_PRESCALER-1;    //Коэффициент предделения
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //Делитель для входного фильтра (не используется)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //Режим счета вверх: от 0 до TIM_Period
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);               //Функция конфигурирования 

  //Задание режима генерации одиночного импульса
  TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Single);
  
  //Разрешение режима ведущего (Master)
  TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);

  //Выбор флага разрешения работы в качестве выходного сигнала запуска для ведомого таймера 
  TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Enable);
}

//---------- Конец файла timer2.c -------------------------------------------
