#ifndef	_OLED_H_
#define _OLED_H_

#define RESET  PTA16_OUT
#define DC     PTA17_OUT
#define D0     PTA24_OUT
#define D1     PTA25_OUT

 extern uint8  reverse; //��ת��ʾ
 typedef unsigned char byte;
 typedef unsigned int word;
 
 void OLED_Set_Pos(byte x, byte y);
 void OLED_Init(void);
 void OLED_CLS(void);
 //����OLED_WrDat
 void OLED_WrDat(byte data, byte revs);
 //��ʾһ��6x8��׼ASCII�ַ�
void OLED_P6x8Char(unsigned char ch);
 void OLED_P6x8Str(byte x,byte y,byte ch[]);
//��һ��char����ת����3λ��������ʾ 
void OLED_PrintValueC(unsigned char x, unsigned char y, int data);
//������������һ��int����ת����5λ��������ʾ
void OLED_PrintValueI(unsigned char x, unsigned char y, int data);
//��ʾunsigned int��
void OLED_PrintValueFP(unsigned int data, unsigned char num);
//������������һ��float����ת������������5λ��С���ͷ��ŵ����ݲ�������ʾ
void OLED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);
 void OLED_Fill(byte dat);
#endif