#include "Motor_Control.h"
#include "math.h"



        /****************************    �������Ʊ���    ****************************/ 
char Set = 0,Speed_Set=0;
uint8   BangBang_Flag = 0;        //������ʽPID��
uint8 	Run_Flag  = 0;                    //���ܱ�־
uint8  Ramp_Flag=0;
uint16 Ramp_Time=0;
uint8  Ramp_OFF_ON=0;
uint16 Ramp_Delay=110;       //�µ���ʱʱ�� 1.1S
int16  Ramp_Speed=180;        //�µ��ٶ�
int16  Annulus_Speed=190;     //180��СԲ���ٶ�
uint8 Ramp_Continue_Flag=1;     //��⵽�µ��Ժ�ڶ��ο�ʼ����µ��ı�־
        /****************************        ����        ****************************/ 

        /****************************    ����ʶ�����    ****************************/ 
uint8   Game_End = 1;
uint8   Stop_Flag = 0;
uint8   Go_Flag = 0;
uint8   zhidao_Flag=0;
uint8   Annulus_Check=0;
uint8   Obstacle_Check=0;
        /****************************        ����        ****************************/ 


        
        /****************************    �ٶȿ��Ʊ���    ****************************/ 
int16 Starting_Time=150;              //������ʱʱ��1.5S
int8 Starting2_Time=0;
int32   Left_MOTOR_Duty  = 0;
int32   Right_MOTOR_Duty  = 0;
int32   Left_MOTOR_Speed = 0;         //FTM1  ��ֵ
int32   Right_MOTOR_Speed = 0;         //FTM2 ��ֵ
int16   High_Speed=0,Max_Speed=0,Min_Speed=0;
int8    Point_Speed;
float   Set_Speed;
int32 Left_High_Speed,Right_High_Speed;

int32  Real_MOTOR_Speed;

int16  Fictitious_Beeline_Num=0;
uchar  Track_Line_Num=0,Track_Line_Time=0;
uchar  Last_Error[5],Point_Error;
        /****************************        ����        ****************************/ 


//float speed_error[47]={ 1,   1.0139,  1.0279, 1.0419, 1.0559, 1.0700, 1.0840,    //7
//                      1.0981, 1.1124, 1.1266, 1.1410, 1.1554, 1.1700, 1.1846,     //7
//                      1.1994, 1.2143, 1.2293, 1.2445, 1.2599, 1.2754, 1.2911,     //7
//                      1.3070, 1.3232, 1.3395, 1.3561, 1.3730, 1.4076, 1.4253,     //7
//                      1.4434, 1.4618, 1.4806, 1.4998, 1.5195, 1.5396, 1.5601,     //7
//                      1.5812, 1.6028, 1.6249, 1.6477, 1.6712, 1.6953, 1.7203,     //7
//                      1.7460, 1.7724, 1.800, 1.900, 2.000};   //5

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
        if (Speed_Set==1)   //18.1
        {
            Max_Speed=210;
            Min_Speed=170;
            CS_P_Right = 0.34;  //����ϵ��
            CS_P_Left = 0.34;
            Set = 0;            //���PID
            Point_Num=0;        //��ǰϵ��
            CS_Point_Num=0;     //��ǰϵ��
            
        }
        /****************************      �ٶ�2��      ****************************/ 
        if (Speed_Set==2) //17.2
        {
            Max_Speed=220;
            Min_Speed=180;
            CS_P_Right = 0.34;  //����ϵ��
            CS_P_Left = 0.34;
            Set = 1;            //���PID
            Point_Num=0;        //��ǰϵ��
            CS_Point_Num=0;     //��ǰϵ��
        }
        /****************************      �ٶ�3��      ****************************/ 
        if (Speed_Set==3)//16.8
        {
            Max_Speed=230;
            Min_Speed=190;
            CS_P_Right = 0.34;  //����ϵ��
            CS_P_Left = 0.34;
            Set = 1;            //���PID
            Point_Num=0;        //��ǰϵ��
            CS_Point_Num=0;     //��ǰϵ��
        }
        /****************************      �ٶ�4��      ****************************/ 
        if (Speed_Set==4)  //16.6
        {
            Max_Speed=230;
            Min_Speed=195;
            CS_P_Right = 0.34;  //����ϵ��
            CS_P_Left = 0.34;
            Set = 0;            //���PID
            Point_Num=0;        //��ǰϵ��
            CS_Point_Num=0;     //��ǰϵ��
        }
        /****************************      �ٶ�5��      ****************************/  
        if (Speed_Set==5)
        {
            Max_Speed=240;
            Min_Speed=200;
            CS_P_Right = 0.34;  //����ϵ��
            CS_P_Left = 0.34;
            Set = 1;            //���PID
            Point_Num=2;        //��ǰϵ��
            CS_Point_Num=2;     //��ǰϵ��
        }
        /****************************      �ٶ�6��      ****************************/ 
        if (Speed_Set==6)
        {
            Max_Speed=240;
            Min_Speed=205;
            CS_P_Right = 0.34;  //����ϵ��
            CS_P_Left = 0.34;
            Set = 1;            //���PID
            Point_Num=2;        //��ǰϵ��
            CS_Point_Num=2;     //��ǰϵ��
        }
        /****************************      �ٶ�7��      ****************************/ 
        if (Speed_Set==7)
        {
            
            Max_Speed=240;
            Min_Speed=210;
            CS_P_Right = 0.34;  //����ϵ��
            CS_P_Left = 0.34;
            Set = 1;            //���PID
            Point_Num=2;        //��ǰϵ��2
            CS_Point_Num=2;     //��ǰϵ��2
        }
        /****************************      �ٶ�8��      ****************************/ 
        if (Speed_Set==8)
        {        
            Max_Speed=240;
            Min_Speed=213;      
            CS_P_Right = 0.34;  //����ϵ��
            CS_P_Left = 0.34;
            Set = 1;            //���PID
            Point_Num=2;        //��ǰϵ��2
            CS_Point_Num=2;     //��ǰϵ��0
        }    
        



        
   //     save_var(VAR2, Foresight);
        
}


