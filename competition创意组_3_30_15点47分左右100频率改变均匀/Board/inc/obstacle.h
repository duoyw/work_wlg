#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_
#include "include.h"
typedef enum{
	NO=0,
	Yes_L,
	Yes_R,
	Null
}BARRIER_E;

void InitObstaclePort(void);
void Obstacle_avoidance(void);

extern BARRIER_E barrier_flag;
extern YesNo_t obastaProtectFlag;
#endif