/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2015,6zhaodong
*     All rights reserved.
*
*     ��ע�������⣬�����������ݰ�Ȩ������δ����������������ҵ��;��
*     �޸�����ʱ�����Ȩ������
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
uint8   a_In_imgbuff[CAMERA0_H][CAMERA0_W];                              //����洢����ͼ�������
volatile s_Img_Status_DE    s_OV7620_Img_Status={IMG_FINISH,0,0,0};
//uint8   *ov7620_img_buff0;
//uint8 a_In_imgbuff[CAMERA0_H][CAMERA0_W];                              //����洢����ͼ�������
//volatile s_Img_Status_DE    s_OV7620_Img_Status={IMG_FINISH,0,0,0};

//volatile IMG_STATUS_e      ov7620_img_flag = IMG_FINISH;   //ͼ��״̬


//�ڲ���������
static uint8 ov7620_reg_init();
static void ov7620_port_init();


/*!
*  @brief      ov7620��ʼ��
*  @since      v5.0
*/
uint8  ov7620_init(uint8 *imgaddr)
{
  ov7620_img_buff0 = imgaddr; 
  while (ov7620_reg_init() == 0);
  ov7620_port_init();
  set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //����DMA0���жϷ�����Ϊ DMA0_IRQHandler
  set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //����PORTA���жϷ�����Ϊ PORTA_IRQHandler
  return 0;
}

/*!
*  @brief      ov7620�ܽų�ʼ�����ڲ����ã�
*  @since      v5.0
*/
static void ov7620_port_init()
{ 
  
  port_init(VSYNC_PIN_0, ALT1 | IRQ_FALLING | PULLDOWN | PF);     //���жϣ��������Ͻ��ش����жϣ����˲�,�½���׼ȷһЩ  
  port_init(HREF_PIN_0, ALT1 | IRQ_RISING | PULLDOWN | PF);           //���ж�,����,�½��ش����ж�,���˲�,�½��غ�������Ч.
  
  port_init(PCLK_PIN_0, ALT1 | DMA_FALLING );         //PCLK,�ߵ�ƽ�������,�͵�ƽ��Ч,������
  
  dma_portx2buff_init(CAMERA0_DMA_CH, (void *)&PTB_B0_IN, (void *)(ov7620_img_buff0), PCLK_PIN_0, DMA_BYTE1, CAMERA0_W, DADDR_KEEPON);
}

/*!
*  @brief      ov7620���жϷ�����
*  @since      v5.0
*attention:		���жϷ����������ж�
*/
void ov7620_vsync_0(void)
{
  
  //���ж���Ҫ�ж��ǳ��������ǳ���ʼ
  // if (s_OV7620_Img_Status[0].Img_Status == IMG_START)                   //��Ҫ��ʼ�ɼ�ͼ��
  {
    //s_OV7620_Img_Status.Img_Status = IMG_GATHER;                  //���ͼ��ɼ���
    //s_OV7620_Img_Status.VSYNC_ENABLE=0;             //���γ��ж�
    //s_OV7620_Img_Status.HREF_ENABLE=1;              //�������ж�
    s_OV7620_Img_Status.Img_Row=0;                  //����ָ����λ
    //s_OV7620_Img_Status.NUM=0;
    DMA_DADDR(CAMERA0_DMA_CH) = (uint32_t)(ov7620_img_buff0);    //�ָ���ַ
    //disable_irq(PORTA_IRQn);
    //���ܽ�ֹPTA�ж�,Ҫ�õ����ж�    
  }
  
  /*
  else                                        //ͼ��ɼ�����
  {
  disable_irq(PORTA_IRQn);                        //�ر�PTA���ж�,�˴����Թر�,���ɼ�ͼ��
  s_OV7620_Img_Status.Img_Status = IMG_FAIL;                    //���ͼ��ɼ�ʧ��
}
  */
  
}

/*!
*  @brief      ov7620���жϷ�����
*  @since      v5.0
*				���жϷ���������DMA���ݴ���
*/
void ov7620_href_0(void)
{
  //s_OV7620_Img_Status.HREF_ENABLE=0;              //��ֹ���ж�
  //DMA_DADDR(CAMERA_DMA_CH) = (uint32_t)(ov7620_img_buff+CAMERA_W*s_OV7620_Img_Status.Img_Row);    //�ָ���ַ
  DMA_EN(CAMERA0_DMA_CH);                //ʹ��ͨ��CHn Ӳ������
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
  //���ͨ�������жϱ�־λ 
  //PORTA_ISFR = 1 << PT27;            //���PCLK��־λ,PTA27   
  //disable_irq(PORTA_IRQn);
  
}

