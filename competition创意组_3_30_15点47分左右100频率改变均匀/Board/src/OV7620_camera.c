/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2015,6zhaodong
*     All rights reserved.
*
*     除注明出处外，以下所有内容版权所属，未经允许，不得用于商业用途，
*     修改内容时必须版权声明。
*
* @file       OV7620_camera.c
* @brief      
* @author     6zhaodong
* @version    v1.0
* @date       2015/10/30
*/

#include "common.h"
#include "include.h"
#include "VCAN_computer.h"
#include "MK60_gpio.h"
#include "MK60_port.h"
#include "MK60_dma.h"
#include "VCAN_camera.h"
#include "MK60_it.h"
//#include "av_ob.h"
#include "OV7620_camera.h"




#define OV7620_Delay_ms(time)  DELAY_MS(time)

#define  C_OFFSET          8
uint8   *ov7620_img_buff0;
uint8   a_In_imgbuff[CAMERA0_H][CAMERA0_W];                              //定义存储接收图像的数组
volatile s_Img_Status_DE    s_OV7620_Img_Status={IMG_FINISH,0,0,0};
//uint8   *ov7620_img_buff0;
//uint8 a_In_imgbuff[CAMERA0_H][CAMERA0_W];                              //定义存储接收图像的数组
//volatile s_Img_Status_DE    s_OV7620_Img_Status={IMG_FINISH,0,0,0};

//volatile IMG_STATUS_e      ov7620_img_flag = IMG_FINISH;   //图像状态


//内部函数声明
static uint8 ov7620_reg_init();
static void ov7620_port_init();


/*!
*  @brief      ov7620初始化
*  @since      v5.0
*/
uint8  ov7620_init(uint8 *imgaddr)
{
  ov7620_img_buff0 = imgaddr; 
  while (ov7620_reg_init() == 0);
  ov7620_port_init();
  set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置DMA0的中断服务函数为 DMA0_IRQHandler
  set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置PORTA的中断服务函数为 PORTA_IRQHandler
  return 0;
}

/*!
*  @brief      ov7620管脚初始化（内部调用）
*  @since      v5.0
*/
static void ov7620_port_init()
{ 
  
  port_init(VSYNC_PIN_0, ALT1 | IRQ_FALLING | PULLDOWN | PF);     //场中断，上拉，上降沿触发中断，带滤波,下降沿准确一些  
  port_init(HREF_PIN_0, ALT1 | IRQ_RISING | PULLDOWN | PF);           //行中断,上拉,下降沿触发中断,带滤波,下降沿后数据无效.
  
  port_init(PCLK_PIN_0, ALT1 | DMA_FALLING );         //PCLK,高电平输出像素,低电平无效,上升沿
  
  dma_portx2buff_init(CAMERA0_DMA_CH, (void *)&PTB_B0_IN, (void *)(ov7620_img_buff0), PCLK_PIN_0, DMA_BYTE1, CAMERA0_W, DADDR_KEEPON);
}

/*!
*  @brief      ov7620场中断服务函数
*  @since      v5.0
*attention:		场中断服务函数打开行中断
*/
void ov7620_vsync_0(void)
{
  
  //场中断需要判断是场结束还是场开始
  // if (s_OV7620_Img_Status[0].Img_Status == IMG_START)                   //需要开始采集图像
  {
    //s_OV7620_Img_Status.Img_Status = IMG_GATHER;                  //标记图像采集中
    //s_OV7620_Img_Status.VSYNC_ENABLE=0;             //屏蔽场中断
    //s_OV7620_Img_Status.HREF_ENABLE=1;              //允许行中断
    s_OV7620_Img_Status.Img_Row=0;                  //行数指针置位
    //s_OV7620_Img_Status.NUM=0;
    DMA_DADDR(CAMERA0_DMA_CH) = (uint32_t)(ov7620_img_buff0);    //恢复地址
    //disable_irq(PORTA_IRQn);
    //不能禁止PTA中断,要用到行中断    
  }
  
  /*
  else                                        //图像采集错误
  {
  disable_irq(PORTA_IRQn);                        //关闭PTA的中断,此处可以关闭,不采集图像
  s_OV7620_Img_Status.Img_Status = IMG_FAIL;                    //标记图像采集失败
}
  */
  
}

/*!
*  @brief      ov7620行中断服务函数
*  @since      v5.0
*				行中断服务函数开启DMA数据传输
*/
void ov7620_href_0(void)
{
  //s_OV7620_Img_Status.HREF_ENABLE=0;              //禁止行中断
  //DMA_DADDR(CAMERA_DMA_CH) = (uint32_t)(ov7620_img_buff+CAMERA_W*s_OV7620_Img_Status.Img_Row);    //恢复地址
  DMA_EN(CAMERA0_DMA_CH);                //使能通道CHn 硬件请求
  //s_OV7620_Img_Status.NUM++;
  //  if(s_Overall_Status.Beacon_Cam_CNT>0)
  {
    if(0==s_OV7620_Img_Status.Img_Row)
    {
//      Seg_Start();
    }
    else;
//      Beacon_Scan_Row(s_OV7620_Img_Status.Img_Row-1);
  }
  //  if(0==s_OV7620_Img_Status[0].Img_Row)
  //  {
  //    Seg_Start();
  //  }
  //  else
  //    OB_Scan_Row(s_OV7620_Img_Status[0].Img_Row-1);
  
  // DMA_IRQ_EN(CAMERA_DMA_CH);
  //DMA_IRQ_CLEAN(CAMERA_DMA_CH);  
  //清除通道传输中断标志位 
  //PORTA_ISFR = 1 << PT27;            //清空PCLK标志位,PTA27   
  //disable_irq(PORTA_IRQn);
  
}

