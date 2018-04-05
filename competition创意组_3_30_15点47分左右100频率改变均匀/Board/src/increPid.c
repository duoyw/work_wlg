#include "common.h"
#include "include.h"
#include  "increPid.h"


/*!
 *  @brief    ����ʽPID
 *  @since      v1.0
 */

int16 UpdataPidIcerment(Dof_PID_t* pPID,float Sample_NewRaw)
{
  /**���²���ֵ���˲�**/
  PID_Update_Sample(pPID, Sample_NewRaw, (YesNo_t)Yes);
  
  /*****�������****/
  pPID->err_last_last=pPID->err_last;//�������ϴ����
  pPID->err_last = pPID->err;//�����ϴ����
  pPID->err = pPID->set - pPID->sample;//���±������
  
  if(pPID->err > 0)//������ֵ
      pPID->err_abs = pPID->err;
  else
      pPID->err_abs = -pPID->err; 
  
     
  pPID->err_p = pPID->err-pPID->err_last;//��������ȥ�ϴ����  
  pPID->err_i = pPID->err;//���I�����ۼ�
  pPID->err_d = pPID->err-2*pPID->err_last+pPID->err_last_last;  
  

  /*****������*****/
  pPID->out_p = pPID->p * pPID->err_p;//���P
  pPID->out_i = pPID->i * pPID->err_i;
  pPID->out_d = pPID->d * pPID->err_d;

  if(pPID->out_i < pPID->out_i_min)//�����޷�
      pPID->out_i = pPID->out_i_min;
  else if(pPID->out_i > pPID->out_i_max)
      pPID->out_i = pPID->out_i_max;

  pPID->out = pPID->out_p + pPID->out_i + pPID->out_d;
  pPID->Out_Int16+= (int16)pPID->out;   
  if(pPID->Out_Int16 < pPID->Out_Min)//����޷�
      pPID->Out_Int16 = (int16)pPID->Out_Min;
  else if(pPID->Out_Int16 > pPID->Out_Max)
      pPID->Out_Int16 = (int16)pPID->Out_Max;   
  
  /***********ֱ�߿�������**************/
   if(GetCountMMTime(1)>=1000&&SpdAct==Car_quicken)
   {
       if((pPID->err)>15)
      {         
            pPID->Out_Int16=10000;
      }
   }
  return pPID->Out_Int16;   
}



