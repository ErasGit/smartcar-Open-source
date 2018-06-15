/*
  * @file       Servo motor control.c
  * @brief      ���������غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-16
*/
#include "common.h"
#include "Servo_motor_control.h"
#include "Servo_motor.h"
#include "Senser.h"
#include "Switch.h"
#include "MKL_gpio.h"
#include "Motor.h"
#include "Buzzer.h"
#include "Fuzzy.h"
#include "OLED.h"
#include "VCAN_LED.H"
#include "Gyroscope.h"

extern uint16 sensor_value[6];
extern uint8 loss_line;
extern int16 speed_set;
extern int32 var[5];
extern float position_accumulative[15];
extern uint8 speed_rank;

extern float fuzzy_kp;          //ģ��������Ľ����KPֵ
extern float fuzzy_kd;          //ģ��������Ľ����KDֵ

struct SERVO_PID_MESSAGE              //���PID����
{
	float error_now;
	float error_last;
        float error_deviation;
        float fuzzy_kp_gain;
        float fuzzy_kd_gain;
	float kp;
        float kd; 
	float output;
} Servo_pid;

/*
 *  @brief      ���PID��ʼ��
 *  @since      v1.0
*/
void Servo_pid_init(void)
{
     Servo_pid.error_now  = 0.0f;
     Servo_pid.error_last = 0.0f;
     Servo_pid.error_deviation = 0.0f;         //����ƫ����ϴ�ƫ��Ľ��
     Servo_pid.kp = 0.0f;
     Servo_pid.kd = 0.0f;
     /*�ٶ�6500*/
     Servo_pid.fuzzy_kp_gain = 15.0f;       //ģ��PҪ�˵ı���ϵ��
     Servo_pid.fuzzy_kd_gain = 17.0f;       //ģ��DҪ�˵ı���ϵ��     
}

/*
 *  @brief      ������κ���PD����
 *  @since      v1.0
 *  position    ��ǰƫ��
 *  pid.output  ����������ռ�ձ�
*/
float Servo_pid_cal(float position)
{
	Servo_pid.error_now = position;		//�����ʵ��ֵ��Ŀ��ֵ֮��Ĳ�ֵ
        Servo_pid.error_deviation = Servo_pid.error_now - Servo_pid.error_last;         //���㱾��ƫ����ϴ�ƫ��֮��Ĳ�ֵ
        KP_Fuzzy(Servo_pid.error_now , Servo_pid.error_deviation);      //ģ������
        
        Servo_pid.kp = fuzzy_kp * Servo_pid.fuzzy_kp_gain;      //ģ������Ľ�����Ա���ϵ����Ϊ���յ�KPֵ
        Servo_pid.kd = fuzzy_kd * Servo_pid.fuzzy_kd_gain;      //ģ������Ľ�����Ա���ϵ����Ϊ���յ�Kdֵ
        Servo_pid.output = (Servo_pid.kp * Servo_pid.error_now) + (Servo_pid.kd * Servo_pid.error_deviation);
       
        Servo_pid.error_last = Servo_pid.error_now;
               
	return Servo_pid.output;		//����ʵ�ʿ���ֵ 
}

