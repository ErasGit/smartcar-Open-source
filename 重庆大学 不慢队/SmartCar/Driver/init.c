#include "include.h"
void Para_Init();
void GPIO_Init();
void Fix_Offset();

void All_Init()
{   
   GPIO_Init(); 
   Read_Switch();

   ftm_pwm_init(FTM0, FTM_CH4, 10000,0);  //PWM��ʼ��
   ftm_pwm_init(FTM0, FTM_CH5, 10000,0);  
   ftm_pwm_init(FTM0, FTM_CH6, 10000,0);  
   ftm_pwm_init(FTM0, FTM_CH7, 10000,0); 
   adc_init(ADC1_SE14);//��ѹ�ɼ�
   adc_init(ADC1_SE15);//��Ųɼ�1
   adc_init(ADC1_SE12);//��Ųɼ�2
   pit_init_ms(PIT0,1); //1ms��ʱ�ж�
   Para_Init();         //������ʼ��
   ftm_quad_init(FTM1);  //���������ʼ��
   ftm_quad_init(FTM2);  
   OLED_Init();  
   I2C_Init();
   Fix_Offset();

}

//������ֵ��ʼ��
void Para_Init()
{
 PID_ANGLE.P=0.1;
 PID_ANGLE.D=0.0007;
 PID_SPEED.P=1;
 PID_SPEED.I=0.02;
 PID_TURN.P=0.030;
 PID_TURN.D=0.015; 
 Fuzzy_Kp=0.005;
 Fuzzy_Kd=0.0005;
 SetSpeed=2.5;
 Set_Angle=10; /////////////
 //Acc_Offset=140;
 Hill_Slow_Ratio=0.4;
}

//GPIO��ʼ��
void GPIO_Init()
{
  //��ص�ѹ
   //gpio_init(PTB10,GPO,0);
 /* //����
  lptmr_pulse_init(LPT0_ALT2,0xFFFF,LPT_Rising);
     
  */
  //���뿪��
  gpio_init(PTB2,GPI,0);                   
  port_init_NoALT(PTB2,PULLUP);
  gpio_init(PTB3,GPI,0);                     
  port_init_NoALT(PTB3,PULLUP);
  gpio_init(PTB4,GPI,0);                     
  port_init_NoALT(PTB4,PULLUP);
  gpio_init(PTB5,GPI,0);                     
  port_init_NoALT(PTB5,PULLUP);
  
  //�������� 
  gpio_init(PTA9,GPI,0);               //"O" ȷ��
  port_init_NoALT(PTA9,PULLUP); 
  gpio_init(PTA10,GPI,0);              //"left"       
  port_init_NoALT(PTA10,PULLUP);
  gpio_init(PTA11,GPI,0);              //"up"
  port_init_NoALT(PTA11,PULLUP);
  gpio_init(PTA12,GPI,0);              //"down"       
  port_init_NoALT(PTA12,PULLUP);
  gpio_init(PTA13,GPI,0);              //"X" ȡ��      
  port_init_NoALT(PTA13,PULLUP);
  gpio_init(PTA14,GPI,0);              //"right"
  port_init_NoALT(PTA14,PULLUP);


  
 // LED��ʼ��&��չ�ӿڳ�ʼ��
  gpio_init(PTE10,GPO,0);  //extern
  gpio_init(PTE11,GPO,0);
  gpio_init(PTE12,GPO,0);   
}
//����������ƫ��
void Fix_Offset()
{
  int Offset_X_Sum=0,Offset_Y_Sum=0;  //ƫ���ۻ�
  int i; 
  LED_GREEN_ON; 
  LED_RED_ON;
  LED_BLUE_ON;
  DELAY_MS(1000); 
  for(i=0;i<100;i++)
  {
   Offset_X_Sum+= Get_X_Gyro();
   DELAY_MS(5); 
   if(i%10==0)LED_RED_TURN;
   Offset_Y_Sum+= Get_Y_Gyro();
   DELAY_MS(5);
  } 
  Gyro_X_Offset=(int)(Offset_X_Sum/100.0);
  Gyro_Y_Offset=(int)(Offset_Y_Sum/100.0);
}
