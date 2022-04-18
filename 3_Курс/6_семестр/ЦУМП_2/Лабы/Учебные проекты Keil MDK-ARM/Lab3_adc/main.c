//---------------------------------------------------------------------------
// 
//  УЧЕБНЫЙ ПРОЕКТ
//  Измерения с помощью аналого-цифрового преобразователя
//  Copyright (C) 2016 МИРЭА
//
//  Для тестирования работы АЦП используется установленный на плате потенциометр
//  (триммер), посредством которого на разряде порта PC0 можно изменять напряжение
//  в пределах 0...3.3 В. Разряд PC0 имеет альтернативные функции: аналоговые входы
//  ADC1_IN10, ADC2_IN10, ADC3_IN10.
//  Проект иллюстрирует измерение медленно меняющихся уровней сигнала, слежение
//  за выходом значений за заданные границы.
//
//---------------------------------------------------------------------------

#include "stm32_p407.h"                         //Файл конфигурации отладочной платы STM32-P407
#include "adc.h"                                //Функции для работы с АЦП
#include "lcd.h"                                //Функции для работы с дисплеем

#define  AVERAGE  200                           //Число усреднений данных с АЦП

//---------------------------------------------------------------------------
// ГЛАВНАЯ ФУНКЦИЯ
int main()
{
  long AdcData;                                 //Суммированные данные с АЦП
  unsigned short UData16;                       //Приведенный смещенный (беззнаковый) код 
  short Data16;                                 //Приведенный дополнительный (знаковый) код
  int Average = 1;                              //Текущее число усреднений
  int i;                                        //Текущая переменная цикла
  NVIC_InitTypeDef NVIC_InitStructure;          //Структура конфигурации прерываний
  
  //Инициализация кнопок
  STM_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO);  //Кнопка WAKEUP выхода из программы
  STM_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);   //Позиция джойстика для включения режима усреднения
  STM_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);    //Позиция джойстика для выключения режима усреднения
  STM_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);    //Позиция джойстика для временной остановки измерений

  //Инициализация индикаторов
  STM_LEDInit(LED3); STM_LEDOff(LED3);
  
  //Инициализация дисплея
  LCD_Init(); LCD_Clear(1);
  LCD_FontSize(11);
  LCD_FontColor(0x0F0);
  LCD_TextPos(1,1); LCD_print("ВЫРАВНИВ.ВЛЕВО");
  LCD_TextPos(1,4); LCD_print("ВЫРАВНИВ.ВПРАВО");
  LCD_FontColor(0x08F);
  LCD_TextPos(1,2); LCD_print("Смещ.  Дополн.");
  LCD_TextPos(1,5); LCD_print("Смещ.  Дополн.");
  LCD_FontColor(0xFFF);

  //Инициализация и первичный запуск АЦП
  ADC_Initialize(ADC2, ADC_Channel_10, 0);
  ADC_SoftwareStartConv(ADC2);

  //Конфигурирование прерываний от АЦП
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;                   //Номер (линия) прерывания
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;        //Приоритет группы
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;               //Приоритет внутри группы
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //Разрешение/запрет прерывания
  NVIC_Init(&NVIC_InitStructure);                                  //Функция конфигурирования

  //Программирование АЦП в режиме "аналогового сторожа"
  ADC_AnalogWatchdogSingleChannelConfig(ADC2, ADC_Channel_10);     //Выбор канала
  ADC_AnalogWatchdogThresholdsConfig(ADC2, 3500, 500);             //Верхний и нижний пороги
  ADC_ITConfig(ADC2, ADC_IT_AWD, ENABLE);                          //Разрешение прерываний от "сторожа"
  ADC_AnalogWatchdogCmd(ADC2, ADC_AnalogWatchdog_SingleRegEnable); //Разрешение "аналогового сторожа"

  //Цикл в основной программе
  while (1)
  { 
    //Включение/отключение режима усреднения
    if (STM_PBGetState(BUTTON_RIGHT))
    { Average = AVERAGE;
      LCD_TextPos(1,0); LCD_print("- Усреднение -");
    }
    if (STM_PBGetState(BUTTON_LEFT))
    { Average = 1;
      LCD_TextPos(1,0); LCD_print("              ");
    }
    
    //Пользовательская остановка измерений для чтения показаний
    while (STM_PBGetState(BUTTON_DOWN));
    
    //Цикл по измерениям
    for (i = Average, AdcData = 0; i > 0; i--)
    {
      while (!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)); //Ожидание окончания преобразования
      AdcData += ADC_GetConversionValue(ADC2);        //Ввод и суммирование показаний
      ADC_SoftwareStartConv(ADC2);                    //Новый запуск преобразования
      Delay_ms(1);
    }
    
    //Преобразование кода с АЦП
    UData16 = AdcData / Average;                      //Вычисление среднего
    Data16 = UData16 ^ 0x8000;                        //Преобразование смещенного кода в дополнительный

    //Вывод результатов на дисплей
    LCD_TextPos(1,3); LCD_print("%5d", UData16);      //Смещенный с выравниванием влево
    LCD_TextPos(8,3); LCD_print("%6d", Data16);       //Дополнительный с выравниванием влево
    Data16 >>= 4; UData16 >>= 4;                      //Имитация выравнивания вправо
    LCD_TextPos(1,6); LCD_print("%5d", UData16);      //Смещенный с выравниванием вправо
    LCD_TextPos(8,6); LCD_print("%6d", Data16);       //Дополнительный с выравниванием вправо
    
    if (Average < AVERAGE) Delay_ms(300);             //Период обновления индикации 
    STM_LEDOff(LED3);                                 //Гашение индикатора "аналогового сторожа"
    
    //Проверка нажатия кнопки WAKEUP завершения работы (сброса процессора)
    if (STM_PBGetState(BUTTON_WAKEUP)) NVIC_SystemReset();
  }
}

//---------------------------------------------------------------------------
// ОБРАБОТЧИК ПРЕРЫВАНИЯ ОТ АЦП
void ADC_IRQHandler(void)
{
  if (ADC_GetFlagStatus(ADC2, ADC_FLAG_AWD) != RESET) //Проверка флага "аналогового сторожа"
  {
    ADC_ClearFlag(ADC2, ADC_FLAG_AWD);                //Сброс флага 
    STM_LEDOn(LED3);                                  //Сигнализация о выходе уровня за пределы
  }
}

//---------------------------------------------------------------------------
