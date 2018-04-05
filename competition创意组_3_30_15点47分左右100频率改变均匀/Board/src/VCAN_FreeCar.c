#include "common.h"
#include "include.h"
#include "ctrl_control.h"
#include "VCAN_Track.h"


/************以下是根据FreeCars2.0协议修正的，用户无需改变**************/


uint8 uSendBuf[UartDataNum * 2] = { 0 };
uint8 FreeCarsDataNum = UartDataNum * 2;
double UartData[9] = { 0 };
SerialPortType SerialPortRx;

extern uint8 flag_num;
void InitFreeCar()
{
  uart_init(UART4, 115200);
  set_vector_handler(UART4_RX_TX_VECTORn, uart4_handler);
  uart_rx_irq_en(UART4);    
  uart_tx_irq_dis(UART4);//开串口发送中断
}

void FreeCarRecDeal(uint8_t a)
{
    int i, b, d1;
    uint32 d;
    
    SerialPortRx.Data = a;
    if (SerialPortRx.Stack < UartRxBufferLen)
    {
      SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;
      
      if (SerialPortRx.Stack >= UartRxDataLen  //UartRxDataLen个数为一帧
          && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen] == 0xff //校验字头
            && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 1] == 0x55
              && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 2] == 0xaa
                && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 3] == 0x10)
      {   //double data 9个通道数据校验
        SerialPortRx.Check = 0;
        b = SerialPortRx.Stack - UartRxDataLen; //起始位
        for (i = b; i<SerialPortRx.Stack - 1; i++)  //除校验位外的位进行校验
        {
          SerialPortRx.Check += SerialPortRx.Buffer[i];//校验
        }
        
        if (SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
        {	//校验成功，进行数据解算
          for (i = 0; i<9; i++)
          {
            d = SerialPortRx.Buffer[b + i * 4 + 4] * 0x1000000L + SerialPortRx.Buffer[b + i * 4 + 5] * 0x10000L + SerialPortRx.Buffer[b + i * 4 + 6] * 0x100L + SerialPortRx.Buffer[b + i * 4 + 7];
            if (d>0x7FFFFFFF)
            {
              d1 = 0x7FFFFFFF - d;
            }
            else
            {
              d1 = d;
            }
            UartData[i] = d1;
            UartData[i] /= 65536.0;
          }
          UartDebug();	//转去处理，把受到的数据付给变量
        }
        
        SerialPortRx.Stack = 0;
      }
      else if (SerialPortRx.Stack >= UartRxCmdLen //UartRxDataLen个数为一帧
               && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen] == 0xff
                 && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 1] == 0x55
                   && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 2] == 0xaa
                     && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 3] == 0x77)//cmd
      {
        SerialPortRx.Check = 0;
        b = SerialPortRx.Stack - UartRxCmdLen; //起始位
        for (i = b; i<SerialPortRx.Stack - 1; i++)  //除校验位外的位进行校验
        {
          SerialPortRx.Check += SerialPortRx.Buffer[i];//校验
        }
        if (SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
        {	//校验成功
          UartCmd(UartCmdNum, UartCmdData);//处理接收到的命令，付给MCU命令变量
        }
        SerialPortRx.Stack = 0;
      }
    }
    else
    {
      SerialPortRx.Stack = 0;
    }


}


/*
向某个通道缓冲区填充数据
chanel：通道
data  ：数据-32768~32767
*/

void push(uint8 chanel, uint16 data)
{
  uSendBuf[chanel * 2] = data / 256;
  uSendBuf[chanel * 2 + 1] = data % 256;
}
/*
轮询法发送一帧数据
消耗时间与数据长度有关
不可以放在中断里面周期性调用
消耗时间计算看帮助文档
*/
void sendDataToScope(void)
{
  uint8 i, sum = 0;
  uint8 Buff[UartDataNum * 2 + 5]; 
  uint8 *p = Buff;  
    
  push(0,(uint16)carSpeed);
//  push(1,(uint16)SysPID[Servo].Out_Int16);          
//  push(2,(uint16)SysPID[Servo].out_p);
//  push(3,(uint16)SysPID[Servo].out_d); 
//  push(4,(uint16)Light_t.Mid.x);  
//  push(5,(uint16)Light_t.Mid.y); 
//  push(6,(uint16)SysPID[Servo].set);
//  push(7,(uint16)SysPID[Servo].sample); 
//  push(8,(uint16)SysPID[LeftMotor_Spd].set);           
//  push(9,(uint16)SysPID[RightMotor_Spd].set);
//  push(10,MotoLeftSpeed); 
//  push(11,MotoRightSpeed); 
//  push(12,(uint16)(SysPID[Servo].err)); 
//  push(13,(uint16)flag_num);
//  push(14,(uint16)(SysPID[Servo].err_d));
  
  *p++ = 251; 
  *p++ = 109;   
  *p++ = 37;
  sum += (251);      //全部数据加入校验
  sum += (109);
  sum += (37);
  
  for (i = 0; i<FreeCarsDataNum; i++)
  {

    *p++ = uSendBuf[i];
    sum += uSendBuf[i];    
  }
  *p++ = sum;  
  Uart_Send(Buff,38); 
}

