#include "common.h"
#include "include.h"
#include "include.h"
#define TimeTypeCountNum    3

uint32	CountTime[TimeTypeCountNum]={0};
void PIT0_IRQHandler()
{
    uint8 i=0;
    static uint16 cnt_1ms=0;//5ms周期
	static uint16 cnt_2ms=0;//5ms周期
   	static uint16 cnt_5ms=0;//5ms周期
	static uint16 cnt_10ms=0;//10ms周期
	static uint16 cnt_15ms=0;//15ms周期
	static uint16 cnt_20ms=0;//20ms周期
	static uint16 cnt_30ms=0;//20ms周期
	static uint16 cnt_xxms=0;//20ms周期
	static uint16 cnt_1000ms=0;//20ms周期

        
	if(++cnt_1ms>=1)
	{
		cnt_1ms=0;
		TimeCtrMoto();
		
		for(i=0;i<TimeTypeCountNum;i++)
		{
			CountTime[i]++;
			if(CountTime[i]>40000000)
					CountTime[i]=0;
		}
		


	}
	
	if(++cnt_2ms>=2)//2ms周期
	{     
	   cnt_2ms=0;//清标志    
	   TimeChangeFreq();//和上面的顺序不能改变
	}
	if(++cnt_5ms>=5)//5ms周期
	{     
	   cnt_5ms=0;//清标志      
	  
	}

	if(++cnt_10ms>=10)
	{
		cnt_10ms=0;	
		
	}

	if(++cnt_15ms>=15)
	{
	  sendDataToScope();
	  cnt_15ms=0;
	}

	if(++cnt_20ms>=20)
	{
	  cnt_20ms=0;
	  
	}
	
	if(++cnt_30ms>=30)
	{
		cnt_30ms=0;
		 
		
	}
	
	if(++cnt_xxms>=50)
	{
		cnt_xxms=0;
		TimeChangeServoPwm();
	}
	
	if(++cnt_1000ms>=1000)
	{
	  led_turn(LED0);
	 	 //CarMove(5000);
		 
		cnt_1000ms=0;
	}
	/**清标志位**/
	PIT_Flag_Clear(PIT0);
}



/*
*功能:开始对时间进行计数
*/
void StartCountTime(uint8 type)
{
	CountTime[type]=0;
}
/*
*	功能:获得计数的时间，必须先开始计数时间后才能使用，否则错误数据
*	返回值:返回秒数
*/
uint32_t GetCountMMTime(uint8 type)
{
	return (uint32)(CountTime[type]);
}
