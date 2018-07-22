#include "Motor_Control.h"
#include "math.h"



        /****************************    �������Ʊ���    ****************************/ 
char Set = 0,Speed_Set=0;
uint8   BangBang_Flag = 0;        //������ʽPID��
uint8 	Run_Flag  = 0;                    //���ܱ�־
uint8 Ramp_Flag=0;
uint16 Ramp_Time=0;
uint16 Ramp_Delay=60;       //�µ���ʱʱ��
int16 Ramp_Speed=185;       //�µ��ٶ�
extern uchar Foresight;
extern int32 S3010_Duty;
        /****************************        ����        ****************************/ 
int8    Point_Num=0;
        /****************************    ����ʶ�����    ****************************/ 
uint8   Game_End = 1;
uint8   Stop_Flag = 0;
uint8   Go_Flag = 0;
uint8   zhidao_Flag=0;
uint8   Annulus_Check=0;
uint8   Obstacle_Check=0;
        /****************************        ����        ****************************/ 

        /****************************    ���ٵ��ڱ���    ****************************/ 
float g_dirControl_P = 3.3;		//����P
float g_dirControl_D = 10;		//����D
int16  g_nDirectionError;  		//����ƫ��
float g_fDirectionError_dot;	//����ƫ��΢��
float g_fDirectionControlOut;	//����������

int8  CS_Point;
int32 CS_Duty;      //       //4320  4740 5180
        /****************************        ����        ****************************/ 

        /****************************   ������ʶ�����   ****************************/ 
//uint8   Went_Curve=0;     //�������־λ
//uint8   Out_Curve=0;      //�������־λ
//uint8   Straight_Time=0;    //ֱ��ʱ�䳣��
//uint8   Point_iE=0;         //����ƫ��
//uint8   Out_Curve_Time=0;   //����ʱ�䳣��
//uint8   Went_Curve_Time=0;  //����ʱ�䳣��
        /****************************        ����        ****************************/ 
        
        /****************************    �ٶȿ��Ʊ���    ****************************/ 
int16 Starting_Time=300;              //������ʱʱ��2S
int8 Starting2_Time=0;
int32   Left_MOTOR_Duty  = 0;
int32   Right_MOTOR_Duty  = 0;
int32   Left_MOTOR_Speed = 0;         //FTM1  ��ֵ
int32   Right_MOTOR_Speed = 0;         //FTM2 ��ֵ
float   Left_Speed = 0;         
float   Right_Speed = 0; 


int16 Speed;
int16 Max_Speed=0;
int16 Min_Speed=0;
int16 High_Speed=0;

int16 Mid_Speed,Left_High_Speed,Right_High_Speed;


        /****************************    ������ر���    ****************************/ 
int16 CS_Flag = 1568;//1568 ��ֵԽ�����Խ��ǰ
int16   error_0;
int16    a;
int16 Speed_iError=0;
uint8    CS_Left=11,CS_Right=11;
float   Temp_Orr;         
float   Temp_Add;
int32    Angle;
float    CS_P_Right,CS_P_Left;
        /****************************        ����        ****************************/ 


float speed_error[47]={ 1,   1.0139,  1.0279, 1.0419, 1.0559, 1.0700, 1.0840,    //7
                      1.0981, 1.1124, 1.1266, 1.1410, 1.1554, 1.1700, 1.1846,     //7
                      1.1994, 1.2143, 1.2293, 1.2445, 1.2599, 1.2754, 1.2911,     //7
                      1.3070, 1.3232, 1.3395, 1.3561, 1.3730, 1.4076, 1.4253,     //7
                      1.4434, 1.4618, 1.4806, 1.4998, 1.5195, 1.5396, 1.5601,     //7
                      1.5812, 1.6028, 1.6249, 1.6477, 1.6712, 1.6953, 1.7203,     //7
                      1.7460, 1.7724, 1.800, 1.900, 2.000};   //5

        /****************************        ����        ****************************/ 

