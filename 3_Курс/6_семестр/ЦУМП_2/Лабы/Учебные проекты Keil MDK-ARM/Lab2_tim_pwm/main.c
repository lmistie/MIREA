//---------------------------------------------------------------------------
// 
//  ������� ������
//  ������������� �������� ��� ��������� ��������� ���������� � ���-�������� 
//  Copyright (C) 2016 �����
//
//---------------------------------------------------------------------------

#include "stm32_p407.h"                 //���� ������������ ���������� ����� STM32-P407


//��������� ���������� �������
void TimerConfig(void);

//---------------------------------------------------------------------------
// ������� ������� 
int main()
{
  //������������� ������� ����������
  STM_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO);  //������ TAMPER
  STM_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO);  //������  WAKEUP
  STM_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);   //������� ���������
  STM_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);
  STM_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);
  STM_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);
  STM_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);

  //������������� ����������� LED3, LED4
  // (LED1, LED2 ����������� �������� �� ��������)
  STM_LEDInit(LED3); STM_LEDOff(LED3); 
  STM_LEDInit(LED4); STM_LEDOff(LED4);

  //���������������� �������� (��. ���� tim_pwm.c)
  TimerConfig();
  
  //���� � �������� ���������
  while (1)
  {
    //�������� ����������� �� ������ WAKEUP, ��� ������� - ����� �� �����
    if (STM_PBGetState(BUTTON_WAKEUP)) break;
  }

  //����� ���������� - ���������� ���������� ������ ���������, ������ ���������� ����������
  NVIC_SystemReset();
}

//---------------------------------------------------------------------------
// ���������� ���������� �� ������� 4
void TIM4_IRQHandler(void)
{
  //�������� �������������� ����� (��������� ����������)
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //������� �����
    STM_LEDToggle(LED3);                         //���������� ����������� LED3
  }
}
//---------------------------------------------------------------------------
