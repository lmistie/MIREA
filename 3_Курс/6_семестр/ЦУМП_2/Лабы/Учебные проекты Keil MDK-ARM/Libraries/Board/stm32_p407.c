/**
  ******************************************************************************
  * Файл      stm32_p407.c
  * Автор     STMicroelectronics, коррекция *AB*
  * Версия    V1.0
  * Дата      19-March-2012, коррекция 01.09.2015
  * Описание  Обслуживание светодиодных индикаторов, кнопок, последовательных портов
  ******************************************************************************
  */ 

/* Подключаемые заголовочные файлы */
#include "stm32_p407.h"


GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT,
                                 LED4_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN,
                                 LED4_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK,
                                 LED4_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {WAKEUP_BUTTON_GPIO_PORT, TAMPER_BUTTON_GPIO_PORT,
                                      RIGHT_BUTTON_GPIO_PORT, LEFT_BUTTON_GPIO_PORT,
                                      UP_BUTTON_GPIO_PORT,DOWN_BUTTON_GPIO_PORT, SEL_BUTTON_GPIO_PORT}; 

const uint16_t BUTTON_PIN[BUTTONn] = {WAKEUP_BUTTON_PIN, TAMPER_BUTTON_PIN,
                                      RIGHT_BUTTON_PIN, LEFT_BUTTON_PIN,
                                      UP_BUTTON_PIN,DOWN_BUTTON_PIN, SEL_BUTTON_PIN}; 

const uint32_t BUTTON_CLK[BUTTONn] = {WAKEUP_BUTTON_GPIO_CLK, TAMPER_BUTTON_GPIO_CLK,
                                      RIGHT_BUTTON_GPIO_CLK, LEFT_BUTTON_GPIO_CLK,
                                      UP_BUTTON_GPIO_CLK,DOWN_BUTTON_GPIO_CLK, SEL_BUTTON_GPIO_CLK};

const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {WAKEUP_BUTTON_EXTI_LINE,
                                            TAMPER_BUTTON_EXTI_LINE, 
                                            RIGHT_BUTTON_EXTI_LINE,
                                            LEFT_BUTTON_EXTI_LINE, 
                                            UP_BUTTON_EXTI_LINE,
                                            DOWN_BUTTON_EXTI_LINE,
                                            SEL_BUTTON_EXTI_LINE};

const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {WAKEUP_BUTTON_EXTI_PORT_SOURCE,
                                              TAMPER_BUTTON_EXTI_PORT_SOURCE, 
                                              RIGHT_BUTTON_EXTI_PORT_SOURCE,
                                              LEFT_BUTTON_EXTI_PORT_SOURCE, 
                                              UP_BUTTON_EXTI_PORT_SOURCE,
                                              DOWN_BUTTON_EXTI_PORT_SOURCE,
                                              SEL_BUTTON_EXTI_PORT_SOURCE};
								 
const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {WAKEUP_BUTTON_EXTI_PIN_SOURCE,
                                             TAMPER_BUTTON_EXTI_PIN_SOURCE, 
                                             RIGHT_BUTTON_EXTI_PIN_SOURCE,
                                             LEFT_BUTTON_EXTI_PIN_SOURCE, 
                                             UP_BUTTON_EXTI_PIN_SOURCE,
                                             DOWN_BUTTON_EXTI_PIN_SOURCE,
                                             SEL_BUTTON_EXTI_PIN_SOURCE}; 
const uint16_t BUTTON_IRQn[BUTTONn] = {WAKEUP_BUTTON_EXTI_IRQn, TAMPER_BUTTON_EXTI_IRQn,
                                       RIGHT_BUTTON_EXTI_IRQn,
                                       LEFT_BUTTON_EXTI_IRQn, UP_BUTTON_EXTI_IRQn,
                                       DOWN_BUTTON_EXTI_IRQn, SEL_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {EVAL_COM1,EVAL_COM2};

GPIO_TypeDef* COM_TX_PORT[COMn] = {EVAL_COM1_TX_GPIO_PORT, EVAL_COM2_TX_GPIO_PORT};

GPIO_TypeDef* COM_RX_PORT[COMn] = {EVAL_COM1_RX_GPIO_PORT,EVAL_COM2_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {EVAL_COM1_CLK,EVAL_COM2_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {EVAL_COM1_TX_GPIO_CLK,EVAL_COM2_TX_GPIO_CLK};

const uint32_t COM_RX_PORT_CLK[COMn] = {EVAL_COM1_RX_GPIO_CLK, EVAL_COM2_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {EVAL_COM1_TX_PIN, EVAL_COM2_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {EVAL_COM1_RX_PIN, EVAL_COM2_RX_PIN};

const uint16_t COM_TX_PIN_SOURCE[COMn] = {EVAL_COM1_TX_SOURCE, EVAL_COM2_TX_SOURCE};

const uint16_t COM_RX_PIN_SOURCE[COMn] = {EVAL_COM1_RX_SOURCE, EVAL_COM1_RX_SOURCE};

const uint16_t COM_TX_AF[COMn] = {EVAL_COM1_TX_AF, EVAL_COM2_TX_AF};

const uint16_t COM_RX_AF[COMn] = {EVAL_COM1_RX_AF, EVAL_COM2_RX_AF};

NVIC_InitTypeDef   NVIC_InitStructure;


/*******************************************************************************
* Функция:   STM_EVAL_GPIOReset  
*            Сброс всех портов общего назначения
* Параметры: Нет
* Возврат:   Нет
*/
void STM_EVAL_GPIOReset(void)
{
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOB);
  GPIO_DeInit(GPIOC);
  GPIO_DeInit(GPIOD);
  GPIO_DeInit(GPIOE);
  GPIO_DeInit(GPIOF);
  GPIO_DeInit(GPIOG);
  GPIO_DeInit(GPIOH);
  GPIO_DeInit(GPIOI);
}

/*******************************************************************************
* Функция:   STM_EVAL_LEDInit  
*            Конфигурирование светодиодного индикатора
* Параметры: Led - индекс индикатора (LED1..LED4 = 0..3)
* Возврат:   Нет
*/
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Разрешение тактирования связанной с портом шины */
  RCC_AHB1PeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Конфигурирование разряда порта, связанного с индикатором */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];       /* Маска разряда порта */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      /* Режим ввода/вывода: вывод */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     /* Выходной буфер: двухтактный */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       /* Подтяжка: к напряжению питания */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  /* Скорость вывода */
  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);    /* Конфигурирование */
}

/*******************************************************************************
* Функция:   STM_EVAL_LEDOn  
*            Включение светодиодного индикатора
* Параметры: Led - индекс индикатора (LED1..LED4 = 0..3)
* Возврат:   Нет
*/
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRRL = GPIO_PIN[Led];
}

