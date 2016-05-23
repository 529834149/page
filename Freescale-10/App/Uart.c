/**
 * --------------����"��������K60�ײ��"�Ĺ��̣�Smart_Car��-----------------
 * @file Uart.c
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
/*-----------------------------------------------------------------------
* CRC_Check: ���ݼ���
*
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
static uint16 CRC_Check(uint8 *buf, uint8 CRC_CNT)
{
    uint16 CRC_temp;
    uint8 i,j;
    CRC_temp = 0xffff;
    for (i=0;i<CRC_CNT; i++){      
        CRC_temp ^= buf[i];
        for (j=0;j<8;j++) {
            if (CRC_temp & 0x01)
                CRC_temp = (CRC_temp >>1 ) ^ 0xa001;
            else
                CRC_temp = CRC_temp >> 1;
        }
    }
    return(CRC_temp);
}
/*-----------------------------------------------------------------------
* OutPut_Data: ��������
*
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
void OutPut_Data()
{
  uint8 databuf[10] = {0};
  uint8 i;
  uint16 CRC16 = 0;
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (uint8)(Send_Data[i]%256);              //Send_Data
    databuf[i*2+1] = (uint8)(Send_Data[i]/256);
  }
  CRC16 = CRC_Check(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  for(i=0;i<10;i++)
     LPLD_UART_PutChar(UART0,databuf[i]);                     //����0
}
