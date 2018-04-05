#include "common.h"
#include "include.h"

int32 MotoLeftSpeed=0;//�����ٶ�
int32 MotoRightSpeed=0;//�ҵ���ٶ�

void InitEncoder()
{
  //Lptmr��ȡ�ҵ���ٶȴ�С
  lptmr_pulse_init(LPT0_ALT2,0xFFFF,LPT_Rising); 
  gpio_init (PTC0, GPI,0);    //����PTC0Ϊ����,��ȡ�ҵ���ٶȷ���
  
  //FTM��ȡ�����ٶȴ�С������
  ftm_quad_init(FTM1);
}

/**
*���������=�ٶ�
**/
int32 GetLeftSpeed(void)
{
  int32 temp=0;
  temp=ftm_quad_get(FTM1);
  ftm_quad_clean(FTM1);
  return temp;  
}

/**
*���������=�ٶ�
**/
int32 GetRightSpeed(void)
{
  int32 temp=0;
  int8	dir=0;
 
  dir=gpio_get(PTC0);//��ȡ����
  if(dir==0)
  	temp=lptmr_pulse_get();//��ȡ����
  else 
	temp=-lptmr_pulse_get();//��ȡ����
  
  lptmr_pulse_clean();
  return temp;
}