/******** �ٶȿ��� *********/
void Adjust_Speed(void)
{
//    Speed = 175;
//    Max_Speed = Speed*1.3;
//    Min_Speed = Speed;
//    
  //   High_Speed = Speed;
 //    High_Speed = (Foresight-30)*(Foresight-30)*(Max_Speed-Min_Speed)/CS_Flag + Min_Speed;
        //if (Foresight>20)Foresight=20;
//        High_Speed = (int16)((20-Foresight)*(20-Foresight)*(Max_Speed-Min_Speed))/400;
//        High_Speed = Max_Speed - High_Speed;
    /****************************      �ٶ�1��      ****************************/  
        if (Speed_Set==1)
        {
            if (Foresight >= 18)
            {
                High_Speed = 180;
            }
            else if (Foresight >= 16)
            {
                High_Speed = 182;
            }
            else if (Foresight >= 14)
            {
                High_Speed = 185;
            }
            else if (Foresight >= 12)
            {
                High_Speed = 189;
            }
            else if (Foresight >= 10)
            {
                High_Speed = 191;
            }
            else if (Foresight >= 8)
            {
                High_Speed = 193;
            }
            else if (Foresight >= 5)
            {
                High_Speed = 195;
            }
            else if (Foresight >= 4)
            {
                High_Speed = 200;
            }
            else if (Foresight >= 3)
            {
                High_Speed = 210;
            }
            else if (Foresight >= 2)
            {
                High_Speed = 220;
            }
            else
            {
                High_Speed = 240;
            }
            CS_P_Right = 0.35;  //����ϵ��
            CS_P_Left = 0.35;
            Set = 1;            //���PID
        }
        /****************************      �ٶ�2��      ****************************/ 
        if (Speed_Set==2)
        {
            High_Speed = 160;
            CS_P_Right = 0.45;  //����ϵ��
            CS_P_Left = 0.45;
            Set = 1;            //���PID
        }
        /****************************      �ٶ�3��      ****************************/ 
        if (Speed_Set==3)
        {
            High_Speed = 170;
            CS_P_Right = 0.45;  //����ϵ��
            CS_P_Left = 0.45;
            Set = 1;            //���PID
        }
        /****************************      �ٶ�4��      ****************************/ 
        if (Speed_Set==4)
        {
            High_Speed = 180;
            CS_P_Right = 0.33;  //����ϵ��
            CS_P_Left = 0.33;
            Set = 1;            //���PID
        }
        /****************************      �ٶ�5��      ****************************/  
        if (Speed_Set==5)
        {
            High_Speed = 200;
            CS_P_Right = 0.35;  //����ϵ��
            CS_P_Left = 0.35;
            Set = 1;            //���PID
        }
        /****************************      �ٶ�6��      ****************************/ 
        if (Speed_Set==6)
        {
            High_Speed = 202;
            CS_P_Right = 0.35;  //����ϵ��
            CS_P_Left = 0.35;
            Set = 1;            //���PID
        }
        /****************************      �ٶ�7��      ****************************/ 
        if (Speed_Set==7)
        {
            
            High_Speed = 205;
            CS_P_Right = 0.35;  //����ϵ��
            CS_P_Left = 0.35;
            Set = 1;            //���PID
        }
        /****************************      �ٶ�8��      ****************************/ 
        if (Speed_Set==8)
        {
            
            High_Speed = 207;
            CS_P_Right = 0.35;  //����ϵ��
            CS_P_Left = 0.35;
            Set = 1;            //���PID
        }    
        /****************************************************************************/  
        /****************************      �µ����      ****************************/  
        /****************************************************************************/ 
//        MPU6050_GetData(&GYRO, &ACC);	// ��ȡ����������
//        Data_Filter();
//
//        if ( GYRO_Real.Y>150 && !Ramp_Flag && !Ramp_Time)
//        {
//                Ramp_Flag=1;
//        }
//        if (Ramp_Flag)
//        {
//                High_Speed=Ramp_Speed;
//        }
//        if (Ramp_Time>Ramp_Delay)
//        { 
//                Ramp_Flag=0;
//        }
//        if (Ramp_Time>150)
//        { 
//                Ramp_Time=0;
//        } 
        
        
        /****************************************************************************/  
        /****************************      ���ܿ���      ****************************/  
        /****************************************************************************/         
        if (!Go_Flag)
        {
                High_Speed=0;  
        }
        else
        {

        }
    

        /****************************************************************************/  
        /****************************    ����������    ****************************/  
        /****************************************************************************/          
//        if (Point>40)   Point_iE=Point-40; else Point_iE=40-Point;
//        if (Went_Curve) Out_Curve_Time++;  
//        if (Out_Curve)  Went_Curve_Time++; 
//        if (Point_iE<5) Straight_Time++;
//        
//        if (Straight_Time>5 && Point_iE>8)
//        {
//                Went_Curve=1;
//                Straight_Time=0;
//        }
//        if (Went_Curve && Went_Curve_Time>30 && Point_iE<15)
//        {
//                Went_Curve=0;
//                Went_Curve_Time=0;
//                Out_Curve=1;
//        }
//        if (Out_Curve_Time>30 || Point_iE<8)
//        {
//                Out_Curve=0;
//        }
        
   //     save_var(VAR2, Foresight);
        
}

