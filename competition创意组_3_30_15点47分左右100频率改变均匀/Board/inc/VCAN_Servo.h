#ifndef __VCAN_SERVO_H__
#define __VCAN_SERVO_H__

#include "MK60_ftm.h"


#define MID_ANGLE (45/100.0)*FTM2_PRECISON//中值
#define MAX_ANGLE (75/100.0)*FTM2_PRECISON//最大打角
#define MIN_ANGLE (23/100.0)*FTM2_PRECISON//最小打角

void SetServoDuty(int16 duty);
void InitServo(void);
void TimeChangeServoPwm(void);

extern uint8_t  isServoMoveOk;
#endif