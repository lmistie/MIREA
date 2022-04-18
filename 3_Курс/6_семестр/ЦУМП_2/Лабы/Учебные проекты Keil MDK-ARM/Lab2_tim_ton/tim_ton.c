//---------------------------------------------------------------------------
// 
//  УЧЕБНЫЙ ПРОЕКТ
//  Работа с таймерами. Режим деления частоты
//  Copyright (C) 2016 МИРЭА
//
//---------------------------------------------------------------------------

#include "stm32f4xx.h"                  //Константы и структуры данных для процессоров семейства STM32F4xx

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ТАЙМЕРА 
/* Таймер 3 подключен к шине APB1, тактируется частотой 84 МГц.
   Предделитель с коэффициентом деления 42 снижает частоту до 2 МГц.
   Основной коэффициент деления определяется частотой музыкального тона,
   при этом следует учесть, выходная частота получается еще в 2 раза меньше,
   так как при каждом переполнении счетного регистра формируется полпериода сигнала.
   Выход таймера - канал CH4 - выведен на разряд порта PB1.
*/
void TimerConfig(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;                   //Структура конфигурации портов общего назначения
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;        //Структура конфигурации базового таймера
  TIM_OCInitTypeDef  TIM_OCInitStructure;                //Структура конфигурации таймера с ШИМ

  //Разрешение тактирования таймера
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  //Разрешение тактирования порта PB
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  //Конфигурирование разряда порта PB1 как выходного с альтернативной функцией
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;              //Номер разряда (маска)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //Альтернативная функция
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //Задание быстродействия
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //Установка типа выходного каскада: двухтактный
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //Подтягивающий резистор: к "+" питания 
  GPIO_Init(GPIOB, &GPIO_InitStructure);                 //Функция конфигурирования
  
  //Подключение выхода канала CH4 таймера TIM3 к разряду порта PB1
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); 

  //Базовая конфигурация таймера
  TIM_TimeBaseStructure.TIM_Period = 1000-1;                    //Основной коэф.деления по умолчанию
  TIM_TimeBaseStructure.TIM_Prescaler = 42-1;                   //Предделение до 2 МГц
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //Счетчик вверх: от 0 до TIM_Period
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);               //Функция конфигурирования 

  //Конфигурирование ШИМ
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;           //Режим выхода: переключение уровня
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //Разрешение аппаратного выхода
  TIM_OCInitStructure.TIM_Pulse = 0;                            //Пороговое значение для переключения уровня
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //Полярность импульса - положительная
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);                      //Функция конфигурирования

  //Работу таймера пока не разрешаем
  TIM_Cmd(TIM3, DISABLE);
}

//---------------------------------------------------------------------------
