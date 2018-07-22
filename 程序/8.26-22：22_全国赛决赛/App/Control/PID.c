#include "PID.h"


PID S3010_PID, Left_MOTOR_PID, Right_MOTOR_PID, CS_PID;	//�������͵����PID�����ṹ��
//int32 S3010[2][4] = {{17, 0, 23, 25},{ 20, 1, 0, 19}}; //0-2.55, 1-2.
//int32 S3010[5][4] = {{ 16, 0, 23, 20},{18, 0, 23, 20},{ 20, 0, 23, 20},{ 12, 0, 23, 20},{ 20, 0, 23, 20},}; //0-2.55, 1-2.

int32 S3010[5][4] = {{ 17, 0, 23, 20},{18, 0, 23, 20},{ 20, 0, 23, 20},{ 12, 0, 23, 20},{ 20, 0, 23, 20},}; //0-2.55, 1-2.


/* PID �� I ��û��ʹ�ã�ֻ�� PD6�˴� I ����Ϊ�ٶ�ģʽ�ı�ţ���PID��Ӱ�� */					
//��ǰ������Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
int32 CS[2][4] = {{ 17, 0, 23, 20},{18, 0, 23, 20}};

//float   Left_MOTOR[3] = {7.8, 0.96, 3.6};	//���PID
//float   Right_MOTOR[3] = {7.8, 0.96, 3.6};	//���PID
//
float   Left_MOTOR[3] = {7.8, 2.96, 3.6};	//���PID
float   Right_MOTOR[3] = {7.8, 2.96, 3.6};	//���PID

//float   Left_MOTOR[3] = {10.0, 2.5, 1.6};	//���PID
//float   Right_MOTOR[3] = {10.0, 2.5, 1.6};	//���PID

//float   Left_MOTOR[3] = {5.8, 2.96, 2.6};	//���PID
//float   Right_MOTOR[3] = {5.8, 2.96, 2.6};	//���PID

//float   Left_MOTOR[3] = {3.5, 2.8, 0.2};	//���PID
//float   Right_MOTOR[3] = {3.5, 2.8, 0.2};	//���PID

//float   Left_MOTOR[3] = {2.5, 4.5, 0.8};	//���PID
//float   Right_MOTOR[3] = {2.5, 4.5, 0.8};	//���PID

//float   Left_MOTOR[3] = {2.5, 0.5, 0.8};	//���PID
//float   Right_MOTOR[3] = {2.5, 0.5, 0.8};	//���PID
//λ��ʽPID������ʼ��
void PlacePID_Init(PID *sptr)
{
	sptr->SumError = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]
}
//����ʽPID������ʼ��
void IncPID_Init(PID *sptr, float *MOTOR_PID)
{
	sptr->SumError = 0;
	sptr->LastError = 0;	//Error[-1]
	sptr->PrevError = 0;	//Error[-2]	
	sptr->LastSpeed = 0;
        sptr->KP = *MOTOR_PID;
        sptr->KI = *(MOTOR_PID+1);
        sptr->KD = *(MOTOR_PID+2);
        
}

//λ��ʽ���ٿ���
int32 PlacePID_CS(PID *sprt, int32 NowPiont, int32 SetPoint)
{
	//����Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
	int32 iError,	//��ǰ���
		  Actual;	//���ó���ʵ�����ֵ
	float	Kp;		//��̬P

	
	iError = SetPoint - NowPiont;	//���㵱ǰ���
	Kp = 1.0 * (iError*iError) / CS[Set][T] + CS[Set][P];	//Pֵ���ֵ�ɶ��κ�����ϵ
	
	Actual = Kp * iError + CS[Set][D] * (iError - sprt->LastError);//ֻ��PD
	
	sprt->LastError = iError;		//�����ϴ����

	
	return S3010_MID-Actual;
}

//λ��ʽPID�������
int32 PlacePID_Control(PID *sprt, int32 NowPiont, int32 SetPoint)
{
	//����Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
	int32 iError,	//��ǰ���
		  Actual;	//���ó���ʵ�����ֵ
	float	Kp;		//��̬P

	
	iError = SetPoint - NowPiont;	//���㵱ǰ���
	Kp = 1.0 * (iError*iError) / S3010[Set][T] + S3010[Set][P];	//Pֵ���ֵ�ɶ��κ�����ϵ
	
	Actual = Kp * iError + S3010[Set][D] * (iError - sprt->LastError);//ֻ��PD
	
	sprt->LastError = iError;		//�����ϴ����

	
	return S3010_MID-Actual;
}
int32 PID_Cascade(PID *sptr, int32 ActualSpeed, int32 SetSpeed)
{
	register int32 iError,	    //��ǰ���
					Increase;	//���ó���ʵ������
	iError = SetSpeed - ActualSpeed;//���㵱ǰ���
	
	Increase =  -sptr->KP * (ActualSpeed - sptr->LastSpeed)	//���ٶ�
			   + sptr->KI * iError
			   + sptr->KD * (iError - 2 * sptr->LastError + sptr->PrevError);
	
	sptr->PrevError = sptr->LastError;	//����ǰ�����
	sptr->LastError = iError;		  	//�����ϴ����
	sptr->LastSpeed = ActualSpeed;		//�����ϴ��ٶ�
	
//	if (iError < -100)
//	{
//		Increase -= 400;
//	}
//	else if (iError < -80)
//	{
//		Increase -= 300;
//	}
//	else if (iError < -50)
//	{
//		Increase -= 250;
//	}
//	else if (iError < -30)
//	{
//		Increase -= 200;
//	}	
	return Increase;
        
}	


//����ʽPID�������    
int32 PID_Realize(PID *sptr, int32 ActualSpeed, int32 SetSpeed)
{
	//��ǰ������Ϊ�Ĵ���������ֻ���������ͺ��ַ��ͱ�������������ٶ�
//	register int32 iError,	    //��ǰ���
//					Increase;	//���ó���ʵ������
	
  	int32 iError,	    //��ǰ���
					Increase;	//���ó���ʵ������
  
	iError = SetSpeed - ActualSpeed;//���㵱ǰ���
	
	Increase = sptr->KP * (iError - sptr->LastError)
			 + sptr->KI * iError
			 + sptr->KD * (iError - 2 * sptr->LastError + sptr->PrevError);
	sptr->PrevError = sptr->LastError;	//����ǰ�����
	sptr->LastError = iError;		  	//�����ϴ����
	
//	if (iError < -50)
//	{
//		BangBang_Flag = 1;
//	}
//	else if (iError < -50 && BangBang_Flag)
//	{
//		BangBang_Flag = 0;
//		Increase -= 300;
//	}
//    if (iError > 100)
//	{
//		Increase +=  800;
//	}
//    else if (iError > 50)
//    {
//        Increase +=  400;
//    }
////	else if (iError < -40)
////	{
////		BangBang_Flag = 1;
////		Increase -= 200;
////	}
//
//        
//        if (iError > 100)
//	{
//		Increase +=  600;
//	}
////	else if ((iError > 40) && (BangBang_Flag == 1))
////	{
////		BangBang_Flag = 0;
////		Increase +=  400;
////	}
//	else if (iError > 50)
//	{
//		Increase += 300;
//                BangBang_Flag = 1;
//	}
	
	return Increase;
}



