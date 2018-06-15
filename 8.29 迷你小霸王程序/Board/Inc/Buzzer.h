/*
* @file         Buzzer.h
  * @brief      ��������غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-14
*/
#ifndef __BUZZER_H__
#define __BUZZER_H__

#define BEEP_TIME 200           //���ζ���ʱ������λms

void Buzzer_init(void);                 //��ʼ��������
void Buzzer_beep(uint8 beep_time);              //����������

#define BEEP_ON (GPIO_SET(PTE6,1))             //��������
#define BEEP_OFF (GPIO_SET(PTE6,0))            //��������

#endif