void S3010_Control(void)
{
        S3010_Duty = PlacePID_Control(&S3010_PID,Point_Mid,Point);	//ʹ��λ��ʽPID����	
		S3010_Duty = range_protect(S3010_Duty, S3010_RIGHT, S3010_LEFT);          //���ռ�ձ��޷�����
        
		ftm_pwm_duty(S3010_FTM, S3010_CH, S3010_Duty);			//�޸Ķ��ռ�ձ�
}
/******** ������� *********/
void MOTOR_Control(void)
{
        CS_Point = Motor_Point_Weight(4);		// ��ֵ ͼ������ƽ��	����Ϊż��
        CS_Duty = PlacePID_CS(&CS_PID,Point_Mid,CS_Point);	//ʹ��λ��ʽPID����	
		CS_Duty = range_protect(CS_Duty, S3010_RIGHT, S3010_LEFT);          //���ռ�ձ��޷�����
        Angle = S3010_MID - CS_Duty;
        
//        Angle = S3010_MID - S3010_Duty;
//        save_var(VAR5, S3010_Duty);
//        save_var(VAR6, Angle);
        /****************************      ���ܼ��      ****************************/ 
        Starting_Check();  
        /****************************      �ٶȵ���      ****************************/ 
        Adjust_Speed();         

        /****************************************************************************/  
        /****************************    ����ǶȲ���    ****************************/  
        /****************************************************************************/ 
       
        if(Angle>0)         //�ҹ�
        {
           a=(int16)(45*(Angle)*1.0/(S3010_LEFT - S3010_MID)) ;  //angle_max   45
           if(a>45)  a=45;
           if(a<0)   a=0;
           Left_Speed = High_Speed ;
           Right_Speed = High_Speed - (High_Speed/45)*a*CS_P_Right;
           Left_High_Speed = Left_Speed;
           Right_High_Speed = Right_Speed; 
        }
        else              //���
        {
           a=(int16)(45*(-Angle)*1.0/(S3010_MID - S3010_RIGHT)) ;  //angle_max   45
           if(a>45)  a=45;
           if(a<0)   a=0;
           Left_Speed = High_Speed - (High_Speed/45)*a*CS_P_Left;
           Right_Speed = High_Speed;
           Left_High_Speed = Left_Speed;
           Right_High_Speed = Right_Speed; 

        }
        
//        Left_High_Speed = High_Speed + g_fDirectionControlOut;
////        if(Left_High_Speed<0)Left_High_Speed=0;
////        else if(Left_High_Speed>2*High_Speed)Left_High_Speed=2*High_Speed;
//           Right_High_Speed = High_Speed - g_fDirectionControlOut;
////           if(Right_High_Speed<0)Right_High_Speed=0;
////        else if(Right_High_Speed>2*High_Speed)Right_High_Speed=2*High_Speed;
        
        /****************************************************************************/  
        /****************************    ����ǶȼӲ���    ****************************/  
        /****************************************************************************/ 
       
//        if(Angle>0)         //�ҹ�
//        {
//           a=(int16)(45*(Angle)*1.0/(S3010_LEFT - S3010_MID)) ;    //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Left_Speed = High_Speed + (High_Speed/45)*a*0.4;
//           Right_Speed = High_Speed;
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//        }
//        else              //���
//        {
//           a=(int16)(45*(-Angle)*1.0/(S3010_MID - S3010_RIGHT)) ;  //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Left_Speed = High_Speed;
//           Right_Speed = High_Speed + (High_Speed/45)*a*0.4;;
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//
//        }
        /****************************************************************************/  
        /****************************      ��ģ������      ****************************/  
        /****************************************************************************/   
        

//         if(Angle>0)
//        {
//           a=(int16)(45*(Angle)*1.0/(S3010_LEFT - S3010_MID)) ;   //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Temp_Orr = 45 - (tan((a*3.14)/180) * 15.4);     //40����Խ��������ǰ����
//           Temp_Add = 45 + (tan((a*3.14)/180) * 15.4);
//           Left_Speed = High_Speed;
//           Right_Speed = High_Speed * Temp_Orr / Temp_Add*1.0;    //1.0����Խ�����Խ��
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//        }
//        else
//        {
//            a=(int16)(45*(-Angle)*1.0/(S3010_MID - S3010_RIGHT)) ; //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Temp_Orr =45 - (tan((a*3.14)/180) * 15.4);
//           Temp_Add = 45 + (tan((a*3.14)/180) * 15.4);
//           Left_Speed = High_Speed * Temp_Orr / Temp_Add*1.0;
//           Right_Speed = High_Speed;
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//
//        }
        
         /****************************************************************************/  
        /****************************      ��ģ�Ӽ�����      ****************************/  
        /****************************************************************************/   
        
//         if(Angle>0)
//        {
//           a=(int16)(45*(Angle)*1.0/(S3010_LEFT - S3010_MID)) ;   //angle_max   45
//           if(a>45)  a=45;
//           if(a<0)   a=0;
//           Temp_Orr = tan((a*3.14)/180) * 15.4 / 40;
//           Left_Speed = 1.0 * High_Speed * (1.0 + Temp_Orr); //��һ���������ԼӴ���٣��ڶ�������������ǰ����
//           Right_Speed = 1.0 * High_Speed * (1.0 - Temp_Orr);
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//        }
//        else
//        {
//          a=(int16)(45*(-Angle)*1.0/(S3010_MID - S3010_RIGHT)) ;  //angle_max   45
//          if(a>45)  a=45;
//           if(a<0)   a=0;
//           Temp_Orr = tan((a*3.14)/180) * 15.4 / 40;
//           Left_Speed = 1.0 * High_Speed * (1.0 - Temp_Orr); 
//           Right_Speed = 1.0 * High_Speed * (1.0 + Temp_Orr);
//           Left_High_Speed = Left_Speed;
//           Right_High_Speed = Right_Speed; 
//        } 
        /****************************************************************************/  
        /****************************       PID����      ****************************/  
        /****************************************************************************/  
//        Right_MOTOR_Speed = -ftm_quad_get(FTM1);//��ȡFTM1 �������� ��������(������ʾ������)  
//        ftm_quad_clean(FTM1);   	//��������Ĵ�������
//        //�������
//        Left_MOTOR_Speed = ftm_quad_get(FTM2);//��ȡFTM2 �������� ��������(������ʾ������)   
//        ftm_quad_clean(FTM2);   	//��������Ĵ�������
//        /****************************************************************************/  
//        /****************************      �ٶȹ���      ****************************/  
//        /****************************************************************************/   
//         Left_MOTOR_Speed      = (Left_MOTOR_Speed>400?400:Left_MOTOR_Speed); 
//         Right_MOTOR_Speed     = (Right_MOTOR_Speed>400?400:Right_MOTOR_Speed);
        Left_MOTOR_Duty += PID_Realize(&Left_MOTOR_PID, Left_MOTOR_Speed, Left_High_Speed);//ʹ�ô�������ʽPID���е���
        Right_MOTOR_Duty += PID_Realize(&Right_MOTOR_PID, Right_MOTOR_Speed, Right_High_Speed);//ʹ�ô�������ʽPID���е���

        if (Left_MOTOR_Duty>0)
        {
          Left_MOTOR_Duty = range_protect(Left_MOTOR_Duty, 0, 900);	//�޷�����
          //�����ת
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR1_PWM,0);	//ռ�ձ����990������
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR2_PWM,Left_MOTOR_Duty);	   		//ռ�ձ����990������

        }
        else
        {
          Left_MOTOR_Duty = range_protect(Left_MOTOR_Duty, -900, 0);
          //�����ת
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR1_PWM,-Left_MOTOR_Duty);	   			//ռ�ձ����990������
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR2_PWM,0);	//ռ�ձ����990������
        }
        if(Right_MOTOR_Duty>0)
        {
          Right_MOTOR_Duty = range_protect(Right_MOTOR_Duty, 0, 900);	//�޷�����
          //�����ת
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR1_PWM,0);	//ռ�ձ����990������
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR2_PWM,Right_MOTOR_Duty);	   		//ռ�ձ����990������
        }
        else
        {
          Right_MOTOR_Duty = range_protect(Right_MOTOR_Duty, -900, 0);	//�޷�����
          //�����ת
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR1_PWM,-Right_MOTOR_Duty);	   			//ռ�ձ����990������
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR2_PWM,0);	//ռ�ձ����990������
        } 
}

