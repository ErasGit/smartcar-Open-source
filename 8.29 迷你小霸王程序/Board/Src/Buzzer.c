/*
* @file         Buzzer.c
  * @brief      ��������غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-14
*/
#include "common.h"
#include "Buzzer.h"
#include "MKL_gpio.h"
#include "MKL_SysTick.h"

/*!
 *  @brief      ��ʼ��������
 *  @since      v1.0
 */
void Buzzer_init(void)
{
  gpio_init(PTE6, GPO, 0);
}

/*!
 *  @brief      ����������
 *  beep_time   �������
 *  @since      v2.0
 */
void Buzzer_beep(uint8 beep_time)
{
  uint8 a = 0;
  for(a = 0; a < beep_time; a ++)
  {
    GPIO_SET(PTE6,1);
    systick_delay_ms(BEEP_TIME);
    GPIO_SET(PTE6,0);
    systick_delay_ms(BEEP_TIME);
  } 
}

