/**
 * --------------����"��������K60�ײ��"�Ĺ��̣�Smart_Car��-----------------
 * @file Gui.c
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
#include "TFT.h"

uint8 Key_Value = 0;                    //����ֵ
uint8 Page_Value = 1;                   //��ʾҳ
uint8 Which_Number_2 = 0;              //�ڶ�ҳ��Ҫ������Ŀ��
uint8 Which_Number_3 = 0;              //����ҳ��Ҫ������Ŀ��

uint8 Ok_Time = 0;                     //��ȷ��������

uint8 Which_Row = 0;                   //ͼ���е�ָ����

void GUI_Init()
{
  TFT_Init();
  TFT_ClearScreen(COLOR_White);                                  //����
}
/*
 * ��ʾ��һҳ
 * 
 * ͼ�������������Ϣ��ʾ
 */
static void GUI_ShowPage1()
{
  TFT_DrawU8Gray(0,0,H-28-1,End_Column-1,Image);                       //��ʾ�ɼ���ͼ��   
  
  TFT_FillBackground(0,80,159,128,COLOR_White);                        //����ָ������

  TFT_ShowString(158,108,"L_Ln",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(130,108,Line_Array[Which_Row][0],COLOR_Red,COLOR_White);
  TFT_ShowString(100,108,"C_Ln",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(75,108,Line_Array[Which_Row][1],COLOR_Red,COLOR_White);
  TFT_ShowString(45,108,"R_Ln",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(20,108,Line_Array[Which_Row][2],COLOR_Red,COLOR_White);
  TFT_ShowString(45,108,"R_Ln",COLOR_Magenta,COLOR_White);
  
  TFT_ShowString(158,98,"W_L",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(130,98,Which_Row,COLOR_Red,COLOR_White);
  
  TFT_ShowString(80,98,"S_C_O",COLOR_Blue,COLOR_White);
  TFT_ShowNum(40,98,Str_Cur_Offset,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(158,88,"L_F",COLOR_Blue,COLOR_White);
  TFT_ShowNum(130,88,Left_Flag,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(100,88,"R_F",COLOR_Blue,COLOR_White);
  TFT_ShowNum(75,88,Right_Flag,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(45,88,"S_F",COLOR_Blue,COLOR_White);
  TFT_ShowNum(20,88,Str_Flag,COLOR_Black,COLOR_White);
  
  TFT_DrawLine(0,Which_Row,160,Which_Row,COLOR_Yellow);
  
  TFT_ShowString(158,118,"C_L",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(130,118,Center_Line,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(100,118,"A_L",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(75,118,Active_Lines,COLOR_Red,COLOR_White);
  
  TFT_ShowString(45,118,"T_Type",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(10,118,Track_Type,COLOR_Red,COLOR_White); 
}
/*
 * ��ʾ�ڶ�ҳ
 * 
 * ת����ƵĲ����趨��״̬���
 */
static void GUI_ShowPage2()
{
  TFT_ClearScreen(COLOR_White); 
  
  TFT_ShowString(100,115,"Direction",COLOR_Magenta,COLOR_White);

  TFT_ShowString(150,100,"Kp_a",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(125,100,D_Kp_a,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(95,100,"Kp_b",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(70,100,D_Kp_b,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(35,100,"Kd",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(20,100,D_Kd,COLOR_Red,COLOR_White); 
  
  //״̬������ʾ
  TFT_ShowString(150,45,"Kp",COLOR_Blue,COLOR_White);
  TFT_ShowNum(125,45,D_Kp,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(95,45,"PWM",COLOR_Blue,COLOR_White);
  TFT_ShowNum(65,45,D_PWM,COLOR_Black,COLOR_White);   
  
  //�˵�ָʾ
  switch(Which_Number_2)
  {
  case 1:
    TFT_ShowChar(155,100,'*',COLOR_Black,COLOR_White);
    break;
  case 2:
    TFT_ShowChar(100,100,'*',COLOR_Black,COLOR_White);
    break; 
  case 3:
    TFT_ShowChar(35,100,'*',COLOR_Black,COLOR_White);   
    break;
  default:break;
  }
}
/*
 * ��ʾ����ҳ
 * 
 * �ٶȿ��ƵĲ����趨��״̬���
 */
static void GUI_ShowPage3()
{
  TFT_ClearScreen(COLOR_White); 

  TFT_ShowString(90,115,"Speed",COLOR_Magenta,COLOR_White);
  
  TFT_ShowString(150,100,"L_Kp",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(125,100,S_L_Kp,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(95,100,"L_Ki",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(70,100,S_L_Ki,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(40,100,"L_Kd",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(15,100,S_L_Kd,COLOR_Red,COLOR_White);
  
  TFT_ShowString(150,90,"R_Kp",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(125,90,S_R_Kp,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(95,90,"R_Ki",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(70,90,S_R_Ki,COLOR_Red,COLOR_White); 
  
  TFT_ShowString(40,90,"R_Kd",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(15,90,S_R_Kd,COLOR_Red,COLOR_White);
  
  TFT_ShowString(150,80,"Start_Straight_S",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(60,80,Start_Straight_Speed,COLOR_Red,COLOR_White);
    
  TFT_ShowString(150,70,"Straight_S",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(80,70,Straight_Speed,COLOR_Red,COLOR_White);
  
  TFT_ShowString(150,60,"Curve_H_S",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(100,60,Curve_High_Speed,COLOR_Red,COLOR_White);
  
  TFT_ShowString(75,60,"Curve_L_S",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(25,60,Curve_Low_Speed,COLOR_Red,COLOR_White);
  
  TFT_ShowString(150,50,"S_a",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(125,50,S_a,COLOR_Red,COLOR_White);
  
  TFT_ShowString(75,50,"S_b",COLOR_Magenta,COLOR_White);
  TFT_ShowNum(50,50,S_b,COLOR_Red,COLOR_White);
  
  //״̬������ʾ
  TFT_ShowString(150,30,"Speed",COLOR_Blue,COLOR_White);
  TFT_ShowNum(120,30,Speed,COLOR_Black,COLOR_White);
 
  TFT_ShowString(100,30,"G_L_S",COLOR_Blue,COLOR_White);
  TFT_ShowNum(70,30,Get_L_Speed,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(50,30,"G_R_S",COLOR_Blue,COLOR_White);
  TFT_ShowNum(20,30,Get_R_Speed,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(150,20,"S_L_S",COLOR_Blue,COLOR_White);
  TFT_ShowNum(120,20,Set_L_Speed,COLOR_Black,COLOR_White); 
  
  TFT_ShowString(100,20,"S_R_S",COLOR_Blue,COLOR_White);
  TFT_ShowNum(70,20,Set_R_Speed,COLOR_Black,COLOR_White); 
  
  //�˵�ָʾ
  switch(Which_Number_3)
  {
  case 1:
    TFT_ShowChar(155,100,'*',COLOR_Black,COLOR_White);
    break;
  case 2:
    TFT_ShowChar(100,100,'*',COLOR_Black,COLOR_White);
    break; 
  case 3:
    TFT_ShowChar(45,100,'*',COLOR_Black,COLOR_White);   
    break;
  case 4:
    TFT_ShowChar(155,90,'*',COLOR_Black,COLOR_White);    
    break;
  case 5:
    TFT_ShowChar(100,90,'*',COLOR_Black,COLOR_White);   
    break;
  case 6:
    TFT_ShowChar(45,90,'*',COLOR_Black,COLOR_White);
    break;
  case 7:
    TFT_ShowChar(155,80,'*',COLOR_Black,COLOR_White);
    break;
  case 8:
    TFT_ShowChar(155,70,'*',COLOR_Black,COLOR_White);
    break; 
  case 9:
    TFT_ShowChar(155,60,'*',COLOR_Black,COLOR_White);
    break; 
  case 10:
    TFT_ShowChar(80,60,'*',COLOR_Black,COLOR_White);
    break; 
  case 11:
    TFT_ShowChar(155,50,'*',COLOR_Black,COLOR_White);
    break;
  case 12:
    TFT_ShowChar(80,50,'*',COLOR_Black,COLOR_White);
    break;
  default:break;
  }
}

/*
 * �˵�
 * 
 * �趨����
 */
void GUI_Menu()
{  
   if(PTE28_I == 0)                      //��ҳ��ʾ
   {
     Page_Value ++;
     if(Page_Value > 3)
       Page_Value = 1;
   }
   if(Page_Value == 1)                     //��ʾ��1ҳ
   {
     if(PTA13_I == 0)                     //ʵʱ��ʾͼ����ָ���е���Ϣ
     {
       if(Which_Row < End_Column)
          Which_Row += 1;
     }
     else if(PTA5_I == 0)
     {
       if(Which_Row > Start_Column)
          Which_Row -= 1;
     }
     GUI_ShowPage1();   
   }
  else if(Page_Value == 2)              //��ʾ��2ҳ��ת�������Ϣ
  {
    if(PTA12_I == 0)
    {
      Ok_Time++;
      if(Ok_Time > 1)
        Ok_Time = 0;
    }
    if(PTA7_I == 0  && Ok_Time == 0)    //�� ��
    {
      Which_Number_2++;
      if(Which_Number_2 > 3)
        Which_Number_2 = 1;
    }
    if(PTA13_I == 0 && Ok_Time == 1)   //�� 
    {
      switch(Which_Number_2)
      {
        case 1:
          D_Kp_a += 1;
          break;
        case 2:
          D_Kp_b += 1;
          break;
        case 3:
          D_Kd += 1;
          break;  
        default:break;
      }
    }
    if(PTA5_I == 0 && Ok_Time == 1)   //��
    {
      switch(Which_Number_2)
      {
         case 1:
          D_Kp_a -= 1;
          break;
        case 2:
          D_Kp_b -= 1;
          break;
        case 3:
          D_Kd -= 1;
          break;
        default:break;
      }
    }
  GUI_ShowPage2();
  }
  else if(Page_Value == 3)
  {
    if(PTA12_I == 0)
    {
      Ok_Time++;
      if(Ok_Time > 1)
        Ok_Time = 0;
    }
    if(PTA7_I == 0  && Ok_Time == 0)    //�� ��
    {
      Which_Number_3++;
      if(Which_Number_3 > 12)
        Which_Number_3 = 1;
    }
    if(PTA13_I == 0 && Ok_Time == 1)   //�� 
    {
      switch(Which_Number_3)
      {
        case 1:
          S_L_Kp += 1;
          break;
        case 2:
          S_L_Ki += 1;
          break;
        case 3:
          S_L_Kd += 1;
          break;  
        case 4:
          S_R_Kp += 1;
          break;
        case 5:
          S_R_Ki += 1;
          break;
        case 6:
          S_R_Kd += 1;
          break; 
        case 7:
          Start_Straight_Speed += 1;
          break;
        case 8:
          Straight_Speed += 1;
          break;
        case 9:
          Curve_High_Speed += 1;
          break;  
        case 10:
          Curve_Low_Speed += 1;
          break;
        case 11:
          S_a += 1;
          break;
        case 12:
          S_b += 1;
          break;     
        default:break;
      }
    }
    if(PTA5_I == 0 && Ok_Time == 1)   //��
    {
      switch(Which_Number_3)
      {
         case 1:
          S_L_Kp -= 1;
          break;
        case 2:
          S_L_Ki -= 1;
          break;
        case 3:
          S_L_Kd -= 1;
          break;  
        case 4:
          S_R_Kp -= 1;
          break;
        case 5:
          S_R_Ki -= 1;
          break;
        case 6:
          S_R_Kd -= 1;
          break; 
        case 7:
          Start_Straight_Speed -= 1;
          break;
        case 8:
          Straight_Speed -= 1;
          break;
        case 9:
          Curve_High_Speed -= 1;
          break;  
        case 10:
          Curve_Low_Speed -= 1;
          break;
        case 11:
          S_a -= 1;
          break;
        case 12:
          S_b -= 1;
          break;     
        default:break;
      }
    }
    GUI_ShowPage3();
  }
}