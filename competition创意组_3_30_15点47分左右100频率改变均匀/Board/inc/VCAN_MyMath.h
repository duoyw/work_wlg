#ifndef __VCAN_MyMath_H
#define __VCAN_MyMath_H

#include "common.h"
/**
*@Name LIMIT
*@function    min<=x<=max
*/
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )

/**
*@Name ABS
*@function    X or -X ¾ø¶ÔÖµ
*/
//#define ABS(x) ( (x)>0?(x):-(x) )

float DataFilter(float* pDataBuf, uint16 Num);
void Data_Quick_Sort(float* pDatas, uint16 Num);
#endif