/*
 *  @brief      Բ������
 *  @note       ʵ�麯����Ч��δ֪
 *  @since      v1.0
 *��position       ƫ��ֵ
*/
uint8 circle_deal(uint16 * sensor_value , float position , uint16 * motor_speed)
{
    uint16 sensor_value_add[3];         
    uint16 sensor_value_minus;
    static uint8 circle_flag;                                             //flag=0��û�м�⵽Բ��1�Ǽ�⵽Բ��δ��Բ��2���Ѿ���Բ
    static uint16 circle_count;
    sensor_value_add[0] = sensor_value[0] + sensor_value [2] + sensor_value[3] + sensor_value[5];
    sensor_value_add[1] = sensor_value[0] + sensor_value[2];
    sensor_value_add[2] = sensor_value[3] + sensor_value[5];
    sensor_value_minus = ABS(sensor_value_add[1] - sensor_value_add[2]);
//    printf("%d %d\r\n",sensor_value_add[0],sensor_value_minus);
//    printf("%d %d %d %d\r\n",sensor_value[0],sensor_value[2],sensor_value[3],sensor_value[5]);
     if(circle_flag == 0)                                                    //�����δ��⵽Բ
     {
         if(ABS(position)<50)
         {
             if((sensor_value_add[0] < 150) && (sensor_value_add[0] > 130))
            {
                if(sensor_value_minus < 30)
                {
                    circle_flag = 1;           //���������ֵ������Բ�������ͽ���־λ��Ϊ��⵽Բ��δ��Բ
                    led(LED0,LED_ON);
                    speed_set = -1500;
                }
            }

         }
         
    }
     
     if(circle_flag == 1)                      //�����⵽Բ��δ��Բ
     { 
         circle_count += *motor_speed;
         printf("%d\r\n",*motor_speed);
         if(((switch_read()) & (0x40)) == 0)   //���ݲ��뿪��ѡ���Ƿ���
         {
             
//              if(circle_count < 6500)       //��ƫ��������������˵����δ��Բ
              if(ABS(sensor_value_minus) < 20)
              {
                   servo_motor_pwm_set(SERVO_LEFT_DEAD_ZONE);        //�����
                   
              }
              else
              {
                  circle_flag = 2;                                          //���ѽ�Բ
                  circle_count = 0;
                  led(LED0,LED_OFF);
                  led(LED1,LED_ON);
                  switch(speed_rank)
                  {
                       case 0:
                           speed_set = 7000;
                           break;
                       case 1:
                           speed_set = 8000;
                           break;
                       case 2:
                           speed_set = 9000;
                           break;
                       case 3:
                           speed_set = 10000;
                           break;
                  }
              }
         }
         else
         {
//             if(circle_count < 6500)       //��ƫ��������������˵����δ��Բ
             if(ABS(sensor_value_minus) < 20)
             {
                 servo_motor_pwm_set(SERVO_RIGHT_DEAD_ZONE);        //�Ҵ���
             }
             else
             {
                 circle_flag = 2;                  //���ѽ�Բ
                 circle_count = 0;
                 led(LED0,LED_OFF);
                 led(LED1,LED_ON);
                 switch(speed_rank)
                 {
                     case 0:
                           speed_set = 7000;
                           break;
                       case 1:
                           speed_set = 8000;
                           break;
                       case 2:
                           speed_set = 9000;
                           break;
                       case 3:
                           speed_set = 10000;
                           break;
                 }
             }
         }
     }
    if(circle_flag == 2)                                                
     {
         if(sensor_value_add[0] > 200)      
         {
              if(sensor_value_minus < 30 ) 
              {
                    circle_flag = 0; 
                    led(LED1,LED_OFF);
              }
         }        
     } 
    return circle_flag;                                                   //����ǰ����״̬����              
}


/*
 *  @brief      �µ�����
 *  @note       ʵ�麯����Ч��δ֪
 *  @since      v1.0
 *��position       ƫ��ֵ
*/
uint8 ramp_deal(uint16 * sensor_value , float position, uint16 * motor_speed)
{
    static uint8 ramp_detect_sign;                                      //�Ƿ������ϵı�־λ
    static uint32 ramp_count;
    static uint8 ramp_flag;                                             //flag=0��û�м�⵽�£�1�Ǽ�⵽�£�2��������
     if(ramp_flag == 0)                                                    //�����δ��⵽��
     {
         
         if(position < 50)
         {
             if((sensor_value[2] > 110) && (sensor_value[3] > 110))
             {
                  ramp_flag = 1;           //���������ֵ�����µ��������ͽ���־λ��Ϊ1
                  ramp_detect_sign = 1;
                  led(LED0,LED_ON);
                  led(LED1,LED_ON);
             }
         } 
    }
    
    if(ramp_flag == 1)                      //�����⵽������
     {
        ramp_count += *motor_speed;
        if(ramp_count > 30000)                //��ʱ��ֹ��⵽����
        {
                led(LED0,LED_OFF);
                led(LED1,LED_OFF);
                ramp_flag = 0;
                ramp_detect_sign = 0;
                ramp_count = 0;
        }
     }      
    return ramp_detect_sign;                                                   //����ǰ����״̬����              
}