/*!
*  @brief      ov7620 DMA中断服务函数
*  @since      v5.0
*/
void ov7620_dma_0()
{
  
  //采集完成产生中断,行数指针向下指
  s_OV7620_Img_Status.Img_Row++;
  DMA_DIS(CAMERA0_DMA_CH); 
  //  if(s_OV7620_Img_Status.NUM>1)
  //  {
  //    s_OV7620_Img_Status.HREF_ENABLE=0;
  //    s_OV7620_Img_Status.VSYNC_ENABLE=1;    
  //  }
  //  s_OV7620_Img_Status.NUM=0;
  //  
  //s_OV7620_Img_Status.HREF_ENABLE=1;              //允许行中断
  if((OV7620_0_H)==s_OV7620_Img_Status.Img_Row)
  {    
    s_OV7620_Img_Status.Img_Status = IMG_FINISH;
    DMA_IRQ_DIS(CAMERA0_DMA_CH);
  } 
  DMA_IRQ_CLEAN(CAMERA0_DMA_CH);           //清除通道传输中断标志位
}


/*!
*  @brief      ov7620采集图像（采集到的数据存储在 初始化时配置的地址上）
*  @since      v5.0
*/
void ov7620_get_img_0()
{
  char table[50];
  s_OV7620_Img_Status.Img_Status = IMG_START;                   //开始采集图像 
  s_OV7620_Img_Status.VSYNC_ENABLE=1;             //允许场中断
  s_OV7620_Img_Status.HREF_ENABLE=0;             //屏蔽行中断
  DMA_IRQ_EN(CAMERA0_DMA_CH); 
  enable_irq(PORTA_IRQn);                         //允许PTA的中断  
  //图像采集起始时刻
  while (s_OV7620_Img_Status.Img_Status != IMG_FINISH)           //等待图像采集完毕
  { 
//    s_OV7620_Img_Status.Img_Status = IMG_START;
  }
  disable_irq(PORTA_IRQn);                        //允许PTA的中断   
  Image_Deal();
  
//   vcan_sendimg(a_In_imgbuff, OV7620_0_SIZE); 
  vcan_sendimg(Image_Bin, CAMERA_Fit_SIZE);
//  uart_putchar (UART4,  Light_t.Mid.x);
//  uart_putchar (UART4,  0XFF);
//  uart_putchar (UART4,  Light_t.Mid.y);
     sprintf(table,"x:%d,y:%d\r\n",Light_t.Mid.x,Light_t.Mid.y);
  uart_putstr(UART5,table);
  
}

/*OV7620初始化配置表*/
//F
reg_s ov7620_reg_0[] =
{	
  { OV7620_SYN  ,0x00},//分频，后加
  { OV7620_COMC ,0x20},	//,QVGA,320*240,不写会导致很多噪点
  { OV7620_COMH, 0x20},         //逐行输出
  {OV7620_COMJ,0x40},
  {OV7620_COMM,0xa0}, 
  //{ OV7620_COMB ,0x01},
  //{ OV7620_SYN,  0x00},
  //{ OV7620_COMH, 0x00},         //隔行输出
  { OV7620_HS, centerX - (CAMERA0_W) / 4 +3},
  { OV7620_HE, centerX + (CAMERA0_W) / 4 +3},
  { OV7620_VS, centerY - CAMERA0_H / 2 +1},
  { OV7620_VE, centerY + CAMERA0_H / 2 },
  { OV7620_VEG, 0x42}, //后加
  //{ OV7620_EHSH, 0xE0},
  //{ OV7620_EHSL, 0xFF},
  
};

uint8 ov7620_cfgnum_0 = ARR_SIZE(ov7620_reg_0); /*结构体数组成员数目*/

/*!
*  @brief      ov7620寄存器 初始化
*  @return     初始化结果（0表示失败，1表示成功）
*  @since      v5.0
*/
uint8 ov7620_reg_init()
{
    uint16 i = 0;
    SCCB_GPIO_init();
    DELAY_MS(10);
    SC_SDA_H(0);
    SC_SCL_H(0);
    if (0 == SCCB_WriteByte(OV7620_COMA, 0x80,0))
    {
        printf("\n警告: 写寄存器失败");

        return 0;
    }
    DELAY_MS(5);
    for (i = 0; i < ov7620_cfgnum_0; i++)
    {
    if (0 == SCCB_WriteByte(ov7620_reg_0[i].addr, ov7620_reg_0[i].val,0))
    {
        printf("\n警告: 写寄存器失败");
        return 0;
    }
    }
    //DEBUG_PRINTF("\nOV7620 Register Config Success!");
    return 1;
}


