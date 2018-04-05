#include "common.h"
#include "include.h"

_xy_St _xy_t;
Light_St Light_t;//坐标
Light_Flag_St Light_Flag_t;//状态标志

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

    Light_Flag_t.HaveTopLight=No;//灯在上半部分
    Light_Flag_t.HaveBottomLight=No;//灯在下半部分
//    Light_Flag_t.HaveLight=No;//视野中是否有灯  	
}
void Seach_Car_XY()
{
  	static uint16 lastMidLine=OV7725_EAGLE_H/2;//上次图像找到的行，从这附近扫描，将加速找点过程
	uint8* imagePoint=imgbuff;
	uint16 scanLine=0;//当前正在扫的行
	uint8 Gap=1;//隔行扫描点数
	uint16 CarLeft_X=0,CarRight_X=0;//本行左右x点
	uint16 Last_Left_X=0;//上一行左右x点
	YesNo_t Have_Dot_Line=No;//本行有点
	YesNo_t Have_Dot_Last_Line=No;//上一行有点
	uint8  TopNoLine=0;//最上方连续没点次数
	uint8  BottomNoLine=0;//最下方连续没点次数

	
	Para_Init();//参数初始化
	for(scanLine=lastMidLine;scanLine<OV7725_EAGLE_H-1;scanLine+=Gap)//从设定的位置开始向下找，默认上次图像mid行开始
	{
	  	ImaPointChange(&imagePoint,scanLine);//移动指针
	  	CarLeft_X=0;
		CarRight_X=0;
		FindLineDot(imagePoint,&CarLeft_X,&CarRight_X);//找点
		
		if(CarLeft_X==0&&CarRight_X==0)//判断有无点
			Have_Dot_Line=No;//本行无点
		else
		{
		  	
			Have_Dot_Line=Yes;//本行有点
			BottomNoLine=0;//清0
		}
	
		if(Have_Dot_Last_Line==Yes&&Have_Dot_Line==Yes&&ABS(CarLeft_X-Last_Left_X)<6)//判断点的有效性
		{
			Light_Flag_t.HaveBottomLight=Yes;//先从下面扫描
			if(scanLine<Light_t.Top.y)//找到上面最高点，数值越小越往上
			{
				Light_t.Top.y=scanLine;
			}
			if(scanLine>Light_t.Bottom.y)//找到下面最低点，越大越往上
			{
				Light_t.Bottom.y=scanLine;
			}
			if(CarLeft_X<Light_t.Left.x)//找到左最低点，越小越往左
			{
				Light_t.Left.x=CarLeft_X;
			}
			if(CarRight_X>Light_t.Right.x)
			{
				Light_t.Right.x=CarRight_X;
			}
		}
		Last_Left_X=CarLeft_X;//保存本次值
		Have_Dot_Last_Line=Have_Dot_Line;
		
		if(Have_Dot_Line==No&&Light_Flag_t.HaveBottomLight==Yes)//已经扫到过点，现在出现无点的行，怀疑可能扫完圆了
		{
		  BottomNoLine++;
		  if(BottomNoLine==4)//连续4行没点
		  {
			BottomNoLine=0;//确定超过圆
			break;//跳出循环
		  }
		}
	}
	for(scanLine=lastMidLine; scanLine>0 ;scanLine-=Gap)//从上次图像的y值开始扫描
	{
	  	ImaPointChange(&imagePoint,scanLine);//移动指针
		CarLeft_X=0;
		CarRight_X=0;
		FindLineDot(imagePoint,&CarLeft_X,&CarRight_X);//找点
		
		if(CarLeft_X==0&&CarRight_X==0)//判断有无点
			Have_Dot_Line=No;//本行无点
		else
		{
			Have_Dot_Line=Yes;//本行有点
			
			TopNoLine=0;//清0
		}
		
		 if(Have_Dot_Last_Line==Yes&&Have_Dot_Line==Yes&&ABS(CarLeft_X-Last_Left_X)<6)//判断点的有效性
		{
			Light_Flag_t.HaveTopLight=Yes;//上方有点
			if(scanLine<Light_t.Top.y)//找到上面最高点，数值越小越往上
			{
				Light_t.Top.y=scanLine;
			}
			if(scanLine>Light_t.Bottom.y)//找到下面最低点，越大越往上
			{
				Light_t.Bottom.y=scanLine;
			}
			if(CarLeft_X<Light_t.Left.x)//找到左最低点，越小越往左
			{
				Light_t.Left.x=CarLeft_X;
			}
			if(CarRight_X>Light_t.Right.x)
			{
				Light_t.Right.x=CarRight_X;
			}
		}
		Last_Left_X=CarLeft_X;//本次值保存
		Have_Dot_Last_Line=Have_Dot_Line;//本行有点保存
		
		if(Have_Dot_Line==No&&Light_Flag_t.HaveTopLight==Yes)//已经扫到过点，现在出现无点的行，怀疑可能扫完圆了
		{
			TopNoLine++;
			if(TopNoLine==4)//连续4行没点
			{
			  TopNoLine=0;
			  break;//跳出循环
			}
		}
	}
	
	if(Light_Flag_t.HaveBottomLight==No&&Light_Flag_t.HaveTopLight==No)//上下都没点
	{
            Light_Flag_t.HaveLight=No;
            Light_Flag_t.HaveLightProtect=Yes;
        }
	else 
	{
            Light_Flag_t.HaveLight=Yes;	
        }
	
	if(Light_Flag_t.HaveLight==Yes)//本次图像有点，更新值
	{
		Light_t.Last_Mid.x=Light_t.Mid.x;
		Light_t.Last_Mid.y=Light_t.Mid.y;
		Light_t.Mid.x=(Light_t.Left.x+Light_t.Right.x)/2;
		Light_t.Mid.y=(Light_t.Top.y+Light_t.Bottom.y)/2;
		lastMidLine=Light_t.Mid.y;
	}
	else 						//本次图像无点，保存上次值
	{
	  	lastMidLine=OV7725_EAGLE_H/2;
		Light_t.Mid.x=Light_t.Last_Mid.x;
		Light_t.Mid.y=Light_t.Last_Mid.y;
	}
        

}

