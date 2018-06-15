/*
* @file       Servo motor.h
  * @brief      �����غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#ifndef __SERVO_MOTOR_H__
#define __SERVO_MOTOR_H__

#define SERVO_FRE 100            //������Ƶ�ʣ�50~330HZ
#define SERVO_MID 13500          //��������ֵռ�ձ�13500
#define SERVO_LEFT_DEAD_ZONE 15500                //��������ֵռ�ձ�  ƫ��ֵΪ����
#define SERVO_RIGHT_DEAD_ZONE 12000               //�������Ҽ�ֵռ�ձ�  ƫ��ֵΪ����

void servo_motor_init(void);    //��ʼ�����
uint32 servo_motor_pwm_set(uint32 pwm_duty);      //���ö��PWM

#endif