/******* ����ٶȲ��� ********/
//int32 var[4];

void speed_measure(void)
{
	//�ұ�����
        Right_MOTOR_Speed = -ftm_quad_get(FTM1);//��ȡFTM1 �������� ��������(������ʾ������)  
        ftm_quad_clean(FTM1);   	//��������Ĵ�������
        //�������
        Left_MOTOR_Speed = ftm_quad_get(FTM2);//��ȡFTM2 �������� ��������(������ʾ������)   
        ftm_quad_clean(FTM2);   	//��������Ĵ�������
        /****************************************************************************/  
        /****************************      �ٶȹ���      ****************************/  
        /****************************************************************************/   
         Left_MOTOR_Speed      = (Left_MOTOR_Speed>400?400:Left_MOTOR_Speed); 
         Right_MOTOR_Speed     = (Right_MOTOR_Speed>400?400:Right_MOTOR_Speed);
        /****************************************************************************/  
        /**************************** ������λ����ʾ�ٶ� ****************************/  
        /****************************************************************************/   
//        var[0]=Right_MOTOR_Speed;
//        var[1]=Left_MOTOR_Speed;
//        var[2]=Left_High_Speed;
//        var[3]=Right_High_Speed;
//        vcan_sendware(var, sizeof(var));
//             DirectionControl(Point);
	MOTOR_Control();
}
void Starting_Check(void)
{
        if (Starting_Time==0 && !Go_Flag && !Stop_Flag)
        {
                Go_Flag=1;
                Starting_Time=50;  //���ܿ�ʼ�رջ�·���
        }
        if (Starting_Time==0 && Go_Flag && !Annulus_Check)
        {
                Annulus_Check=1;
                Obstacle_Check=1;
        }
        if (Stop_Flag && Go_Flag)
        {
              if (Starting2_Time==0)   
              {
                  Starting2_Time=40;  //��⵽�������Ժ���ʱͣ��              
              }
              

              if (Starting2_Time==10)
              {
                Go_Flag=0;
            //    Starting_Time=-10;
              }
        }

}