/*
*	功能:将鹰眼摄像头的指针改变到指定的x,y值
*	输入:存储移动后image图像指针
*		nextLine：移动到第几行
*/

void ImaPointChange(uint8** image,uint16 nextLine)
{
	*image=imgbuff+(nextLine*40);
}

/*
*功能：扫描一行，返回对应的最左边和最右边的点
*image:图像指针,必须指针每行最开始列，将扫描40列
*leftX，rightX：存储图像最左最右点
* @函数备注   如果本行无点,leftX和rightX会被置0
*/
#define LEN 3 //一行黑线起码存在的黑点,别改
void FindLineDot(uint8* image,uint16* leftX,uint16* rightX)
{
	uint8* image2=image;//为了不改变原指针
	uint8 i=0,j=0;//循环
	uint8 temp=0,temp2=0;//暂存字节
	uint8 bit=0x80;//辅助取位值
	uint8 lastColor=Eagle_Black;//存储上次颜色值
	uint16 length=0;//暂存白点长度
	uint16 max_len=0;//最大白点长度
		
	for(i=0;i<40;i++)
	{
	  	temp=*(image2+i);//别动
		bit=0x80;

		if(temp==0xff&&lastColor==Eagle_Black)//全黑，无灯.检验上次为黑防止上行全白本行全黑
		{
		  	lastColor=Eagle_Black;
			continue;//跳出本次		
		}	
		for(j=0;j<8;j++)
		{
			if( (lastColor==Eagle_Black) && ((bit&temp)==Eagle_White) )//上个颜色为黑，本次颜色为白
			{
				if(j<=(8-LEN) && (( (bit>>1)&temp)==Eagle_White) && (( (bit>>2)&temp)==Eagle_White))//字节剩余大于等于3个位
				{
					*leftX=8*i+j;//记录最左边的值
				}
				else if(j==6&&i<39&& (( (bit>>1)&temp)==Eagle_White))//字节剩余2个位同时都是白,，且不是最后一个字节
				{
					temp2=*(image2+i+1);
					if( (temp2&0x80)==Eagle_White)//白
					  *leftX=8*i+j;//记录最左边的值
				}
				else if(j==7&&i<39)	//字节剩余1个位且是白,，且不是最后一个字节
				{
					temp2=*(image2+i+1);
					if( ((temp2&0x80)==Eagle_White) &&((temp2&0x40)==Eagle_White)  )//开头两个字节都是白
					  *leftX=8*i+j;//记录最左边的值
				}
			}
			
			if ( (lastColor==Eagle_White) && ((bit&temp)!=Eagle_White)&&max_len>=LEN)//上个颜色为白，本次颜色为黑,max_len>=LEN使单个白点在图像中的跳变不起作用。但是也不行
			{
				if(j<=(8-LEN) && (( (bit>>1)&temp)!=Eagle_White) && (( (bit>>2)&temp)!=Eagle_White))//字节剩余大于等于3个位
				{
					*rightX=8*i+j;//记录最左边的值
				}
				else if(j==6&&i<39&& (( (bit>>1)&temp)!=Eagle_White))//字节剩余2个位同时都是黑,，且不是最后一个字节
				{
					temp2=*(image2+i+1);
					if( (temp2&0x80)!=Eagle_White)//黑
					  *rightX=8*i+j;//记录最左边的值
				}
				else if(j==7&&i<39)	//字节剩余1个位且是黑,，且不是最后一个字节
				{
					temp2=*(image2+i+1);
					if( ((temp2&0x80)!=Eagle_White) &&((temp2&0x40)!=Eagle_White)  )//开头两个字节都是黑
					  *rightX=8*i+j;//记录最左边的值
				}
			}
			if( (bit&temp) ==Eagle_White )//更新上次颜色值
			{
			  lastColor=Eagle_White;
			  length++;//白点个数
			}
			else
			{
			  length=0;
			  lastColor=Eagle_Black;
			}
			if(length>max_len)//连续白点数量
				max_len=length;
			bit>>=1;//右移一位
		}	
	}
	if(*rightX==0&&max_len>=LEN)//滤除掉白点同右边缘接触的情况以及全白的情况，有点问题感觉
	{
		*rightX=OV7725_EAGLE_W-1;
	}
	if(max_len<LEN)
	{
		*leftX=0;
		*rightX=0;
	}
}









