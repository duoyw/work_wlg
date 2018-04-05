#include "common.h"
#include "include.h"

_xy_St _xy_t;
Light_St Light_t;//����
Light_Flag_St Light_Flag_t;//״̬��־

void Image_Deal(void)
{
    Seach_Car_XY();
}


void Para_Init(void)
{
    Light_t.Top.x=0;
    Light_t.Top.y=OV7725_EAGLE_H-1;
    Light_t.Bottom.x=0;
    Light_t.Bottom.y=0;

    Light_t.Left.x=OV7725_EAGLE_W-1;
    Light_t.Left.y=0;
    Light_t.Right.x=0;
    Light_t.Right.y=0;

    Light_Flag_t.HaveTopLight=No;//�����ϰ벿��
    Light_Flag_t.HaveBottomLight=No;//�����°벿��
//    Light_Flag_t.HaveLight=No;//��Ұ���Ƿ��е�  	
}
void Seach_Car_XY()
{
  	static uint16 lastMidLine=OV7725_EAGLE_H/2;//�ϴ�ͼ���ҵ����У����⸽��ɨ�裬�������ҵ����
	uint8* imagePoint=imgbuff;
	uint16 scanLine=0;//��ǰ����ɨ����
	uint8 Gap=1;//����ɨ�����
	uint16 CarLeft_X=0,CarRight_X=0;//��������x��
	uint16 Last_Left_X=0;//��һ������x��
	YesNo_t Have_Dot_Line=No;//�����е�
	YesNo_t Have_Dot_Last_Line=No;//��һ���е�
	uint8  TopNoLine=0;//���Ϸ�����û�����
	uint8  BottomNoLine=0;//���·�����û�����

	
	Para_Init();//������ʼ��
	for(scanLine=lastMidLine;scanLine<OV7725_EAGLE_H-1;scanLine+=Gap)//���趨��λ�ÿ�ʼ�����ң�Ĭ���ϴ�ͼ��mid�п�ʼ
	{
	  	ImaPointChange(&imagePoint,scanLine);//�ƶ�ָ��
	  	CarLeft_X=0;
		CarRight_X=0;
		FindLineDot(imagePoint,&CarLeft_X,&CarRight_X);//�ҵ�
		
		if(CarLeft_X==0&&CarRight_X==0)//�ж����޵�
			Have_Dot_Line=No;//�����޵�
		else
		{
		  	
			Have_Dot_Line=Yes;//�����е�
			BottomNoLine=0;//��0
		}
	
		if(Have_Dot_Last_Line==Yes&&Have_Dot_Line==Yes&&ABS(CarLeft_X-Last_Left_X)<6)//�жϵ����Ч��
		{
			Light_Flag_t.HaveBottomLight=Yes;//�ȴ�����ɨ��
			if(scanLine<Light_t.Top.y)//�ҵ�������ߵ㣬��ֵԽСԽ����
			{
				Light_t.Top.y=scanLine;
			}
			if(scanLine>Light_t.Bottom.y)//�ҵ�������͵㣬Խ��Խ����
			{
				Light_t.Bottom.y=scanLine;
			}
			if(CarLeft_X<Light_t.Left.x)//�ҵ�����͵㣬ԽСԽ����
			{
				Light_t.Left.x=CarLeft_X;
			}
			if(CarRight_X>Light_t.Right.x)
			{
				Light_t.Right.x=CarRight_X;
			}
		}
		Last_Left_X=CarLeft_X;//���汾��ֵ
		Have_Dot_Last_Line=Have_Dot_Line;
		
		if(Have_Dot_Line==No&&Light_Flag_t.HaveBottomLight==Yes)//�Ѿ�ɨ�����㣬���ڳ����޵���У����ɿ���ɨ��Բ��
		{
		  BottomNoLine++;
		  if(BottomNoLine==4)//����4��û��
		  {
			BottomNoLine=0;//ȷ������Բ
			break;//����ѭ��
		  }
		}
	}
	for(scanLine=lastMidLine; scanLine>0 ;scanLine-=Gap)//���ϴ�ͼ���yֵ��ʼɨ��
	{
	  	ImaPointChange(&imagePoint,scanLine);//�ƶ�ָ��
		CarLeft_X=0;
		CarRight_X=0;
		FindLineDot(imagePoint,&CarLeft_X,&CarRight_X);//�ҵ�
		
		if(CarLeft_X==0&&CarRight_X==0)//�ж����޵�
			Have_Dot_Line=No;//�����޵�
		else
		{
			Have_Dot_Line=Yes;//�����е�
			
			TopNoLine=0;//��0
		}
		
		 if(Have_Dot_Last_Line==Yes&&Have_Dot_Line==Yes&&ABS(CarLeft_X-Last_Left_X)<6)//�жϵ����Ч��
		{
			Light_Flag_t.HaveTopLight=Yes;//�Ϸ��е�
			if(scanLine<Light_t.Top.y)//�ҵ�������ߵ㣬��ֵԽСԽ����
			{
				Light_t.Top.y=scanLine;
			}
			if(scanLine>Light_t.Bottom.y)//�ҵ�������͵㣬Խ��Խ����
			{
				Light_t.Bottom.y=scanLine;
			}
			if(CarLeft_X<Light_t.Left.x)//�ҵ�����͵㣬ԽСԽ����
			{
				Light_t.Left.x=CarLeft_X;
			}
			if(CarRight_X>Light_t.Right.x)
			{
				Light_t.Right.x=CarRight_X;
			}
		}
		Last_Left_X=CarLeft_X;//����ֵ����
		Have_Dot_Last_Line=Have_Dot_Line;//�����е㱣��
		
		if(Have_Dot_Line==No&&Light_Flag_t.HaveTopLight==Yes)//�Ѿ�ɨ�����㣬���ڳ����޵���У����ɿ���ɨ��Բ��
		{
			TopNoLine++;
			if(TopNoLine==4)//����4��û��
			{
			  TopNoLine=0;
			  break;//����ѭ��
			}
		}
	}
	
	if(Light_Flag_t.HaveBottomLight==No&&Light_Flag_t.HaveTopLight==No)//���¶�û��
	{
            Light_Flag_t.HaveLight=No;
            Light_Flag_t.HaveLightProtect=Yes;
        }
	else 
	{
            Light_Flag_t.HaveLight=Yes;	
        }
	
	if(Light_Flag_t.HaveLight==Yes)//����ͼ���е㣬����ֵ
	{
		Light_t.Last_Mid.x=Light_t.Mid.x;
		Light_t.Last_Mid.y=Light_t.Mid.y;
		Light_t.Mid.x=(Light_t.Left.x+Light_t.Right.x)/2;
		Light_t.Mid.y=(Light_t.Top.y+Light_t.Bottom.y)/2;
		lastMidLine=Light_t.Mid.y;
	}
	else 						//����ͼ���޵㣬�����ϴ�ֵ
	{
	  	lastMidLine=OV7725_EAGLE_H/2;
		Light_t.Mid.x=Light_t.Last_Mid.x;
		Light_t.Mid.y=Light_t.Last_Mid.y;
	}
        

}

