#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "ctrl_pid.h"


extern void SysCtrl_Update_PID(void);
extern void SysCtrl_Update_PID_Set(void);
extern void SysCtrl_Update_PID_Sample(void);
extern void SysCtrl_Update_PID_Error(void);
extern void SysCtrl_Update_PID_Calculate(void);
extern void SysCtrl_Update_PID_Change(void);
extern void SysCtrl_Update_Act(void);
void ChangePidAccordingToError(void);

extern YesNo_t Is_On_Node(YesNo_t F, YesNo_t B, YesNo_t L, YesNo_t R);
extern YesNo_t En_Update_PID ;
extern uint8_t EN_Track;

#endif  // __CONTROL_H__