/******** ���ٿ��� *********/
void Control_Speed(void)
{
 /****************************     ����     ****************************/ 
        if(Speed_Set<=8)                //����
        {
            if(Track_Line_Num==1)
            {
                Set_Speed +=1.7;   
            }
            else if (Foresight >= 18)
            {
                Set_Speed = Min_Speed; 
            }
            else if (Foresight >= 15)
            {
                Set_Speed = Min_Speed+2; 
            }
            else if (Foresight >= 13)
            {
                Set_Speed = Min_Speed+5; 
            }
            else if (Foresight >= 11)
            {
                Set_Speed = Min_Speed+7; 
            }
            else if (Foresight >= 9)
            {
                Set_Speed = Min_Speed+10; 
            }

            else if (Foresight >= 8)
            {
                Set_Speed -=0.7;
                if(Set_Speed>((Max_Speed+Min_Speed)/2))
                {
                        Set_Speed=Min_Speed+10;
                }
            }
            else
            {
                Set_Speed +=0.7;
            }
            
            
            if(Set_Speed>Max_Speed)Set_Speed=Max_Speed;
            if(Set_Speed<Min_Speed)Set_Speed=Min_Speed;
            High_Speed = Set_Speed;
        }
        /****************************     ����     ****************************/ 
        else                           //���� 
        {
            High_Speed = Max_Speed;  
        }
        
        /****************************************************************************/  
        /****************************      �µ����      ****************************/  
        /****************************************************************************/ 
        if(!Ramp_OFF_ON)
        {
            if (Ramp_Flag)
            {         
                    High_Speed=Ramp_Speed;
                    Ramp_Continue_Flag=0;  //�ر��µ����
            }
            if (Ramp_Time>Ramp_Delay)
            { 
                    Ramp_Flag=0;
            }
            if (Ramp_Time>250) //3S �� �����µ����
            { 
                    Ramp_Time=0;
                    Ramp_Continue_Flag=1; //�����µ����
            } 
        }
        /****************************      180��СԲ�����      ****************************/  
        if(Annulus_Small)
        {
            if(Speed_Set>3)  //���ٵ�λ180��СԲ������
            {
                High_Speed=Annulus_Speed;
            }
        }
        /****************************      �ϰ����ټ��      ***********************/ 
        if(Left_Hazard_Flag || Right_Hazard_Flag)
        {
            if(Speed_Set>2)
            {
                High_Speed=Annulus_Speed-10;
            }
            
        }       
        /****************************************************************************/  
        /****************************      ���ܿ���      ****************************/  
        /****************************************************************************/         
        if (!Go_Flag)
        {
                High_Speed=0;  
        }
        
        
        
  
}

