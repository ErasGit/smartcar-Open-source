/*
* @file       Servo motor control.h
  * @brief      ���������غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-16
*/
#ifndef __SERVO_MOTOR_CONTROL_H__
#define __SERVO_MOTOR_CONTROL_H__



void Servo_pid_init(void);
float Servo_pid_cal(float position);            //���PID����
uint8 circle_deal(uint16 * sensor_value , float position, uint16 * motor_speed);         //Բ������
uint8 ramp_deal(uint16 * sensor_value , float position , uint16 * motor_speed);          //�µ�����
#endif