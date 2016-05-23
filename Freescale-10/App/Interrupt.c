/**
 * --------------基于"拉普兰德K60底层库"的工程（Smart_Car）-----------------
 * @file Interrupt.c
 * @version 0.0
 * @date 2015-2-7
 * @brief 关于该工程的描述
 *
 * 版权所有:北华航天工业学院 第十届飞思卡尔  摄像头2队 
 * 硬件平台:  MK60FX512
 * 
 */
#include "common.h"
#include "global.h"

int8 Flag_VS = 0;  

void Stop_isr()
{
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin3))
  {
      Speed = 0;
      Stop_Flag = 1;
  }
}
/*-----------------------------------------------------------------------
* VS_isr: 摄像头场中断
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void VS_isr()
{
  if(LPLD_GPIO_IsPinxExt(PORTB, GPIO_Pin9))
  {
     //检测到场开始信号，加载目的地址
     LPLD_DMA_LoadDstAddr(DMA_CH0, Image); 
     //清行中断标志，防止进入无效行中断
     LPLD_GPIO_ClearIntFlag(PORTA);
     enable_irq(PORTA_IRQn);
  }
}
/*-----------------------------------------------------------------------
* HS_isr: 摄像头行中断
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void HS_isr()
{
  if(LPLD_GPIO_IsPinxExt(PORTA, GPIO_Pin26))
  {       
     //检测到行开始信号，使能DMA请求
     if(Flag_VS < V)
     {  
       LPLD_DMA_EnableReq(DMA_CH0);        
     }
     //行数采集已满，关闭中断
    else
    {
      //关GPIO中断 
      disable_irq(PORTA_IRQn);
      disable_irq(PORTB_IRQn);
      Photo_Done_Flag = 1;                    //图像采集完成
      Flag_VS = 0;     
    } 
    Flag_VS ++;
  }
}