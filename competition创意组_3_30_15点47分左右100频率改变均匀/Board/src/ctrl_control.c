#include "ctrl_control.h"
#include "include.h"

YesNo_t En_Update_PID = No;
uint8_t EN_Track=1;
extern BARRIER_E barrier_flag;
extern void SysCtrl_Update_PID(void)
{
    if(Yes == En_Update_PID)
    {
        //En_Update_PID = No;
//        SysCtrl_Update_PID_Set();//²»¿ª
        SysCtrl_Update_PID_Sample();
        SysCtrl_Update_PID_Error();
        SysCtrl_Update_PID_Calculate();		
        SysCtrl_Update_PID_Change();
    }
}

extern void SysCtrl_Update_PID_Set(void)
{
}

extern void SysCtrl_Update_PID_Sample(void)
{
  PID_Update_Sample(&SysPID[Servo], Light_t.Mid.x, (YesNo_t)Yes);
}

extern void SysCtrl_Update_PID_Error(void)
{
    PID_Update_Error(&SysPID[Servo], (YesNo_t)Yes);
}

extern void SysCtrl_Update_PID_Calculate(void)
{
     Dof_PID_Update_Calculate(&SysPID[Servo]);
}


extern void SysCtrl_Update_PID_Change(void)
{

    SetServoDuty(SysPID[Servo].Out_Int16);
}