void Page0_debug()
{
   	 carSpeed =100* UartData[0];
//    SysPID[LeftMotor_Spd].i = UartData[1];
//    
//    SysPID[RightMotor_Spd].p =SysPID[LeftMotor_Spd].p ;
//    SysPID[RightMotor_Spd].i =SysPID[LeftMotor_Spd].i ;
//    
//    SysPID[Servo].p = UartData[2];   
//    SysPID[Servo].d = UartData[3];    
////    = UartData[3];    
////         = UartData[4];
//  //  ServoValue    = UartData[5];
}

void Page1_debug()
{    //修改期望值
//    SysPID[LeftMotor_Spd].set = UartData[6];
//    SysPID[RightMotor_Spd].set     = UartData[7];  
}
extern int16_t dRule[7];
extern float goAroundSpeed;
void Page2_debug()
{
//    dRule[0] = (int16_t)UartData[0];
//    dRule[1] = (int16_t)UartData[1];
//    dRule[2] = (int16_t)UartData[2];
//    dRule[3] = (int16_t)UartData[3];
//    dRule[4] = (int16_t)UartData[4];
//    dRule[5] = (int16_t)UartData[5];
//    dRule[6] = (int16_t)UartData[6];
//    goAroundSpeed = (int16_t)UartData[7];
    
}
extern int16_t speed_DF[7];
void Page3_debug()
{
//    speed_DF[0] = (int16_t)UartData[0];
//    speed_DF[1] = (int16_t)UartData[1];
//    speed_DF[2] = (int16_t)UartData[2];
//    speed_DF[3] = (int16_t)UartData[3];
//    speed_DF[4] = (int16_t)UartData[4];
//    speed_DF[5] = (int16_t)UartData[5];
//    speed_DF[6] = (int16_t)UartData[6];
}

void UartDebug(void)
{
   switch((uint16_t)UartData[8])
   {
      case 0:Page0_debug();break;
      case 1:Page1_debug();break;
      case 2:Page2_debug();break;
      case 3:Page3_debug();break;
      default: break;
   }
    
}
/*
命令接收函数
CmdNum：0~255号命令
DATA  ：0~255个命令所对应的命令字
*/
void UartCmd(uint8 CmdNum, uint8 Data)
{
   if (CmdNum == 1 && Data == 10)                            //F10一键停车
  {
		StopAll();
  }
  if (CmdNum == 1 && Data == 11)                          //F11发送图像
  {
     
  }
  if (CmdNum == 1 && Data == 6)                             //F6的作用
  {
	CarMove(car_H,5000000,carSpeed);
  }
  if (CmdNum == 1 && Data == 7)                            //F7的作用
  {
	CarMove(car_LeftTurn,5000000,carSpeed);
  }
  if (CmdNum == 1 && Data == 8)                            //F8的作用
  {
  	CarMove(car_L,5000000,carSpeed);
  }
  if (CmdNum == 1 && Data == 9)                           //F9的作用
  {
   	CarMove(car_R,5000000,carSpeed);
  }
  if (CmdNum == 1 && Data == 12)                          //F12的作用
  {
   CarMove(car_LH,5000000,carSpeed);
  }
 
}


uint8 Tx1Buffer[256];
uint8 Tx1Counter=0;
uint8 count1=0; 
UARTn_e uratn = UART4;
/*!
*  @brief      UART串口发送中断服务函数
*  @since      v5.0
*  @warning    
*  Sample usage:       set_vector_handler(UART4_RX_TX_VECTORn , uart4_handler);    //把 uart3_handler 函数添加到中断向量表，不需要我们手动调用
*/
void uart4_handler(void)
{ 
    uint8_t a;
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        uart_getchar(UART4, &a);
        FreeCarRecDeal(a);
    }
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //发送数据寄存器空
    {
      //用户需要处理发送数据
      uart_putchar(uratn, Tx1Buffer[Tx1Counter++]);//写数据    
      if(Tx1Counter == count1)
      {
          uart_tx_irq_dis(uratn);                   //关串口发送中断
      }
    }
}

void Uart_Send(uint8 *DataToSend ,uint8 data_num)
{

  uint8 i;
	for(i=0;i<data_num;i++)
	{
		Tx1Buffer[count1++] = *(DataToSend+i);
	}

	 if(!(UART_C2_REG(UARTN[uratn]) & UART_C2_TIE_MASK) )
	{
		uart_tx_irq_en(uratn); //′打开发送中断
	}
}