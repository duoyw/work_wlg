#include "include.h"
#include "duty.h"

#define AimNum  OV7725_EAGLE_H
uint16 beaconAim_L[AimNum]={0};//左数据
uint16 beaconAim_R[AimNum]={0};
/*
循环等待灯亮发车
*/
void IsStart()
{
  do{
	 En_Update_PID = No;
	 ImaegDecideDeal();
  }while(Light_Flag_t.HaveLight == No);

  En_Update_PID = Yes;
}


/*路径拟合曲线
* 近似处理为一条直线
* 采用两点确定直线
*/
void road_ValueInit(void)
{
	int i=0;
	double k1=0.8308,b1=143.0664;//向左参数
	double k2=-0.8840,b2=165.8684;//向右参数
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
*功能：获得左边的设定值
*dir:0返回左边，1返回右边,其他数据则卡死
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
