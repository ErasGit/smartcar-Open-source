#include "include.h"
//�Ƕ������
int16  Acc_Z,Gyro_X,Gyro_Y;
//int16  Acc_Max=8192;
//int16  Acc_Min=-8192;
int16  Acc_Max=3780;
int16  Acc_Min=-2600;
int16  Acc_Offset;
float Angle,Angle_Speed,Car_Angle=0;
int16  Gyro_Z_Offset=427,Gyro_Y_Offset=47,Gyro_X_Offset=740;
//�ٶ������
float SpeedControlOutNew;
float SpeedControlOutOld;
float SpeedControlIntegral=0,Hill_Slow_Ratio;
uint8  Set_Angle;   //����ǰ��Ƕ�
int   SpeedCount;
int   Speed_Filter_Times=15;    //�ٶ�ƽ�����  50
float CarSpeed=0,ControlSpeed=0,AverageSpeed,SetSpeed=0,Distance;
//���������
float DirectionControlOutNew;
float DirectionControlOutOld;
float Turn_Speed=0;
int   DirectionCount;
float Delt_error,Middle_Err;
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
uint8   Starting,Stop,CarStopedJustNow;
uint8 Encoder_Disable=0;

float shuzhi;
//
void Get_Attitude()
{
  Acc_Z =Get_Z_Acc(); 
  Gyro_X= Get_X_Gyro();
  Gyro_Y= Get_Y_Gyro();            
}
//******Kalman�˲�******//
//-------------------------------------------------------
static  float Q_angle=0.001, Q_gyro=0.001, R_angle=5, dt=0.004;
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
 Angle =(Acc_Z-Gyro_Z_Offset)*180.0/(Acc_Max-Acc_Min); 
 Angle_Speed=(Gyro_Y-Gyro_Y_Offset) * xishu;
 //Car_Angle+=Angle_Speed*0.005;//û�� 
 Kalman_Filter(Angle,Angle_Speed);            //���ÿ������˲�����
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
  shuzhi = Angle_Speed;
}
void Get_Speed()                     //5msִ��һ��
{  
  int qd1_result,qd2_result;
  qd1_result = FTM_QUAD_get(FTM1); 
  qd2_result = FTM_QUAD_get(FTM2);
  
  FTM_QUAD_clean(FTM1);
  FTM_QUAD_clean(FTM2);

  Distance+=(qd1_result+qd2_result)/6100.0;  //ת��Ϊ����
  CarSpeed=CarSpeed*0.1+0.9*(qd1_result+qd2_result)*250.0/6100.0;    //�������ת��ΪM/S
   if(CarSpeed>4)CarSpeed=3.5;//3.5
 
}
//�ٶȿ���������
void Speed_Control(void)
{  
      
  static float PreError[20]={0};
  float  SpeedError;
  uint8 i;
  //�趨�ٶ��˲�
  //�ٶ��˲�����ֹ��Ϊ�ٶȱ仯���������ζ�
  //if(Starting||Stop)  setspeed=0; //������ʱ����ٶ���Ϊ��
  
   SpeedError=SetSpeed-CarSpeed;  
   
  //������20��ƫ����ܺ���Ϊ������
   SpeedControlIntegral=0;
   for(i=0;i<19;i++)
   {
     PreError[i]=PreError[i+1]; 
     SpeedControlIntegral+=PreError[i];
   }
    PreError[19]=SpeedError;
    SpeedControlIntegral+=PreError[19]; 
  //�ٶȸ���
  SpeedControlOutOld=SpeedControlOutNew;
  //��ֹ��λ��
//  if(Starting)
//  {
//   SpeedControlIntegral=-50*Distance; //û��
//   SpeedError=0;
//  } 
  SpeedControlOutNew=PID_SPEED.P*SpeedError+PID_SPEED.I*SpeedControlIntegral;   //PI����
  SpeedControlOutNew= SpeedControlOutOld*0.9+SpeedControlOutNew*0.1;
}
//�ٶȿ���
void Speed_Control_Output(void) 
{ 
  float fValue; 
  
  fValue = SpeedControlOutNew - SpeedControlOutOld; 
  PID_SPEED.OUT = fValue * (SpeedCount+1)/Speed_Filter_Times+SpeedControlOutOld;
//    shuzhi = PID_SPEED.OUT;
}
/********************�������������***************/
void Direction_Control(void)
{
  float ratio=0.005;
  static int Calculate_Length=0;
  Turn_Speed=(Gyro_X-Gyro_X_Offset)*ratio;            
  
   
  if(RoadType==0)  //ֻ������ͨ�����͵�������ģ��
  {
    if(Calculate_Length<8) 
    {  
     Calculate_Length++;
     Delta_P=0;
     Delt_error=0;
     Delta_D=0;
    }
    else
    {
      Delt_error=-10*Slope_Calculate(0,Calculate_Length,Previous_Error);
      Delta_P=Fuzzy( Middle_Err,Delt_error)* Fuzzy_Kp*0.1;
      Delta_D=Fuzzy( Middle_Err,Delt_error)* Fuzzy_Kd*0.1;
    }
  }
  else 
  {
    Delta_P=0;
    Delta_D=0;
    Calculate_Length=0;
  }
  
  PID_TURN.pout=(PID_TURN.P+Delta_P)*Middle_Err;
  PID_TURN.dout=(PID_TURN.D+Delta_D)*Turn_Speed;
  Turn_Out= PID_TURN.pout+ PID_TURN.dout;
  Turn_Out=Turn_Out_Filter(Turn_Out);         //ת������˲� 
  PID_TURN.OUT=Turn_Out;

}

