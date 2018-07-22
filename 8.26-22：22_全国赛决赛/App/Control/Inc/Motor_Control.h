#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

#include "common.h"
#include "include.h"

//�������˿�		//PTC1~	PTC4
#define MOTOR_FTM   FTM3
#define Left_MOTOR1_PWM  FTM_CH6     //PTC3
#define Left_MOTOR2_PWM  FTM_CH7     //PTC4
#define Right_MOTOR1_PWM FTM_CH4     //PTC1
#define Right_MOTOR2_PWM FTM_CH5     //PTC2
#define MOTOR_HZ    15000	//����ģʽ�£�Ƶ��Ӧ���� 30~100��ѡ��20kHz
						//����ģʽ�£�Ƶ��Ӧ���� 20k ����

#define N 6		

extern int32 Left_MOTOR_Duty;       //�����
extern int32 Right_MOTOR_Duty;       //�����
extern int32 Left_MOTOR_Speed;
extern int32 Right_MOTOR_Speed;
extern uint8 Run_Flag,Ramp_Flag;
extern uint8 Game_End;
extern uint8 Stop_Flag;
extern uint8 BangBang_Flag;
extern uint8 Point;
extern int16 High_Speed;
extern int16 Ramp_Speed;
extern char Set,Speed_Set;
extern uint16 Ramp_Delay; 
extern float g_fDirectionControlOut;
extern int32 Left_Value_Buf[N];
extern int32 Right_Value_Buf[N];
extern int32 Left_High_Speed,Right_High_Speed;

extern int16 Fictitious_Beeline_Num;
extern uchar  Track_Line_Num;
extern uchar  Last_Error[5],Point_Error;
extern uint8  Ramp_OFF_ON;

extern uint8 Ramp_Continue_Flag;     //��⵽�µ��Ժ�ڶ��ο�ʼ����µ��ı�־

//�������˿�		//PTC9		
#define S3010_FTM FTM0
#define S3010_CH  FTM_CH0
#define S3010_HZ  50

//#define S3010_MID 4750	//  4750
//#define S3010_LEFT 5180	//  5180
//#define S3010_RIGHT 4320	//  4320
#define S3010_MID 4750	//  4750
#define S3010_LEFT 5180	//  5180
#define S3010_RIGHT 4320	//  4320

extern uint8   Point_Mid;//Ŀ��������





/*********** �������� ************/
void Adjust_Speed(void);
void Control_Speed(void);
void speed_measure(void);		//����ٶȲ���
void MOTOR_Control(void);	//����ʽPID���ص���ٶ�
void Starting_Check(void);
void Scope_Display(void);
int32 N32_Average_Filter(int32 Data, int32 *Pointer);
int16 Fictitious_Beeline(void);
void Track_Line(void);
int32 range_protect(int32 duty, int32 min, int32 max); //�޷�����

#endif /* __CONTROL_H__ */