/*******************************************************************************
* Функция:   STM_EVAL_LEDOff  
*            Выключение светодиодного индикатора
* Параметры: Led - индекс индикатора (LED1..LED4 = 0..3)
* Возврат:   Нет
*/
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRRH = GPIO_PIN[Led];  
}

/*******************************************************************************
* Функция:   STM_EVAL_LEDToggle  
*            Переключение светодиодного индикатора
* Параметры: Led - индекс индикатора (LED1..LED4 = 0..3)
* Возврат:   Нет
*/
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}

/*******************************************************************************
* Функция:   STM_EVAL_PBInit  
*            Конфигурирование органа управления - кнопки
* Параметры: Button - индекс кнопки: BUTTON_WAKEUP = 0, BUTTON_TAMPER, BUTTON_RIGHT,
*                BUTTON_LEFT, BUTTON_UP, BUTTON_DOWN, BUTTON_SEL
*            Button_Mode - режим ввода состояния кнопки:
*                BUTTON_MODE_GPIO - простой ввод
*                BUTTON_MODE_EXTI - генерация прерывания (кнопка подключена к линии EXTI)
* Возврат:   Нет
*/
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Разрешение тактирования шины */
  RCC_AHB1PeriphClockCmd(BUTTON_CLK[Button], ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Конфигурирование разряда порта как входного */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

  /* Конфигурирование прерывания */
  if (Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Подключение линии запроса прерывания EXTI к соответствующему разряду порта */
    SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

    /* Конфигурирование линии запроса прерывания */
    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

    /* Для кнопки WAKEUP прерывание по фронту, для остальных - по спаду */
    if(Button != BUTTON_WAKEUP)
    {
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    }
    else
    {
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    }
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Задание низкого приоритета и разрешение прерывания от линии EXTI */
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  }
}

/*******************************************************************************
* Функция:   STM_EVAL_PBGetState  
*            Получение состояния кнопки
* Параметры: Button - индекс кнопки: BUTTON_WAKEUP = 0, BUTTON_TAMPER, BUTTON_RIGHT,
*                BUTTON_LEFT, BUTTON_UP, BUTTON_DOWN, BUTTON_SEL
* Возврат:   Уровень сигнала с кнопки (0 или 1)
*/
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
  return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/*******************************************************************************
* Функция:   STM_EVAL_COMInit  
*            Конфигурирование последовательного порта (USART)
* Параметры: COM - индекс порта: COM1, COM2
*            USART_InitStruct - указатель на структуру параметров порта
* Возврат:   Нет
*/
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Разрешение тактирования шины */
  RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

  if (COM == COM1)
  {
    /* Разрешение тактирования UART */
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }

  /* Подключение PXx к USARTx_Tx */
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* Подключение PXx to USARTx_Rx */
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

  /* Конфигурирование разряда USART Tx как альтернативной функции */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* Конфигурирование разряда USART Rx как альтернативной функции */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  /* Конфигурование USART */
  USART_Init(COM_USART[COM], USART_InitStruct);
    
  /* Разрешение работы USART */
  USART_Cmd(COM_USART[COM], ENABLE);
}

/*******************************************************************************
* ПОДПРОГРАММЫ ВРЕМЕННЫХ ЗАДЕРЖЕК
* Входной аргумент: значение задержки в миллисекундах / микросекундах
* Задержка подобрана для уровня оптимизации "Level 0" и загрузки программы в ОЗУ
*/
void Delay_ms(int ms)
{
  volatile int i;
  while (ms--) { i = 28000; while (--i); }
}
void Delay_us(int us)
{
  volatile int i;
  while (us--) { i = 20; while (--i); }
}
    
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
