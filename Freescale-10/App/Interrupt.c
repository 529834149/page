/**
 * --------------����"��������K60�ײ��"�Ĺ��̣�Smart_Car��-----------------
 * @file Interrupt.c
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
* VS_isr: ����ͷ���ж�
*
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
void VS_isr()
{
  if(LPLD_GPIO_IsPinxExt(PORTB, GPIO_Pin9))
  {
     //��⵽����ʼ�źţ�����Ŀ�ĵ�ַ
     LPLD_DMA_LoadDstAddr(DMA_CH0, Image); 
     //�����жϱ�־����ֹ������Ч���ж�
     LPLD_GPIO_ClearIntFlag(PORTA);
     enable_irq(PORTA_IRQn);
  }
}
/*-----------------------------------------------------------------------
* HS_isr: ����ͷ���ж�
*
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
void HS_isr()
{
  if(LPLD_GPIO_IsPinxExt(PORTA, GPIO_Pin26))
  {       
     //��⵽�п�ʼ�źţ�ʹ��DMA����
     if(Flag_VS < V)
     {  
       LPLD_DMA_EnableReq(DMA_CH0);        
     }
     //�����ɼ��������ر��ж�
    else
    {
      //��GPIO�ж� 
      disable_irq(PORTA_IRQn);
      disable_irq(PORTB_IRQn);
      Photo_Done_Flag = 1;                    //ͼ��ɼ����
      Flag_VS = 0;     
    } 
    Flag_VS ++;
  }
}