/*!
*  @brief      ov7620 DMA�жϷ�����
*  @since      v5.0
*/
void ov7620_dma_0()
{
  
  //�ɼ���ɲ����ж�,����ָ������ָ
  s_OV7620_Img_Status.Img_Row++;
  DMA_DIS(CAMERA0_DMA_CH); 
  //  if(s_OV7620_Img_Status.NUM>1)
  //  {
  //    s_OV7620_Img_Status.HREF_ENABLE=0;
  //    s_OV7620_Img_Status.VSYNC_ENABLE=1;    
  //  }
  //  s_OV7620_Img_Status.NUM=0;
  //  
  //s_OV7620_Img_Status.HREF_ENABLE=1;              //�������ж�
  if((OV7620_0_H)==s_OV7620_Img_Status.Img_Row)
  {    
    s_OV7620_Img_Status.Img_Status = IMG_FINISH;
    DMA_IRQ_DIS(CAMERA0_DMA_CH);
  } 
  DMA_IRQ_CLEAN(CAMERA0_DMA_CH);           //���ͨ�������жϱ�־λ
}


/*!
*  @brief      ov7620�ɼ�ͼ�񣨲ɼ��������ݴ洢�� ��ʼ��ʱ���õĵ�ַ�ϣ�
*  @since      v5.0
*/
void ov7620_get_img_0()
{
  char table[50];
  s_OV7620_Img_Status.Img_Status = IMG_START;                   //��ʼ�ɼ�ͼ�� 
  s_OV7620_Img_Status.VSYNC_ENABLE=1;             //�����ж�
  s_OV7620_Img_Status.HREF_ENABLE=0;             //�������ж�
  DMA_IRQ_EN(CAMERA0_DMA_CH); 
  enable_irq(PORTA_IRQn);                         //����PTA���ж�  
  //ͼ��ɼ���ʼʱ��
  while (s_OV7620_Img_Status.Img_Status != IMG_FINISH)           //�ȴ�ͼ��ɼ����
  { 
//    s_OV7620_Img_Status.Img_Status = IMG_START;
  }
  disable_irq(PORTA_IRQn);                        //����PTA���ж�   
  Image_Deal();
  
//   vcan_sendimg(a_In_imgbuff, OV7620_0_SIZE); 
  vcan_sendimg(Image_Bin, CAMERA_Fit_SIZE);
//  uart_putchar (UART4,  Light_t.Mid.x);
//  uart_putchar (UART4,  0XFF);
//  uart_putchar (UART4,  Light_t.Mid.y);
     sprintf(table,"x:%d,y:%d\r\n",Light_t.Mid.x,Light_t.Mid.y);
  uart_putstr(UART5,table);
  
}

/*OV7620��ʼ�����ñ�*/
//F
reg_s ov7620_reg_0[] =
{	
  { OV7620_SYN  ,0x00},//��Ƶ�����
  { OV7620_COMC ,0x20},	//,QVGA,320*240,��д�ᵼ�ºܶ����
  { OV7620_COMH, 0x20},         //�������
  {OV7620_COMJ,0x40},
  {OV7620_COMM,0xa0}, 
  //{ OV7620_COMB ,0x01},
  //{ OV7620_SYN,  0x00},
  //{ OV7620_COMH, 0x00},         //�������
  { OV7620_HS, centerX - (CAMERA0_W) / 4 +3},
  { OV7620_HE, centerX + (CAMERA0_W) / 4 +3},
  { OV7620_VS, centerY - CAMERA0_H / 2 +1},
  { OV7620_VE, centerY + CAMERA0_H / 2 },
  { OV7620_VEG, 0x42}, //���
  //{ OV7620_EHSH, 0xE0},
  //{ OV7620_EHSL, 0xFF},
  
};

uint8 ov7620_cfgnum_0 = ARR_SIZE(ov7620_reg_0); /*�ṹ�������Ա��Ŀ*/

/*!
*  @brief      ov7620�Ĵ��� ��ʼ��
*  @return     ��ʼ�������0��ʾʧ�ܣ�1��ʾ�ɹ���
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
        printf("\n����: д�Ĵ���ʧ��");

        return 0;
    }
    DELAY_MS(5);
    for (i = 0; i < ov7620_cfgnum_0; i++)
    {
    if (0 == SCCB_WriteByte(ov7620_reg_0[i].addr, ov7620_reg_0[i].val,0))
    {
        printf("\n����: д�Ĵ���ʧ��");
        return 0;
    }
    }
    //DEBUG_PRINTF("\nOV7620 Register Config Success!");
    return 1;
}


