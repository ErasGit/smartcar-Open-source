#include "include.h"
 float Voltage;
//�Ƕ������

int16  Acc_Z,Acc_X,Gyro_X,Gyro_Y;
int16  Acc_Y;
int16  Acc_Y_old=0;
int16  Acc_Max=2048;
int16  Acc_Min=-2048;
//int16  Acc_Offset=-3000;
int16  Acc_Offset=0;
float Angle,Angle_Speed,Car_Angle=0;

int16  Gyro_X_Offset=0,Gyro_Y_Offset=0;

//�ٶ������
float SpeedControlOutNew;
float SpeedControlOutOld;
float SpeedControlIntegral=0,Hill_Slow_Ratio;
uint8  Set_Angle;   //����ǰ��Ƕ�
int   SpeedCount;
int   Speed_Filter_Times=10;    //�ٶ�ƽ�����
float CarSpeed=0,ControlSpeed=0,AverageSpeed,SetSpeed=0,Distance;
//���������
float DirectionControlOutNew;
float DirectionControlOutOld;
float Turn_Speed=0;
int   DirectionCount;
float Delt_error;
float Turn_Out;
float Turn_Angle_Integral;
//ģ����ϵ��
float  Delta_P;
float  Delta_D;
float  Fuzzy_Kp;
float  Fuzzy_Kd;
//PID���������
PID PID_ANGLE,PID_SPEED,PID_TURN;

float  LeftMotorOut,RightMotorOut;   //��������
float  L_DeadValue=0,R_DeadValue=0;
float  RunTime=0;                    //�ܶ�ʱ�����
uint8 Protect=1;
uint8 ForceStop=0;
uint8 Encoder_Disable=0;

void Get_Attitude()
{
  Acc_Z = Get_Z_Acc()>>2;
  Gyro_X= Get_X_Gyro()>>2;
  Gyro_Y= Get_Y_Gyro();
}

//******Kalman�˲�
//-------------------------------------------------------
static  float Q_angle=0.01, Q_gyro=0.001, R_angle=1, dt=0.005;
//Q���󣬶�̬��Ӧ����
static float Pk[2][2] = { {1, 0}, {0, 1 }};
	
static float Pdot[4] ={0,0,0,0};

static float q_bias=0, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
//-------------------------------------------------------
void Kalman_Filter(float angle_m,float gyro_m)			
{
	Car_Angle+=(gyro_m-q_bias) * dt; ///Ԥ��ֵ
	Pdot[0]=Q_angle - Pk[0][1] - Pk[1][0];
	Pdot[1]=- Pk[1][1];
	Pdot[2]=- Pk[1][1];
	Pdot[3]=Q_gyro;
	
	Pk[0][0] += Pdot[0] * dt;
	Pk[0][1] += Pdot[1] * dt;
	Pk[1][0] += Pdot[2] * dt;
	Pk[1][1] += Pdot[3] * dt;
	
	angle_err = angle_m -Car_Angle;///����ֵ-Ԥ��ֵ
	
	PCt_0 =  Pk[0][0];
	PCt_1 =  Pk[1][0];
	
	E = R_angle + PCt_0;
	
	K_0 = PCt_0 / E; ///����������
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = Pk[0][1];

	Pk[0][0] -= K_0 * t_0;
	Pk[0][1] -= K_0 * t_1;
	Pk[1][0] -= K_1 * t_0;
	Pk[1][1] -= K_1 * t_1;
	
	Car_Angle+= K_0 * angle_err; ///���ŽǶ�=Ԥ��ֵ+����������*(����ֵ-Ԥ��ֵ)
	q_bias	+= K_1 * angle_err;
	Angle_Speed = gyro_m-q_bias;
}


