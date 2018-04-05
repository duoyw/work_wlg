#include "common.h"
#include "include.h"
#include "CrtlCar.h"
CarDir_E CarDir_e;
uint32_t carSpeed=0;
/*
*控制车移动
*CarDir_E:车方向
*step:距离
*freq:左右对角线相同频率
*/
void CarMove(CarDir_E CarDir_e,uint32_t step,uint32_t freq)
{
  	switch(CarDir_e)
	{
	  case car_H:
	  {
	 	MoveWheel(LH,Wheel_Head,step,freq);
		MoveWheel(RH,Wheel_Head,step,freq);
		MoveWheel(LB,Wheel_Head,step,freq);
		MoveWheel(RB,Wheel_Head,step,freq);
	  }break;
	  
	  case car_B:
	  {
	  	MoveWheel(LH,Wheel_Back,step,freq);
		MoveWheel(RH,Wheel_Back,step,freq);
		MoveWheel(LB,Wheel_Back,step,freq);
		MoveWheel(RB,Wheel_Back,step,freq);
	  }break;
	  
	  case car_L:
	  {
	  	MoveWheel(LH,Wheel_Back,step,freq);
		MoveWheel(RH,Wheel_Head,step,freq);
		MoveWheel(LB,Wheel_Head,step,freq);
		MoveWheel(RB,Wheel_Back,step,freq);
	  }break;
	  
	  case car_R:
	  {
	  	MoveWheel(LH,Wheel_Head,step,freq);
		MoveWheel(RH,Wheel_Back,step,freq);
		MoveWheel(LB,Wheel_Back,step,freq);
		MoveWheel(RB,Wheel_Head,step,freq);
	  }break;
	  
	  case car_LH:
	  {
	  	MoveWheel(RH,Wheel_Head,step,freq);
		MoveWheel(LB,Wheel_Head,step,freq);
	  }break;
	  
	  case car_RH:
	  {
	  	MoveWheel(LH,Wheel_Head,step,freq);
		MoveWheel(RB,Wheel_Head,step,freq);
	  }break;
	  
	  case car_LB:
	  {
	  	MoveWheel(LH,Wheel_Back,step,freq);
		MoveWheel(RB,Wheel_Back,step,freq);
	  }break;
	  
	  case car_RB:
	  {
	  	MoveWheel(RH,Wheel_Back,step,freq);
		MoveWheel(LB,Wheel_Back,step,freq);
	  }break;
	  
	  case car_LeftTurn:
	  {
	  	MoveWheel(LH,Wheel_Back,step,freq);
		MoveWheel(RH,Wheel_Head,step,freq);
	  	MoveWheel(LB,Wheel_Back,step,freq);
		MoveWheel(RB,Wheel_Head,step,freq);
	  }break;
	  
	  case car_RightTurn:
	  {
		MoveWheel(LH,Wheel_Head,step,freq);
		MoveWheel(RH,Wheel_Back,step,freq);
	  	MoveWheel(LB,Wheel_Head,step,freq);
		MoveWheel(RB,Wheel_Back,step,freq);
	  }break;
	}
}

