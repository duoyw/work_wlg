#include "common.h"
#include "include.h"

/**
*�ж����ȼ���ʼ��
*/
void InitNvic()
{
   /*******************************�ж����ȼ�����*******************************************/
   DisableInterrupts;    
  NVIC_SetPriorityGrouping(2);   
  //    NVIC_SetPriority(PIT0_IRQn, 2);             //PID�ж�
  //   NVIC_SetPriority(PIT1_IRQn, 3);             //�������ϵͳʱ��
//  NVIC_SetPriority(UART5_RX_TX_IRQn, 3);      //��˼���������ж�  SysTick_IRQn
  EnableInterrupts;
}