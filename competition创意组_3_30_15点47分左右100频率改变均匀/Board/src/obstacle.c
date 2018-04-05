#include "obstacle.h"
#include "include.h"
BARRIER_E barrier_flag = NO;
YesNo_t obastaProtectFlag;
void InitObstaclePort(void)
{
    gpio_init (PTE9, GPI,0);
    gpio_init (PTE8, GPI,0);
}

	
void Obstacle_avoidance(void)
{
  if(GetCountMMTime(0)>50)
  {
	  obastaProtectFlag=No;
		  if(Light_t.Mid.y <50)
	  {
		  if(gpio_get(PTE9) == 1)          //×ó
		  {
			  barrier_flag = Yes_L;	
			   SetServoDuty(80);
				StartCountTime(0);
				obastaProtectFlag=Yes;
				
		  }
		  else if(gpio_get(PTE8) == 1)     //ÓÒ
		  {
			  barrier_flag = Yes_R;    
			   SetServoDuty(-80);
				StartCountTime(0);
				obastaProtectFlag=Yes;
	   
		  }
		  else
		  {          
			  barrier_flag = NO;
		  }
	  }
	  else
		  barrier_flag = NO;
  }
}