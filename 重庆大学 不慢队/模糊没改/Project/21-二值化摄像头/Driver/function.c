#include "common.h"
#include "include.h"

     //�����жϸ�λ����

void PORTC_IRQHandler();
void DMA0_IRQHandler();
void PIT_IRQHandler();

void init()
{
   OLED_Init();
   button_init(); 
   switch_init();
 //  BEEP_ON;
   OLED_Draw_Logo();
   led_init(); 
   led_flash(); 
   adc_init (ADC0_SE14);// ��ص�ѹ�����ӿ� װ������Ϊ3.3/65535*5.7 
   
   pit_init_ms(PIT0,2); //2ms��ʱ�ж�
 
   set_vector_handler(PIT0_VECTORn ,PIT_IRQHandler);
   EnableInterrupts; //���ж�
   
   EEPROM_init();
   I2C_Init();
   
     //�����PWM Ƶ��Ϊ10khz ռ�ձȾ���Ϊ10000 
   FTM_PWM_init(FTM0,FTM_CH0,10*1000,0);   
   FTM_PWM_init(FTM0,FTM_CH1,10*1000,0);  
   FTM_PWM_init(FTM0,FTM_CH2,10*1000,0);  
   FTM_PWM_init(FTM0,FTM_CH3,10*1000,0);   

   FTM_QUAD_Init(FTM1); //���������ʼ��; //
   FTM_QUAD_Init(FTM2); //���������ʼ��; //
  
   //BEEP_OFF;
   
   OLED_CLS();
 
    
   camera_init();
   set_vector_handler(PORTC_VECTORn ,PORTC_IRQHandler);    
   set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);   
        enable_irq(PORTC_IRQn); 
   SD_OK= SD_Initialize();
   if(SD_OK) LED_BLUE_ON;
   OLED_CLS();
    
   set_vector_handler(UART0_RX_TX_VECTORn,UART0_RX_IRQHandler);
     
   uart_rx_irq_en(UART0);
   NVIC_SetPriority(UART0_RX_TX_IRQn,0);
 
   enable_irq (PIT0_IRQn); 
}
/* ��ʼ
void Para_Init()
{
 PID_ANGLE.P=0.10;   //�Ƕȿ��Ʊ�������
 PID_ANGLE.D=0.001;  //�Ƕȿ���΢�ֲ���
 PID_SPEED.P=1.5;    //�ٶȿ��Ʊ�������
 PID_SPEED.I=0.05;   //�ٶȿ���΢�ֲ���
 PID_TURN.P=0.01;
 PID_TURN.D=0.005; 
 Fuzzy_Kp=0.005;
 Fuzzy_Kd=0.0005;
 SetSpeed=3.8;
 Set_Angle=50;
}
*/

void Para_Init()
{
//   PID_ANGLE.P=0.5;   //ֱ����pid   pwm  2000
//   PID_ANGLE.D=0.001;//���˻ᶶ
   
    PID_ANGLE.P=0.23;   //ֱ����pid
   PID_ANGLE.D=0.0038;//���˻ᶶ  
   
   PID_SPEED.P=1.0;//�ٶȵ�pid
   PID_SPEED.I=0.01;   
   
   PID_TURN.P=0.035;//ת���pid
   PID_TURN.D=0.007; 

   Fuzzy_Kp=0.005;
   Fuzzy_Kd=0.0005;
   SetSpeed=2.5;
   Set_Angle=50;
}

//530
//void Para_Init 
//{
//   PID_ANGLE.P=0.05;
//   PID_ANGLE.D=0.001;
//   PID_SPEED.P=0;
//   PID_SPEED.I=0.001;   
//   PID_TURN.P=0.1;
//   PID_TURN.D=0.005; 
//   Fuzzy_Kp=0.005;
//   Fuzzy_Kd=0.0005;
//   SetSpeed=3.8;
//   Set_Angle=30;
//}

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
