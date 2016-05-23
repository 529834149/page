/**
 * --------------����"��������K60�ײ��"�Ĺ��̣�Smart_Car��-----------------
 * @file Global.c
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

uint8 Image[V][H];                 //ͼ��
                        
uint8 Photo_Done_Flag = 0;
uint8 Stop_Flag = 1;

int16 D_Kp_a = 60;
int16 D_Kp_b = 150;
int16 D_Kp = 80;                     //�Ŵ���10��
int16 D_Kd = 30;                     //�Ŵ���10��
int32 D_PWM = 1850;                  //���PWM����ֵ

int16 S_L_Kp = 430;                    //�Ŵ�10��
int16 S_L_Ki = 55;                     //�Ŵ�10��
int16 S_L_Kd = 0;                     //�Ŵ�10��

int16 S_R_Kp = 430;                    //�Ŵ�10��
int16 S_R_Ki = 55;                     //�Ŵ�10��
int16 S_R_Kd = 0;                     //�Ŵ�10��

int32 S_L_PWM = 0;                    //��������PWMֵ
int32 S_R_PWM = 0;                    //�ҵ������PWMֵ

int16 Get_L_Speed = 0;                //����ʵ���ٶ�
int16 Get_R_Speed = 0;                //����ʵ���ٶ�

int16 Speed = 130;

int16 Start_Straight_Speed;            //���ܵ�ֱ���ٶ�
int16 Straight_Speed;                  //ֱ���ٶ�

int16 Curve_High_Speed;                //�������ٶ�
int16 Curve_Low_Speed;                 //�������ٶ�

int16 S_a;                             //����ٶȶ��ι�ϵ�Ķ�����
int16 S_b;                             //����ٶȶ��ι�ϵ�ĳ�����

int16 Set_L_Speed = 0;                 //�����ٶ�
int16 Set_R_Speed = 0; 

uint16 Send_Data[4] = {0};                                 //��������

int16 Line_Array[End_Column-Start_Column][3];              //�洢���б��ߺ�����-->��ʽ��Line_Array[][0]--�����     Line_Array[][1]--����   Line_Array[][2]--�ұ���
int16 Center_Line = 94;                                    //���� 

int16 Active_Lines = 0;                                    //��Ч����
int16 Str_Cur_Offset = 0;                                  //ֱ��������ı���
int8 Left_Flag = 0; 
int8 Right_Flag = 0;
int8 Str_Flag = 0;
uint8 Track_Type = 0;                                      //��������
