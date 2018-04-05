#ifndef __VCAN_SERVO_H__
#define __VCAN_SERVO_H__

#include "MK60_ftm.h"


#define MID_ANGLE (45/100.0)*FTM2_PRECISON//��ֵ
#define MAX_ANGLE (75/100.0)*FTM2_PRECISON//�����
#define MIN_ANGLE (23/100.0)*FTM2_PRECISON//��С���

void SetServoDuty(int16 duty);
void InitServo(void);
void TimeChangeServoPwm(void);

extern uint8_t  isServoMoveOk;
#endif