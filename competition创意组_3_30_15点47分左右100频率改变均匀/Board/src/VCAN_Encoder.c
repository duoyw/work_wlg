#include "common.h"
#include "include.h"

int32 MotoLeftSpeed=0;//左电机速度
int32 MotoRightSpeed=0;//右电机速度

void InitEncoder()
{
  //Lptmr读取右电机速度大小
  lptmr_pulse_init(LPT0_ALT2,0xFFFF,LPT_Rising); 
  gpio_init (PTC0, GPI,0);    //设置PTC0为输入,读取右电机速度方向
  
  //FTM读取左电机速度大小及方向
  ftm_quad_init(FTM1);
}

/**
*获得脉冲数=速度
**/
int32 GetLeftSpeed(void)
{
  int32 temp=0;
  temp=ftm_quad_get(FTM1);
  ftm_quad_clean(FTM1);
  return temp;  
}

/**
*获得脉冲数=速度
**/
int32 GetRightSpeed(void)
{
  int32 temp=0;
  int8	dir=0;
 
  dir=gpio_get(PTC0);//读取方向
  if(dir==0)
  	temp=lptmr_pulse_get();//读取脉冲
  else 
	temp=-lptmr_pulse_get();//读取脉冲
  
  lptmr_pulse_clean();
  return temp;
}
