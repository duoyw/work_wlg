#include "common.h"
#include "include.h"
#include  "increPid.h"


/*!
 *  @brief    增量式PID
 *  @since      v1.0
 */

int16 UpdataPidIcerment(Dof_PID_t* pPID,float Sample_NewRaw)
{
  /**更新采样值并滤波**/
  PID_Update_Sample(pPID, Sample_NewRaw, (YesNo_t)Yes);
  
  /*****计算误差****/
  pPID->err_last_last=pPID->err_last;//更新上上次误差
  pPID->err_last = pPID->err;//更新上次误差
  pPID->err = pPID->set - pPID->sample;//更新本次误差
  
  if(pPID->err > 0)//误差绝对值
      pPID->err_abs = pPID->err;
  else
      pPID->err_abs = -pPID->err; 
  
     
  pPID->err_p = pPID->err-pPID->err_last;//本次误差减去上次误差  
  pPID->err_i = pPID->err;//误差I，不累加
  pPID->err_d = pPID->err-2*pPID->err_last+pPID->err_last_last;  
  

  /*****计算结果*****/
  pPID->out_p = pPID->p * pPID->err_p;//输出P
  pPID->out_i = pPID->i * pPID->err_i;
  pPID->out_d = pPID->d * pPID->err_d;

  if(pPID->out_i < pPID->out_i_min)//积分限幅
      pPID->out_i = pPID->out_i_min;
  else if(pPID->out_i > pPID->out_i_max)
      pPID->out_i = pPID->out_i_max;

  pPID->out = pPID->out_p + pPID->out_i + pPID->out_d;
  pPID->Out_Int16+= (int16)pPID->out;   
  if(pPID->Out_Int16 < pPID->Out_Min)//输出限幅
      pPID->Out_Int16 = (int16)pPID->Out_Min;
  else if(pPID->Out_Int16 > pPID->Out_Max)
      pPID->Out_Int16 = (int16)pPID->Out_Max;   
  
  /***********直线开启棒棒**************/
   if(GetCountMMTime(1)>=1000&&SpdAct==Car_quicken)
   {
       if((pPID->err)>15)
      {         
            pPID->Out_Int16=10000;
      }
   }
  return pPID->Out_Int16;   
}



