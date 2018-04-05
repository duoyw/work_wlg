#include "common.h"
#include "include.h"
#include "VCAN_PwmMoto.h"       //电机

#define PWMFRE 1000
 
uint8_t Motor_flag =1;//电机启动标志
uint32_t currentLeftPwmFreq=PWMFRE;//左上，右下电机频率
uint32_t currentRightPwmFreq=PWMFRE;
uint32_t setLeftPwmFreq=0;
uint32_t setRightPwmFreq=0;

//电机初始化
void InitPwmMoto(void)
{
  ftm_pwm_init(FTM0,FTM_CH5,PWMFRE,0);//初始化 FTM0_CH5（PD5） 为 频率 PWMFRE  的PWM，占空比为 0,左前
  ftm_pwm_init(FTM1,FTM_CH0,PWMFRE,0);//初始化 FTM0_CH2（PA5） 为 频率 PWMFRE 的PWM，占空比为 0	，右前
  ftm_pwm_init(FTM1,FTM_CH1,PWMFRE,0);//初始化 FTM0_CH3（PA6） 为 频率 PWMFRE 的PWM，占空比为 0 ，左后
  ftm_pwm_init(FTM0,FTM_CH4,PWMFRE,0);//初始化 FTM0_CH4（PA7） 为 频率 PWMFRE 的PWM，占空比为 0，右后
  
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
*初始化方向控制端口
*/
void InitCtrDir(void)
{
	gpio_init (LH_CW, GPO,0);    //初始化管脚左前		
	gpio_init (RH_CW, GPO,0); 	//右前	
	gpio_init (LB_CW, GPO,0); 	//左后
	gpio_init (RB_CW, GPO,0); 	//右后

}

/*
*设置车轮占空比
*Wheel_E:车轮
*duty:占空比，百分制
*/
void SetPwm_Duty(Wheel_E wheel_e,uint16_t duty)
{
	switch(wheel_e)
	{
		case LH:{
			ftm_pwm_duty(FTM0, FTM_CH5, (uint16_t)((duty/100.0)*FTM0_PRECISON));    //设置 FTM0_CH6占空比为 10/FTM0_PRECISON
			
		}break;
		case RH:{
			ftm_pwm_duty(FTM1, FTM_CH0, (uint16_t)((duty/100.0)*FTM1_PRECISON));    //设置 FTM0_CH6占空比为 10/FTM0_PRECISON

		}break;
		case LB:{
			ftm_pwm_duty(FTM1, FTM_CH1, (uint16_t)((duty/100.0)*FTM1_PRECISON));    //设置 FTM0_CH6占空比为 10/FTM0_PRECISON
			
		}break;
		case RB:{
			ftm_pwm_duty(FTM0, FTM_CH4, (uint16_t)((duty/100.0)*FTM0_PRECISON));    //设置 FTM0_CH6占空比为 10/FTM0_PRECISON
		}break;
	}
}

/*
*设置左斜线PWM频率，改变频率后需要重新设定占空比
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
*定时器20ms匀速改变频率
*/
#define ChangePwmValue	(100)
void TimeChangeFreq()
{
  	/*************左斜***********/
	if(currentLeftPwmFreq>setLeftPwmFreq)//逐渐靠近设定频率
	  currentLeftPwmFreq-=ChangePwmValue;
	else if(currentLeftPwmFreq<setLeftPwmFreq)
	  currentLeftPwmFreq+=ChangePwmValue;
	
	/*************右斜***********/
	if(currentRightPwmFreq>setRightPwmFreq)//逐渐靠近设定频率
	  currentRightPwmFreq-=ChangePwmValue;
	else if(currentRightPwmFreq<setRightPwmFreq)
	  currentRightPwmFreq+=ChangePwmValue;
		
	ftm_pwm_freq(FTM0,currentLeftPwmFreq);//频率设置
	ftm_pwm_freq(FTM1,currentRightPwmFreq);

	if(MoveBaseDir_t.LH_Stop==0)//启动车轮
	   SetPwm_Duty(LH,50);
	if(MoveBaseDir_t.RH_Stop==0)
	  SetPwm_Duty(RH,50);
	if(MoveBaseDir_t.LB_Stop==0)
	  SetPwm_Duty(LB,50);
	if(MoveBaseDir_t.RB_Stop==0)
	  SetPwm_Duty(RB,50);
	
	if(MoveBaseDir_t.LH_Stop==1&&currentLeftPwmFreq==1000)//停止
	 	 SetPwm_Duty(LH,0);
	if(MoveBaseDir_t.RH_Stop==1&&currentRightPwmFreq==1000)
		SetPwm_Duty(RH,0);
	if(MoveBaseDir_t.LB_Stop==1&&currentRightPwmFreq==1000)
	  	SetPwm_Duty(LB,0);
	if(MoveBaseDir_t.RB_Stop==1&&currentLeftPwmFreq==1000)
	  	SetPwm_Duty(RB,0);
	
	
}

