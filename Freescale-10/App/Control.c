/**
 * --------------����"��������K60�ײ��"�Ĺ��̣�Smart_Car��-----------------
 * @file Control.c
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

int16 D_Error = 0;
int16 D_Last_Error = 0;

int16 S_L_Error = 0;
int16 S_L_DError = 0;
int16 S_L_DDError = 0;
int16 S_L_Pre_Error = 0;
int16 S_L_Pre_DError = 0;

int16 S_R_Error = 0;
int16 S_R_DError = 0;
int16 S_R_DDError = 0;
int16 S_R_Pre_Error = 0;
int16 S_R_Pre_DError = 0;

int8  S_PWM_Dead = 5;                                           

int16 S_PWM_Max = 7000;
int16 S_PWM_Min = -7000;

uint8 Turn_Flag = 1;                                                //���ת��

float32 P1 = 0.009569,P2 = 2.962,P3 = 1000.0,Speed_Ratio = 1.0;     //ǰ������ϵ���Ŵ���1000��
/*-----------------------------------------------------------------------
* Get_Speed: ��ȡ��ǰ�ٶ�
*
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
void Get_Speed()
{
  Get_L_Speed = -(LPLD_FTM_GetCounter(FTM1));         //��ȡ�����ٶ�(�������)
  Get_R_Speed = LPLD_FTM_GetCounter(FTM2);           //��ȡ�����ٶ�
  LPLD_FTM_ClearCounter(FTM1);                       //��ռ�����
  LPLD_FTM_ClearCounter(FTM2); 
}  
/*-----------------------------------------------------------------------
* Set_Speed: �趨��ǰ�ٶ�
*
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
void Set_Speed()
{
 //�����������趨�ٶ�
  if(Turn_Flag == 1)                                 //��ת
  {
     Set_R_Speed = Speed;
     Set_L_Speed = (int32)(Set_R_Speed * (1.0/Speed_Ratio));
  }
  else                                              //��ת
  {
     Set_L_Speed = Speed;
     Set_R_Speed = (int32)(Set_L_Speed * (1.0/Speed_Ratio));
  }
}
/*-----------------------------------------------------------------------
* Speed_L_Control: �����ٶȿ���
*
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
static void Speed_L_Control()
{
  int32 pwm_p,pwm_i,pwm_d,s_pwm;
  
  S_L_Error = Set_L_Speed - Get_L_Speed;                            //e(k)
  S_L_DError = S_L_Error - S_L_Pre_Error;                         //e(k)-e(k-1)
  S_L_DDError = S_L_DError - S_L_Pre_DError;                      //e(k)-e(k-1)-(e(k-1)-e(k-2))
  S_L_Pre_Error = S_L_Error;                                       //Ų������
  S_L_Pre_DError = S_L_DError;  
 
  if(S_L_Error > 40)           S_L_PWM = 4000;         //BangBang
  else if(S_L_Error < -30)    S_L_PWM = -3000;      //BangBang
  else                                                //PID              
  {
    if((S_L_Error < S_PWM_Dead) && (S_L_Error > -S_PWM_Dead))     //���õ�������
    {
      s_pwm = 0;              
    }
    else
    {
      pwm_p = S_L_Kp * S_L_DError;   //������
      pwm_i = S_L_Ki * S_L_Error;    //������
      pwm_d = S_L_Kd * S_L_DDError;  //΢����
      s_pwm = (int32)((pwm_p + pwm_i + pwm_d)/10.0);
    }
    S_L_PWM += s_pwm;
    
    if(S_L_PWM >= S_PWM_Max)            S_L_PWM = S_PWM_Max;         //�޷�
    else if(S_L_PWM <= S_PWM_Min)      S_L_PWM = S_PWM_Min;
   }
}
/*-----------------------------------------------------------------------
* Speed_R_Control: �����ٶȿ���
*
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
static void Speed_R_Control()
{
  int32 pwm_p,pwm_i,pwm_d,s_pwm;
  
  S_R_Error = Set_R_Speed - Get_R_Speed;                            //e(k)
  S_R_DError = S_R_Error - S_R_Pre_Error;                          //e(k)-e(k-1)
  S_R_DDError = S_R_DError - S_R_Pre_DError;                      //e(k)-e(k-1)-(e(k-1)-e(k-2))
  S_R_Pre_Error = S_R_Error;                                      //Ų������
  S_R_Pre_DError = S_R_DError;  
  
  if(S_R_Error > 40)           S_R_PWM = 4000;      //BangBang
  else if(S_R_Error < -30)    S_R_PWM = -3000;     //BangBang
  else                                             //PID 
  {
    if((S_R_Error < S_PWM_Dead) && (S_R_Error > -S_PWM_Dead))     //���õ�������
    {
      s_pwm = 0;              
    }
    else
    {
      pwm_p = S_R_Kp * S_R_DError;   //������
      pwm_i = S_R_Ki * S_R_Error;    //������
      pwm_d = S_R_Kd * S_R_DDError;  //΢����
      s_pwm = (int32)((pwm_p + pwm_i + pwm_d)/10.0);
    }
    S_R_PWM += s_pwm;
    
    if(S_R_PWM >= S_PWM_Max)            S_R_PWM = S_PWM_Max;      //�޷�
    else if(S_R_PWM <= S_PWM_Min)      S_R_PWM = S_PWM_Min;
   }
}
/*-----------------------------------------------------------------------
* Speed_PWM_Update: �ٶ�PWMֵ����
*
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
static void Speed_PWM_Update()
{
  if(S_L_PWM < 0)
  {
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch2,0);
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch1,(-S_L_PWM));   
  }
  else
  {
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch1,0);
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch2,S_L_PWM);
  } 
  if(S_R_PWM < 0)
  {
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch4,0);
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch3,(-S_R_PWM));   
  }
  else
  {
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch3,0);
    LPLD_FTM_PWM_ChangeDuty(FTM3,FTM_Ch4,S_R_PWM);
  }
}
/*-----------------------------------------------------------------------
* Speed_Control: �ٶȿ���
*
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
void Speed_Control()
{
  Get_Speed();
  Set_Speed();
  Speed_L_Control();
  Speed_R_Control();
  Speed_PWM_Update();
}
/*-----------------------------------------------------------------------
* Direction_Control: λ��ʽPID-�������
*
*                           �������
*
*                       ��      ��     ��
*                      1650   1850    2050
* ���룺��
*
* �������
-----------------------------------------------------------------------*/
void Direction_Control()
{
  //λ��ʽPID
  int32 pwm_p,pwm_d,d_pwm;
  float32 p1,p2;
  D_Last_Error = D_Error;
  D_Error = Center_Line - 90;
  D_Kp = (int16)((D_Error*D_Error*10.0)/D_Kp_a+D_Kp_b/10.0);
  pwm_p = D_Kp * D_Error;                                            //���������
  pwm_d = D_Kd * (D_Error - D_Last_Error);
  d_pwm = (int32)((pwm_p + pwm_d)/10.0);                            //ǰ������Ŵ���10�����ʴ˴���Ҫ��С10��                     
  //�޷�
  if(d_pwm < -200)
    d_pwm = -200;
  else if(d_pwm > 200)
    d_pwm = 200;
  
  D_PWM =d_pwm + 1850; 
  
  if(d_pwm < 0)                            //��ת
  {
    Turn_Flag = 0;
    d_pwm = -d_pwm;
  }
  else                                    //��ת
  {
    Turn_Flag = 1;
  }
  //����ϵ������
  p1 = P1*d_pwm*d_pwm;
  p2 = P2*d_pwm;
  Speed_Ratio=(p1 + p2 + P3)/1000.0;    
  
  LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch7, D_PWM);     //��������
}