void DirectionControl(uchar Point)
{
	static float g_nDirectionErrorTemp[7];
		
	g_nDirectionError = (int16)Point-40;//����ƫ�80��-80��
	g_nDirectionError = (g_nDirectionError>=30?30:g_nDirectionError);//�޷�
	g_nDirectionError = (g_nDirectionError<=-30?-30:g_nDirectionError);
	 
	g_nDirectionErrorTemp[5] = g_nDirectionErrorTemp[4];
	g_nDirectionErrorTemp[4] = g_nDirectionErrorTemp[3];
	g_nDirectionErrorTemp[3] = g_nDirectionErrorTemp[2];
	g_nDirectionErrorTemp[2] = g_nDirectionErrorTemp[1];
	g_nDirectionErrorTemp[1] = g_nDirectionErrorTemp[0];
	g_nDirectionErrorTemp[0] = g_nDirectionError;
	g_fDirectionError_dot = g_nDirectionErrorTemp[0]-g_nDirectionErrorTemp[3];//ƫ��΢��
	g_fDirectionError_dot = (g_fDirectionError_dot>30?30:g_fDirectionError_dot);//΢���޷�
	g_fDirectionError_dot = (g_fDirectionError_dot<-30?-30:g_fDirectionError_dot);
	
	//�����ٶȵĶ�̬PD
/*
	if(g_fSpeedFilter<2000)	   {g_dirControl_P =180 ;g_dirControl_D = 300;}
	else if(g_fSpeedFilter<2000){g_dirControl_P =220 ;g_dirControl_D = 330;}
	else if(g_fSpeedFilter<2100){g_dirControl_P =250 ;g_dirControl_D = 340;}
	else if(g_fSpeedFilter<2200){g_dirControl_P =300 ;g_dirControl_D = 360;}
	else if(g_fSpeedFilter<2300){g_dirControl_P =320 ;g_dirControl_D = 380;}
	else if(g_fSpeedFilter<2400){g_dirControl_P =360 ;g_dirControl_D = 410;}
	else if(g_fSpeedFilter<2500){g_dirControl_P =380 ;g_dirControl_D = 430;}
	else if(g_fSpeedFilter<2600){g_dirControl_P =410 ;g_dirControl_D = 450;}
	else if(g_fSpeedFilter<2700){g_dirControl_P =420 ;g_dirControl_D = 470;}
	else if(g_fSpeedFilter<2800){g_dirControl_P =430 ;g_dirControl_D = 500;}
	else if(g_fSpeedFilter<2900){g_dirControl_P =440 ;g_dirControl_D = 550;}
	else if(g_fSpeedFilter<3000){g_dirControl_P =450 ;g_dirControl_D = 600;}
	else if(g_fSpeedFilter>3000){g_dirControl_P =470 ;g_dirControl_D = 750;}
	else if(g_fSpeedFilter>3100){g_dirControl_P =470 ;g_dirControl_D = 800;}
*/	
	
	g_fDirectionControlOut = g_nDirectionError*g_dirControl_P + g_fDirectionError_dot*g_dirControl_D;//ת��������
//	g_fDirectionControlOut = g_nDirectionError*float05 + g_fDirectionError_dot*float06;//PDת��

//	g_fDirectionControlOut = (g_fDirectionControlOut > 350? 350: g_fDirectionControlOut);//ת������޷�
//	g_fDirectionControlOut = (g_fDirectionControlOut < -350? -350: g_fDirectionControlOut);	
}


/******** �޷����� *********/
int32 range_protect(int32 duty, int32 min, int32 max)//�޷�����
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}