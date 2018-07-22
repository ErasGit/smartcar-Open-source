#include "include.h"
float Middle_Err;
int   CCD_Diff[128],RisingEdgeIndex,FallingEdgeIndex;
uint8 RisingEdgeCnt,FallingEdgeCnt,RisingEdge[5],FallingEdge[5];

uint8 state_change=0;

uint8 Hill_Flag=0;

float Character_Distance=0;

static float Previous_Error[15]={0};

uint8 Confirm_Cnt=0;

float Last_Hill_Distance=0;


uint8 RoadType;
uint8 RoadTypeConfirm;             //��������ȷ��
uint8 Threshold;                 //��ֵ
 
uint8 RoadTypeSwitch=40;

void RoadType_Distinguish()
{
  Normal(); 
  Is_Hill();
  if(RoadType==5)
  {
    Hill();
  }
}
void Prejudge()   //Ԥ�к���
{
}
void Normal()
{
}
void Hill()
{
   if((Distance-Character_Distance)>1)   //�����µ����Ȳ�����3m
     /*            ����������·            */
   {
     Last_Hill_Distance=Distance;
     RoadType=0;
     Hill_Flag=1;
     Confirm_Cnt=0;
     RoadTypeConfirm=0;
   }
}

//�µ����
void Is_Hill()
{
  float Average_Error=0;
  uint8  i=0; 
  for(i=0;i<10;i++)//��¼10*5=50ms�ڵ�Mid_Error
  {
     Previous_Error[i]=Previous_Error[i+1];
     Average_Error=Average_Error+Previous_Error[i];
  }
  Average_Error=Average_Error/10.0;
  if(ABS(Middle_Err)>ABS(Average_Error)+10)
  {
    Confirm_Cnt++;
    Previous_Error[10]=0.8*Average_Error+0.2*Middle_Err;
  }
  else
  {
     Previous_Error[10]=Middle_Err;
  }
  
  if(Confirm_Cnt>=15)
  {
    RoadType=5;
    RoadTypeConfirm=1;
    Confirm_Cnt=0;
    Character_Distance=Distance;
  }
}
