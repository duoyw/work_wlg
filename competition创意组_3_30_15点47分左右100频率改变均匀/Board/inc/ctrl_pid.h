#ifndef __PID_H__
#define __PID_H__

#include "ctrl_basic.h"

#define SAMPLE_CACHE_MAX  10
typedef enum
{
    Servo = 0,
    LeftMotor_Spd,
	RightMotor_Spd,
    Dof_Num,
}Dof_t;

typedef struct
{
    float Out_Float;
    int16 Out_Int16;
    float Out_Min;
    float Out_Max;
    
    float p;
    float i;
    float d;
    float sample;//ֵ采样值
    float sample_last;//上次采样值
    float sample_last_last;
    float sample_cache[SAMPLE_CACHE_MAX];//采样存储数组
    uint16 sample_cache_cnt;//采样数组计数
    uint16 sample_cache_max;//采样数组最大值
    float set;
    float last_set; 
    float err;
    float err_abs;
    float err_last;
    float err_last_last;
    float err_p;
    float err_i;
    float err_d;
    float err_last_d;
    float err_i_abs_max;//积分最大值
    float i_act_err_abs_max;//积分分离
    
    float out;
    float out_p;
    float out_i;
    float out_i_min;
    float out_i_max;
    float out_d;
}Dof_PID_t;

typedef struct
{
    float sample;
    YesNo_t valid;
}PID_Sample_t;

extern Dof_PID_t SysPID[Dof_Num];
void PID_Parameter_Init(void);
void PID_Update_Sample(Dof_PID_t* pPID, float Sample_NewRaw, YesNo_t Sample_Valid);//采样
void PID_Update_Error(Dof_PID_t* pPID, YesNo_t Sample_Valid);//误差
int16 Dof_PID_Update_Calculate(Dof_PID_t* pPID);//计算
float PID_Update_Out(Dof_PID_t* pPID);//输出

#endif  // __PID_H__

