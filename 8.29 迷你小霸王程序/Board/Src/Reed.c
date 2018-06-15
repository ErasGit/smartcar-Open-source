/*
* @file         Reed.c
  * @brief      �ɻɹ���غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-5-23
*/
#include "common.h"
#include "Reed.h"
#include "Buzzer.h"
#include "MKL_gpio.h"
#include "Motor.h"

extern uint8 stop_car_sign;

/*
 *  @brief      �ɻɹ�ͣ�����
 *  @since      v1.0
*/
void reed_detection(void)
{
    static uint16 stop_lenth_count;
    static uint8 detection_enable_sign;
    if(detection_enable_sign == 0)
    {
        stop_lenth_count += 1;
        if(stop_lenth_count > 200)         //��Խ���������Ժ�ſ�ʼ���ͣ��
      {
            port_init (PTA13, IRQ_FALLING | PF | ALT1);         //��ʼ��PTA13����
            set_vector_handler(PORTA_VECTORn , portaIRQHandler);        //��ʼ���ⲿ�ж�
            enable_irq(PORTA_IRQn);
            detection_enable_sign = 1;
      }
    }
}

/*
 *  @brief      �ɻɹ��ⲿ�ж�
 *  @since      v1.0
*/
void portaIRQHandler(void)              //����ͣ�����ⲿ�ж�
{
    PORTA_ISFR  = (1 << 13);            //д1���жϱ�־λ
    systick_delay_ms(1);
    if(gpio_get(PTA13) == 0)
    {
        BEEP_ON;
        motor_pwm_set(0);               //��⵽ͣ����رյ��
        stop_car_sign = 1;              //ͣ����־λ��λ
    }
    
}