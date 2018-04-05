#ifndef __VCAN_ENCODER_H__
#define __VCAN_ENCODER_H__


void InitEncoder(void);
int32 GetLeftSpeed(void);
int32 GetRightSpeed(void);

extern int32 MotoLeftSpeed;//左电机速度
extern int32 MotoRightSpeed;//右电机速度
#endif