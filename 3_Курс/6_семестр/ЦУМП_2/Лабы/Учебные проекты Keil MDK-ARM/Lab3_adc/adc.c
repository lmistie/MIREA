/**
  ******************************************************************************
  * Файл      adc.c
  * Copyright (C) 2016 МИРЭА
  * Версия    2.1
  * Описание: Функции инициализации и обслуживания АЦП
  *
  * Формат цифровых данных от АЦП - 12-разрядный смещенный код:
  *   код 1111 1111 1111  соответствует Uвх = максимум (практически Vref);
  *   код 1000 0000 0000  соответствует Uвх = Vref / 2;
  *   код 0000 0000 0000  соответствует Uвх = минимум (практически 0); 
  * где Vref - опорное напряжение (для отладочной платы равно напряжению питания)
  *
  * Сигналы запуска АЦП: выходы таймеров TIM1, TIM2, TIM3, TIM4, TIM5, TIM8;
  * внешний сигнал.
  *
  * Распределение аналоговых входов по разрядам портов и источникам:
  *   Порт     ADC      Channel
  *   PA0      1,2,3    0
  *   PA1      1,2,3    1
  *   PA2      1,2,3    2
  *   PA3      1,2,3    3
  *   PA4      1,2      4
  *   PA5      1,2      5
  *   PA6      1,2      6
  *   PA7      1,2      7
  *   PB0      1,2      8
  *   PB1      1,2      9
  *   PC0      1,2,3    10
  *   PC1      1,2,3    11
  *   PC2      1,2,3    12
  *   PC3      1,2,3    13
  *   PC4      1,2      14
  *   PC5      1,2      15
  *   PF3      3        9
  *   PF4      3        14
  *   PF5      3        15
  *   PF6      3        4
  *   PF7      3        5
  *   PF8      3        6
  *   PF9      3        7
  *   PF10     3        8
  *    -       1        16   (внутренний температурный датчик)
  *    -       1        17   (опорное напряжение Vref внутреннего источника)
  *    -       1        18   (напряжение батареи Vbat)
  *
  * Примечания.
  *   1. Значение температуры с температурного датчика вычисляется по формуле:
  *          Температура в °C = {(VSENSE – V25) / Avg_Slope} + 25,
  *      где VSENSE - измеренное напряжение с датчика;
  *          V25 - напряжение с датчика при 25°C, для STM32F407 равно 0.76 В;
  *          Avg_Slope - температурный коэффициент, для STM32F407 равен 2.5 мВ/°C.
  *      Время преобразования АЦП должно быть не менее 10 мкс.
  *   2. На входе канала измерения напряжения батареи включен резистивный делитель на 2.
  *   3. Для разрешения измерений по внутренним каналам небходимо дополнительно вызывать
  *      функции ADC_TempSensorVrefintCmd(ENABLE) или ADC_VBATCmd(ENABLE).
  *
  ******************************************************************************
  */ 

