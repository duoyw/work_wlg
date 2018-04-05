#include "include.h"
#include "duty.h"

#define AimNum  OV7725_EAGLE_H
uint16 beaconAim_L[AimNum]={0};//������
uint16 beaconAim_R[AimNum]={0};
/*
ѭ���ȴ���������
*/
void IsStart()
{
  do{
	 En_Update_PID = No;
	 ImaegDecideDeal();
  }while(Light_Flag_t.HaveLight == No);

  En_Update_PID = Yes;
}


/*·���������
* ���ƴ���Ϊһ��ֱ��
* ��������ȷ��ֱ��
*/
void road_ValueInit(void)
{
	int i=0;
	double k1=0.8308,b1=143.0664;//�������
	double k2=-0.8840,b2=165.8684;//���Ҳ���
        b1+=10;
	for(i=0;i<OV7725_EAGLE_H;i++)
	{
			beaconAim_L[i] = (uint16)(k1*i+b1);
	}
	for(i=0;i<OV7725_EAGLE_H;i++)
	{
			beaconAim_R[i] = (uint16)(k2*i+b2);
	}
}

/*
*���ܣ������ߵ��趨ֵ
*dir:0������ߣ�1�����ұ�,������������
*/
uint16_t GetRecordValueX(DIR_E dir)
{
  int16_t temp=(uint16)Light_t.Mid.y-1;
  temp=LIMIT(temp,0,AimNum);
  
  if(dir==Left_E)
    return beaconAim_L[temp];
  else if(dir==Right_E)
    return beaconAim_R[temp];
  else 
    while(1);
}
