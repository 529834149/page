/**
 * --------------����"��������K60�ײ��"�Ĺ��̣�Smart_Car��-----------------
 * @file Smart_Car.c
 * @version 0.0
 * @date 2015-2-7
 * @brief ���ڸù��̵�����
 *
 * ��Ȩ����:�������칤ҵѧԺ ��ʮ���˼����  ����ͷ2�� 
 * Ӳ��ƽ̨:  MK60FX512
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
   DisableInterrupts;                  //�ر�ȫ���ж�
   Init();
   EnableInterrupts;                   //ʹ��ȫ���ж�  
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
        GUI_Menu();                         //���ܺ��в�ˢ��
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
        LPLD_GPIO_ClearIntFlag(PORTB);      //������ͷ���жϱ�־
        enable_irq(PORTB_IRQn);             //ʹ������ͷ���ж�                 
     }
  }
}

