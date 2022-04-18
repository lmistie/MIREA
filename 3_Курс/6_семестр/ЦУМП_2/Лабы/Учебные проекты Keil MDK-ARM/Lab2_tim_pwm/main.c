//---------------------------------------------------------------------------
// 
//  УЧЕБНЫЙ ПРОЕКТ
//  Использование таймеров для генерации временных интервалов и ШИМ-сигналов 
//  Copyright (C) 2016 МИРЭА
//
//---------------------------------------------------------------------------

#include "stm32_p407.h"                 //Файл конфигурации отладочной платы STM32-P407


//Прототипы вызываемых функций
void TimerConfig(void);

//---------------------------------------------------------------------------
// ГЛАВНАЯ ФУНКЦИЯ 
int main()
{
  //Инициализация органов управления
  STM_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO);  //Кнопка TAMPER
  STM_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO);  //Кнопка  WAKEUP
  STM_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);   //Позиции джойстика
  STM_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);

  //Инициализация индикаторов LED3, LED4
  // (LED1, LED2 управляются напрямую от таймеров)
  STM_LEDInit(LED3); STM_LEDOff(LED3); 
  STM_LEDInit(LED4); STM_LEDOff(LED4);

  //Конфигурирование таймеров (см. файл tim_pwm.c)
  TimerConfig();
  
  //Цикл в основной программе
  while (1)
  {
    //Проверка воздействия на кнопку WAKEUP, при нажатии - выход из цикла
    if (STM_PBGetState(BUTTON_WAKEUP)) break;
  }

  //Сброс процессора - завершение выполнения данной программы, запуск начального загрузчика
  NVIC_SystemReset();
}

//---------------------------------------------------------------------------
// ОБРАБОТЧИК ПРЕРЫВАНИЯ ОТ ТАЙМЕРА 4
void TIM4_IRQHandler(void)
{
  //Проверка установленного флага (источника прерывания)
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //Очистка флага
    STM_LEDToggle(LED3);                         //Управление индикатором LED3
  }
}
//---------------------------------------------------------------------------
