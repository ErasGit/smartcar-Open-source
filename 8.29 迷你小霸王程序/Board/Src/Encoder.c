/*
* @file         Encoder.c
  * @brief      ������������غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-14
*/
#include "common.h"
#include "Encoder.h"
#include  "MKL_TPM.h"
#include "MKL_lptmr.h"
#include "MKL_SysTick.h"

/*!
 *  @brief      ��������ʼ��
 *  @since      v1.0
 */
void encoder_init(void)
{
  lptmr_pulse_init(LPT0_ALT2, 0xffff, LPT_Rising);  //lptmr���������ʼ��
}

/*!
 *  @brief      ����һ�β��ٲ���ȡ�ٶ�ֵ
 *  encoder     Ҫ��ȡ�ı�����
 *  @since      v2.0
 */
void encoder_get(uint16 * motor_speed)
{
    static uint8 count_sign;
    
    if(count_sign == 1)
    {
      *motor_speed = lptmr_pulse_get();
      lptmr_pulse_clean ();
      count_sign = 0;
    }
    
    if(count_sign == 0)
    {
      lptmr_pulse_clean ();
      count_sign = 1;
    }
    
}