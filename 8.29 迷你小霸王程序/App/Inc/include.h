#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */
#include  "MKL_BME.h"           //λ����
#include  "MKL_wdog.h"          //���Ź�
#include  "MKL_gpio.h"          //IO�ڲ���
#include  "MKL_uart.h"          //����
#include  "MKL_SysTick.h"       //�δ�ʱ��
#include  "MKL_lptmr.h"         //�͹��Ķ�ʱ��(��ʱ�������������ʱ����ʱ)
#include  "MKL_i2c.h"           //I2C
#include  "MKL_spi.h"           //SPI
#include  "MKL_tpm.h"           //TPM������K60�� FTM ��pwm�����������
#include  "MKL_pit.h"           //PIT
#include  "MKL_adc.h"           //ADC
#include  "MKL_dac.h"           //DAC
#include  "MKL_dma.h"           //DMA
#include  "MKL_FLASH.h"         //FLASH


#include  "VCAN_LED.H"          //LED
#include  "VCAN_NRF24L0.h"      //����ģ��NRF24L01+
#include "Servo_motor.h"        //�����غ���
#include "Senser.h"             //�������ɼ���غ���
#include "24C02.h"              //24C02��غ���ʵ��
#include "Switch.h"             //���뿪����غ���ʵ��
#include "Motor.h"              //�����غ���ʵ��
#include "Encoder.h"            //������������غ���ʵ��
#include "Servo_motor_control.h"  //���������غ���ʵ��
#include "MKL_Scope.h"          //ɽ������ʾ��������
#include "Motor_control.h"
#include "OLED.h"
#include "Fuzzy.h"
#include "Reed.h"
#include "Gyroscope.h"
#include "IIC_sim.h"
#endif  //__INCLUDE_H__
