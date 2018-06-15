/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      ɽ��KL26 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.2
 * @date       2014-10-26
 */

#include "common.h"
#include "include.h"

extern uint16 sensor_value[6];
extern uint16 normalization_threshold[6];
extern float position_accumulative[15];


int32 var[5];                                           //���͵���λ��������
float position = 0.0f;                                 //λ��ƫ��
float pwm = 0.0f;                                      //���ռ�ձ�����
uint32 pwm_duty_now = 0;                                //���ռ�ձ�
uint8 loss_line = 0;                                    //���߱�־λ
uint8 loss_line_lock =0;                                //������
uint8 circle_sign = 0;                                  //Բ����־
uint8 ramp_sign = 0;                                    //�µ���־
uint8 stop_car_sign = 0;                                //ͣ����־

uint16 motor_speed = 0;                            //��ǰ����ٶ�
int16 speed_set = 250;                            //���Ŀ���ٶ�
uint8 speed_rank = 0;                            //�ٶȵȼ�


void pit2IRQHandler(void);                             //��ʱ���жϺ�������


/*!
 *  @brief      main����
 *  @since      v5.2
 */
void main()
{
    int16 gx, gy, gz,ax,ay,az; 
//    Sensor_init();
//    Gyroscope_init();
//    servo_motor_init();
//    switch_button_init();
//    motor_init();
//    encoder_init();
//    Servo_pid_init();
//    flash_24c02_init();
//    Senser_normalization_threshold_get();
//    Senser_normalization_threshold_read(normalization_threshold);       //��ȡ��һ����ֵ
//    speed_stall_select();               //�ٶȵ�λѡ��
//    NVIC_SetPriority(PORTA_IRQn,0);                     //Ϊ�˷�ֹ©��ͣ���ߣ���ͣ���õ��ⲿ�ж϶�Ϊ������ȼ�
//    NVIC_SetPriority(PIT_IRQn,1);
//    pit_init_ms(PIT0,5);                                //��ʼ����ʱ���жϣ�10ms
//    set_vector_handler(PIT_VECTORn , pit2IRQHandler);   //��ʼ����ʱ���жϣ�10ms
//    enable_irq(PIT_IRQn);
    MPU_Init();
    while(1)
    {
        systick_delay_ms(20);
        MPU_Get_Gyroscope(&gx,&gy,&gz);
        MPU_Get_Accelerometer(&ax,&ay,&az);
        printf("%5d   %5d   %5d    %5d    %5d    %5d    %5d\r\n",gx ,gy ,gz,ax,ay,az,MPU_Get_Temperature());
    }
}

void pit2IRQHandler(void)
{
      encoder_get(&motor_speed);
      reed_detection();         //�ɻɹ�ͣ�����
      Sensor_value_get(sensor_value);
      Senser_normalization(sensor_value);
//      printf("%5d %5d %5d %5d %5d %5d\r\n",sensor_value[0],sensor_value[1],sensor_value[2],sensor_value[3],sensor_value[4],sensor_value[5]);
//      printf("%5d  %d\r\n",sensor_value[0]+sensor_value[2]+sensor_value[3]+sensor_value[5],(int32)(position * 100));      
      position = cal_deviation(sensor_value);
      position = position_filter(position);
      pwm = Servo_pid_cal(position);
      ramp_sign = ramp_deal(sensor_value,position,&motor_speed);                     //�µ�����
      if(ramp_sign == 0)                                                //���µ��ϲ�����Բ������
      {
          circle_sign = circle_deal(sensor_value,position,&motor_speed);             //Բ�����
      } 
      /***Բ������***/
     if(circle_sign == 0)    //û�м�⵽Բ�������ߴ���ԭֵ
       {
            if(ramp_sign == 0)
            {
                loss_line_deal(sensor_value , position , LEFT_LOSE_LINE_THRESHOLD , RIGHT_LOSE_LINE_THRESHOLD);
            }            
        }
      else if(circle_sign == 2)    //������Ѿ���Բ��ʹ��Բ�ڵĶ��ߴ���
      {
          loss_line_deal(sensor_value , position , LEFT_LOSE_LINE_THRESHOLD - 10 , RIGHT_LOSE_LINE_THRESHOLD - 10);
       }
      switch(loss_line)
      {
          case 0:
              pwm_duty_now = servo_motor_pwm_set(SERVO_MID - (int)(pwm)); 
              break;
              
          case 1:
              if(circle_sign == 2)
              {
                  if(((switch_read())&(0x40)) == 0)
                  {
                      position = POSITION_BOUND;
                      pwm_duty_now = servo_motor_pwm_set(SERVO_RIGHT_DEAD_ZONE);
                  }
                  else
                  {
                      position = (-(POSITION_BOUND));
                      pwm_duty_now = servo_motor_pwm_set(SERVO_LEFT_DEAD_ZONE);
                  }
              }
              else
              {
                  loss_line_lock = 1;
                  position = POSITION_BOUND;
                  pwm_duty_now = servo_motor_pwm_set(SERVO_RIGHT_DEAD_ZONE);
              }
              break;
              
          case 2:
              if(circle_sign == 2)
              {
                  if(((switch_read())&(0x40)) == 0)
                  {
                      position = POSITION_BOUND;
                      pwm_duty_now = servo_motor_pwm_set(SERVO_RIGHT_DEAD_ZONE);
                  }
                  else
                  {
                      position = (-(POSITION_BOUND));
                      pwm_duty_now = servo_motor_pwm_set(SERVO_LEFT_DEAD_ZONE);                     
                  }
              }
              else
              {
                  loss_line_lock = 1;
                  position = (-(POSITION_BOUND));
                  pwm_duty_now = servo_motor_pwm_set(SERVO_LEFT_DEAD_ZONE);
              }
              break;
      }
      if((((switch_read()) & (0x80)) == 0) | (stop_car_sign == 1))
      {
          motor_pwm_set(0);
      }
      else
      {
          motor_pwm_set((int32)speed_set);
      }         
//      vcan_scope((uint8_t *)var,sizeof(var));         //����ʾ������������
      PIT_Flag_Clear(PIT0); 
}
