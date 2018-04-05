#include "common.h"
#include "include.h"

StatusFlag_St StatusFlag_t={0};
void test1();
void test2();
void  main(void)
{
	InitServo();
    InitPwmMoto();//电机初始化    占空比输出正确     //FTM0_CH5  //PTD5
	InitCtrDir();//初始化方向端口
    led_init(LED0);//测试成功                                //PTE2,PTB21,PTC3,PTB23
    InitPit();//定时器初始化 测试成功5ms                   
    InitFreeCar();//蓝牙调试初始化              //串口4  PTC14，15
    InitNvic();//中断优先级配置
	 
    while(1)
   {    
	// test2();
   }
	 	
}



void test1()
{
    static uint16_t freq1=3000;
	static uint16_t freq2=10000;
	static uint8_t staLB=0,staLH=0,staRH=0,staRB=0,sta=0;
	if(sta)
	{
		StopAll();
		sta=0;
	}
	if(MoveBaseDir_t.LH_MoveOk==1&&staLH==1)
	{
	  MoveWheel(LH,Wheel_Head,3000000,freq1);
	  staLH=0;
	}
	if(MoveBaseDir_t.RH_MoveOk==1&&staRH==1)
	{
	  MoveWheel(RH,Wheel_Head,3000000,freq2);
	  staRH=0;
	}
	if(MoveBaseDir_t.LB_MoveOk==1&&staLB==1)
	{
	  MoveWheel(LB,Wheel_Head,3000000,freq2);
	  staLB=0;
	}
	if(MoveBaseDir_t.RB_MoveOk==1&&staRB==1)
	{
	  MoveWheel(RB,Wheel_Head,3000000,freq1);
	  staRB=0;
	}
  
	SetLeftPwm_Frequence(freq1);
	SetRightPwm_Frequence(freq2);
}


void test2()
{

	SetServoDuty(MIN_ANGLE);
	while(isServoMoveOk!=1);
	SetServoDuty(MID_ANGLE+3000);
	while(isServoMoveOk!=1);

	//SetServoDuty(MID_ANGLE+3000);
	//DELAY_MS(1000);
	
}
