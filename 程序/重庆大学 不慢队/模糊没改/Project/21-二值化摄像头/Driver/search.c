#include "include.h"
uint8  LMR[3][60];  //�ֱ��Ӧ����ߣ����ߣ��ұ��ߡ�
uint8  RoadType=0;
float Previous_Error[12];
void Push_And_Pull(float *buff,int len,float newdata)
{
 int i;
 for(i=len-1;i>0;i--)
 {
   *(buff+i)=*(buff+i-1);
 }
   *buff=newdata; 
}
void Search()
{
  //�ӵײ���������
  int i,j,cnt=0;
  float miderror=0;
  int white_begin=0; //��ɫ����ʼ�ĺ�����0
  int white_end=79;   //��ɫ��������ĺ�����79
//  int old_white_begin=0; //��ɫ����ʼ�ĺ�����0
//  int old_white_end=0;   //��ɫ��������ĺ�����79
//  uint8 buxianc_count=0;
  uint8 *start;
  for(i=0;i<60;i++)  //�������
  {
    
          LMR[0][i]=0;
          LMR[1][i]=0;
          LMR[2][i]=0;
  }
  for(i=59;i>0;i--) //�ӵ�59�п�ʼ����
  {
    start=&(img[i*80]);//�еĳ�ʼ��ַ
    white_begin=0;
    white_end=79; 
//    if(i!=59&&((LMR[0][i+1]!=0)||(LMR[2][i+1]!=0))) //��һ������һ���ѵ�������
//    {
//      if(LMR[0][i+1]!=0)
//     {  
//       for(j=LMR[0][i+1]-3;j<(LMR[0][i+1]+3);j++)   //��Ϊ�ϴ��ѵ������ˣ�����һ�еı��ߴ���һ�б��ߵĸ�����
//       {
//        if(j<0) break;
//            if((*(start+j)!=0)&&(*(start+j-1)==0)) //�ɺڱ��
//            { 
//              LMR[0][i]=j; //�ҵ�һ����������Ϊ�ѵ�����
//      
//              if(LMR[2][i+1]==0) 
//              {
//                for(j=j+10;j<79;j++)
//                {
//                     if((*(start+j)==0)&&(*(start+j-1)!=0)) //�ɰױ��
//                    { 
//                      LMR[2][i]=j; //�ҵ�һ����������Ϊ�ܵ�����
//                      break; 
//                    }
//                 }
//              }
//              break; 
//            }
//        }
//      } 
//      if(LMR[2][i+1]!=0)
//      {
//        for(j=LMR[2][i+1]-3;j<(LMR[2][i+1]+3);j++)   //��Ϊ�ϴ��ѵ������ˣ�����һ�еı��ߴ���һ�б��ߵĸ�����
//       {
//          if(j>79) break;
//              if((*(start+j)==0)&&(*(start+j-1)!=0)) //�ɰױ��
//            { 
//              LMR[2][i]=j; //�ҵ�һ����������Ϊ�ܵ�����
//               if(LMR[0][i+1]==0) 
//              {
//                for(j=j-10;j>0;j--)
//                {
//                      if((*(start+j)!=0)&&(*(start+j-1)==0)) //�ɺڱ��
//                    { 
//                       LMR[0][i]=j; //�ҵ�һ����������Ϊ�ܵ�����
//                      break; 
//                    }
//                 }
//              }   
//              break; 
//            }
//        }
//       }
//    }
//    else 
      if(i>1)           //��һ��û��ͬʱ�ѵ������Ҳ��Ƕ������У������������׳����ŵ���
    {
        for(j=40;j>1;j--)  //���м�����������
        {
       
            if((*(start+j)!=0)&&(*(start+j-1)==0)&&(*(start+j-2)==0)&&(white_begin==0))                  white_begin=j;  //�ɺڱ��    
            if((*(start+80-j+1)==0)&&(*(start+80-j)==0)&&(*(start+80-j-1)!=0)&&(white_end==79))            white_end=80-j;    //�ɰױ��
            
            if((white_end!=79)&&(white_begin!=0)&&(white_begin<white_end)&&(white_end-white_begin>10))
            {
              LMR[0][i]=white_begin;  //�����
              LMR[2][i]=white_end;    //�ұ���
//              old_white_begin=white_begin;
//              old_white_end=white_end;
              break;
            }
//            buxianc_count++;
//            if(j<5&&(*(start+j)!=0)&&(*(start+80-j-1)!=0)&&(white_begin==0)&&(white_end==79))
//            {
//              LMR[0][i]=old_white_begin;  //�����
//              LMR[2][i]=old_white_end;    //�ұ���
//            }
        }
    }
  
    if(white_begin!=0&&white_end==79)
    {
         LMR[0][i]=white_begin;
    }
    if(white_begin==0&&white_end!=79)
    {
         LMR[2][i]=white_end;
    }
    if((LMR[2][i]!=0)&&(LMR[0][i]!=0))
     LMR[1][i]=(LMR[2][i]+LMR[0][i])/2;  //������
    else
    {
       if(LMR[2][i]!=0)
       LMR[1][i]=(LMR[2][i])/2;  //������
        if(LMR[0][i]!=0)
       LMR[1][i]=(LMR[0][i]+80)/2;  //������
    }
    
//    if(i>30&&i<50&&LMR[1][i]!=0)
//    {
//      cnt++;
//      miderror+=(LMR[1][i]-46);
//    }
//     buxianc_count=0;
  }
  for(i=31;i>30&&i<50&&LMR[1][i]!=0;i++)
  {
    cnt++;
    miderror+=(LMR[1][i]-46);
  }
  if(cnt!=0)  Middle_Err=miderror*1.0/cnt;
//  Middle_Err=Middle_Err_Filter(Middle_Err);
  Push_And_Pull(Previous_Error,10,Middle_Err);
  
}
