#include "common.h"
#include "include.h"
#include "VCAN_UART.h"             //串口


void InitUart5(uint32 baud)
{
  uart_init(UART5, baud);
  set_vector_handler(UART5_RX_TX_VECTORn, uart5_handler);
  uart_rx_irq_en(UART5);    
  uart_tx_irq_dis(UART5);//开串口发送中断
 
}


uint8 Tx5Buffer[256];
uint8 Tx5Counter=0;
uint8 count5=0; 
/*!
*  @brief      UART串口发送中断服务函数
*  @since      v5.0
*  @warning    
*  Sample usage:       set_vector_handler(_RX_TX_VECTORn , uart4_handler);    //把 uart3_handler 函数添加到中断向量表，不需要我们手动调用
*/
void uart5_handler(void)
{ 
    uint8_t a;
    if(UART_S1_REG(UARTN[UART5]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        uart_getchar(UART5, &a);
    
    }
    if(UART_S1_REG(UARTN[UART5]) & UART_S1_TDRE_MASK )  //发送数据寄存器空
    {
      //用户需要处理发送数据
      uart_putchar(UART5, Tx5Buffer[Tx5Counter++]);//写数据    
      if(Tx5Counter == count5)
      {
          uart_tx_irq_dis(UART5);                   //关串口发送中断
      }
    }
}

void Uart5_Send(uint8 *DataToSend ,uint8 data_num)
{

  uint8 i;
	for(i=0;i<data_num;i++)
	{
		Tx5Buffer[count5++] = *(DataToSend+i);
	}

	 if(!(UART_C2_REG(UARTN[UART5]) & UART_C2_TIE_MASK) )
	{
		uart_tx_irq_en(UART5); //′打开发送中断
	}
}
