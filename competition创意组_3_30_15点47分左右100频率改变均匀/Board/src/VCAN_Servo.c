#include "common.h"
#include "include.h"

uint32_t ServoPwm=MAX_ANGLE;
uint32_t currentServoPwm=MID_ANGLE;
uint8_t  isServoMoveOk=1;
/**初始化舵机PWM**/
void InitServo(void)
{
  ftm_pwm_init(FTM2,FTM_CH1,300,MID_ANGLE);
}

/**设置舵机占空比**/
void SetServoDuty(int16 duty)
{
  if(duty>MAX_ANGLE)//限幅
	duty=MAX_ANGLE;
  else if(duty<MIN_ANGLE)
	duty=MIN_ANGLE;
  ServoPwm=duty;
  isServoMoveOk=0;
  
}   

#define ChangePwmValue	(100)
void TimeChangeServoPwm()
{
  if(ServoPwm==currentServoPwm)
	isServoMoveOk=1;
  
  if(ServoPwm>currentServoPwm)
	currentServoPwm+=ChangePwmValue;
  else if(ServoPwm<currentServoPwm)
	currentServoPwm-=ChangePwmValue;
  
  if(currentServoPwm>MAX_ANGLE)//限幅
	currentServoPwm=MAX_ANGLE;
  else if(currentServoPwm<MIN_ANGLE)
	currentServoPwm=MIN_ANGLE;
  

  ftm_pwm_duty(FTM2,FTM_CH1,currentServoPwm);
}