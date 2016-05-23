/**
 * --------------基于"拉普兰德K60底层库"的工程（Smart_Car）-----------------
 * @file Smart_Car.c
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
#include "control.h"
#include "interrupt.h"
#include "init.h"
#include "gui.h"
#include "analyze.h"  
#include "uart.h"

uint8 Start_Flag = 1;

void main (void)
{
   DisableInterrupts;                  //关闭全局中断
   Init();
   EnableInterrupts;                   //使能全局中断  
   while(1)
  { 
     if(PTA6_I == 0)
     {
        LPLD_GPIO_ClearIntFlag(PORTC);
        Start_Flag = 0;    
        Stop_Flag = 0;
     }
     if(Start_Flag == 1)
     {
        GUI_Menu();                         //起跑后中不刷屏
     }
     if(Photo_Done_Flag == 1)
     {  
        Photo_Done_Flag = 0;
        LPLD_GPIO_Output_b(PTE,24,0);  
        Find_Line();
        Get_Center_Line();
        Recognition_Type();
        Direction_Control();   
        if(Start_Flag == 0)
        {
           Speed_Control();
        }
        Send_Data[0] = Get_L_Speed;
        Send_Data[1] = Set_L_Speed;
        Send_Data[2] = Get_R_Speed;
        Send_Data[3] = Set_R_Speed;
        OutPut_Data();
        LPLD_GPIO_Output_b(PTE,24,1);
        LPLD_GPIO_ClearIntFlag(PORTB);      //清摄像头场中断标志
        enable_irq(PORTB_IRQn);             //使能摄像头场中断                 
     }
  }
}

