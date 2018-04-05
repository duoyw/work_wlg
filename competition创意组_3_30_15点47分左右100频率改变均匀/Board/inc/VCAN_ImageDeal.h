#ifndef _VCAN_IMAGEDEAL_H_
#define _VCAN_IMAGEDEAL_H_

#include "VCAN_OV7725.h"

#define Dot_White 255
#define Dot_Black 0

#define Eagle_White	0
#define Eagle_Black	1

typedef struct
{
	uint16 x;
	uint16 y;
}_xy_St;
extern _xy_St _xy_t;

typedef struct
{
	_xy_St Top;
	_xy_St Bottom;
	_xy_St Left;
	_xy_St Right;
	_xy_St Mid;
	_xy_St Lost;	
	_xy_St Last_Mid;
}Light_St;
extern Light_St Light_t;

typedef struct
{
	YesNo_t	HaveLight;
	YesNo_t	HaveTopLight;
	YesNo_t	HaveBottomLight;
	YesNo_t	HaveLightProtect;
}Light_Flag_St;

extern Light_Flag_St Light_Flag_t;
void edge_Track_Line(uint8 x,uint8 HEIGHT,uint8* Line_Begin,uint8* Line_End);
void scan_One_Line(uint8 LINE,uint8* X_Begin,uint8* X_End);
uint16 Sque_Dif(uint16 x1,uint16 y1,uint16 x2,uint16 y2);
void Para_Init(void);
void Seach_Car_XY(void);
void Image_Deal(void);

//new
void FindLineDot( uint8* image,uint16* leftX,uint16* rightX);
void ImaPointChange(uint8** image,uint16 nextLine);

extern uint8 Image_Bin[CAMERA_Fit_H][CAMERA_Fit_W];

#endif


