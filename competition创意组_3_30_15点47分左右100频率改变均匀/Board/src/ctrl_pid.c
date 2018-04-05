#include "ctrl_pid.h"
Dof_PID_t SysPID[Dof_Num] = {0};//PID数组初始化
static void Dof_PID_Init_Similar_Parameter(Dof_PID_t* pDof_PID, float Out_Min, float Out_Max);

/*
*PID采样并对采样数据进行滤波
*Sample_NewRaw：本次采样值
*Sample_Valid：本次采样有效性，默认Yes
*/
void PID_Update_Sample(Dof_PID_t* pPID, float Sample_NewRaw, YesNo_t Sample_Valid)
{    
    uint16 i;   
    for(i=SAMPLE_CACHE_MAX-1; i>0; i--)//采样数组更新
        pPID->sample_cache[i] = pPID->sample_cache[i-1];
    
    pPID->sample_cache[0] = Sample_NewRaw;//更新最新的采样
    
    if(Yes == Sample_Valid)
    {
        pPID->sample_cache_cnt++;
        if(pPID->sample_cache_cnt > pPID->sample_cache_max)
            pPID->sample_cache_cnt = pPID->sample_cache_max;
       
        pPID->sample = DataFilter(pPID->sample_cache, pPID->sample_cache_cnt);//数据滤波
    }    
    else
    {
        pPID->sample_cache_cnt = 0;
    }
}

void  PID_Update_Error (Dof_PID_t* pPID, YesNo_t Sample_Valid)
{
  pPID->err_last = pPID->err;//保存上次误差
    if(Yes == Sample_Valid)
    {
        pPID->err = pPID->set - pPID->sample;//计算误差
        
        if(pPID->err > 0)//误差绝对值
            pPID->err_abs = pPID->err;
        else
            pPID->err_abs = -pPID->err;
        
        pPID->err_p = pPID->err;//比例P
        
        if(Abs(pPID->err) < pPID->i_act_err_abs_max)//计算积分I
        {
            pPID->err_i += pPID->err;//积分累加
            Constraint_By_Abs(&pPID->err_i, pPID->err_i_abs_max);//积分限幅
        }
        else
            pPID->err_i = 0;//积分清0
        

//        pPID->err_d =  pPID->err_last_d*0.7 + pPID->err_d*0.3;//低通滤波
        pPID->err_d = pPID->err - pPID->err_last;//计算误差
		if(ABS(pPID->err_d)>30)        //微分D最大值限制
			pPID->err_d=0;
    }
    else
    {
        pPID->err = 0;
        pPID->err_p = 0;
        pPID->err_i = 0;
        pPID->err_d = 0;
    }
}

int16 Dof_PID_Update_Calculate(Dof_PID_t* pPID)
{ 
    pPID->Out_Float = 0.00;
    pPID->Out_Float += PID_Update_Out(pPID);
    
    if(pPID->Out_Float < pPID->Out_Min)//输出限幅最小值
        pPID->Out_Float = pPID->Out_Min;
    else if(pPID->Out_Float > pPID->Out_Max)//输出限幅最大值
        pPID->Out_Float = pPID->Out_Max;
    
    pPID->Out_Int16 = (int16)pPID->Out_Float;//输出化为整型
    
    return pPID->Out_Int16;
}


float PID_Update_Out(Dof_PID_t* pPID)
{
    pPID->out_p = pPID->p * pPID->err_p;//输出P
    pPID->out_i = pPID->i * pPID->err_i;//输出I
    pPID->out_d = pPID->d * pPID->err_d;//输出D
    
    if(pPID->out_i < pPID->out_i_min)//输出积分最小值限幅
        pPID->out_i = pPID->out_i_min;
    else if(pPID->out_i > pPID->out_i_max)//输出积分最大值限幅
        pPID->out_i = pPID->out_i_max;
    if(pPID->out_d>=2000)
    {
        pPID->out_d = 0;
    }
    pPID->out = pPID->out_p + pPID->out_i + pPID->out_d;//输出累加
    return pPID->out; 
}


void PID_Parameter_Init(void)
{
    Dof_PID_Init_Similar_Parameter(&SysPID[Servo], -100, 100);//最大输出，最小输出
    Dof_PID_Init_Similar_Parameter(&SysPID[LeftMotor_Spd], -10000, 10000);
    Dof_PID_Init_Similar_Parameter(&SysPID[RightMotor_Spd], -10000, 10000);
       
    SysPID[Servo].p = 0.75; 
    SysPID[Servo].i = 0;
    SysPID[Servo].d = 2.3;
    SysPID[Servo].sample_cache_max = 1;
    SysPID[Servo].set = 0;
    SysPID[Servo].err_i_abs_max = 2000;
    SysPID[Servo].i_act_err_abs_max = 10;
    
    SysPID[RightMotor_Spd].p = 325;
    SysPID[RightMotor_Spd].i = 30;
    SysPID[RightMotor_Spd].d = 0;
    SysPID[RightMotor_Spd].sample_cache_max = 1;
    SysPID[RightMotor_Spd].set = 0;
    SysPID[RightMotor_Spd].err_i_abs_max = 10000;
    SysPID[RightMotor_Spd].i_act_err_abs_max = 10000;
  
	
    SysPID[LeftMotor_Spd].p = 325;
    SysPID[LeftMotor_Spd].i = 30;
    SysPID[LeftMotor_Spd].d = 0;
    SysPID[LeftMotor_Spd].sample_cache_max = 1;
    SysPID[LeftMotor_Spd].set = 0;
    SysPID[LeftMotor_Spd].err_i_abs_max = 10000;
    SysPID[LeftMotor_Spd].i_act_err_abs_max = 10000;
   
    
}

static void Dof_PID_Init_Similar_Parameter(Dof_PID_t* pPID, float Out_Min, float Out_Max)
{
    pPID->Out_Float = 0;
    pPID->Out_Int16 = 0;
    pPID->Out_Min = Out_Min;
    pPID->Out_Max = Out_Max;

    Fill_Mem((uint8*)pPID->sample_cache, sizeof(float)*SAMPLE_CACHE_MAX, 0);
    
    pPID->sample_cache_cnt = 0;
    pPID->err = 0;
    pPID->err_p = 0;
    pPID->err_i = 0;
    pPID->err_d = 0;
    pPID->err_last = 0;
    pPID->out = 0; 
    pPID->out_p = 0;
    pPID->out_i = 0;
    pPID->out_d = 0;
    pPID->out_i_min = pPID->Out_Min * 1;
    pPID->out_i_max = pPID->Out_Max * 1;
    
}



