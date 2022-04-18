/*******************************************************************************
 *  ����:     stm32_ports.h 
 *  �����:    �����
 *  ������:   V1.1
 *  ����:     01.09.2015
 *  ��������: ������� ������ � ������� �����-������
 *******************************************************************************
*/
  
#ifndef __STM32_PORTS_H
#define __STM32_PORTS_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
#include "stm32f4xx.h"

//---------------------------------------------------------------------------
// ������������� �������� �������� �����
// ������� ���������:
//     GPIO_Port - ��������� �� ���� (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//     GPIO_Pin  - ����� �������(��) (GPIO_Pin_0...GPIO_Pin_15 ��� 0x0001...0x8000)
// ������ �� ���������: ����������� �����, �������� 25 ���
void PortOutputInit(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin);

// �� ��, � ���������� ��������� ������ ���. 1
void PortOutput1Init(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin);

//---------------------------------------------------------------------------
// ������������� �������� ������� �����
// ������� ���������:
//     GPIO_Port      - ��������� �� ���� (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//     GPIO_Pin       - ����� ������� (GPIO_Pin_0...GPIO_Pin_15 ��� 0x0001...0x8000)
//     GPIO_PuPd      - �������� (GPIO_PuPd_NOPULL, GPIO_PuPd_UP, GPIO_PuPd_DOWN)
//     EXTI_Trigger   - 0 - ��� ���������� ��� �����:
//                      EXTI_Trigger_Rising, EXTI_Trigger_Falling, EXTI_Trigger_Rising_Falling
void PortInputInit(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin, GPIOPuPd_TypeDef GPIO_PuPd,
                   uint32_t EXTI_Trigger);

//---------------------------------------------------------------------------
// ���������/�����/�������� ������ ������� �����
// ������� ���������:  
//     GPIOx    - ��������� �� ���� (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//     GPIO_Pin - ����� ������� (GPIO_Pin_0...GPIO_Pin_15 ��� 0x0001...0x8000)
//     BitVal   - �������� ���� (true/false ��� 0/1)
void PortWriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, bool BitVal);
void PortToggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

//---------------------------------------------------------------------------
// ������ ��������� ������� �������� �����
// ������� ���������:  
//     GPIOx    - ��������� �� ���� (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//     GPIO_Pin - ����� ������� (GPIO_Pin_0...GPIO_Pin_15 ��� 0x0001...0x8000)
// ������������: ��������� - 0 ��� 1 (� ������� 8-������� ����)
uint8_t PortReadBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

//---------------------------------------------------------------------------
// ���� �� �����
// ������� ���������:  GPIOx - ��������� �� ���� (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
// ������������: 16-������ ���
uint16_t PortRead(GPIO_TypeDef* GPIOx);

//---------------------------------------------------------------------------
// ����� � ����
// ������� ���������:  
//    GPIOx - ��������� �� ���� (GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH)
//    PortVal - 16-��������� ��������� ��������
void PortWrite(GPIO_TypeDef* GPIOx, uint16_t PortVal);


#define PortReadBit  GPIO_ReadInputDataBit      //�������� ���� �������
#define PortRead     GPIO_ReadInputData
#define PortWrite    GPIO_Write

#ifdef __cplusplus
}
#endif

#endif /* __STM32_PORTS_H */

/************************END OF FILE****/
