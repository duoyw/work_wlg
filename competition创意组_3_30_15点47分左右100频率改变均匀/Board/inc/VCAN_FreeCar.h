#ifndef __VCAN_FREECAR_H__
#define __VCAN_FREECAR_H__
#include "include.h"
#include "common.h"


#define FreeCarsUARTPort UART4
#define FreeCarsIRQ      IRQ_UART4
#define FreeCarsBR       115200       //波特率

//FreeCars上位机 串口数据显示框 接收通道数，按照上位机设置改变
#define UartDataNum      17	    

//以下不要修改
#define UartRxBufferLen  100
#define UartRxDataLen    41           //FreeCars上位机发送浮点数据MCU接收，不要改
#define UartRxCmdLen     7	      //FreeCars上位机接收命令数据长度，不要改


#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack-3]//命令号
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack-2]//命令数据

typedef struct
{
	int Stack;
	uint8 Data;
	uint8 PreData;
	uint8 Buffer[UartRxBufferLen];
	uint8 Enable;
	uint8 Check;
}SerialPortType;


/************以下是根据FreeCars2.0协议修正的，用户无需改变**************/
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
void FreeCarRecDeal(uint8_t a);//接受处理

#endif