#include "CrtlMoto.h"
#include "VCAN_PwmMoto.h"       //���
#include "common.h"
#include "include.h"

DirBase_E DirBase_e=0;//����ö��
MoveBaseDir_ST MoveBaseDir_t={0};//��ͬ�����ƶ��ľ���

/*
*���Ƴ����ƶ�
*wheel_e:����ѡ��
*DirBase_E������
*step:����
*freq:Ƶ��
*/
void MoveWheel(Wheel_E wheel_e,DirBase_E dir_e,uint32_t step,uint32_t freq)
{
    StatusFlag_t.StopFlag=0;
	switch(wheel_e)
	{
		case LH:{
			SetDutyAndDir(wheel_e,dir_e);//ռ�ձȼ�����
			MoveBaseDir_t.LH_Step+=step;//����
			MoveBaseDir_t.LH_Stop=0;//�������±�־
			MoveBaseDir_t.LH_MoveOk=0;//�ƶ���ɱ�־
			SetLeftPwm_Frequence(freq);//�����ٶ�		
		}break;
		case RH:{
			SetDutyAndDir(wheel_e,dir_e);
			MoveBaseDir_t.RH_Step+=step;//����
			MoveBaseDir_t.RH_Stop=0;//�������±�־
			MoveBaseDir_t.RH_MoveOk=0;//�ƶ���ɱ�־
			SetRightPwm_Frequence(freq);
		}break;
		
		case LB:{
			SetDutyAndDir(wheel_e,dir_e);
			MoveBaseDir_t.LB_Step+=step;//����
			MoveBaseDir_t.LB_Stop=0;//�������±�־
			MoveBaseDir_t.LB_MoveOk=0;//�ƶ���ɱ�־
			SetRightPwm_Frequence(freq);
		}break;
		case RB:{
			SetDutyAndDir(wheel_e,dir_e);
			MoveBaseDir_t.RB_Step+=step;//����
			MoveBaseDir_t.RB_Stop=0;//�������±�־
			MoveBaseDir_t.RB_MoveOk=0;//�ƶ���ɱ�־
			SetLeftPwm_Frequence(freq);//�����ٶ�		
		}break;
	}
	
	
}

/*
*��ʱ��1ms��ѯ���Ƶ�������ر�
*/
void TimeCtrMoto()
{
	static double LH_time=0;
	static double RH_time=0;
	static double LB_time=0;
	static double RB_time=0;
	static uint32_t nowLeftFreq=0;
	static uint32_t nowRightFreq=0;
	nowLeftFreq=currentLeftPwmFreq;//��ֹƵ������������б��ı�
	nowRightFreq=currentRightPwmFreq;
	/*************��ǰ��***********/
	if(MoveBaseDir_t.LH_MoveOk==0)//�ƶ�δ���
	{
		LH_time+=nowLeftFreq/1000.0;				
		if(LH_time>=MoveBaseDir_t.LH_Step)
		{
			StopMove(LH);//ֹͣ�ƶ�
			MoveBaseDir_t.LH_MoveOk=1;
			MoveBaseDir_t.LH_Step=0;
			LH_time=0;
		}
	}

	/*************��ǰ��***********/
	if(MoveBaseDir_t.RH_MoveOk==0)//�ƶ�δ���
	{
		RH_time+=nowRightFreq/1000.0;	
		if(RH_time>=MoveBaseDir_t.RH_Step)
		{
			StopMove(RH);//ֹͣ�ƶ�
			MoveBaseDir_t.RH_MoveOk=1;
			MoveBaseDir_t.RH_Step=0;
			RH_time=0;
		}
	}
	
	
	/*************���***********/
	if(MoveBaseDir_t.LB_MoveOk==0)//�ƶ�δ���
	{
		LB_time+=nowRightFreq/1000.0;	
		if(LB_time>=MoveBaseDir_t.LB_Step)
		{
			StopMove(LB);//ֹͣ�ƶ�
			MoveBaseDir_t.LB_MoveOk=1;
			MoveBaseDir_t.LB_Step=0;
			LB_time=0;
		}
	}
	
	/*************�Һ�***********/
	if(MoveBaseDir_t.RB_MoveOk==0)//�ƶ�δ���
	{
		RB_time+=nowLeftFreq/1000.0;	
		if(RB_time>=MoveBaseDir_t.RB_Step)
		{
			StopMove(RB);//ֹͣ�ƶ�
			MoveBaseDir_t.RB_MoveOk=1;
			MoveBaseDir_t.RB_Step=0;
			RB_time=0;
		}
	}
	
	
}


/*
*����PWM���,�Լ�����
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
*ֹͣĳ��������ƶ�
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
*ֹͣ�ĸ�������ƶ�
*/
void StopAll(void)
{
	StopMove(LH);
	StopMove(RH);
	StopMove(LB);
	StopMove(RB);
	
}