//�Ƕȼ������˲�
void Angle_Calculate()
{
 float xishu=0.048;
 Angle =(Acc_Z-Acc_Offset)*1.0/(Acc_Max-Acc_Min); 
 Angle=asin(Angle)*180;
 Angle_Speed=(Gyro_Y-Gyro_Y_Offset) * xishu;
 Car_Angle+=Angle_Speed*0.005;
// Kalman_Filter(Angle,Angle_Speed);            //���ÿ������˲�����
}
//�Ƕȿ��ƺ���
void Angle_Control()  
{ 
  PID_ANGLE.pout=PID_ANGLE.P*Car_Angle;
  PID_ANGLE.dout=PID_ANGLE.D*Angle_Speed;
  if(ABS(Angle_Speed)>30&&ABS(Angle_Speed)<80)
  {
       PID_ANGLE.dout*=(1+(ABS(Angle_Speed)-30.0)/30.0);
  }
  
  PID_ANGLE.OUT= PID_ANGLE.pout+ PID_ANGLE.dout;

}

void Get_Speed()                     //5msִ��һ��
{  
  static int qd1_result,qd2_result;
  qd1_result = -2*ftm_quad_get(FTM1); 
  qd2_result =-2* ftm_quad_get(FTM2);
  ftm_quad_clean(FTM1);
  ftm_quad_clean(FTM2);

  
  Distance+=(qd1_result+qd2_result)/4220.0;  //ת��Ϊ����M��7233��1M��������
  CarSpeed=CarSpeed*0.1+0.9*(qd1_result+qd2_result)*200.0/4220.0;    //�������ת��ΪM/S����Ȩ֮ǰ�ĳ���
  if(CarSpeed>4)CarSpeed=4; //�޷�������4M/S
  //if(CarSpeed<-4)CarSpeed=-4; //�޷�������4M/S
  //////////////////////////////////////////////////////////
    if(RoadType!=5)
  { 
    ControlSpeed=ControlSpeed*0.95+CarSpeed*0.05;
  }

}

//�ٶȿ���������
void Speed_Control(void)
{  
  static float setspeed=0;       
  static float PreError[20]={0};
  float  SpeedError,Speed_temp;
  uint8 i;
  float  SpeedFilterRatio=0.85;     //�ٶ��趨ֵ�˲�����ֹ�ٶȿ��Ʊ仯̫����
        
  //�趨�ٶ��˲�


  //�ٶ��˲�����ֹ��Ϊ�ٶȱ仯���������ζ�
  Speed_temp=SetSpeed;
  
  //Speed_temp=Speed_temp*0.9;
  
  
  setspeed = Speed_temp*(1-SpeedFilterRatio)+setspeed*SpeedFilterRatio;
  
  SpeedError=setspeed-ControlSpeed;
  
 SpeedControlIntegral=0;
  for(i=0;i<19;i++)
  {  
    SpeedControlIntegral+=PreError[i];
    PreError[i]=PreError[i+1];
  }
  PreError[19]=SpeedError;
  SpeedControlIntegral+=PreError[19];
  
  //�ٶȸ���
  SpeedControlOutOld=SpeedControlOutNew;
  
  
  SpeedControlOutNew=PID_SPEED.P*SpeedError+PID_SPEED.I*SpeedControlIntegral;   //PI����
 
   if(RoadType==5)
  {
       SpeedControlOutNew*=Hill_Slow_Ratio;
       SpeedControlOutNew= SpeedControlOutOld*0.6+SpeedControlOutNew*0.4;
       SpeedControlOutOld=SpeedControlOutNew;
  }
  
  SpeedControlOutNew= SpeedControlOutOld*0.9+SpeedControlOutNew*0.1;
}
//�ٶȿ���
void Speed_Control_Output(void) 
{ 
  float fValue; 
  fValue = SpeedControlOutNew - SpeedControlOutOld; 
  PID_SPEED.OUT = fValue * (SpeedCount)/Speed_Filter_Times+SpeedControlOutOld;  
}

/********************�������������***************/

