#include "Address.h"


uint8  imgbuff1[CAMERA_SIZE];         //�ɼ�ͼ��������1
uint8  imgbuff2[CAMERA_SIZE];         //�ɼ�ͼ��������2
uint8 *  p_imgbuff1 = (uint8 *)((uint8 *)&imgbuff1);   //ͼ���ַ1
uint8 *  p_imgbuff2 = (uint8 *)((uint8 *)&imgbuff2);   //ͼ���ַ2
uint8  flag_imgbuff = 0;
uint8 img[CAMERA_W*CAMERA_H]; //����ӥ������ͷ��һ�ֽ�8�����أ������Ҫ��ѹΪ 1�ֽ�1�����أ����㴦��

//����oled��ʾͼ�����������
uint8  bmp_buff[1024]={0};			
uint8  compress_buff[120][128]={0};
