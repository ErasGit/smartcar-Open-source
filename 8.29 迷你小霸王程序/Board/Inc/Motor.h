/*
* @file         Motor.h
  * @brief      �����غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-13
*/
#ifndef __MOTOR_H__
#define __MOTOR_H__

#define MOTOR_FRE 10000               //����������PWM���ڣ���λHZ

void motor_init(void);          //��ʼ�����
void motor_pwm_set(int32 pwm_duty);   //���õ��PWM

#endif