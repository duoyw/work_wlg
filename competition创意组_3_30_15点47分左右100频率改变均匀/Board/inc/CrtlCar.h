#ifndef _CTRL_CAR_H_
#define _CTRL_CAR_H_

typedef enum
{
	car_H=0,//上
	car_B,//下
	car_L,//左
	car_R,//右
	car_LH,//左上
	car_LB,//左下
	car_RH,//右上
	car_RB,//右下
	car_LeftTurn,//左旋转
	car_RightTurn,//右旋转  
}CarDir_E;
void CarMove(CarDir_E CarDir_e,uint32_t step,uint32_t freq);

extern uint32_t carSpeed;
extern CarDir_E CarDir_e;
#endif