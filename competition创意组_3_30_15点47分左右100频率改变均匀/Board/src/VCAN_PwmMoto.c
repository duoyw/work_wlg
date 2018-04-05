#include "common.h"
#include "include.h"
#include "VCAN_PwmMoto.h"       //���

#define PWMFRE 1000
 
uint8_t Motor_flag =1;//���������־
uint32_t currentLeftPwmFreq=PWMFRE;//���ϣ����µ��Ƶ��
uint32_t currentRightPwmFreq=PWMFRE;
uint32_t setLeftPwmFreq=0;
uint32_t setRightPwmFreq=0;

//�����ʼ��
void InitPwmMoto(void)
{
  ftm_pwm_init(FTM0,FTM_CH5,PWMFRE,0);//��ʼ�� FTM0_CH5��PD5�� Ϊ Ƶ�� PWMFRE  ��PWM��ռ�ձ�Ϊ 0,��ǰ
  ftm_pwm_init(FTM1,FTM_CH0,PWMFRE,0);//��ʼ�� FTM0_CH2��PA5�� Ϊ Ƶ�� PWMFRE ��PWM��ռ�ձ�Ϊ 0	����ǰ
  ftm_pwm_init(FTM1,FTM_CH1,PWMFRE,0);//��ʼ�� FTM0_CH3��PA6�� Ϊ Ƶ�� PWMFRE ��PWM��ռ�ձ�Ϊ 0 �����
  ftm_pwm_init(FTM0,FTM_CH4,PWMFRE,0);//��ʼ�� FTM0_CH4��PA7�� Ϊ Ƶ�� PWMFRE ��PWM��ռ�ձ�Ϊ 0���Һ�
  
  MoveBaseDir_t.LH_MoveOk=1;
  MoveBaseDir_t.LB_MoveOk=1;
  MoveBaseDir_t.RB_MoveOk=1;
  MoveBaseDir_t.RH_MoveOk=1;
  
  MoveBaseDir_t.LH_Stop=1;
  MoveBaseDir_t.LB_Stop=1;
  MoveBaseDir_t.RB_Stop=1;
  MoveBaseDir_t.RH_Stop=1;
}

/*
*��ʼ��������ƶ˿�
*/
void InitCtrDir(void)
{
	gpio_init (LH_CW, GPO,0);    //��ʼ���ܽ���ǰ		
	gpio_init (RH_CW, GPO,0); 	//��ǰ	
	gpio_init (LB_CW, GPO,0); 	//���
	gpio_init (RB_CW, GPO,0); 	//�Һ�

}

/*
*���ó���ռ�ձ�
*Wheel_E:����
*duty:ռ�ձȣ��ٷ���
*/
void SetPwm_Duty(Wheel_E wheel_e,uint16_t duty)
{
	switch(wheel_e)
	{
		case LH:{
			ftm_pwm_duty(FTM0, FTM_CH5, (uint16_t)((duty/100.0)*FTM0_PRECISON));    //���� FTM0_CH6ռ�ձ�Ϊ 10/FTM0_PRECISON
			
		}break;
		case RH:{
			ftm_pwm_duty(FTM1, FTM_CH0, (uint16_t)((duty/100.0)*FTM1_PRECISON));    //���� FTM0_CH6ռ�ձ�Ϊ 10/FTM0_PRECISON

		}break;
		case LB:{
			ftm_pwm_duty(FTM1, FTM_CH1, (uint16_t)((duty/100.0)*FTM1_PRECISON));    //���� FTM0_CH6ռ�ձ�Ϊ 10/FTM0_PRECISON
			
		}break;
		case RB:{
			ftm_pwm_duty(FTM0, FTM_CH4, (uint16_t)((duty/100.0)*FTM0_PRECISON));    //���� FTM0_CH6ռ�ձ�Ϊ 10/FTM0_PRECISON
		}break;
	}
}

/*
*������б��PWMƵ�ʣ��ı�Ƶ�ʺ���Ҫ�����趨ռ�ձ�
*/
void SetLeftPwm_Frequence(uint32_t leftFre )
{	
	setLeftPwmFreq=leftFre;	
}

void SetRightPwm_Frequence( uint32_t rightFre )
{	
	setRightPwmFreq=rightFre;	
}


/*
*��ʱ��20ms���ٸı�Ƶ��
*/
#define ChangePwmValue	(100)
void TimeChangeFreq()
{
  	/*************��б***********/
	if(currentLeftPwmFreq>setLeftPwmFreq)//�𽥿����趨Ƶ��
	  currentLeftPwmFreq-=ChangePwmValue;
	else if(currentLeftPwmFreq<setLeftPwmFreq)
	  currentLeftPwmFreq+=ChangePwmValue;
	
	/*************��б***********/
	if(currentRightPwmFreq>setRightPwmFreq)//�𽥿����趨Ƶ��
	  currentRightPwmFreq-=ChangePwmValue;
	else if(currentRightPwmFreq<setRightPwmFreq)
	  currentRightPwmFreq+=ChangePwmValue;
		
	ftm_pwm_freq(FTM0,currentLeftPwmFreq);//Ƶ������
	ftm_pwm_freq(FTM1,currentRightPwmFreq);

	if(MoveBaseDir_t.LH_Stop==0)//��������
	   SetPwm_Duty(LH,50);
	if(MoveBaseDir_t.RH_Stop==0)
	  SetPwm_Duty(RH,50);
	if(MoveBaseDir_t.LB_Stop==0)
	  SetPwm_Duty(LB,50);
	if(MoveBaseDir_t.RB_Stop==0)
	  SetPwm_Duty(RB,50);
	
	if(MoveBaseDir_t.LH_Stop==1&&currentLeftPwmFreq==1000)//ֹͣ
	 	 SetPwm_Duty(LH,0);
	if(MoveBaseDir_t.RH_Stop==1&&currentRightPwmFreq==1000)
		SetPwm_Duty(RH,0);
	if(MoveBaseDir_t.LB_Stop==1&&currentRightPwmFreq==1000)
	  	SetPwm_Duty(LB,0);
	if(MoveBaseDir_t.RB_Stop==1&&currentLeftPwmFreq==1000)
	  	SetPwm_Duty(RB,0);
	
	
}

