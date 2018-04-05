#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "VCAN_PID.h"
#include "VCAN_MyMath.h"
#include "VCAN_Servo.h"

/*******************
        SysCtrl_Update_PID_Set();//�趨ֵ
        SysCtrl_Update_PID_Sample();//���²���ֵ���˲�
        SysCtrl_Update_PID_Error();//�������
        SysCtrl_Update_PID_Calculate();//�������ֵ
*************************/
PID_t SysPID[Dof_Num] = {0};


void UpdataMotoPid(void)
{
    int a=0;//���
    PID_Update_Sample(&SysPID[ServoAngle_E],a );//���¶���Ĳ���ֵ
    PID_Update_Error(&SysPID[ServoAngle_E]);//�������   
    PID_Update_Calculate(&SysPID[ServoAngle_E]);//������
    SetServoDuty(SysPID[ServoAngle_E].Out_Int16);
}

void UpdataServoPid(void)
{
    int a=0;//���
    PID_Update_Sample(&SysPID[ServoAngle_E],a );//���¶���Ĳ���ֵ
    PID_Update_Error(&SysPID[ServoAngle_E]);//�������   
    PID_Update_Calculate(&SysPID[ServoAngle_E]);//������
    SetServoDuty(SysPID[ServoAngle_E].Out_Int16);
}


void PID_Parameter_Init(void)
{
    Dof_PID_Init_Similar_Parameter(&SysPID[ServoAngle_E], -1000, 1000);    
    SysPID[ServoAngle_E].p = 0;
    SysPID[ServoAngle_E].i = 0;
    SysPID[ServoAngle_E].d = 0;
    SysPID[ServoAngle_E].sample_cache_max = 2;
    SysPID[ServoAngle_E].set = 0;
    SysPID[ServoAngle_E].err_i_abs_max = 200;//�����ۼ��޷�
    SysPID[ServoAngle_E].i_act_err_abs_max = 80;//���ַ��� 
}
void PID_Update_Sample(PID_t* pPID, float Sample_NewRaw)
{
    uint16 i;   
    for(i=SAMPLE_CACHE_MAX-1; i>0; i--)//�����洢�����£��˲�
        pPID->sample_cache[i] = pPID->sample_cache[i-1];
    pPID->sample_cache[0] = Sample_NewRaw;      
    pPID->sample_cache_cnt++;
    if(pPID->sample_cache_cnt > pPID->sample_cache_max)
        pPID->sample_cache_cnt = pPID->sample_cache_max;
    pPID->sample = DataFilter(pPID->sample_cache, pPID->sample_cache_cnt);   
}

void PID_Update_Error(PID_t* pPID)
{
    pPID->err_last = pPID->err;//�����ϴ����
    pPID->err = pPID->set - pPID->sample;//���±������
    pPID->err_p = pPID->err;//���P
    
    if(ABS(pPID->err) < pPID->i_act_err_abs_max)//���ַ���
    {
        pPID->err_i += pPID->err;
        pPID->err_i=LIMIT(pPID->err_i,-pPID->err_i_abs_max,pPID->err_i_abs_max);
    }
    else
        pPID->err_i = 0;
    pPID->err_d = pPID->err - pPID->err_last;
}
int16 PID_Update_Calculate(PID_t* pDof_PID)
{
    pDof_PID->Out_Int16 = PID_Update_Out(pDof_PID);    
    if(pDof_PID->Out_Int16 < pDof_PID->Out_Min)
        pDof_PID->Out_Int16 = pDof_PID->Out_Min;
    else if(pDof_PID->Out_Int16 > pDof_PID->Out_Max)
        pDof_PID->Out_Int16 = pDof_PID->Out_Max;    
    return pDof_PID->Out_Int16;
}
int16 PID_Update_Out(PID_t* pPID)
{
    pPID->out_p = pPID->p * pPID->err_p;
    pPID->out_i = pPID->i * pPID->err_i;
    pPID->out_d = pPID->d * pPID->err_d;
    
    if(pPID->out_i < pPID->out_i_min)
        pPID->out_i = pPID->out_i_min;
    else if(pPID->out_i > pPID->out_i_max)
        pPID->out_i = pPID->out_i_max;
    
    pPID->out = pPID->out_p + pPID->out_i + pPID->out_d;
    
    return (int16)pPID->out;
}
void PID_Update_Set(PID_t* pPID, float Set_New)
{
    pPID->set = Set_New;
}
void Dof_PID_Init_Similar_Parameter(PID_t* pDof_PID, int16 Out_Min, int16 Out_Max)
{
    pDof_PID->Out_Int16 = 0;
    pDof_PID->Out_Min = Out_Min;
    pDof_PID->Out_Max = Out_Max;
    //Fill_Mem((uint8*)pDof_PID->PID[Pos].sample_cache, sizeof(float)*SAMPLE_CACHE_MAX, 0);
    pDof_PID->sample_cache_cnt = 0;
    pDof_PID->err = 0;
    pDof_PID->err_last = 0;
    pDof_PID->err_p = 0;
    pDof_PID->err_i = 0;
    pDof_PID->err_d = 0;    
    pDof_PID->out = 0;
    pDof_PID->out_p = 0;
    pDof_PID->out_i = 0;
    pDof_PID->out_i_min = pDof_PID->Out_Min * 0.5;
    pDof_PID->out_i_max = pDof_PID->Out_Max * 0.5;
    pDof_PID->out_d = 0;    
}