#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO�ڲ���
#include  "MK60_uart.h"     //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //�͹��Ķ�ʱ��(��ʱ)
#include  "MK60_i2c.h"      //I2C
#include  "MK60_spi.h"      //SPI
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_rtc.h"      //RTC
#include  "MK60_adc.h"      //ADC
#include  "MK60_dac.h"      //DAC
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_it.h"    //FLASH

#include  "VCAN_LED.H"          //LED
#include  "VCAN_KEY.H"          //KEY
#include "VCAN_PwmMoto.h"       //���
#include "VCAN_Encoder.h"       //������
#include "VCAN_Servo.h"         //���
#include "VCAN_PIT.h"           //��ʱ��
#include "VCAN_ImageDeal.h"     //ͼ����    
#include  "VCAN_computer.h"     //�๦�ܵ�������
#include "VCAN_FreeCar.h"        //FreeCar��λ��
#include "VCAN_MyMath.h"          //��ѧ��
#include "VCAN_SCCB.h"              //SCCBģ��
#include "VCAN_UART.h"             //����
#include "VCAN_Track.h"             // 
#include "VCAN_OV7725.h"
#include  "VCAN_NVIC.h"
#include "ctrl_pid.h"
#include "ctrl_basic.h"
#include "ctrl_control.h"
#include  "increPid.h"
#include  "duty.h"
#include "CrtlMoto.h"
#include "CrtlCar.h"

typedef struct
{
	uint8_t StopFlag;
	
}StatusFlag_St;

void ImaegDecideDeal();


extern StatusFlag_St StatusFlag_t;

#endif  //__INCLUDE_H__
