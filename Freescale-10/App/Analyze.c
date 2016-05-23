/**
 * --------------基于"拉普兰德K60底层库"的工程（Smart_Car）-----------------
 * @file Analyze.c
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

uint8 r_line_flag = 0,l_line_flag = 0;
uint8 cross_flag = 0;
/*-----------------------------------------------------------------------
* int16 ABS:两数差值的绝对值
*
* 输入：x,y
*
* 输出：差值的绝对值
-----------------------------------------------------------------------*/
int16 ABS(int16 x, int16 y)
{
  if((x-y)>0)
    return (x-y);
  else
    return (y-x);
}
/*-----------------------------------------------------------------------
* Find_Line:提线
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Find_Line()
{
  int16 l_line = Start_Row,l_line_old = Start_Row,r_line_old=End_Row,r_line=End_Row,center_line = 90,center_line_old=90;
  int16 track_width = 110,track_width_old = 110;                              //赛道宽度
  int16 i,j,Cur_Flag;
  uint8 lose_line_flag=0;
  Left_Flag = 0;
  Right_Flag = 0;
  Str_Flag = 0;
  for(i=Start_Column;i<End_Column;i+=1)                                      //隔行扫描，共扫描40行
  {
    r_line = End_Row;
    l_line = Start_Row;
    r_line_flag=0;
    l_line_flag=0;
    for(j=center_line;j<=End_Row;j++)
    {        
       if((Image[i][j]==0x00) && (Image[i][j-1]!=0x00) && (Image[i][j-2]!=0x00))
       {   
          r_line_flag = 1;                          //搜索到边线置1
          r_line = j;             
          break;
       }
       //边线限幅和未搜到线的情况
       if((r_line_old != 0) && (ABS(r_line,r_line_old)>10) && (r_line_flag == 1))   
          r_line = r_line_old;
    } 
    for(j=center_line;j>=Start_Row;j--)
    {
       if((Image[i][j+2]!=0x00) && (Image[i][j+1]!=0x00) && (Image[i][j]==0x00))
       {
          l_line_flag = 1;                          //搜索到边线置1
          l_line = j;         
          break;
       } 
       //边线限幅和未搜到线的情况
       if((l_line_old != 0) && (ABS(l_line,l_line_old)>10) && (l_line_flag == 1))
          l_line = l_line_old;
    }
    //赛道宽度限幅
    if((r_line-l_line)>140)
    {
      r_line = r_line_old;
      l_line = l_line_old;
    }  
    if((r_line_flag == 0) && (l_line_flag == 1))
    {
      if(i>2)
      {
         if(((r_line-l_line)<(Line_Array[i-1][2]- Line_Array[i-1][0])) && ((Line_Array[i-1][2]- Line_Array[i-1][0])<(Line_Array[i-2][2]- Line_Array[i-2][0])) && (Line_Array[i-1][2] == End_Row))
           lose_line_flag = 1;                              //右边丢线
      } 
      center_line = l_line + track_width_old/2;
    //track_width = track_width_old;
    }
    else if((r_line_flag == 1) && (l_line_flag == 0))
    {
      if(i>2) 
      {
         if(((r_line-l_line)<(Line_Array[i-1][2]- Line_Array[i-1][0])) && ((Line_Array[i-1][2]- Line_Array[i-1][0])<(Line_Array[i-2][2]- Line_Array[i-2][0])) && (Line_Array[i-1][0] == Start_Row))
           lose_line_flag = 2;                              //左边丢线
      }
      center_line = r_line - track_width_old/2;
    //track_width = track_width_old;
    }
    else if(r_line_flag == 1 && l_line_flag == 1) 
    {
      center_line = (l_line+r_line) >>1;                 //计算中线
      track_width = r_line-l_line;                       //记录赛道宽度
    }
    if((lose_line_flag == 1 && (r_line_flag == 1 || center_line >160)) || (lose_line_flag == 2 && (l_line_flag == 1 || center_line <27)))
      break;    
    //赛道类型识别
    if((center_line-center_line_old) > 1)
      Left_Flag++;
    else if((center_line-center_line_old) < -1)
      Right_Flag++;
    else
      Str_Flag++;
    //十字识别--特点：赛道宽度先由近到远变化规律，先变小-变小-后变大-变大/折点识别
 
    //标记左右边线，TFT显示右边线
    Image[i][r_line] = 0x11;                         
    Image[i][l_line] = 0x11;                        
    r_line_old = r_line;
    l_line_old = l_line; 
    center_line_old = center_line;
    track_width_old = track_width;
    Image[i][center_line] = 0x12;                        //标记中线，TFT显示中线    
    //存储本行左、中、右线
    Line_Array[i][0] = l_line;
    Line_Array[i][1] = center_line;
    Line_Array[i][2] = r_line;    
  }
  Cur_Flag = Left_Flag + Right_Flag;
  Active_Lines = Cur_Flag + Str_Flag;
  Str_Cur_Offset = Str_Flag - Cur_Flag;
}
/*-----------------------------------------------------------------------
* Get_Center_Line:求取中线
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Get_Center_Line()
{
  int32 sum = 0;
  int8 number = 0;
  for(uint8 i=0;i<Active_Lines; )
  {
    sum += Line_Array[i][1];
    number ++;
    if(Active_Lines<=25)
    {
      i+=4;
    }
    else if(Active_Lines<=55 && Active_Lines>25)
    {
      if(i<20)  i+=4;
      else      i+=2;
    }
    else if(Active_Lines>55)
    {
      if(i<20)                           i+=4;
      else if(i<45 && i>=20)            i+=2;
      else if(i>=45 && i<Active_Lines)  i+=2;    
    } 
  }
  Center_Line = sum/number;        //中线
//Send_Data[0] = Center_Line;
}
/*-----------------------------------------------------------------------
* Recognition_Type：赛道类型识别
*
* 输入：无
*
* 输出：无
-----------------------------------------------------------------------*/
void Recognition_Type()
{
  if(Active_Lines>70 && Str_Cur_Offset>50)
  {
    Track_Type = 1;
   // BUZZ_ON;
  }
  else 
  {
//    Track_Type = 0;
   // BUZZ_OFF; 
  }
}