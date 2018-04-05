#ifndef __CTRLMOTO_H
#define __CTRLMOTO_H
#include "common.h"

typedef enum
{
	Wheel_Head=0,
	Wheel_Back,
}DirBase_E;

typedef enum
{
	LH=0,
	RH,
	LB,
	RB,
}Wheel_E;

typedef struct
{
	uint32_t LH_Step;
	uint32_t RH_Step;
	uint32_t LB_Step;
	uint32_t RB_Step;
	
	DirBase_E LH_Dir;
	DirBase_E RH_Dir;
	DirBase_E LB_Dir;
	DirBase_E RB_Dir;
	
	uint32_t LH_StepAllNum;
	uint32_t RH_StepAllNum;
	uint32_t LB_StepAllNum;
	uint32_t RB_StepAllNum;
	
	uint8_t LH_Stop;
	uint8_t RH_Stop;
	uint8_t LB_Stop;
	uint8_t RB_Stop;
	
	uint8_t LH_MoveOk;
	uint8_t RH_MoveOk;
	uint8_t LB_MoveOk;
	uint8_t RB_MoveOk;
}MoveBaseDir_ST;

extern MoveBaseDir_ST MoveBaseDir_t;
extern DirBase_E DirBase_e;
extern uint32_t CurrentX,CurrentY;//µ±Ç°XY×ø±ê
void MoveWheel(Wheel_E wheel_e,DirBase_E dir_e,uint32_t step,uint32_t freq);
void SetDutyAndDir(Wheel_E wheel_e,DirBase_E dir_e);
void StopMove(Wheel_E wheel_e);
void TimeCtrMoto(void);
void StopAll(void);
#endif
