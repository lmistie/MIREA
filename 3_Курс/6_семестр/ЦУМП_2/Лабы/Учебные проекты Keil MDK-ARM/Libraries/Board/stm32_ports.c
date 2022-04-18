/*******************************************************************************
 *
 *  Файл:     stm32_ports.c 
 *  Автор:    МИРЭА
 *  Версия:   V1.1
 *  Дата:     01.09.2015
 *  Описание: Функции работы с портами ввода-вывода
 * 
 *******************************************************************************
*/
  
#include "stm32_ports.h"

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ВЫХОДНЫХ РАЗРЯДОВ ПОРТА
// Вход:  GPIO_Port - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//        GPIO_Pin  - маска разряда(ов) (GPIO_Pin_0...GPIO_Pin_15 или 0x0001...0x8000)
// Режимы по умолчанию: двухтактный выход, скорость 25 МГц
void PortOutputInit(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  uint32_t RCC_AHB1Periph = 0;

  switch ((uint32_t)GPIO_Port)
  { case GPIOA_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOA; break;
    case GPIOB_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOB; break;
    case GPIOC_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC; break;
    case GPIOD_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOD; break;
    case GPIOE_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOE; break;
    case GPIOF_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOF; break;
    case GPIOG_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOG; break;
    case GPIOH_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOH; break;
  }
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO_Port, &GPIO_InitStructure);
}

// То же, с установкой выходного уровня лог. 1
void PortOutput1Init(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  uint32_t RCC_AHB1Periph = 0;

  switch ((uint32_t)GPIO_Port)
  { case GPIOA_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOA; break;
    case GPIOB_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOB; break;
    case GPIOC_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC; break;
    case GPIOD_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOD; break;
    case GPIOE_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOE; break;
    case GPIOF_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOF; break;
    case GPIOG_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOG; break;
    case GPIOH_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOH; break;
  }
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);
  PortWriteBit(GPIO_Port, GPIO_Pin, 1);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO_Port, &GPIO_InitStructure);
}

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ВХОДНОГО РАЗРЯДА ПОРТА
// Вход:  GPIO_Port      - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//        GPIO_Pin       - маска разряда (GPIO_Pin_0...GPIO_Pin_15 или 0x0001...0x8000)
//        GPIO_PuPd      - подтяжка (GPIO_PuPd_NOPULL, GPIO_PuPd_UP, GPIO_PuPd_DOWN)
//        EXTI_Trigger   - 0 - нет прерывания или фронт:
//                         EXTI_Trigger_Rising, EXTI_Trigger_Falling, EXTI_Trigger_Rising_Falling
void PortInputInit(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin, GPIOPuPd_TypeDef GPIO_PuPd,
                   uint32_t EXTI_Trigger)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  uint32_t RCC_AHB1Periph = 0;
  uint8_t EXTI_PortSource = 0;
  uint8_t NVIC_IRQChannel;
  uint16_t PinSource, Pin;
  
  switch ((uint32_t)GPIO_Port)
  { case GPIOA_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOA; EXTI_PortSource = EXTI_PortSourceGPIOA; break;
    case GPIOB_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOB; EXTI_PortSource = EXTI_PortSourceGPIOB; break;
    case GPIOC_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOC; EXTI_PortSource = EXTI_PortSourceGPIOC; break;
    case GPIOD_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOD; EXTI_PortSource = EXTI_PortSourceGPIOD; break;
    case GPIOE_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOE; EXTI_PortSource = EXTI_PortSourceGPIOE; break;
    case GPIOF_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOF; EXTI_PortSource = EXTI_PortSourceGPIOF; break;
    case GPIOG_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOG; EXTI_PortSource = EXTI_PortSourceGPIOG; break;
    case GPIOH_BASE: RCC_AHB1Periph = RCC_AHB1Periph_GPIOH; EXTI_PortSource = EXTI_PortSourceGPIOH; break;
  }
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_Init(GPIO_Port, &GPIO_InitStructure);

  if (EXTI_Trigger)
  {
    for (PinSource = 0, Pin = GPIO_Pin; PinSource < 15; PinSource++, Pin >>= 1)
      if (Pin & 1) break;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSource, PinSource);
    EXTI_InitStructure.EXTI_Line = GPIO_Pin;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = (EXTITrigger_TypeDef)EXTI_Trigger;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    if (PinSource >= GPIO_PinSource10) NVIC_IRQChannel = EXTI15_10_IRQn;
    else if (PinSource >= GPIO_PinSource5) NVIC_IRQChannel = EXTI9_5_IRQn;
         else NVIC_IRQChannel = EXTI0_IRQn + PinSource;

    NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
  }  
}

//---------------------------------------------------------------------------
// УСТАНОВКА/СБРОС/ИНВЕРСИЯ ОДНОГО РАЗРЯДА ПОРТА
// Вход:  GPIOx    - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//        GPIO_Pin - маска разряда (GPIO_Pin_0...GPIO_Pin_15 или 0x0001...0x8000)
//        BitVal   - значение бита (true/false или 0/1)
// В stm32f4xx_gpio.c : void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal) 
//                      void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
void PortWriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, bool BitVal)
{
  if (BitVal) GPIOx->BSRRL = GPIO_Pin;
  else GPIOx->BSRRH = GPIO_Pin ;
}

void PortToggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->ODR ^= GPIO_Pin;
}

//---------------------------------------------------------------------------
// ЧТЕНИЕ СОСТОЯНИЯ РАЗРЯДА ВХОДНОГО ПОРТА
// Вход:  GPIOx    - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//        GPIO_Pin - маска разряда (GPIO_Pin_0...GPIO_Pin_15 или 0x0001...0x8000)
// Возвращается: состояние - 0 или 1 (в формате 8-битного кода)
// В stm32f4xx_gpio.c :   uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
// Альтернативное имя :           PortReadBit


//---------------------------------------------------------------------------
// ВВОД ИЗ ПОРТА
// Вход:  GPIOx - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
// Возвращается: 16-битный код
// В stm32f4xx_gpio.c :   uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
// Альтернативное имя :            PortRead

//---------------------------------------------------------------------------
// ВЫВОД В ПОРТ
// Вход:  GPIOx - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//        PortVal - 16-разрядное выводимое значение
// В stm32f4xx_gpio.c :   void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
// Альтернативное имя :        PortWrite

//---------------------------------------------------------------------------
