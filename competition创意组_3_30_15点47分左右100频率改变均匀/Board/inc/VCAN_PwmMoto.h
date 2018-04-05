#ifndef __VCAN_PWMMOTO_H__
#define __VCAN_PWMMOTO_H__

#include "CrtlMoto.h"
#define LH_CW	PTD10	//cw+
#define RH_CW	PTD11
#define LB_CW	PTD12
#define RB_CW	PTD13

void TimeChangeFreq(void);
void InitPwmMoto(void);
void InitCtrDir(void);
void SetLeftPwm_Frequence( uint32_t leftFre );
void SetRightPwm_Frequence( uint32_t rightFre );
void SetPwm_Duty(Wheel_E wheel_e,uint16_t duty);
extern uint8_t Motor_flag;
extern uint32_t	currentLeftPwmFreq;
extern uint32_t	currentRightPwmFreq;
extern uint32_t setRightPwmFreq;
extern uint32_t setLeftPwmFreq;
#endif