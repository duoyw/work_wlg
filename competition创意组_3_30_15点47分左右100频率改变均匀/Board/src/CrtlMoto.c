#include "CrtlMoto.h"
#include "VCAN_PwmMoto.h"       //电机
#include "common.h"
#include "include.h"

DirBase_E DirBase_e=0;//方向枚举
MoveBaseDir_ST MoveBaseDir_t={0};//向不同方向移动的距离

/*
*控制车轮移动
*wheel_e:车轮选择
*DirBase_E：方向
*step:距离
*freq:频率
*/
void MoveWheel(Wheel_E wheel_e,DirBase_E dir_e,uint32_t step,uint32_t freq)
{
    StatusFlag_t.StopFlag=0;
	switch(wheel_e)
	{
		case LH:{
			SetDutyAndDir(wheel_e,dir_e);//占空比及方向
			MoveBaseDir_t.LH_Step+=step;//步数
			MoveBaseDir_t.LH_Stop=0;//步数更新标志
			MoveBaseDir_t.LH_MoveOk=0;//移动完成标志
			SetLeftPwm_Frequence(freq);//设置速度		
		}break;
		case RH:{
			SetDutyAndDir(wheel_e,dir_e);
			MoveBaseDir_t.RH_Step+=step;//步数
			MoveBaseDir_t.RH_Stop=0;//步数更新标志
			MoveBaseDir_t.RH_MoveOk=0;//移动完成标志
			SetRightPwm_Frequence(freq);
		}break;
		
		case LB:{
			SetDutyAndDir(wheel_e,dir_e);
			MoveBaseDir_t.LB_Step+=step;//步数
			MoveBaseDir_t.LB_Stop=0;//步数更新标志
			MoveBaseDir_t.LB_MoveOk=0;//移动完成标志
			SetRightPwm_Frequence(freq);
		}break;
		case RB:{
			SetDutyAndDir(wheel_e,dir_e);
			MoveBaseDir_t.RB_Step+=step;//步数
			MoveBaseDir_t.RB_Stop=0;//步数更新标志
			MoveBaseDir_t.RB_MoveOk=0;//移动完成标志
			SetLeftPwm_Frequence(freq);//设置速度		
		}break;
	}
	
	
}

/*
*定时器1ms轮询控制电机开启关闭
*/
void TimeCtrMoto()
{
	static double LH_time=0;
	static double RH_time=0;
	static double LB_time=0;
	static double RB_time=0;
	static uint32_t nowLeftFreq=0;
	static uint32_t nowRightFreq=0;
	nowLeftFreq=currentLeftPwmFreq;//防止频率在这个函数中被改变
	nowRightFreq=currentRightPwmFreq;
	/*************左前方***********/
	if(MoveBaseDir_t.LH_MoveOk==0)//移动未完成
	{
		LH_time+=nowLeftFreq/1000.0;				
		if(LH_time>=MoveBaseDir_t.LH_Step)
		{
			StopMove(LH);//停止移动
			MoveBaseDir_t.LH_MoveOk=1;
			MoveBaseDir_t.LH_Step=0;
			LH_time=0;
		}
	}

	/*************右前方***********/
	if(MoveBaseDir_t.RH_MoveOk==0)//移动未完成
	{
		RH_time+=nowRightFreq/1000.0;	
		if(RH_time>=MoveBaseDir_t.RH_Step)
		{
			StopMove(RH);//停止移动
			MoveBaseDir_t.RH_MoveOk=1;
			MoveBaseDir_t.RH_Step=0;
			RH_time=0;
		}
	}
	
	
	/*************左后方***********/
	if(MoveBaseDir_t.LB_MoveOk==0)//移动未完成
	{
		LB_time+=nowRightFreq/1000.0;	
		if(LB_time>=MoveBaseDir_t.LB_Step)
		{
			StopMove(LB);//停止移动
			MoveBaseDir_t.LB_MoveOk=1;
			MoveBaseDir_t.LB_Step=0;
			LB_time=0;
		}
	}
	
	/*************右后方***********/
	if(MoveBaseDir_t.RB_MoveOk==0)//移动未完成
	{
		RB_time+=nowLeftFreq/1000.0;	
		if(RB_time>=MoveBaseDir_t.RB_Step)
		{
			StopMove(RB);//停止移动
			MoveBaseDir_t.RB_MoveOk=1;
			MoveBaseDir_t.RB_Step=0;
			RB_time=0;
		}
	}
	
	
}


/*
*控制PWM输出,以及方向
*/

void SetDutyAndDir(Wheel_E wheel_e,DirBase_E dir_e)
{
 	SetPwm_Duty(wheel_e,50);
	switch(wheel_e)
	{
		case LH:{
			if(dir_e==Wheel_Head)
			  gpio_set (LH_CW, 0);
			else
			  gpio_set (LH_CW, 1);
			
		}break;
		case RH:{
			if(dir_e==Wheel_Head)
			  gpio_set (RH_CW, 1);
			else
			  gpio_set (RH_CW, 0);

		}break;
		case LB:{
			if(dir_e==Wheel_Head)
			  gpio_set (LB_CW, 0);
			else
			  gpio_set (LB_CW, 1);
			
		}break;
		case RB:{
			if(dir_e==Wheel_Head)
			  gpio_set (RB_CW, 1);
			else
			  gpio_set (RB_CW, 0);
		}break;
		
	}
}
/*
*停止某个方向的移动
*/
void StopMove(Wheel_E wheel_e)
{
	switch(wheel_e)
	{
		case LH:{
			MoveBaseDir_t.LH_Stop=1;
			MoveBaseDir_t.LH_Step=0;
			setLeftPwmFreq=1000;
		}break;
		
		case RH:{
			MoveBaseDir_t.RH_Stop=1;
			MoveBaseDir_t.RH_Step=0;
			setRightPwmFreq=1000;
		}break;
		
		case LB:{
			MoveBaseDir_t.LB_Stop=1;
			MoveBaseDir_t.LB_Step=0;
			setRightPwmFreq=1000;
		}break;
		
		case RB:{
			MoveBaseDir_t.RB_Stop=1;
			MoveBaseDir_t.RB_Step=0;
			setLeftPwmFreq=1000;
		}break;
	}
}
/*
*停止四个方向的移动
*/
void StopAll(void)
{
	StopMove(LH);
	StopMove(RH);
	StopMove(LB);
	StopMove(RB);
	
}