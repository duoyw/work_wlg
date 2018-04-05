#include "common.h"
#include "include.h"
#include "ctrl_control.h"
#include "VCAN_Track.h"


/************�����Ǹ���FreeCars2.0Э�������ģ��û�����ı�**************/


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
  uart_tx_irq_dis(UART4);//�����ڷ����ж�
}

void FreeCarRecDeal(uint8_t a)
{
    int i, b, d1;
    uint32 d;
    
    SerialPortRx.Data = a;
    if (SerialPortRx.Stack < UartRxBufferLen)
    {
      SerialPortRx.Buffer[SerialPortRx.Stack++] = SerialPortRx.Data;
      
      if (SerialPortRx.Stack >= UartRxDataLen  //UartRxDataLen����Ϊһ֡
          && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen] == 0xff //У����ͷ
            && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 1] == 0x55
              && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 2] == 0xaa
                && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxDataLen + 3] == 0x10)
      {   //double data 9��ͨ������У��
        SerialPortRx.Check = 0;
        b = SerialPortRx.Stack - UartRxDataLen; //��ʼλ
        for (i = b; i<SerialPortRx.Stack - 1; i++)  //��У��λ���λ����У��
        {
          SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
        }
        
        if (SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
        {	//У��ɹ����������ݽ���
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
          UartDebug();	//תȥ�������ܵ������ݸ�������
        }
        
        SerialPortRx.Stack = 0;
      }
      else if (SerialPortRx.Stack >= UartRxCmdLen //UartRxDataLen����Ϊһ֡
               && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen] == 0xff
                 && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 1] == 0x55
                   && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 2] == 0xaa
                     && SerialPortRx.Buffer[SerialPortRx.Stack - UartRxCmdLen + 3] == 0x77)//cmd
      {
        SerialPortRx.Check = 0;
        b = SerialPortRx.Stack - UartRxCmdLen; //��ʼλ
        for (i = b; i<SerialPortRx.Stack - 1; i++)  //��У��λ���λ����У��
        {
          SerialPortRx.Check += SerialPortRx.Buffer[i];//У��
        }
        if (SerialPortRx.Check == SerialPortRx.Buffer[SerialPortRx.Stack - 1])
        {	//У��ɹ�
          UartCmd(UartCmdNum, UartCmdData);//������յ����������MCU�������
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
��ĳ��ͨ���������������
chanel��ͨ��
data  ������-32768~32767
*/

void push(uint8 chanel, uint16 data)
{
  uSendBuf[chanel * 2] = data / 256;
  uSendBuf[chanel * 2 + 1] = data % 256;
}
/*
��ѯ������һ֡����
����ʱ�������ݳ����й�
�����Է����ж����������Ե���
����ʱ����㿴�����ĵ�
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
  sum += (251);      //ȫ�����ݼ���У��
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
{    //�޸�����ֵ
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
������պ���
CmdNum��0~255������
DATA  ��0~255����������Ӧ��������
*/
void UartCmd(uint8 CmdNum, uint8 Data)
{
   if (CmdNum == 1 && Data == 10)                            //F10һ��ͣ��
  {
		StopAll();
  }
  if (CmdNum == 1 && Data == 11)                          //F11����ͼ��
  {
     
  }
  if (CmdNum == 1 && Data == 6)                             //F6������
  {
	CarMove(car_H,5000000,carSpeed);
  }
  if (CmdNum == 1 && Data == 7)                            //F7������
  {
	CarMove(car_LeftTurn,5000000,carSpeed);
  }
  if (CmdNum == 1 && Data == 8)                            //F8������
  {
  	CarMove(car_L,5000000,carSpeed);
  }
  if (CmdNum == 1 && Data == 9)                           //F9������
  {
   	CarMove(car_R,5000000,carSpeed);
  }
  if (CmdNum == 1 && Data == 12)                          //F12������
  {
   CarMove(car_LH,5000000,carSpeed);
  }
 
}


uint8 Tx1Buffer[256];
uint8 Tx1Counter=0;
uint8 count1=0; 
UARTn_e uratn = UART4;
/*!
*  @brief      UART���ڷ����жϷ�����
*  @since      v5.0
*  @warning    
*  Sample usage:       set_vector_handler(UART4_RX_TX_VECTORn , uart4_handler);    //�� uart3_handler ������ӵ��ж�����������Ҫ�����ֶ�����
*/
void uart4_handler(void)
{ 
    uint8_t a;
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
    {
        uart_getchar(UART4, &a);
        FreeCarRecDeal(a);
    }
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //�������ݼĴ�����
    {
      //�û���Ҫ����������
      uart_putchar(uratn, Tx1Buffer[Tx1Counter++]);//д����    
      if(Tx1Counter == count1)
      {
          uart_tx_irq_dis(uratn);                   //�ش��ڷ����ж�
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
		uart_tx_irq_en(uratn); //��򿪷����ж�
	}
}