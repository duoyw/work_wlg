#include "common.h"
#include "include.h"

StatusFlag_St StatusFlag_t={0};
void test1();
void test2();
void  main(void)
{
	InitServo();
    InitPwmMoto();//�����ʼ��    ռ�ձ������ȷ     //FTM0_CH5  //PTD5
	InitCtrDir();//��ʼ������˿�
    led_init(LED0);//���Գɹ�                                //PTE2,PTB21,PTC3,PTB23
    InitPit();//��ʱ����ʼ�� ���Գɹ�5ms                   
    InitFreeCar();//�������Գ�ʼ��              //����4  PTC14��15
    InitNvic();//�ж����ȼ�����
	 
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
