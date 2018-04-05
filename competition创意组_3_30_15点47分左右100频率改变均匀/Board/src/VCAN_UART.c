#include "common.h"
#include "include.h"
#include "VCAN_UART.h"             //����


void InitUart5(uint32 baud)
{
  uart_init(UART5, baud);
  set_vector_handler(UART5_RX_TX_VECTORn, uart5_handler);
  uart_rx_irq_en(UART5);    
  uart_tx_irq_dis(UART5);//�����ڷ����ж�
 
}


uint8 Tx5Buffer[256];
uint8 Tx5Counter=0;
uint8 count5=0; 
/*!
*  @brief      UART���ڷ����жϷ�����
*  @since      v5.0
*  @warning    
*  Sample usage:       set_vector_handler(_RX_TX_VECTORn , uart4_handler);    //�� uart3_handler ������ӵ��ж�����������Ҫ�����ֶ�����
*/
void uart5_handler(void)
{ 
    uint8_t a;
    if(UART_S1_REG(UARTN[UART5]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
    {
        uart_getchar(UART5, &a);
    
    }
    if(UART_S1_REG(UARTN[UART5]) & UART_S1_TDRE_MASK )  //�������ݼĴ�����
    {
      //�û���Ҫ����������
      uart_putchar(UART5, Tx5Buffer[Tx5Counter++]);//д����    
      if(Tx5Counter == count5)
      {
          uart_tx_irq_dis(UART5);                   //�ش��ڷ����ж�
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
		uart_tx_irq_en(UART5); //��򿪷����ж�
	}
}
