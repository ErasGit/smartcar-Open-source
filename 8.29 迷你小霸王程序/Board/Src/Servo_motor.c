/*
  * @file       Servo motor.c
  * @brief      �����غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/

#include "common.h"
#include "Servo_motor.h"
#include  "MKL_TPM.h"


/*
 *  @brief      ��ʼ�����
 *  @since      v1.0
 *  �������PTA12��Ƶ��50~330HZ������10000
 *  ��ʼ��ʱ�������
*/
void servo_motor_init(void)
{
  tpm_pwm_init( TPM1, TPM_CH0, SERVO_FRE, SERVO_MID);
  
}

/*!
 *  @brief      ���ö��PWM
 *  @param      pwm_duty        ���PWMֵ
 *  @since      v2.0
 */
uint32 servo_motor_pwm_set(uint32 pwm_duty)
{
  if(pwm_duty < SERVO_RIGHT_DEAD_ZONE)
  {
    pwm_duty = SERVO_RIGHT_DEAD_ZONE;
  }
  if(pwm_duty > SERVO_LEFT_DEAD_ZONE)
  {
    pwm_duty = SERVO_LEFT_DEAD_ZONE;
  }
  tpm_pwm_duty( TPM1, TPM_CH0, pwm_duty);
  return pwm_duty;
}