/*
 * File: fuzzy_Speed.c
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 10-May-2017 16:16:47
 */

/* Include Files */
#include "fuzzy.h"

#define E_Max 160
#define E_Min 0
#define D_Max 100
#define D_Min 0

/***********************************************************************************************************
* Name          double fuzzy_Speed(double dir_Error, double distance)
* Input_param   dir_Error：方向误差   distance：距离
* Output_param  速度
* Brief         由模糊算法耦合方向误差与距离得到速度
* Version       v1.0
* Author        2017/5/10, by ahou(郑康华)
* Problem      
*************************************************************************************************************/

double speed_Out;
	
int8_t  PE,PD;
int16_t eRule[7]     = {0 ,10 ,25 ,30 ,50  ,110 ,160};   /* 方向误差的模糊论域 */
int16_t dRule[7]     =   {35 ,40 ,45 ,55,65,80 ,100};   /* 距离的模糊论域 */
//int16_t dRule[7]     = {0 ,35 ,45 ,55 ,70,80 ,100};   /* 距离的模糊论域 */
double  eFuzzy[2]    = {0.0 ,0.0};                        /* 隶属于方向误差的隶属程度 */
double  dFuzzy[2]    = {0.0 ,0.0};                        /* 隶属于距离的隶属程度 */
int16_t speedRule[4] = {0.0 ,0.0 ,0.0 ,0.0};              /* 隶属于速度的模糊子集 */   
double  speedFuzzy[4]= {0.0 ,0.0 ,0.0 ,0.0};              /* 隶属于速度的隶属程度 */
	
int16_t speed_DF[7]    = {0,10,20,30,40,50,60};           /* 速度解模糊论域 */
double fuzzy_Temp,rule_Temp;


//		        /*  distance  6   5   4   3   2   1   0   */
int8_t speed_Rules[49] ={6 , 6 , 5 , 3 , 2 , 1 , 0 , //0
                         6 , 5 , 5 , 3 , 2 , 1 , 0 , //1  
                         5 , 4 , 3 , 2 , 1 , 1 , 0 , //2      
                         3 , 3 , 2 , 2 , 1 , 1 , 0 , //3 
                         2 , 2 , 1 , 1 , 1 , 0 , 0 , //4 
                         1 , 1 , 1 , 1 , 0 , 0 , 0 , //5
                         1 , 0 , 0 , 0 , 0 , 0 , 0 };//6   dir_err

double fuzzy_Speed(double dir_Error, double distance)
{

	//限幅
	if (dir_Error>E_Max)
			dir_Error=E_Max;
	if (dir_Error<E_Min)
			dir_Error=E_Min;

	if (distance>D_Max)
			distance=D_Max;
	if (distance<D_Min)
			distance=D_Min;
	distance=D_Max-distance;   //方向转换以匹配数组方向

  /* %方向误差隶属函数描述% */
  if(dir_Error < eRule[0])
	{
	  PE = 0;
    eFuzzy[0] = 1;
	}
  else if (dir_Error < eRule[1]) 
	{
    PE = 0;
    eFuzzy[0] = (eRule[1] - dir_Error) / (eRule[1]-eRule[0]);
  }
	else if (dir_Error < eRule[2]) 
	{
    PE = 1;
    eFuzzy[0] = (eRule[2] - dir_Error) / (eRule[2]-eRule[1]);
  }
	else if (dir_Error < eRule[3]) 
	{
    PE = 2;
    eFuzzy[0] = (eRule[3] - dir_Error) / (eRule[3]-eRule[2]);
  } 
	else if (dir_Error < eRule[4]) 
	{
    PE = 3;
    eFuzzy[0] = (eRule[4] - dir_Error) / (eRule[4]-eRule[3]);
  } 
	else if (dir_Error < eRule[5]) 
	{
    PE = 4;
    eFuzzy[0] = (eRule[5] - dir_Error) / (eRule[5]-eRule[4]);
  } 
	else if (dir_Error < eRule[6]) 
	{
    PE = 5;
    eFuzzy[0] = (eRule[6] - dir_Error) / (eRule[6]-eRule[5]);
  } 
	else 
	{
    PE = 5;
    eFuzzy[0] = 0.0;
  }
  eFuzzy[1] = 1-eFuzzy[0];

  /* %距离隶属函数描述%% */
  if(distance<dRule[0])
	{
	  PD = 0;
    dFuzzy[0] = 1;
	}
  else if (distance < dRule[1]) 
	{
    PD = 0;
    dFuzzy[0] = (dRule[1] - distance) / (dRule[1]-dRule[0]);
  } 
	else if (distance < dRule[2]) 
	{
    PD = 1;
    dFuzzy[0] = (dRule[2] - distance) / (dRule[2]-dRule[1]);
  } 
	else if (distance < dRule[3]) 
	{
    PD = 2;
    dFuzzy[0] = (dRule[3] - distance) / (dRule[3]-dRule[2]);
  } 
	else if (distance < dRule[4]) 
	{
    PD = 3;
    dFuzzy[0] = (dRule[4] - distance) / (dRule[4]-dRule[3]);
  } 
	else if (distance < dRule[5]) 
	{
    PD = 4;
    dFuzzy[0] = (dRule[5] - distance) / (dRule[5]-dRule[4]);
  } 
	else if (distance < dRule[6]) 
	{
    PD = 5;
    dFuzzy[0] = (dRule[6] - distance) / (dRule[6]-dRule[5]);
  }
	else 
	{
    PD = 5;
    dFuzzy[0] = 0.0;
  }
  dFuzzy[1] = 1-dFuzzy[0];

		/* %查询模糊规则表%% */
	speedRule[0]=speed_Rules[PE*7+PD];
	if (eFuzzy[0]<dFuzzy[0])
			speedFuzzy[0]=eFuzzy[0];
	else
			speedFuzzy[0]=dFuzzy[0];

	speedRule[1]=speed_Rules[(PE+1)*7+PD];
	if (eFuzzy[1]<dFuzzy[0])
			speedFuzzy[1]=eFuzzy[1];
	else
			speedFuzzy[1]=dFuzzy[0];

	speedRule[2]=speed_Rules[PE*7+PD+1];
	if (eFuzzy[0]<dFuzzy[1])
			speedFuzzy[2]=eFuzzy[0];
	else
			speedFuzzy[2]=dFuzzy[1];

	speedRule[3]=speed_Rules[(PE+1)*7+PD+1];
	if (eFuzzy[1]<dFuzzy[1])
			speedFuzzy[3]=eFuzzy[1];
	else
			speedFuzzy[3]=dFuzzy[1];


  /* %重心法解模糊% */
  speedRule[0]=speed_DF[speedRule[0]];
  speedRule[1]=speed_DF[speedRule[1]];
  speedRule[2]=speed_DF[speedRule[2]];
  speedRule[3]=speed_DF[speedRule[3]];

	rule_Temp  = speedFuzzy[0]*speedRule[0] + speedFuzzy[1]*speedRule[1] + speedFuzzy[2]*speedRule[2] + speedFuzzy[3]*speedRule[3];
	fuzzy_Temp = speedFuzzy[0]+speedFuzzy[1]+speedFuzzy[2]+speedFuzzy[3];
	speed_Out = rule_Temp/fuzzy_Temp;

  return speed_Out;
}

