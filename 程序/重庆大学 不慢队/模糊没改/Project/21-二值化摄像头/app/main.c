/*
 * @file       main.c
 * @brief      ������
 */

#include "common.h"
#include "include.h"
#include "function.h"

/*************
��������ʾ�����ſƴ�����ͷƽ�⳵������
����˵����

����ͷ��
 SCCB_SCL        PTC17
 SCCB_SDA        PTC16
 PCLK            PTC12
 ���ж�          PTC13
 ���ݿ�          PTD0- PTD7

oled ���� ��oled12864.h�ж��壺
#define RST    PTA16
#define DC     PTB2
#define D0     PTB9
#define D1     PTB3

���ڣ�  ��PORT_cfg.h  �ж���
������ 115200
#define UART0_RX    PTA15       
#define UART0_TX    PTA14  

6.1����
**************/

/*!
 *  @brief      ����ͼ����λ����ʾ  
 */
void sendimg(uint8 *imgaddr, uint32 imgsize)
{
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0xac);
  my_putchar(8);
  //һ��800������λ
  uart_putbuff(UART_PORT, imgaddr, imgsize); //����ͼ��
  uart_putbuff(UART_PORT, (uint8*)(&LMR[0][0]),     180); //���ͱ��߼�����
  uart_putbuff(UART_PORT, (uint8*)(&LMR[0][0]),     20); //Ԥ��20������λ
  my_putchar(0x0c);
}



void  main(void)
{ 
  init();
 // OLED_Draw_camera();
  Para_Init();
 
   while(1)
    {
//       if(beep)            //�ε���      //13512254304  6289y3
//      {
//        BEEP_ON;
//        DELAY_MS(50);
//        BEEP_OFF;
//        beep=0;
//      }  
//       Read_Switch();
//
//      if(SendPara)   //�Ĳ��� ����..
//      {
//        SendPara=0;
//        Send_Parameter();
//      }
//      
//      if(SendSD)
//     {
//       Send_Begin();
//       Send_SD();
//       SendSD=false;
//      }
////      
//       if((SD_Save||SD_Write_info)&&SD_OK)
//      { 
//        Variable_update();
//        if(SD_Save) Record();
//        else
//        {
//          Write_Information();
//          SD_Write_info=0;
//        }
//        
//      }
      
      
      Check_BottonPress();//��ⰴ��
      
//      if(Uart_Send) //���뿪��2���ϲ� �������� 1
//      {
//       Send_Begin();
//       Variable_update();
//       Send_Variable();
//       sendimg(imgbuff,CAMERA_SIZE);
//       //printf("\n���ո���");
//      }
      if(OLED_Refresh)
      {
        OLED_Draw_UI();
      }      
     } 
}
void PIT_IRQHandler()  //2msһ���ж�
{
  static uint8 flag_100ms,cnt=0;
  PIT_Flag_Clear(PIT0);       //���жϱ�־λ 
  flag_100ms++;
  if(flag_100ms>50)
  {
   flag_100ms=0;  
   Speed_Control();  //100ms����һ���ٶȿ���
  
   LED_RED_TURN;
   SpeedCount=0;
  }
   if(imgprocessed==0)  //�˶β�����0.5ms 200ms��Ƶ  6ms ��8ms ִ��һ��
   {
      img_extract(img,imgbuff,CAMERA_SIZE);
      Search();
      Direction_Control();
      imgprocessed=1;
   }
   cnt++;
  
   if(cnt==1)      //4ms����һ��
   {
     Get_Attitude(); 
     Angle_Calculate();
     Angle_Control(); 
     Get_Speed();
   }
   if(cnt>=2)
   {
     cnt=0;
   }
   SpeedCount++;
   Speed_Control_Output();
   Moto_Out();
}






//
//void  main(void)
//{ 
//// init();
//OLED_Init();
////   FTM_PWM_init(FTM0,FTM_CH0,200,10);   
////   FTM_PWM_init(FTM0,FTM_CH1,10*1000,0);  
////   FTM_PWM_init(FTM0,FTM_CH2,200,10);  
////  FTM_PWM_init(FTM0,FTM_CH3,10*1000,0); 
// //while(1){
//    OLED_Draw_Logo();
//    DELAY_MS(5000);
//    OLED_Draw_Logo1();
//      DELAY_MS(5000);
//// FTM_PWM_Duty(FTM0,FTM_CH2,100); //ռ�ձȾ���Ϊ10000  20 31
// // FTM_PWM_Duty(FTM0,FTM_CH0,0);
//     
// // FTM_PWM_Duty(FTM0,FTM_CH3,5000); 
////   FTM_PWM_Duty(FTM0,FTM_CH1,0);
// //}
//}