/******** ������� *********/
void MOTOR_Control(void)
{       
        Left_MOTOR_Duty += PID_Realize(&Left_MOTOR_PID, Left_MOTOR_Speed, Left_High_Speed);//ʹ�ô�������ʽPID���е���
        Right_MOTOR_Duty += PID_Realize(&Right_MOTOR_PID, Right_MOTOR_Speed, Right_High_Speed);//ʹ�ô�������ʽPID���е���
        
        
        
//        if (Left_MOTOR_Duty>0)
//        {
//          Left_MOTOR_Duty = range_protect(Left_MOTOR_Duty, 0, 990);	//�޷�����
//          //�����ת
//          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR1_PWM,0);	//ռ�ձ����990������
//          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR2_PWM,Left_MOTOR_Duty);	   		//ռ�ձ����990������
//
//        }
//        else
//        {
//          Left_MOTOR_Duty = range_protect(-Left_MOTOR_Duty, 0, 990);
//          //�����ת
//          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR1_PWM,Left_MOTOR_Duty);	   			//ռ�ձ����990������
//          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR2_PWM,0);	//ռ�ձ����990������
//        }
//        if(Right_MOTOR_Duty>0)
//        {
//          Right_MOTOR_Duty = range_protect(Right_MOTOR_Duty, 0, 990);	//�޷�����
//          //�����ת
//          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR1_PWM,0);	//ռ�ձ����990������
//          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR2_PWM,Right_MOTOR_Duty);	   		//ռ�ձ����990������
//        }
//        else
//        {
//          Right_MOTOR_Duty = range_protect(-Right_MOTOR_Duty, 0, 990);	//�޷�����
//          //�����ת
//          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR1_PWM,Right_MOTOR_Duty);	   			//ռ�ձ����990������
//          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR2_PWM,0);	//ռ�ձ����990������
//        } 
        
        if (Left_MOTOR_Duty>0)
        {
          Left_MOTOR_Duty = range_protect(Left_MOTOR_Duty, 0, 990);	//�޷�����
          //�����ת
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR1_PWM,0);	//ռ�ձ����990������
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR2_PWM,Left_MOTOR_Duty);	   		//ռ�ձ����990������

        }
        else
        {
          Left_MOTOR_Duty = range_protect(Left_MOTOR_Duty, -990, 0);
          //�����ת
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR1_PWM,-Left_MOTOR_Duty);	   			//ռ�ձ����990������
          ftm_pwm_duty(MOTOR_FTM, Left_MOTOR2_PWM,0);	//ռ�ձ����990������
        }
        if(Right_MOTOR_Duty>0)
        {
          Right_MOTOR_Duty = range_protect(Right_MOTOR_Duty, 0, 990);	//�޷�����
          //�����ת
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR1_PWM,0);	//ռ�ձ����990������
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR2_PWM,Right_MOTOR_Duty);	   		//ռ�ձ����990������
        }
        else
        {
          Right_MOTOR_Duty = range_protect(Right_MOTOR_Duty, -990, 0);	//�޷�����
          //�����ת
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR1_PWM,-Right_MOTOR_Duty);	   			//ռ�ձ����990������
          ftm_pwm_duty(MOTOR_FTM, Right_MOTOR2_PWM,0);	//ռ�ձ����990������
        }
        
}

/******* ����ٶȲ��� ********/


void Scope_Display(void)
{
        int32 var[4];
        var[0]=Right_MOTOR_Speed;
        var[1]=Right_High_Speed;
        var[2]=Left_MOTOR_Speed;
        var[3]=Left_High_Speed;
        
        vcan_sendware(var, sizeof(var));
}

