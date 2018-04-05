#ifndef _CTRL_CAR_H_
#define _CTRL_CAR_H_

typedef enum
{
	car_H=0,//��
	car_B,//��
	car_L,//��
	car_R,//��
	car_LH,//����
	car_LB,//����
	car_RH,//����
	car_RB,//����
	car_LeftTurn,//����ת
	car_RightTurn,//����ת  
}CarDir_E;
void CarMove(CarDir_E CarDir_e,uint32_t step,uint32_t freq);

extern uint32_t carSpeed;
extern CarDir_E CarDir_e;
#endif