#include "stm32_p407.h"                         //Файл конфигурации отладочной платы STM32-P407
#include "adc.h"                                //Функции для работы с АЦП

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ АЦП ДЛЯ РАБОТЫ С ОДИНОЧНЫМ КАНАЛОМ
// Входные аргументы функции:
//   ADCx - указатель на объект: ADC1, ADC2, ADC3
//   ADC_Channel - номер канала: ADC_Channel_0...ADC_Channel_18
//   ADC_ExternalTrigConv - источник запуска, одна из следущих констант:
//     ADC_ExternalTrigConv_T1_CC1
//     ADC_ExternalTrigConv_T1_CC2
//     ADC_ExternalTrigConv_T1_CC3
//     ADC_ExternalTrigConv_T2_CC2
//     ADC_ExternalTrigConv_T2_CC3
//     ADC_ExternalTrigConv_T2_CC4
//     ADC_ExternalTrigConv_T2_TRGO
//     ADC_ExternalTrigConv_T3_CC1
//     ADC_ExternalTrigConv_T3_TRGO
//     ADC_ExternalTrigConv_T5_CC1
//     ADC_ExternalTrigConv_T5_CC2
//     ADC_ExternalTrigConv_T5_CC3
//     ADC_ExternalTrigConv_T8_CC1
//     ADC_ExternalTrigConv_T8_TRGO
//     ADC_ExternalTrigConv_Ext_IT11
//   Если соответствующий источник не сконфигурирован для запуска, аргумент может быть любой, например, 0,
//   в этом случае запуск осуществляется программно - вызовом функции ADC_SoftwareStartConv.
//
void ADC_Initialize(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint32_t ADC_ExternalTrigConv)
{
  ADC_CommonInitTypeDef ADC_CommonInitStruct;   //Структура общей конфигурации АЦП
  ADC_InitTypeDef ADC_InitStructure;            //Структура конфигурации АЦП
  GPIO_InitTypeDef GPIO_InitStructure;          //Структура конфигурации портов общего назначения
  GPIO_TypeDef* GPIO_Port;                      //Указатель на базовый адрес порта
  uint32_t RCC_AHB1Periph;                      //Параметр для разрешения тактирования порта
  uint16_t GPIO_Pin;                            //Маска на разряд порта
  uint8_t flag_port = 1;                        //Признак использования разряда порта как входного
  
  //Сопоставление номеров АЦП и каналов разрядам портов
  switch (ADC_Channel)
  {
    case 0:  RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_0; break;
    case 1:  RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_1; break;
    case 2:  RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_2; break;
    case 3:  RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_3; break;
    case 4:  if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_6; 
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_4;
             break;
    case 5:  if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_7;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_5;
             break;
    case 6:  if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_8;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_6;
             break;
    case 7:  if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_9;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOA, GPIO_Port = GPIOA, GPIO_Pin = GPIO_Pin_7;
             break;
    case 8:  if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_10;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOB, GPIO_Port = GPIOB, GPIO_Pin = GPIO_Pin_0;
             break;
    case 9:  RCC_AHB1Periph = RCC_AHB1Periph_GPIOB, GPIO_Port = GPIOB, GPIO_Pin = GPIO_Pin_1; break;
    case 10: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_0; break;
    case 11: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_1; break;
    case 12: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_2; break;
    case 13: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_3; break;
    case 14: if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_4;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_4;
             break;
    case 15: if (ADCx == ADC3) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF, GPIO_Port = GPIOF, GPIO_Pin = GPIO_Pin_5;
             else RCC_AHB1Periph = RCC_AHB1Periph_GPIOC, GPIO_Port = GPIOC, GPIO_Pin = GPIO_Pin_5;
             break;
    default: flag_port = 0;        
  }
  
  //Конфигурирование разряда порта как аналогового входа
  if (flag_port)
  { RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);       //Разрешение тактирования порта
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;               //Номер разряда (маска)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;         //Альтернативная функция: аналоговый вход
    GPIO_Init(GPIO_Port, &GPIO_InitStructure);            //Функция конфигурирования
  }

  //Разрешение тактирования АЦП
  if (ADCx == ADC1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  if (ADCx == ADC2) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  if (ADCx == ADC3) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  //Задание общей конфигурации АЦП по умолчанию:
  // - режим одиночного АЦП;
  // - предварительный делитель тактовой частоты на 2;
  // - запрет режима прямого доступа к памяти при совместной работе АЦП;
  // - сдвиг между преобразованиями при совместной работе - 5 тактов.
  ADC_CommonStructInit(&ADC_CommonInitStruct);
  ADC_CommonInit(&ADC_CommonInitStruct);                         
  
  //Конфигурирование АЦП
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //Разрядность преобразования: 12 (возможно 10, 8, 6)
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;           //Многоканальное сканирование: не разрешено
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;     //Непрерывное преобразование: запрещено
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //Фронт сигнала запуска
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv;                //Источник запуска
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;   //Выравнивание кода в 16-разрядном поле: влево
  ADC_InitStructure.ADC_NbrOfConversion = 1;              //Число каналов для преобразования: 1
  ADC_Init(ADCx, &ADC_InitStructure);                     //Функция конфигурирования

  //Дополнительные параметры для АЦП:
  // - номер входного канала,
  // - номер группы преобразования - 1 (возможно до 16 групп),
  // - время преобразования - 56 тактов (возможные значения: 3, 15, 28, 56, 84, 112, 144, 480)
  ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_56Cycles);

  //Разрешение работы АЦП
  ADC_Cmd(ADCx, ENABLE);
}

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ АЦП ДЛЯ РАБОТЫ С НЕСКОЛЬКИМИ КАНАЛАМИ
// Входные аргументы функции:
//   ADCx - указатель на объект: ADC1, ADC2, ADC3
//   ADC_NbrOfConversion  - число каналов (от 1 до 16)
//   ADC_ExternalTrigConv - источник запуска, одна из констант (см. функцию выше)
//
void ADC_Init_MultiChannels(ADC_TypeDef* ADCx, uint8_t ADC_NbrOfConversion, uint32_t ADC_ExternalTrigConv)
{
  ADC_CommonInitTypeDef ADC_CommonInitStruct;             //Структура общей конфигурации АЦП
  ADC_InitTypeDef ADC_InitStructure;                      //Структура конфигурации АЦП

  //Разрешение тактирования АЦП
  if (ADCx == ADC1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  if (ADCx == ADC2) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  if (ADCx == ADC3) RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  //Задание общей конфигурации АЦП по умолчанию:
  // - режим одиночного АЦП;
  // - предварительный делитель тактовой частоты на 2;
  // - запрет режима прямого доступа к памяти при совместной работе АЦП;
  // - сдвиг между преобразованиями при совместной работе - 5 тактов.
  ADC_CommonStructInit(&ADC_CommonInitStruct);
  ADC_CommonInit(&ADC_CommonInitStruct);                         
  
  //Конфигурирование АЦП
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //Разрядность преобразования: 12 (возможно 10, 8, 6)
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;            //Многоканальное сканирование: разрешено
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;     //Непрерывное преобразование: запрещено
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //Фронт сигнала запуска
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv;                //Источник запуска
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;   //Выравнивание кода в 16-разрядном поле: влево
  ADC_InitStructure.ADC_NbrOfConversion = ADC_NbrOfConversion;                  //Число каналов
  ADC_Init(ADCx, &ADC_InitStructure);                     //Функция конфигурирования
}

//---------------------------------------------------------------------------
// КОНФИГУРИРОВАНИЕ КАНАЛА ПРИ МНОГОКАНАЛЬНОМ РЕЖИМЕ РАБОТЫ АЦП
// Входные аргументы функции:
//   ADCx - указатель на объект: ADC1, ADC2, ADC3
//   ADC_Channel - номер канала: ADC_Channel_0...ADC_Channel_18
//   Rank  - номер канала в последовательности опроса
//   ADC_SampleTime - время преобразования в тактах, одна из констант:
//     ADC_SampleTime_3Cycles, ADC_SampleTime_15Cycles, ADC_SampleTime_28Cycles, 
//     ADC_SampleTime_56Cycles, ADC_SampleTime_84Cycles, ADC_SampleTime_112Cycles,
//     ADC_SampleTime_144Cycles, ADC_SampleTime_480Cycles
//   GPIO_Port - базовый адрес порта: GPIOA, GPIOB, GPIOC GPIOF
//   GPIO_Pin - номер разряда порта в форме маски: GPIO_Pin_0...GPIO_Pin_15 
void ADC_ChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime,
                       GPIO_TypeDef* GPIO_Port, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;          //Структура конфигурации портов общего назначения
  uint32_t RCC_AHB1Periph;                      //Параметр для разрешения тактирования порта

  //Конфигурирование разряда порта
  if (GPIO_Port == GPIOA) RCC_AHB1Periph = RCC_AHB1Periph_GPIOA;
  else if (GPIO_Port == GPIOB) RCC_AHB1Periph = RCC_AHB1Periph_GPIOB;
       else if (GPIO_Port == GPIOC) RCC_AHB1Periph = RCC_AHB1Periph_GPIOC;
            else if (GPIO_Port == GPIOF) RCC_AHB1Periph = RCC_AHB1Periph_GPIOF;
                 else RCC_AHB1Periph = 0;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);       //Разрешение тактирования порта
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;               //Номер разряда (маска)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;         //Альтернативная функция: аналоговый вход
  GPIO_Init(GPIO_Port, &GPIO_InitStructure);            //Функция конфигурирования

  ADC_RegularChannelConfig(ADCx, ADC_Channel, Rank, ADC_SampleTime);
}
