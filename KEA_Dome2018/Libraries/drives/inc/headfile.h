
#ifndef _headfile_h
#define _headfile_h


#include "common.h"
#include "KEA128_port_cfg.h"
#include "stdio.h"

//--------������--------
#include "KEA128_ftm.h"
#include "KEA128_pit.h"
#include "KEA128_gpio.h"
#include "KEA128_port.h"
#include "KEA128_adc.h"
#include "KEA128_irq.h"
#include "KEA128_uart.h"
#include "KEA128_kbi.h"
#include "KEA128_flash.h"
#include "KEA128_systick.h"
#include "KEA128_i2c.h"
#include "KEA128_spi.h"




#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_FUN.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_NRF24L01.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_TSL1401.h"


/*������GPIO�궨��*/ //һ�����ݱ�����
#define ACC_Z           adc_once(ADC0_SE0, ADC_10bit)
#define ANGLE           adc_once(ADC0_SE1, ADC_10bit)
#define GYRO_Z          adc_once(ADC0_SE2, ADC_10bit)
#define GYRO_X          adc_once(ADC0_SE3, ADC_10bit)

#define L1              adc_once(ADC0_SE4, ADC_10bit)//14
#define L2              adc_once(ADC0_SE5, ADC_10bit)
#define L3              adc_once(ADC0_SE6, ADC_10bit)
#define L4              adc_once(ADC0_SE7, ADC_10bit)


//�˴�Ӧ�޸�  ����ʵ��Ӳ��  �޸Ĺܽ� ���� CarBalanceSystemInit���ʼ��ʹ�õ�������
#define DOWN            gpio_get(I0) //DOWN
#define LIFE            gpio_get(I0)//LIFE 
#define RIGHT           gpio_get(I0) //Right
#define UP              gpio_get(I0) //UP
#define ENTER           gpio_get(I0) //ENTER


#define SW6             gpio_get(I0) //BM6
#define SW5             gpio_get(I0)//BM5
#define SW4             gpio_get(I0) //BM4
#define SW3             gpio_get(I0) //BM3
#define SW2             gpio_get(I0) //BM2
#define SW1             gpio_get(I0) //BM1



#define PWMMAX          2000//1800
#define PWMMIN          0//200

 
  
typedef struct { //PID�ṹ����������
  
  float Kp;
  float Ki;
  float Kd;
  float dt;
  float err;
  float err_k[10];
  float next_err;
  float last_err;
  float offset;
  float llast_err;
  float integral; 
  float out;
  float out_k[10];
 
}PID_Type_Def;

typedef struct { //������ ���ټƽṹ�� ��������
  
  int16 X_Value[3]; 
  int16 X_Final;//X����ֵ
  int16 X_Last;
  int16 X_Offset;//X��̬ƫ��
  
  int16 Y_Value[3];
  int16 Y_Final;
  int16 Y_Last;
  int16 Y_Offset;
  
  int16 Z_Value[3];
  int16 Z_Final;
  int16 Z_Last;
  int16 Z_Offset;

}Acc_Type_Def,Gyro_Type_Def;

typedef struct { //�ǶȽṹ����������
  
  int16 PiontValue;//Ŀ��ֵ
  int16 ActualValue[3];//ʵ��ֵ  123��
  int16 FinalValue; //����ֵ  ��ʵ��ֵ�˲�������
  int16 LastValue;

  
}Angle_Type_Def;

typedef struct { //PWM�ṹ����������
  
  
  int16 PiontValue;
  int16 ActualValue;
  int16 LiftValue;
  int16 RightValue;
  int16 Differential;
  
  
}PWM_Type_Def;

typedef struct { //�ٶȽṹ����������
  
  int16 PiontSpeed;       //�ٶ�Ŀ��
  int16 PiontSpeedMax;
  int16 PiontSpeedMin;
  int16 PiontDifferential;//����Ŀ��
  int16 PiontLiftPulse;  //������Ŀ��ֵ
  int16 PiontRightPulse; //������Ŀ��ֵ
  
  int16 ActualSpeed[3];
  int16 ActualDifferential[3];
  int16 ActualLiftPulse[16];//������ʵ��ֵ
  int16 ActualRightPulse[16];//������ʵ��ֵ
  int16 ActualInstantaneous[3];//ʵ��˲ʱֵ
  int16 ActualAcceleration;//ʵ�ʼ��ٶ�
  
  uint32 ActualDistance;//ʵ��·��
  uint32 FaultDistance;//����·��
  int16  SlowDistance;//���پ���  һ�� 3000  3��
  int16  AccDistance;
  
}Speed_Type_Def;

typedef struct { //��нṹ����������
  
  int16 LeftValue[3];
  int16 LeftFinal;
  int16 LeftMax;
  int16 LeftMin;
  
  int16 RightValue[3];
  int16 RightFinal;
  int16 RightMax;
  int16 RightMin;
  
  int16 AndValues;//��ֵ
  int16 LeftDerivative;//�����仯��
  int16 RightDerivative;

  
}Inductor_Type_Def;

typedef struct { //�����ṹ����������
    
  int8 Up[10];
  int8 Up_Final;
  
  int8 Down[10];
  int8 Down_Final;
  
  int8 Life[10];
  int8 Life_Final;
  
  int8 Right[10];
  int8 Right_Final;
  
  int8 Enter[10];
  int8 Enter_Final;
  
}Button_Type_Def;

typedef struct { //���뿪�ؽṹ����������
  
  int8 Bm1[3];
  int8 Bm1_Final;
  int8 Bm2[3];
  int8 Bm2_Final;
  int8 Bm3[3];
  int8 Bm3_Final;
  int8 Bm4[3];
  int8 Bm4_Final;
  int8 Bm5[3];
  int8 Bm5_Final;
  int8 Bm6[3];
  int8 Bm6_Final;
  
}Switch_Type_Def;

typedef struct { //��־λ�ṹ����������
  
  uint8 Flag[10];
  
}RUN_Type_Def, Ramp_Type_Def, OUT_Type_Def, Stop_Type_Def, Curve_Type_Def;



#endif