void Direction_Control(void)
{
  float ratio=-0.005;
  Delta_P=0;
  Delta_D=0;
  Turn_Speed=-(Gyro_X-Gyro_X_Offset)*ratio;            
  PID_TURN.pout=(PID_TURN.P+Delta_P)*Middle_Err;
  PID_TURN.dout=(PID_TURN.D+Delta_D)*Turn_Speed;
  
  DirectionControlOutOld=DirectionControlOutNew;
   
  Turn_Out=PID_TURN.pout+PID_TURN.dout;
  
    
  if(RoadType==5&&RoadTypeConfirm)//�µ�
  {
   Turn_Out*=0.5;
   if(Turn_Out>0.2)Turn_Out=0.2;
   if(Turn_Out<-0.2)Turn_Out=-0.2;
   //Set_Angle = 18;
  }
  
  Turn_Out=Turn_Out_Filter(Turn_Out);         //ת������˲� 

  
  
  DirectionControlOutNew=Turn_Out;
  
  //DirectionControlOutNew = PID_TURN.pout+PID_TURN.dout;
}



/*********************����������********************/

void Direction_Control_Output(void)
{
  float fValue; 
  fValue =DirectionControlOutNew -DirectionControlOutOld; 
  PID_TURN.OUT=fValue * (DirectionCount)/5+DirectionControlOutOld;
}


//���pwmֵ���
void Moto_Out() 
{
 int L_Value,R_Value;
 static float  Forward_Safe_Angle=30;//ǰ��İ�ȫ�Ƕ�
 static int Motor_Abnormal_Cnt=0;    //���ת���쳣����
 int Backward_Safe_Angle=10;          //����İ�ȫ�Ƕ�
 float Sum;
 
 //ǰ����ٵ����Ƕ�
 // if(RunTime<0.4) Forward_Safe_Angle=Set_Angle;
 // else
  //{
  //  if(Forward_Safe_Angle>15)
  //  {
   //  Forward_Safe_Angle-=0.01;
   // }
  //}

 //����ģʽ�����
 if(Debug_Index==1)  //ֻ��ֱ������ģʽ
 {
   PID_SPEED.OUT=0;
   PID_TURN.OUT=0;
 }
 if(Debug_Index==2)  //û���ٶȿ���ģʽ
 {
   PID_SPEED.OUT=0;
 }
  if(Debug_Index==3)  //û�з������ģʽ
 {
   PID_TURN.OUT=0;
 }

 //�ٶȿ�������޷�

 if(PID_SPEED.OUT>PID_ANGLE.P*Forward_Safe_Angle)//�������ǰ�㣬��ģ���ٶȿ������Ϊ������֮Ϊ��
     PID_SPEED.OUT=PID_ANGLE.P*Forward_Safe_Angle;                       //�Ѿ���б������ȫ�Ƕ���
 if(PID_SPEED.OUT<-PID_ANGLE.P*Backward_Safe_Angle)
     PID_SPEED.OUT=-PID_ANGLE.P*Backward_Safe_Angle;
 
 Sum=PID_ANGLE.OUT-PID_SPEED.OUT;
 
  Sum=constrain(Sum,-0.9,0.9);
  
  if(RoadType==5)
 {
   Sum=constrain(Sum,-0.85,0.85);
 }
  
  LeftMotorOut=Sum+PID_TURN.OUT;   //�������ֵ
  RightMotorOut=Sum-PID_TURN.OUT; 
  

   
  if(LeftMotorOut<0)LeftMotorOut-=L_DeadValue;
  if(RightMotorOut<0)RightMotorOut-=R_DeadValue;
  
  if(RightMotorOut>0.999)RightMotorOut=0.999;                     
  if(RightMotorOut<-0.999)RightMotorOut=-0.999; 
  if(LeftMotorOut>0.999)LeftMotorOut=0.999;                     
  if(LeftMotorOut<-0.999)LeftMotorOut=-0.999; 
  
  L_Value=(int)(10000*LeftMotorOut);      //*//   ת���ɿɿ��Ƶ���Ĳ���
  R_Value=(int)(10000*RightMotorOut);
 
  
 //////////////////////����/////////////////////////////////////////////////////

 if(Car_Angle<-10||Car_Angle>40)             //*//����
 {
   if(Stop==false&&RunTime>2)               //*//��¼����Ϊ������ȫ��б��Χʱ����ͣ������һһ״̬
   {
    Stop=true;
    CarStopedJustNow=true;
    Motor_Abnormal_Cnt=0;
   }
    L_Value=0;
    R_Value=0;
 }
 if((Distance>1)&&(Stop==false)&&Protect)               //*//������ʩ������Ƿ����ٶ��쳣
 {
    if((CarSpeed<0.5)||(CarSpeed>4)) 
    {
       Motor_Abnormal_Cnt++;
       if(Motor_Abnormal_Cnt>600) //*//����쳣�ĳ���ʱ�����600ms��
       {
          Stop=true;
          CarStopedJustNow=true; 
       }
    } 
    else
    {
       Motor_Abnormal_Cnt=0;
    }
 }
///////////////////////////////////////////////////////////////////////////////
 /*
 if(Stop)                                //���ֹͣ���������
 {
   ForceStop=false;
   if(Debug_Motor_ON==false)
   {
    L_Value=0;
    R_Value=0;
   }
 }*/
////////////////////////////////////////////////////////////////////////////////
  if(ForceStop)                      //*//��֪��forcestop������
  {
   L_Value=4000;
   R_Value=4000;
  }
 ///////////////////////////////////////////////////////////////////////////////
 if(R_Value>=0)
 {
  ftm_pwm_duty(FTM0,FTM_CH4,0);
  ftm_pwm_duty(FTM0,FTM_CH5,R_Value);
 }
 else
 {
   R_Value=-R_Value;
   ftm_pwm_duty(FTM0,FTM_CH4,R_Value);
   ftm_pwm_duty(FTM0,FTM_CH5,0);
 }
 
  if(L_Value>=0)
 { 
   ftm_pwm_duty(FTM0,FTM_CH6,0);
   ftm_pwm_duty(FTM0,FTM_CH7,L_Value);

 }
 else
 {
   L_Value=-L_Value;
   ftm_pwm_duty(FTM0,FTM_CH7,0);
   ftm_pwm_duty(FTM0,FTM_CH6,L_Value);
 }
}
////////////////////////////////////////////////////////////////////////////////

