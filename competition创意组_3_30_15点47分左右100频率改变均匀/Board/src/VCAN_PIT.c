#include "common.h"
#include "include.h"

void InitPit()
{
  pit_init_ms(PIT0,PIT_TIME);//PIT0��ʱ�ж�1ms
  set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //����PIT0���жϸ�λ����Ϊ PIT0_IRQHandler
  enable_irq(PIT0_IRQn);                         // ʹ��PIT�ж�
}

