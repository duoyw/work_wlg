#ifndef __VCAN_FREECAR_H__
#define __VCAN_FREECAR_H__
#include "include.h"
#include "common.h"


#define FreeCarsUARTPort UART4
#define FreeCarsIRQ      IRQ_UART4
#define FreeCarsBR       115200       //������

//FreeCars��λ�� ����������ʾ�� ����ͨ������������λ�����øı�
#define UartDataNum      17	    

//���²�Ҫ�޸�
#define UartRxBufferLen  100
#define UartRxDataLen    41           //FreeCars��λ�����͸�������MCU���գ���Ҫ��
#define UartRxCmdLen     7	      //FreeCars��λ�������������ݳ��ȣ���Ҫ��


#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack-3]//�����
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack-2]//��������

typedef struct
{
	int Stack;
	uint8 Data;
	uint8 PreData;
	uint8 Buffer[UartRxBufferLen];
	uint8 Enable;
	uint8 Check;
}SerialPortType;


/************�����Ǹ���FreeCars2.0Э�������ģ��û�����ı�**************/
extern uint8 uSendBuf[UartDataNum * 2];
extern uint8 FreeCarsDataNum;
extern SerialPortType SerialPortRx;
extern double UartData[9];

extern void push(uint8 chanel, uint16 data);
extern void UartDebug(void);
extern void UartCmd(uint8 CmdNum, uint8 Data);
extern void  sendDataToScope(void);
void uart4_handler(void);
void Uart_Send(uint8 *DataToSend ,uint8 data_num);
void InitFreeCar(void);
void FreeCarRecDeal(uint8_t a);//���ܴ���

#endif