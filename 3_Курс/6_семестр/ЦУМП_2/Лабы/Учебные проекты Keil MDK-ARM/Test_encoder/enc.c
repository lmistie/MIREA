//---------------------------------------------------------------------------
// 
//  ПОДПРОГРАММЫ РАБОТЫ С ЭНКОДЕРОМ
//
//  (C) 2016 МИРЭА
//
//  Квадратурный инкрементный энкодер подключен к разрядам портов:
//   PE9  - канал таймера TIM1_CH1
//   PE11 - канал таймера TIM1_CH2
//   PE10 - кнопка выбора
//
//  При каждом полном цикле изменения сигналов энкодера выходной код изменяется:
//  при константе TIM_EncoderMode_TI1 - на 2, при константе TIM_EncoderMode_TI12 - на 4.
//  При подключении одного из выводов кнопки к общему проводу состояние кнопки
//  равно 1 в ненажатом состоянии и 0 - при нажатии.
//
//---------------------------------------------------------------------------

#include "stm32f4xx.h"          //Константы и структуры данных для процессоров семейства STM32F4xx

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ЭНКОДЕРА
void EncoderConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                   //Структура конфигурации портов общего назначения
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;        //Структура конфигурации базового таймера
  TIM_ICInitTypeDef  TIM_ICInitStructure;                //Структура конфигурации таймера с входными сигналами
  
  //Разрешение тактирования порта PE
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  //Конфигурирование разряда PE10 как логического входа общего назначения
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //Номер разряда (маска)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //Функция: вход
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //Задание быстродействия (здесь не исп.)
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //Установка типа выходного каскада (здесь не исп.)
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //Подтягивающий резистор: к "+" питания 
  GPIO_Init(GPIOE, &GPIO_InitStructure);                 //Функция конфигурирования
  
  //Конфигурирование разрядов PE9, PE11 для альтернативной функции
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;//Номера разрядов (маски)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //Альтернативная функция
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //Задание быстродействия (здесь не исп.)
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //Установка типа выходного каскада (здесь не исп.)
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //Подтягивающий резистор: к "+" питания 
  GPIO_Init(GPIOE, &GPIO_InitStructure);                 //Функция конфигурирования
  
  //Связь каналов CH1, CH2 таймера с разрядами порта (альтернативная функция AF1)
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 

  //Разрешение тактирования таймера
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  //Базовая конфигурация таймера
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                    //Максимальное значение для автоперезагрузки
  TIM_TimeBaseStructure.TIM_Prescaler = 0;                      //Предделение не используется
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;                  //Тактирование для входного фильтра (по умолч.)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //Счетчик вверх: от 0 до TIM_Period
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;              //Число повторений переполнений (здесь не исп.)
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);               //Функция конфигурирования 

  //Конфигурирование входных каналов захвата
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;              //Номер канала
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;  //Полярность фронта запуска (по спаду)
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//Соответствие входных каналов регистрам захвата
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;         //Предделитель тактов работы входного фильтра 
  TIM_ICInitStructure.TIM_ICFilter = 0xF;                       //Настройка входного фильтра
  TIM_ICInit(TIM1, &TIM_ICInitStructure);                       //Функция конфигурирования для первого канала
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;              //Номер канала
  TIM_ICInit(TIM1, &TIM_ICInitStructure);                       //Функция конфигурирования для второго канала
  
  //Конфигурирование таймера в режиме интерфейса энкодера
  TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);

  //Разрешение работы таймера
  TIM_Cmd(TIM1, ENABLE);
}

//---------------------------------------------------------------------------
// ПОЛУЧЕНИЕ СОСТОЯНИЯ ЭНКОДЕРА - ЗНАЧЕНИЯ СЧЕТЧИКА И КНОПКИ ВЫБОРА
// Значения возвращаются через указатели
void GetEncoderValue(unsigned short * counter_value, unsigned char * button_set)
{
  if (counter_value) *counter_value = TIM_GetCounter(TIM1);
  if (button_set) *button_set = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10);
}

//---------------------------------------------------------------------------
