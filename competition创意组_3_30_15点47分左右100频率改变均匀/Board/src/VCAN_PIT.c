#include "common.h"
#include "include.h"

void InitPit()
{
  pit_init_ms(PIT0,PIT_TIME);//PIT0定时中断1ms
  set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断复位函数为 PIT0_IRQHandler
  enable_irq(PIT0_IRQn);                         // 使能PIT中断
}

