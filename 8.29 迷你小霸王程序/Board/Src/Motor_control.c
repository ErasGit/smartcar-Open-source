/*
  * @file       Motor control.c
  * @brief      ���������غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-4-7
*/
#include "common.h"
#include "Motor.h"
#include "Motor_control.h"
#include "Buzzer.h"
#include "Servo_motor.h"
#include "Switch.h"
extern uint16 motor_speed;
extern int16 speed_set;
extern int32 var[5];
extern uint8 speed_rank;

/*!
 *  @brief      �ٶȵ�λѡ��
 *  @since      v1.0
 */
void speed_stall_select(void)
{
    if(((switch_read())&(0x02)) == 0)
    {
        speed_set = 8000;
        speed_rank = 1;
    }
    else if(((switch_read())&(0x04)) == 0)
    {
        speed_set = 9000;
        speed_rank = 2;
    }
    else if(((switch_read())&(0x08)) == 0)
    {
        speed_set = 10000;
        speed_rank = 3;
    }
    else
    {
        speed_set = 7000;
        speed_rank = 0;
    }
}

