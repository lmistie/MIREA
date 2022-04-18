//---------------------------------------------------------------------------
// 
//  УЧЕБНЫЙ ПРОЕКТ
//  Использование таймеров для измерений
//  Copyright (C) 2016 МИРЭА
//
//---------------------------------------------------------------------------

#include "math.h"                       //Математические функции стандартной библиотеки C/C++
#include "stm32_p407.h"                 //Файл конфигурации отладочной платы STM32-P407
#include "lcd.h"                        //Функции работы с дисплеем

uint32_t Screen = 0;                    //Номер экрана (0 - исходный, 1 - метки времени)
uint32_t SysTickPeriod = 10000000;      //Период счета системного таймера, тактов

//Прототипы функций, реализованных в этом же файле, но после операторов их вызова 
void BaseScreen(void);
void TimeMarkerOut(void);
void TimeMarker1(void);
void TimeMarker2(void);
void TimeMarker3(void);
void TimeMarker4(void);
void TimeMarker5(void);
void TimeMarker6(void);

//---------------------------------------------------------------------------
// ГЛАВНАЯ ФУНКЦИЯ 
int main()
{
  volatile int i;                             //Счетчик циклов
  float farg = 2;                             //Аргументы функции
  double darg = 1;

  //Инициализация кнопок
  STM_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO);
  STM_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO);

  //Инициализация дисплея, вывод начального сообщения
  LCD_Init();
  BaseScreen();

  //Инициализация измерительного таймера
  SysTick->LOAD = SysTickPeriod - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

  //Начало бесконечного цикла в программе
  while (1)
  {
    //ФИКСАЦИЯ ВРЕМЕН ВЫЧИСЛЕНИЙ ФРАГМЕНТОВ ПРОГРАММЫ
    TimeMarker1();                            //Метка времени 1
    for (i = 200; i; i--) sin(darg);

    TimeMarker2();                            //Метка времени 2
    for (i = 200; i; i--) sinf(farg);

    TimeMarker3();                            //Метка времени 3
    for (i = 200; i; i--) sinhf(farg);

    TimeMarker4();                            //Метка времени 4
    for (i = 200; i; i--) fabsf(farg);

    TimeMarker5();                            //Метка времени 5
    for (i = 200; i; i--) powf(farg, farg);
    
    TimeMarker6();                            //Метка времени 6
    
/*
    //Варианты с другими функциями
    TimeMarker1();
    for (i = 100; i; i--) tanf(farg);
    
    TimeMarker2();
    for (i = 100; i; i--) hypotf(farg, farg);

    TimeMarker3();
    for (i = 100; i; i--) asinf(farg);

    TimeMarker4();
    for (i = 100; i; i--) log10f(farg);
    //for (i = 100; i; i--) expf(farg);

    TimeMarker5();
    for (i = 100; i; i--) sqrtf(farg);
    //for (i = 100; i; i--) sqrt(farg);
    
    TimeMarker6();
*/

    //Проверка нажатия кнопки TAMPER
    if (!STM_PBGetState(BUTTON_TAMPER))
    {
      if (Screen) BaseScreen();                //Переключение между исходным экраном
      else TimeMarkerOut();                    // и экраном вывода меток времени
      while (!STM_PBGetState(BUTTON_TAMPER));  //Ожидание отпускания кнопки
    }
      
    //Проверка нажатия кнопки WAKEUP
    if (STM_PBGetState(BUTTON_WAKEUP)) NVIC_SystemReset();
  }
}

//---------------------------------------------------------------------------
// ФОРМИРОВАНИЕ ИСХОДНОГО СООБЩЕНИЯ
void BaseScreen(void)
{
  Screen = 0;
  LCD_Clear(1);
  LCD_FontSize(11);
  LCD_FontColor(0xF0F);
  LCD_print(" Вывод меток \r\n"
            " времени \r\n"
            " выполнения: \r\n"
            " кнопка \r\n"
            " \"TAMPER\" \r\n\r\n"
            "Выход - \"WAKEUP\"");
}

//---------------------------------------------------------------------------

uint32_t TimeCur1, TimeCur2, TimeCur3,   //Текущие значения меток времени
         TimeCur4, TimeCur5, TimeCur6;  
uint32_t TimeFix1, TimeFix2, TimeFix3,   //Зафиксированные значения меток времени
         TimeFix4, TimeFix5, TimeFix6;  

// ВЫВОД ВРЕМЕННЫХ МЕТОК
void TimeMarkerOut(void)
{
  Screen = 1;
  LCD_Clear(1);
  LCD_FontSize(11);
  LCD_FontColor(0xFFF);
  LCD_TextPos(0,0);
  LCD_print("N  Такты\r\n");
  LCD_FontColor(0x4F4);
  LCD_print("1: %07d \r\n", TimeFix1);
  LCD_print("2: %07d \r\n", TimeFix2);
  LCD_print("3: %07d \r\n", TimeFix3);
  LCD_print("4: %07d \r\n", TimeFix4);
  LCD_print("5: %07d \r\n", TimeFix5);
  LCD_print("6: %07d ",     TimeFix6);
}

//---------------------------------------------------------------------------
// ФИКСАЦИЯ МЕТОК ВРЕМЕНИ ВЫПОЛНЕНИЯ
// Для последнего измерения - сохранение всех значений для вывода
void TimeMarker1(void)
{
  TimeCur1 = SysTick->VAL;
}
void TimeMarker2(void)
{
  TimeCur2 = SysTick->VAL;
}
void TimeMarker3(void)
{
  TimeCur3 = SysTick->VAL;
}
void TimeMarker4(void)
{
  TimeCur4 = SysTick->VAL;
}
void TimeMarker5(void)
{
  TimeCur5 = SysTick->VAL;
}
void TimeMarker6(void)
{
  TimeCur6 = SysTick->VAL;
  TimeFix1 = TimeCur1, TimeFix2 = TimeCur2, TimeFix3 = TimeCur3,
  TimeFix4 = TimeCur4, TimeFix5 = TimeCur5, TimeFix6 = TimeCur6;
}

//---------------------------------------------------------------------------
