#ifndef __VCAN_UART_H__
#define __VCAN_UART_H__
typedef struct
{
    uint16 St;      // 0xAAAA
    
    int16 left;      // 
    int16 right;      // 
    int16 turnNum;      // 
    int16 haveLight;      // 
    int16 temp3;      // 
    int16 temp4;      // 
    
    uint16 Sp;      // 0x5555
}Msg_CtrlToFMU_Pkg_t;
void InitUart5(uint32 baud);
void uart5_handler(void);
void Uart5_Send(uint8 *DataToSend ,uint8 data_num);
void ReceiveMsgFromSecond(uint8 data);
#endif