/*
*	����:��ӥ������ͷ��ָ��ı䵽ָ����x,yֵ
*	����:�洢�ƶ���imageͼ��ָ��
*		nextLine���ƶ����ڼ���
*/

void ImaPointChange(uint8** image,uint16 nextLine)
{
	*image=imgbuff+(nextLine*40);
}

/*
*���ܣ�ɨ��һ�У����ض�Ӧ������ߺ����ұߵĵ�
*image:ͼ��ָ��,����ָ��ÿ���ʼ�У���ɨ��40��
*leftX��rightX���洢ͼ���������ҵ�
* @������ע   ��������޵�,leftX��rightX�ᱻ��0
*/
#define LEN 3 //һ�к���������ڵĺڵ�,���
void FindLineDot(uint8* image,uint16* leftX,uint16* rightX)
{
	uint8* image2=image;//Ϊ�˲��ı�ԭָ��
	uint8 i=0,j=0;//ѭ��
	uint8 temp=0,temp2=0;//�ݴ��ֽ�
	uint8 bit=0x80;//����ȡλֵ
	uint8 lastColor=Eagle_Black;//�洢�ϴ���ɫֵ
	uint16 length=0;//�ݴ�׵㳤��
	uint16 max_len=0;//���׵㳤��
		
	for(i=0;i<40;i++)
	{
	  	temp=*(image2+i);//��
		bit=0x80;

		if(temp==0xff&&lastColor==Eagle_Black)//ȫ�ڣ��޵�.�����ϴ�Ϊ�ڷ�ֹ����ȫ�ױ���ȫ��
		{
		  	lastColor=Eagle_Black;
			continue;//��������		
		}	
		for(j=0;j<8;j++)
		{
			if( (lastColor==Eagle_Black) && ((bit&temp)==Eagle_White) )//�ϸ���ɫΪ�ڣ�������ɫΪ��
			{
				if(j<=(8-LEN) && (( (bit>>1)&temp)==Eagle_White) && (( (bit>>2)&temp)==Eagle_White))//�ֽ�ʣ����ڵ���3��λ
				{
					*leftX=8*i+j;//��¼����ߵ�ֵ
				}
				else if(j==6&&i<39&& (( (bit>>1)&temp)==Eagle_White))//�ֽ�ʣ��2��λͬʱ���ǰ�,���Ҳ������һ���ֽ�
				{
					temp2=*(image2+i+1);
					if( (temp2&0x80)==Eagle_White)//��
					  *leftX=8*i+j;//��¼����ߵ�ֵ
				}
				else if(j==7&&i<39)	//�ֽ�ʣ��1��λ���ǰ�,���Ҳ������һ���ֽ�
				{
					temp2=*(image2+i+1);
					if( ((temp2&0x80)==Eagle_White) &&((temp2&0x40)==Eagle_White)  )//��ͷ�����ֽڶ��ǰ�
					  *leftX=8*i+j;//��¼����ߵ�ֵ
				}
			}
			
			if ( (lastColor==Eagle_White) && ((bit&temp)!=Eagle_White)&&max_len>=LEN)//�ϸ���ɫΪ�ף�������ɫΪ��,max_len>=LENʹ�����׵���ͼ���е����䲻�����á�����Ҳ����
			{
				if(j<=(8-LEN) && (( (bit>>1)&temp)!=Eagle_White) && (( (bit>>2)&temp)!=Eagle_White))//�ֽ�ʣ����ڵ���3��λ
				{
					*rightX=8*i+j;//��¼����ߵ�ֵ
				}
				else if(j==6&&i<39&& (( (bit>>1)&temp)!=Eagle_White))//�ֽ�ʣ��2��λͬʱ���Ǻ�,���Ҳ������һ���ֽ�
				{
					temp2=*(image2+i+1);
					if( (temp2&0x80)!=Eagle_White)//��
					  *rightX=8*i+j;//��¼����ߵ�ֵ
				}
				else if(j==7&&i<39)	//�ֽ�ʣ��1��λ���Ǻ�,���Ҳ������һ���ֽ�
				{
					temp2=*(image2+i+1);
					if( ((temp2&0x80)!=Eagle_White) &&((temp2&0x40)!=Eagle_White)  )//��ͷ�����ֽڶ��Ǻ�
					  *rightX=8*i+j;//��¼����ߵ�ֵ
				}
			}
			if( (bit&temp) ==Eagle_White )//�����ϴ���ɫֵ
			{
			  lastColor=Eagle_White;
			  length++;//�׵����
			}
			else
			{
			  length=0;
			  lastColor=Eagle_Black;
			}
			if(length>max_len)//�����׵�����
				max_len=length;
			bit>>=1;//����һλ
		}	
	}
	if(*rightX==0&&max_len>=LEN)//�˳����׵�ͬ�ұ�Ե�Ӵ�������Լ�ȫ�׵�������е�����о�
	{
		*rightX=OV7725_EAGLE_W-1;
	}
	if(max_len<LEN)
	{
		*leftX=0;
		*rightX=0;
	}
}