/*
*	����ƽ���˲�
*
*	˵�����Ƚ��ȳ���ȡƽ��ֵ
*/


int32 Left_Value_Buf[N];
int32 Right_Value_Buf[N];

int32 N32_Average_Filter(int32 Data, int32 *Pointer)
{
    uchar i=0;
    int32 Temp=0;
    int32  Sum=0;

    for(i=0;i<(N-1);i++)           //���ݵ���
    {
       Temp = *++Pointer;       //Pointer++�����ú����ӣ�����ע��
       *--Pointer = Temp; 
       Pointer++;
    }
    *Pointer = Data;
    Pointer = Pointer-(N-1);
    
    for(i=0;i<N;i++)
    {
        Sum += *Pointer++;
    }
    return (int32)(Sum/N);
}

/*
*	�����ж�
*
*	˵����1,�����ֱ����2��ֱ���С�3��ֱ���������4�������
*/

void Track_Line(void)
{
    uchar i=0;
    int32 Temp=0;
    uchar  Num=0;
    
    Fictitious_Beeline_Num = Fictitious_Beeline();
     Track_Line_Time++;
     if(Track_Line_Time==5)
     {
        Track_Line_Time=0;
        Last_Error[0] = Last_Error[1];
        Last_Error[1] = Last_Error[2];
        Last_Error[2] = Last_Error[3];
        Last_Error[3] = Last_Error[4];
        Last_Error[4] = Point_Error;
     }
     
     if(Last_Error[0]>5 && Last_Error[1]>5 && Last_Error[2]>5 && Fictitious_Beeline_Num<30 && Line_Count<25) //�����ֱ��
     {
            Track_Line_Num = 1;
     }
     else if(Last_Error[0]<5 && Last_Error[1]<5 && Last_Error[2]<5 && Foresight<=5) //ֱ��
     {
            Track_Line_Num = 2;
     }
     else if(Last_Error[0]<5 && Last_Error[1]<5 && Last_Error[2]<5 && Foresight>=5) //ֱ�������
     {
            Track_Line_Num = 3;
     }
     else                                                                          //�����
     {
            Track_Line_Num=4;
     }
    
}

/*
*	����ֱ��
*
*	˵������������ֱ�ߣ�����������֮�Ƚϣ��������ƶ�
*/

int16 Fictitious_Beeline(void)
{
    float KA,KB;
    uchar End;
    uchar i;
    int16 Sum=0;
    int16 Num;
    float res;
	uchar Result;
	
	
    
    
    End = 59 - (59-Line_Count)/3;
    if((End%2)==0)End=End-1;         //ͼ����д���������Ҫע��
    
    KA = 1.0*(Mid_Line[59] - Mid_Line[End]) / (59-End);
    KB = 1.0*Mid_Line[End] - (KA * End);
    
    for(i=59;i>Line_Count+2;i-=2)
    {
        res = i * KA + KB;
	    Result = range_protect((int32)res, 1, 79);
        Num = Mid_Line[i]- Result;
        Num = ABS(Num);
        Sum+=Num;
    }
    
    return Sum;
}


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
//             DirectionControl(Point);

}
void Starting_Check(void)
{
        if (Starting_Time==0 && !Go_Flag && !Stop_Flag)
        {
                Go_Flag=1;
                Starting_Time=60;  //���ܿ�ʼ�رջ�·���
        }
        if (Starting_Time==0 && Go_Flag && !Annulus_Check)
        {
                Point_Mid=40;
                Annulus_Check=1;   //������·�ϰ����
                Obstacle_Check=1;
        }
        if (Stop_Flag && Go_Flag)
        {
              if (Starting2_Time==0)   
              {
                  Starting2_Time=55;  //��⵽�������Ժ���ʱͣ�� 
                  if(Hazard_Left_Right)Point_Mid=Point_Right;
                  else Point_Mid=Point_Left;
              }
              

              if (Starting2_Time==10)
              {
                Go_Flag=0;
                Point_Mid=40;
            //    Starting_Time=-10;
              }
        }

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