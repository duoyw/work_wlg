#ifndef __VCAN_PIT_H__
#define __VCAN_PIT_H__


#define PIT_TIME   1    //定时中断时间，1为1ms


void InitPit(void);
void StartCountTime(uint8 type);
uint32_t GetCountMMTime(uint8 type);
#endif