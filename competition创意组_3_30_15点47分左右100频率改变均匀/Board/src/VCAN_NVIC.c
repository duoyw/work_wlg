#include "common.h"
#include "include.h"

/**
*中断优先级初始化
*/
void InitNvic()
{
   /*******************************中断优先级配置*******************************************/
   DisableInterrupts;    
  NVIC_SetPriorityGrouping(2);   
  //    NVIC_SetPriority(PIT0_IRQn, 2);             //PID中断
  //   NVIC_SetPriority(PIT1_IRQn, 3);             //任务管理系统时基
//  NVIC_SetPriority(UART5_RX_TX_IRQn, 3);      //飞思卡儿接收中断  SysTick_IRQn
  EnableInterrupts;
}