//���pwmֵ���
void Moto_Out() 
{
 int L_Value,R_Value;
 //static float  Forward_Safe_Angle=35;//ǰ��İ�ȫ�Ƕ�
 static float  Forward_Safe_Angle=-25;
 static int Motor_Abnormal_Cnt=0;    //���ת���쳣����
 //int Backward_Safe_Angle=20;          //����İ�ȫ�Ƕ�
  int Backward_Safe_Angle=20;
 float Sum;
 //�ٶȿ�������޷�
// if(PID_SPEED.OUT>PID_ANGLE.P*Forward_Safe_Angle)//�������ǰ�㣬��ģ���ٶȿ������Ϊ������֮Ϊ��
// PID_SPEED.OUT=PID_ANGLE.P*Forward_Safe_Angle;                       //�Ѿ���б������ȫ�Ƕ���
// if(PID_SPEED.OUT<-PID_ANGLE.P*Backward_Safe_Angle)
// PID_SPEED.OUT=-PID_ANGLE.P*Backward_Safe_Angle;

//  if(PID_SPEED.OUT>PID_ANGLE.P*Forward_Safe_Angle)//�������ǰ�㣬��ģ���ٶȿ������Ϊ������֮Ϊ��
//      PID_SPEED.OUT=PID_ANGLE.P*Forward_Safe_Angle;//�Ѿ���б������ȫ�Ƕ���  PID_ANGLE.P*(Forward_Safe_Angle)
// if(PID_SPEED.OUT<-PID_ANGLE.P*Backward_Safe_Angle)
//     PID_SPEED.OUT=-PID_ANGLE.P*(Backward_Safe_Angle);   //-PID_ANGLE.P*(Backward_Safe_Angle)
 
 Sum=PID_ANGLE.OUT+PID_SPEED.OUT;//+
//
//  Sum=PID_ANGLE.OUT;
//  LeftMotorOut=Sum-PID_TURN.OUT;   //�������ֵ
//  RightMotorOut=Sum+PID_TURN.OUT; 
  LeftMotorOut=Sum;   //�������ֵ
  RightMotorOut=Sum; 
 //��ֵ�޷�����ֹ���ٹ���

  if(RightMotorOut>0.99)RightMotorOut=0.99;                     
  if(RightMotorOut<-0.99)RightMotorOut=-0.99; 
  if(LeftMotorOut>0.99)LeftMotorOut=0.99;                     
  if(LeftMotorOut<-0.99)LeftMotorOut=-0.99; 
  
  L_Value=(int)(3000*LeftMotorOut);            //10000 
  R_Value=(int)(3000*RightMotorOut);            //10000
 
 /////////////////////����////////////////////////////
 if(Car_Angle<-100||Car_Angle>100)             //���� -50
 {
   if(Stop==false&&RunTime>2)
   {
    Stop=true;
    CarStopedJustNow=true; 
    Motor_Abnormal_Cnt=0;
   }
    L_Value=0;
    R_Value=0;
 }
// if((Distance>1)&&(Stop==false))               //              ����Ƿ����ٶ��쳣
// {
//    if((CarSpeed<0.5)||(CarSpeed>2)) //4
//    {
//       Motor_Abnormal_Cnt++;
//       if(Motor_Abnormal_Cnt>250) //����쳣�ĳ���ʱ�����500ms��
//       {
//          Stop=true;
//          CarStopedJustNow=true; 
//       }
//    } 
//    else
//    {
//       Motor_Abnormal_Cnt=0;
//    }
// }
// if(Stop)                                //���ֹͣ���������
// {
//    
//    //L_Value=0;
//   // R_Value=0;
// }
   //   PID_SPEED.OUT=0;
     // PID_TURN.OUT=0;
   if(L_Value>=0) //��ת
  {
     FTM_PWM_Duty(FTM0,FTM_CH2,L_Value);//ռ�ձȾ���Ϊ10000  20 31
     FTM_PWM_Duty(FTM0,FTM_CH0,0);
  }
  else   //��ת
  {
     FTM_PWM_Duty(FTM0,FTM_CH2,0);
     FTM_PWM_Duty(FTM0,FTM_CH0,-L_Value);
  }
    if(R_Value>=0) //��ת
  {
     FTM_PWM_Duty(FTM0,FTM_CH3,R_Value); 
     FTM_PWM_Duty(FTM0,FTM_CH1,0);
  }
  else   //��ת
  {
     FTM_PWM_Duty(FTM0,FTM_CH3,0);
     FTM_PWM_Duty(FTM0,FTM_CH1,-R_Value);
  }
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
float  Middle_Err_Filter(float middle_err)    //����ƫ���˲�      
{
  float Middle_Err_Fltered; 
  static float Pre3_Error[4]; 
  Pre3_Error[3]=Pre3_Error[2];
  Pre3_Error[2]=Pre3_Error[1];
  Pre3_Error[1]=Pre3_Error[0];
  Pre3_Error[0]=middle_err;
  Middle_Err_Fltered=Pre3_Error[0]*0.4+Pre3_Error[1]*0.3+Pre3_Error[2]*0.2+Pre3_Error[3]*0.1;
  return Middle_Err_Fltered;
}