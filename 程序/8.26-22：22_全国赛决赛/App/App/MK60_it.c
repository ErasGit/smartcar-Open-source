/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       MK60_it.c
 * @brief      ɽ��K60 ƽ̨�жϷ�����
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */

#include    "MK60_it.h"
#include    "common.h"
#include    "include.h"

/*********************************�жϷ�����********************************/

/*!
 *  @brief      UART4�жϷ�����
 *  @since      v5.0
 */
void uart4_handler(void)
{
    char str[2];

    if(uart_querychar (UART4, str) != 0)   //�������ݼĴ�����
    {
        //�û���Ҫ�����������
        /*if (str[0] == 1)
		{
			Run_Flag = 1;
		}
		else if (str[0] == 2)
		{
			Run_Flag = 0;
		}*/
    }
}
/*!
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    //while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif

}

void PORTE_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    flag = PORTE_ISFR;
    PORTE_ISFR  = ~0;                                   //���жϱ�־λ

    n = 27;
    if(flag & (1 << n))                                 //PTE27�����ж�
    {
        nrf_handler();
    }
}

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
//    led(LED_MAX,LED_ON);            //�����߼�����

    camera_dma();
    camera_get_img();	//�ȴ�ͼ������ϣ���ɱ�־��Ϊ��ʼ��־��
    flag_imgbuff =! flag_imgbuff;
    ov7725_eagle_img_buff = (flag_imgbuff == 0?p_imgbuff1:p_imgbuff2);//�����ʼ��ͼ���ַ

    flag_imgbuff != 0?img_extract(img, p_imgbuff1,CAMERA_SIZE):img_extract(img, p_imgbuff2,CAMERA_SIZE); 
    image_processing(img);            //ͼ���㷨����313u   
    Point = Dynamic_Point_Weight(Point_Num);		//��ֵ ͼ������ƽ�� ��ǰ���	����Ϊż���������
    S3010_Control();                  //�������
//    speed_measure();	              //����
//    MOTOR_Control();                //�������
//    led(LED_MAX,LED_OFF);            //�����߼�����
}

/*!
*  @brief      PIT0�жϷ�����
*  @since      v5.0
*/
extern uchar Annulus_Flag;
extern uint16 Annulus_DelayMs;
extern int16 Starting_Time;
extern int8 Starting2_Time;
extern uint8 Ramp_Flag;
extern uint16 Ramp_Time;
extern uint8 Hazard_Time;
extern uchar Left_Hazard_Flag;
extern uchar Right_Hazard_Flag;

void PIT0_IRQHandler()
{	
    static uchar Time_150ms = 0;
/******** ������ʱ *********/      
    Time_150ms++;
    if (Starting_Time>0)
    {
          Starting_Time--;
    }
    if (Starting2_Time>0)
    {
          Starting2_Time--;
    }
/******** �µ���ʱ *********/  
    if (Ramp_Flag || Ramp_Time)
    {
        Ramp_Time++;
    }
    
 /******** �ϰ���ʱ *********/  
//     if (Left_Hazard_Flag || Right_Hazard_Flag)
//     {
//        Hazard_Time++;
//        if(Hazard_Time>200)
//        {
//            Left_Hazard_Flag=0;
//            Right_Hazard_Flag=0;
//        }
//     }   
/******** ͼ����ʱ *********/ 
    if (Time_150ms == 15)
    {
      Time_150ms = 0;
      Send_OK = 1;	// ͼ��150ms����һ��
    }
/******** ��·��ʱ *********/ 
//    if (Annulus_Flag)
//    {
//       Annulus_DelayMs++;
//    }
    if (Annulus2_Delay)
   {
        Annulus2_Delay--;
    }
/******** ���ܼ�� *********/ 
    Starting_Check(); 
/******** ������� *********/    
    Track_Line();
/******** �ٶȿ��� *********/ 
    Adjust_Speed(); 
/******** ���ٿ��� *********/ 
    Control_Speed();
/******** ���ٿ��� *********/ 
    CS_Control();
/******** �ٶȼ�� *********/ 
    speed_measure();	//���ټ�ͣ��	�����ڼ���ʱ����
/******** ������� *********/ 
    MOTOR_Control();
/******** ������ʾ *********/ 
//    Scope_Display();
    PIT_Flag_Clear(PIT0);       //���жϱ�־λ
}


/*!
*  @brief      PIT1�жϷ�����
*  @since      v5.0
*/

//void PIT1_IRQHandler()
//{
//    PIT_Flag_Clear(PIT1);       //���жϱ�־λ
//    if (Annulus_Flag==1)
//    {
//        led(LED_MAX,LED_ON);// ��LEDָʾ��		ȫ����ʼ���ɹ�
////        Track_Line_Num=0;
//    }
//    else
//    {
//        led(LED_MAX,LED_OFF);// ��LEDָʾ��		ȫ����ʼ���ɹ�
//    }
//}

/******************************�жϷ���������***********8*******************/


