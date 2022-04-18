/*******************************************************************************
 *  Файл:     stm32_ports.h 
 *  Автор:    МИРЭА
 *  Версия:   V1.1
 *  Дата:     01.09.2015
 *  Описание: Функции работы с портами ввода-вывода
 *******************************************************************************
*/
  
#ifndef __STM32_PORTS_H
#define __STM32_PORTS_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
#include "stm32f4xx.h"

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ВЫХОДНЫХ РАЗРЯДОВ ПОРТА
// Входные параметры:
//     GPIO_Port - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//     GPIO_Pin  - маска разряда(ов) (GPIO_Pin_0...GPIO_Pin_15 или 0x0001...0x8000)
// Режимы по умолчанию: двухтактный выход, скорость 25 МГц
void PortOutputInit(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin);

// То же, с установкой выходного уровня лог. 1
void PortOutput1Init(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin);

//---------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ВХОДНОГО РАЗРЯДА ПОРТА
// Входные параметры:
//     GPIO_Port      - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//     GPIO_Pin       - маска разряда (GPIO_Pin_0...GPIO_Pin_15 или 0x0001...0x8000)
//     GPIO_PuPd      - подтяжка (GPIO_PuPd_NOPULL, GPIO_PuPd_UP, GPIO_PuPd_DOWN)
//     EXTI_Trigger   - 0 - нет прерывания или фронт:
//                      EXTI_Trigger_Rising, EXTI_Trigger_Falling, EXTI_Trigger_Rising_Falling
void PortInputInit(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin, GPIOPuPd_TypeDef GPIO_PuPd,
                   uint32_t EXTI_Trigger);

//---------------------------------------------------------------------------
// УСТАНОВКА/СБРОС/ИНВЕРСИЯ ОДНОГО РАЗРЯДА ПОРТА
// Входные параметры:  
//     GPIOx    - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//     GPIO_Pin - маска разряда (GPIO_Pin_0...GPIO_Pin_15 или 0x0001...0x8000)
//     BitVal   - значение бита (true/false или 0/1)
void PortWriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, bool BitVal);
void PortToggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

//---------------------------------------------------------------------------
// ЧТЕНИЕ СОСТОЯНИЯ РАЗРЯДА ВХОДНОГО ПОРТА
// Входные параметры:  
//     GPIOx    - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//     GPIO_Pin - маска разряда (GPIO_Pin_0...GPIO_Pin_15 или 0x0001...0x8000)
// Возвращается: состояние - 0 или 1 (в формате 8-битного кода)
uint8_t PortReadBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

//---------------------------------------------------------------------------
// ВВОД ИЗ ПОРТА
// Входные параметры:  GPIOx - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
// Возвращается: 16-битный код
uint16_t PortRead(GPIO_TypeDef* GPIOx);

//---------------------------------------------------------------------------
// ВЫВОД В ПОРТ
// Входные параметры:  
//    GPIOx - указатель на порт (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//    PortVal - 16-разрядное выводимое значение
void PortWrite(GPIO_TypeDef* GPIOx, uint16_t PortVal);


#define PortReadBit  GPIO_ReadInputDataBit      //Синонимы имен функций
#define PortRead     GPIO_ReadInputData
#define PortWrite    GPIO_Write

#ifdef __cplusplus
}
#endif

#endif /* __STM32_PORTS_H */

/************************END OF FILE****/