float Slope_Calculate(uint8 begin,uint8 end,float *p)    //��С���˷����б��
{
  float xsum=0,ysum=0,xysum=0,x2sum=0;
   uint8 i=0;
   float result=0;
   static float resultlast;
   p=p+begin;
   for(i=begin;i<end;i++)
   {
	   xsum+=i;
	   ysum+=*p;
	   xysum+=i*(*p);
	   x2sum+=i*i;
	   p=p+1;
   }
  if((end-begin)*x2sum-xsum*xsum) //�жϳ����Ƿ�Ϊ�� 
  {
    result=((end-begin)*xysum-xsum*ysum)/((end-begin)*x2sum-xsum*xsum);
    resultlast=result;
  }
  else
  {
   result=resultlast;
  }
  return result;
}


float  Turn_Out_Filter(float turn_out)    //ת���������˲�      
{
  float Turn_Out_Filtered; 
  static float Pre1_Error[4]; 
  Pre1_Error[3]=Pre1_Error[2];
  Pre1_Error[2]=Pre1_Error[1];
  Pre1_Error[1]=Pre1_Error[0];
  Pre1_Error[0]=turn_out;
  Turn_Out_Filtered=Pre1_Error[0]*0.4+Pre1_Error[1]*0.3+Pre1_Error[2]*0.2+Pre1_Error[3]*0.1;
  return Turn_Out_Filtered;
}
