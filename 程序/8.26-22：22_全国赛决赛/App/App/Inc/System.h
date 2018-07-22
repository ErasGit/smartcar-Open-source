#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include "common.h"
#include "include.h"

#define MaxLengh  6

extern uchar Annulus_Count[4];
extern uint8 *imgbuff;
extern uint8 flag_run;
extern int32 S_D5_Duty;
extern uint8 nrf_rx_buff[];         //Ԥ��
extern uint8 nrf_tx_buff[];         //Ԥ��
extern uint8 threshold_value, var_speed;
extern uint8 Image_Send_Flag;	// ͼ���Ϳ��ƣ�1Ϊ���ͣ�0Ϊ������
extern uint8 Send_OK,CNST_Value;
extern uint8 Img_Display;
void System_Init();		 //����ģ���ʼ��
void data_send();		 //���߷��ͺͽ�������

void Boma_Key_Init(void);
void Parameter_Init(void);
uint8 wait_key_down();
void KEY_Adjuet(void);

#endif