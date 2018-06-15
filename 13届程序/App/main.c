/*******************************************************************************/
//
//                      oo����NXP2018_PRO����oo
//
//                          PART1:��ʼ������
/*******************************************************************************/
/************************������ͷ�ļ�****************************/
#include "common.h"
#include "include.h"
#include "OLED.h"
#include "SEEKFREE_18TFT.h"
/***********************��������&����****************************/
//---------------------------------------------------------------
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//---------------------------------------------------------------
/***********************ϵͳ���в���*****************************/
uint8 code_Time_flag=0;                 //��������ʱ��
/**************************���*********************************/
uint8  KP_A=6,KP_B=27,KD=150;            //MAIN���PID
uint32 DJ_midpoint=7330;                //�����ֵ
uint32 DJ_PWM;                          //���PWM
/************************�ٶȿ���*******************************/
uint8  speed_need=20;                   //Ŀ���ٶ�
uint8  speed_need_Boost=30;             //Ŀ�����
uint8  speed_need_normal=30;            //Ŀ���ٶ�
uint8  speed_need_L=30;                 //Ŀ�����
uint8  speed_SW_flag=1;                 //�ٶ�ѡ���־
/*************************�������******************************/
float Speed_P=4,Speed_I=0.15,Speed_D=1; //MAIN���PID
uint8 Block_motor_time_flag=0;          //��ת��ʱ��־
uint8 Block_motor_duty_flag=0;          //��ת�¼���־
uint8 Block_motor_delay_flag=0;         //��ת��и��־
/**************************������********************************/
float feed_fix=10.6;           //����������ϵ��
uint32 Feed_flag=0;             //�������ɼ�����
uint32 Feed_speed=0;            //�������ɼ��ٶ�
/***********************����ͷ�йز���***************************/
/*���ز���*/
uint8 img_y_max=50;             //ɨ�����������ֵ
uint8 img_y_min=10;             //ɨ����������Զֵ
uint8 img_y_control=30;         //ɨ�����������ֵ
/*���ݲ���*/
uint8 imgbuff[CAMERA_SIZE];     //����洢����ͼ�������
uint8 img[CAMERA_W*CAMERA_H];   //����ͷ��ѹ����
uint8 img_x=40;                 //ɨ�������
uint8 img_y=30;                 //ɨ��������
uint8 start_point=40;           //ɨ����ʼ��
uint8 mid_point[60];            //��ȡ������  
uint8 mid_point_His[10];        //��ʷ������ 
uint8 left_point[60];           //��߽�
int right_point[60];          //�ұ߽�
uint8 init_len[60];             //��ʼ���������
uint8 point_len[60];            //ʵʱ�������
uint8 street_len=0;             //ֱ������
uint8 len_His[10];              //ֱ��������ʷ����
/*Բ������*/
float L_Cur_K=0;                //��Բ������б��
float R_Cur_K=0;                //��Բ������б��
/******************ʶ�����жϱ�־***************************/
/*���߱�־*/
uint8 all_lost=0;               //ȫ����־
uint8 lost_left[60];            //���߱�־
uint8 lost_right[60];           //�Ҷ��߱�־
/*��ͣ*/
uint8 KEY_start_flag=0;         //һ��������־
uint16 KEY_start_time_flag=0;   //����ʱ���־
/*ʮ��*/
uint8 lost_flag=0;              //ȫ���߳�ʼʶ���־
uint8 lost_delay_flag=0;        //ʮ�ֳ�и��־
uint16 lost_car_time_flag=0;    //ʮ�ּ�ʱ����
uint8 lost_duty_flag=0;         //ȫ����׼ȷʶ���־(ʮ��)
/*Բ��Ԥ�ж�*/
uint8 cur_L_ready_flag=0;         //��Բ��Ԥ�жϳ�ʼʶ���־
uint8 cur_L_ready_delay_flag=0;   //��Բ��Ԥ�жϳ�и��־
uint16 cur_L_ready_time_flag=0;   //��Բ��Ԥ�жϼ�ʱ����
uint8 cur_L_ready_rest_flag=0;    //��Բ��Ԥ�жϸ�λ����
uint8 cur_R_ready_flag=0;         //��Բ��Ԥ�жϳ�ʼʶ���־
uint8 cur_R_ready_delay_flag=0;   //��Բ��Ԥ�жϳ�и��־
uint16 cur_R_ready_time_flag=0;   //��Բ��Ԥ�жϼ�ʱ����
uint8 cur_R_ready_rest_flag=0;    //��Բ��Ԥ�жϸ�λ����
/*Բ��׼ȷʶ��*/
uint8 cur_L_real_flag=0;          //��Բ��׼ȷ�ж�ʶ���־
uint8 cur_L_real_delay_flag=0;    //��Բ��׼ȷ��иʶ���־
uint8 cur_L_real_rest_flag=0;     //��Բ��׼ȷ��λʶ���־
uint16 cur_L_real_time_flag=0;    //��Բ��׼ȷ��иʶ���־
uint16 cur_L_real_time_flag1=0;   //��Բ��׼ȷ��иʶ���־1

uint8 cur_R_real_flag=0;          //��Բ��׼ȷ�ж�ʶ���־
uint8 cur_R_real_delay_flag=0;    //��Բ��׼ȷ��иʶ���־
uint8 cur_R_real_rest_flag=0;     //��Բ��׼ȷ��λʶ���־
uint16 cur_R_real_time_flag=0;    //��Բ��׼ȷ��иʶ���־
uint16 cur_R_real_time_flag1=0;   //��Բ��׼ȷ��иʶ���־1
/***************************END**********************************/
//---------------------------------------------------------------
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//---------------------------------------------------------------
/*************************��������*******************************/
/*��ʼ������*/
void IO_init(void);                     //�����˿ڳ�ʼ��
/*���Խ��沿��*/
void speed_SW(void);                    //�ٶȵ�λѡ��
void Board_led_duty(void);              //����LED��˸����
void BEEP_duty(void);                   //���������ƺ���        
uint8 Button_Duty(uint8 Button_val);     //�������ú���
void GUI_Duty(void);                    //���Խ��溯��
/*ͼ������*/
void img_ext(uint8 *dst,uint8 *src,uint32 srclen);//�����ѹ����
void len_init(void);                    //������ȳ�ʼ��
void street_duty(void);                 //����ֱ�����Ȳ���
uint8 PointL_DUTY(uint8 L_Start,uint8 L_y);//����ɨ�躯��
uint8 PointR_DUTY(uint8 R_Start,uint8 R_y);//����ɨ�躯��
void mid_Point_DUTY(uint8 Mid_len,uint8 Mid_y);//�����㷨
void IMG_DUTY(void);                    //��ͼ������
/*�¼�������*/
void EVENT_Duty(void);                  //�¼�������
/*�������Ʋ���*/
void DJ_PID(unsigned char midpoint_in);//���PD
void FEED_COUNT(void);                  //��������������
void speed_PID(unsigned char speed_in);//ת��PID���ƺ���
void CAR_Drive_duty(void);              //ִ�л����ۺϿ��ƺ���
/*�жϺ�������*/
void PORTC_IRQHandler();                //PORTC�жϷ�����
void DMA0_IRQHandler();                 //DMA�жϷ�����
void PIT0_IRQHandler();                 //PIT0�жϷ�����
void PORTD_IRQHandler();                //PORTD�жϷ�����
/**********************�����˿ڳ�ʼ��****************************/
void IO_init(void)
{
  /*****����������˿�******/
  gpio_init (PTE28 ,GPO,0);
  /******����LED�˿�*******/
  gpio_init (PTA17,GPO,1);
  gpio_init (PTC0, GPO,1);
  gpio_init (PTD15,GPO,1);
  gpio_init (PTE26,GPO,1);
  /*****���뿪�ؽӿ�*******/
  gpio_init (PTE4, GPI,0);
  gpio_init (PTE6, GPI,0);
  gpio_init (PTE8, GPI,0);
  gpio_init (PTE10,GPI,0);
  /*******���ᰴ���ӿ�*****/
  gpio_init (PTE7,GPI,0);  //��     
  gpio_init (PTE5,GPI,0);  //��     
  gpio_init (PTE12,GPI,0); //��
  gpio_init (PTE11,GPI,0); //��
  gpio_init (PTE9,GPI,0);  //��
  /*******�������ӿ�*******/
  gpio_init (PTD0,GPI,0);
  gpio_init (PTD1,GPI,0);
  port_init (PTD0,ALT1|IRQ_EITHER|PULLUP );
  /******���PWM�ӿ�*******/
  ftm_pwm_init(FTM0, FTM_CH0,1000,0); 
  ftm_pwm_init(FTM0, FTM_CH1,1000,0); 
  /******���PWM�ӿ�*******/
  ftm_pwm_init(FTM2, FTM_CH0,50,7200);
}
/*******************************************************************************/
//                      ��һ���֣���ʼ������
//
//                      ��һ���֣����Խ��沿��
/*******************************************************************************/
/********************�ٶȵ�λѡ��************************/
void speed_SW(void)
{
  if(speed_SW_flag==1)           //�ٶȵ�λ1      
  {
    speed_need_Boost=30;         //Ŀ����� 30
    speed_need_normal=25;        //Ŀ���ٶ� 25
    speed_need_L=26;             //Ŀ�����
    KP_A=10,KP_B=27,KD=150;  //6
  }
  else if(speed_SW_flag==2)     //�ٶȵ�λ2      
  {
    speed_need_Boost=32;         //Ŀ�����
    speed_need_normal=26;        //Ŀ���ٶ�
    speed_need_L=27;             //Ŀ�����
    KP_A=20,KP_B=27,KD=180;
  }
  else if(speed_SW_flag==3)     //�ٶȵ�λ3      
  {
    speed_need_Boost=33;         //Ŀ�����
    speed_need_normal=27;        //Ŀ���ٶ�
    speed_need_L=28;             //Ŀ�����
    KP_A=20,KP_B=27,KD=180;
  }
  else if(speed_SW_flag==4)     //�ٶȵ�λ4      
  {
    speed_need_Boost=34;         //Ŀ�����
    speed_need_normal=27;        //Ŀ���ٶ�
    speed_need_L=28;             //Ŀ�����
    KP_A=25,KP_B=27,KD=180;
  }
  else;
}
/********************����LED��˸����************************/
void Board_led_duty(void)
{
  gpio_turn (PTD15);
  gpio_turn (PTE26);
  gpio_turn (PTA17);
  gpio_turn (PTC0);
}
/********************���������ƺ���************************/
void BEEP_duty(void)
{
  //if(!gpio_get(PTE11)||lost_flag)     //��������
  if(!gpio_get(PTE11)||cur_L_real_delay_flag||cur_R_real_delay_flag)    
    gpio_set (PTE28,0);        
  else gpio_set (PTE28,1);
}
/********************�������ú���****************************/
uint8 Button_Duty(uint8 Button_val)     //������������
{
  if(!gpio_get(PTE9))                   //������                   
  {
    DELAY_MS(10);
    if(!gpio_get(PTE9))                 //�����ж�
    {
      Button_val++;
      if(Button_val>250)                //�޷�
        Button_val=250;
      while(!gpio_get(PTE9));
    }
  }
  if(!gpio_get(PTE5))                   //������
  {
    DELAY_MS(10);
    if(!gpio_get(PTE5))                 //�����ж�
    {
      Button_val--;
      if(Button_val<2)                  //�޷�
        Button_val=1;
      while(!gpio_get(PTE5));
    }
  }
   return Button_val;
}
/*******************���Խ��溯��**************************/
void GUI_Duty(void)   
{
  /*********ϵͳ����***********/
  unsigned char static GUI_flag=0;     //���Խ������
  unsigned char static display_y=25;   //������������
  if(!gpio_get(PTE12))                  //�ı����
  {
    DELAY_MS(10);
    if(!gpio_get(PTE12))                //����ȷ��
    {
      LCD_Fill(0x00);                   //����
      GUI_flag++;
      if(GUI_flag>2)                    //���ý���ҳ��
        GUI_flag=0;
      while(!gpio_get(PTE12));
    }
  }
  /*********����LEDָʾ********/
  Board_led_duty();
  /*********����������********/
  BEEP_duty();
  /*********���Խ���#0***********/
  if(GUI_flag==0)               
  {
    LED_PrintImage(img,60,80);                  //OLED��ʾͼ��
    display_y=Button_Duty(display_y);           //������������
    Display_uint8(display_y,85,0);              //OLED��ʾ����
    Display_uint8(mid_point[display_y],85,2);   //OLED��ʾ����
    Display_uint8(street_len,85,4);             //����ֱ�����Ȳ���
    //Display_uint8(code_Time_flag,85,6);       //��ʾ��������ʱ��
    Display_uint8(point_len[display_y],85,6);   //OLED��ʾ�������
  }
  /*********���Խ���#1***********/
  else if(GUI_flag==1)
  {
    LCD_P8x16Str(1,0,"DJ_PWM");                 //OLED��ʾ���PWM
    Display(DJ_PWM,1,2);
    LCD_P8x16Str(1,4,"Speed_SET");              //OLED��ʾ�趨�ٶ�
    /***�ٶȵ�λѡ��***/
    Display(speed_SW_flag,1,6);                 //��ʾ�ٶ�ѡ���־
    speed_SW_flag=Button_Duty(speed_SW_flag);   //�ٶȵ�λѡ��
    if(speed_SW_flag>4)speed_SW_flag=1;
    speed_SW();                                 //�ٶȵ�λѡ����
  }
  else if(GUI_flag==2)
  {
    LCD_P8x16Str(10,2,"START ENGINE!");         //һ������
    if(!gpio_get(PTE11)) 
    {KEY_start_flag=1;Block_motor_delay_flag=0;}//������־ֵ��1+�����ת������λ
    else;
    if(Block_motor_duty_flag)
      LCD_P8x16Str(10,5,"Block_motor!");       //�����ת
    else if(!KEY_start_flag)
      LCD_P8x16Str(10,5,"    Loop    ");       //ͣ��
    else if(KEY_start_flag && KEY_start_time_flag<100)
      LCD_P8x16Str(10,5,"    READY   ");       //׼��
    //else if(KEY_start_time_flag>100 && !stop_motors_flag)
    else if(KEY_start_time_flag>100)
      LCD_P8x16Str(10,5,"    Voom!    ");      //������
    else
      LCD_P8x16Str(10,5,"    STOP    ");       //ͣ��
  }
  else;
  /************END**************/ 
}
/*******************************************************************************/
//                      ��һ���֣����Խ��沿��
//
//                      ��һ���֣�ͼ������
/*******************************************************************************/
/********************����ͷ�����ѹ*************************/
void img_ext(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255, 0}; //0 �� 1 �ֱ��Ӧ����ɫ
    //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}
/*********************������ȳ�ʼ��************************/
void len_init(void)                   //������ȳ�ʼ��
{
  for(img_y=img_y_min;img_y<=img_y_max;img_y++)
  {
    init_len[img_y]=img_y+30;
  }
}
/********************����ֱ�����Ȳ���**********************/
void street_duty(void)                //����ֱ�����Ȳ���
{
  /*����ֱ�����Ȳ���*/
  for(img_y=55;img_y>=1;img_y--)
  {
    if(img[40+img_y*80]==0)
      break;
    else;
  }
  street_len=59-img_y;
  /*��Բ����������*/
  for(img_y=55;img_y>=1;img_y--)
  {
    if(img[right_point[img_y_max-3]-3+img_y*80]==0)
      break;
    else;
  }
  R_Cur_K=10*(right_point[img_y_max-3]-35)/(59-img_y);
  /*��Բ����������*/
  for(img_y=55;img_y>=1;img_y--)
  {
    if(img[left_point[img_y_max-3]+3+img_y*80]==0)
      break;
    else;
  }
  L_Cur_K=10*(50-left_point[img_y_max-3])/(59-img_y);
}
/*******************�߽�ɨ�躯��***********************/
//����1��ɨ�����ʼ�㣻�������ɨ���Y
//��� ��ɨ��ı߽�ֵ
uint8 PointL_DUTY(uint8 L_Start,uint8 L_y)
{
  unsigned char static L_point;
  for(img_x=L_Start;img_x>1;img_x--) //��߽���ȡ
  {
    if(img[img_x+L_y*80]==0)
    {
      L_point=img_x;
      break;
    }
    else L_point=1;
  }
  return L_point;
}
uint8 PointR_DUTY(uint8 R_Start,uint8 R_y)
{
  unsigned char static R_point;
  for(img_x=R_Start;img_x<79;img_x++)//�ұ߽���ȡ
  {
    if(img[img_x+R_y*80]==0)
    {
      R_point=img_x;
      break;
    }
    else R_point=79;
  }
  return R_point;
}
/********************���ߴ�����*************************/
void mid_Point_DUTY(uint8 Mid_len,uint8 Mid_y)          //�����㷨
{  
  /*********Բ�����ߴ���***********/
  /*�뻷*/
  if(cur_R_real_delay_flag||cur_L_real_delay_flag)
  {
    if(cur_R_real_delay_flag&&cur_R_real_time_flag<20)
    {
      left_point[Mid_y]=(uint8)(R_Cur_K*(59-Mid_y)/10);
      if(left_point[Mid_y]+Mid_len/2<79)
        mid_point[Mid_y]=left_point[Mid_y]+Mid_len/2;
      else mid_point[Mid_y]=79;
    }
    else if(cur_L_real_delay_flag&&cur_L_real_time_flag<20)
    {
      right_point[Mid_y]=(uint8)(80-L_Cur_K*(59-Mid_y)/10);
      if(right_point[Mid_y]-Mid_len/2>1)
        mid_point[Mid_y]=right_point[Mid_y]-Mid_len/2;
      else mid_point[Mid_y]=1;
    }
     /*����*/
    else if(cur_R_real_rest_flag&&cur_R_real_delay_flag)
    {
      if(mid_point_His[9]<79)
        mid_point[Mid_y]=mid_point_His[9];
      else mid_point[Mid_y]=mid_point_His[9];
    }
    else if(cur_L_real_rest_flag&&cur_L_real_delay_flag)
    {
      if(mid_point_His[9]>21)
        mid_point[Mid_y]=mid_point_His[9];
      else mid_point[Mid_y]=mid_point_His[9];
    }
    /*����*/
    else
    {
      if(left_point[Mid_y]<5 && !lost_flag)           //��߶��߲���
      {
        if(right_point[Mid_y]>30)
          mid_point[Mid_y]=right_point[Mid_y]-Mid_len/2;
        else mid_point[Mid_y]=1;
      }
      else if(right_point[Mid_y]>75 && !lost_flag)   //�ұ߶��߲���
      {
        if(left_point[Mid_y]<50)
          mid_point[Mid_y]=left_point[Mid_y]+Mid_len/2;
        else mid_point[Mid_y]=79;
      }
      else mid_point[Mid_y]=(left_point[Mid_y]+right_point[Mid_y])/2; //���߼���
    }
  }
 
  /*********ʮ�ֱ��ߴ���***********/
  else if(lost_duty_flag&&!(cur_R_real_delay_flag||cur_L_real_delay_flag))
  {
       mid_point[Mid_y]=mid_point_His[6];
  }
  /*********�������ߴ���***********/
  else
  {
    if(left_point[Mid_y]<5 && !lost_flag)           //��߶��߲���
    {
      if(right_point[Mid_y]>30)
        mid_point[Mid_y]=right_point[Mid_y]-Mid_len/2;
      else mid_point[Mid_y]=1;
    }
    else if(right_point[Mid_y]>75 && !lost_flag)   //�ұ߶��߲���
    {
      if(left_point[Mid_y]<50)
        mid_point[Mid_y]=left_point[Mid_y]+Mid_len/2;
      else mid_point[Mid_y]=79;
    }
    else mid_point[Mid_y]=(left_point[Mid_y]+right_point[Mid_y])/2; //���߼���
  }
}
/*******************��ͼ������***********************/
void IMG_DUTY(void)                                     //��ͼ������
{
  street_duty();                                         //����ֱ�����Ȳ���
  /*****MAIN������ȡ+ʶ�����******/
  lost_flag=0;                                           //���߱�������
  all_lost=0;                                            //ȫ����־����
  start_point=(left_point[img_y_max]+right_point[img_y_max])/2;//ɨ���ֵ�趨
  for(img_y=img_y_max;img_y>=img_y_min;img_y--)          //15-40����ȡ
  {
    /*********�߽���ȡ***********/
    if(all_lost==0)
    {
      left_point[img_y]=PointL_DUTY(start_point,img_y);  //��߽���ȡ
      right_point[img_y]=PointR_DUTY(start_point,img_y); //�ұ߽���ȡ
    }
    else if(all_lost==1)
    {
      left_point[img_y]=1;
      right_point[img_y]=1;
    }
    else if(all_lost==2)
    {
      left_point[img_y]=79;
      right_point[img_y]=79;
    }else;
    /*********��������***********/
    start_point=(left_point[img_y]+right_point[img_y])/2;//ɨ���ֵ�趨
    /*��ֵ�޷�8.17*/
    if(start_point>70){start_point=79;all_lost=2;}
    else if(start_point<10){start_point=1;all_lost=1;}
    else;
    /************END*************/
    point_len[img_y]=right_point[img_y]-left_point[img_y];//�����������
    if(left_point[img_y]<5)lost_left[img_y]=1;           //�����ж�
    else lost_left[img_y]=0;
    if(right_point[img_y]>75)lost_right[img_y]=1;        //�Ҷ����ж�
    else lost_right[img_y]=0;
  }
  /**********Բ��Ԥ�ж�************/
  if(lost_right[23]&&lost_right[25]&&!lost_left[23]&&!lost_left[25]&&street_len>47&&point_len[25]>55&&point_len[25]<75)
    cur_R_ready_flag=1;//��Բ��Ԥʶ��
  else cur_R_ready_flag=0;
  if(lost_left[23]&&lost_left[25]&&!lost_right[23]&&!lost_right[25]&&street_len>47&&point_len[25]>55&&point_len[25]<75)
    cur_L_ready_flag=1;//��Բ��Ԥʶ��
  else cur_L_ready_flag=0;
  /**********�����ж�************/
  if(right_point[25]>75&&left_point[25]<5&&right_point[30]>75&&left_point[30]<5)//ʮ��ʶ��
    {
      lost_flag=1;                                       //ȫ������
    }else;
  /********�¼�������**********/
  EVENT_Duty();
  /*********20-40�в��ߴ���********/
  for(img_y=img_y_max;img_y>=img_y_min;img_y--)          //15-40�д���
  {
    mid_Point_DUTY(init_len[img_y],img_y);               //���߼���
    img[mid_point[img_y]+img_y*80]=0;                    //��ӡ���ߵ�
    img[left_point[img_y]+img_y*80]=0;                   //��ӡ�����
    img[right_point[img_y]+img_y*80]=0;                  //��ӡ�ұ���
  }
  /**********������ʷ����************/
  mid_point_His[9]=mid_point_His[8];
  mid_point_His[8]=mid_point_His[7];
  mid_point_His[7]=mid_point_His[6];
  mid_point_His[6]=mid_point_His[5];
  mid_point_His[5]=mid_point_His[4];
  mid_point_His[4]=mid_point_His[3];
  mid_point_His[3]=mid_point_His[2];
  mid_point_His[2]=mid_point_His[1];
  mid_point_His[1]=mid_point_His[0];
  mid_point_His[0]=mid_point[img_y_control];
  /**********ֱ��������ʷ����************/
  len_His[9]=len_His[8];
  len_His[8]=len_His[7];
  len_His[7]=len_His[6];
  len_His[6]=len_His[5];
  len_His[5]=len_His[4];
  len_His[4]=len_His[3];
  len_His[3]=len_His[2];
  len_His[2]=len_His[1];
  len_His[1]=len_His[0];
  len_His[0]=street_len;
}
/*******************************************************************************/
//                      ��һ���֣�ͼ������
//
//                      ��һ���֣��¼�������
/*******************************************************************************/
void EVENT_Duty(void)
{
  /************����ʱ��*****************/
  lost_car_time_flag++;
  cur_R_ready_time_flag++;
  cur_R_real_time_flag++;
  cur_L_ready_time_flag++;
  cur_L_real_time_flag++;
  /************������ʱ*****************/
  if(KEY_start_flag)KEY_start_time_flag++;
  else;
  if(KEY_start_time_flag>5000) KEY_start_time_flag=5000;
  else;
  /*************ʮ�ֲ���****************/
  if(lost_flag|| lost_delay_flag)//ʮ��ʶ��
  {
    lost_delay_flag=1;
    lost_duty_flag=1;
    if((lost_flag==0&&lost_car_time_flag>15)||lost_car_time_flag>30)
    {
      lost_delay_flag=0;
    }else;
  }
  else
  {
    lost_car_time_flag=0;
    lost_duty_flag=0;
  }
  /*************Բ��Ԥʶ��****************/
  /*��Ԥʶ��*/
  if(cur_R_ready_flag||cur_R_ready_delay_flag)
  { cur_R_ready_delay_flag=1;
    if((!lost_right[23]&&!lost_right[25])&&!lost_left[23]&&!lost_left[25]&&street_len>50&&cur_R_ready_time_flag>3)//����Ϊ1
    {cur_R_ready_rest_flag=1;}else;
    if(cur_R_ready_time_flag>150||lost_left[23]||lost_left[25])
    {cur_R_ready_delay_flag=0;cur_R_ready_rest_flag=0;}
    else if(cur_R_ready_rest_flag&&lost_right[23]&&lost_right[25]&&!lost_left[23]&&!lost_left[25]&&street_len>50)
    { cur_R_ready_delay_flag=0;cur_R_ready_rest_flag=0;cur_R_real_flag=1;}else;
  }
  else
  {cur_R_ready_delay_flag=0;
    cur_R_ready_time_flag=0;
    cur_R_ready_rest_flag=0;
  }
  /*��Ԥʶ��*/
  if(cur_L_ready_flag||cur_L_ready_delay_flag)
  { cur_L_ready_delay_flag=1;
    if((!lost_left[23]&&!lost_left[25])&&!lost_right[23]&&!lost_right[25]&&street_len>50&&cur_L_ready_time_flag>3)
    {cur_L_ready_rest_flag=1;}else;
    if(cur_L_ready_time_flag>50||lost_right[23]||lost_right[25])
    {cur_L_ready_delay_flag=0 ;cur_R_ready_rest_flag=0;}
    else if(cur_L_ready_rest_flag&&lost_left[23]&&lost_left[25]&&!lost_right[23]&&!lost_right[25]&&street_len>50)
    { cur_L_ready_delay_flag=0;cur_L_ready_rest_flag=0;cur_L_real_flag=1;}else;
  }
  else
  { cur_L_ready_delay_flag=0;
    cur_L_ready_time_flag=0;
    cur_L_ready_rest_flag=0;
  }
  /*************Բ��׼ȷʶ��****************/
  /*��׼ȷʶ��*/
  if(cur_R_real_flag||cur_R_real_delay_flag)
  { cur_R_real_flag=0;
    cur_R_real_delay_flag=1;
    if(cur_R_real_time_flag>50&&(lost_left[23]&&lost_left[25]))
      cur_R_real_rest_flag=1;else;
    if(cur_R_real_rest_flag)cur_R_real_time_flag1++;else;
    if(cur_R_real_time_flag1>100)cur_R_real_time_flag=100;else;
    if(cur_R_real_time_flag1>20)//&&!lost_left[23]&&!lost_left[25]&&!lost_right[23]&&!lost_right[25])
    {cur_R_real_delay_flag=0;}else;
  }
  else
  {cur_R_real_delay_flag=0;
    cur_R_real_time_flag=0;
    cur_R_real_time_flag1=0;
    cur_R_real_rest_flag=0;
  }
  /*��׼ȷʶ��*/
  if(cur_L_real_flag||cur_L_real_delay_flag)
  { cur_L_real_flag=0;
    cur_L_real_delay_flag=1;
    if(cur_L_real_time_flag>50&&(lost_right[23]&&lost_right[25]))
      cur_L_real_rest_flag=1;else;
    if(cur_L_real_rest_flag)cur_L_real_time_flag1++;else;
    if(cur_L_real_time_flag1>100)cur_L_real_time_flag1=100;
    if(cur_L_real_time_flag1>20)//&&!lost_left[23]&&!lost_left[25]&&!lost_right[23]&&!lost_right[25])
    {cur_L_real_delay_flag=0;}else;
  }
  else
  {cur_L_real_delay_flag=0;
    cur_L_real_time_flag=0;
    cur_L_real_time_flag1=0;
    cur_L_real_rest_flag=0;
  }
}
/*******************************************************************************/
//                      ��һ���֣��¼�������
//
//                      ��һ���֣��������Ʋ���
/*******************************************************************************/
/***********************DJ_PID**************************/
void DJ_PID(unsigned char midpoint_in)  //���PD
{
  signed int static   midpoint_error;   //�������
  signed int static  lastpoint_error;   //�ϴ����
  if(lost_duty_flag&&!(cur_R_real_delay_flag||cur_L_real_delay_flag))
    midpoint_error=(signed int)((midpoint_in-40)*0.6); //�������ʮ��
  else
  midpoint_error=midpoint_in-40;         //�������
  DJ_PWM=DJ_midpoint+(KP_A*midpoint_error*midpoint_error/1000+KP_B)*midpoint_error+KD*(midpoint_error-lastpoint_error);
  lastpoint_error=midpoint_error;        //��¼�ϴ����
  if(DJ_PWM<DJ_midpoint-1300)            //����޷�
    DJ_PWM=DJ_midpoint-1300;
  else if(DJ_PWM>DJ_midpoint+1300)
    DJ_PWM=DJ_midpoint+1300;
  else;
  ftm_pwm_duty(FTM2,FTM_CH0,DJ_PWM);     //���PWM
}
/*******************��������������************************/
void FEED_COUNT(void)
{
  Feed_speed=(uint32)(Feed_flag/feed_fix);
  Feed_flag=0;
}
/*******************ת��PID���ƺ���*************************/
void speed_PID(unsigned char speed_in)
{
  signed int static speed_out;         //���ʵ���ٶ�
  signed int static PWM_CH5;           //���PWM_CH5
  signed int static PWM_CH2;           //���PWM_CH2
  signed int static speed_error;       //�ٶ����       
  signed int static last_error_1;      //�ϴ�ƫ��
  signed int static last_error_2;      //���ϴ�ƫ��
  speed_error=speed_in-Feed_speed;      //����ת�����
  speed_out +=Speed_P*(speed_error-last_error_1)+Speed_I*speed_error+   \
              Speed_D*(speed_error-2*last_error_1+last_error_2);
  last_error_2=last_error_1;            //��¼���ϴ����
  last_error_1=speed_error;             //��¼�ϴ����
  /*******����޷� Σ�գ��󶯣�*********/
  if(speed_out>230)speed_out=230;
  else if(speed_out<-230)speed_out=-230;
  else;
  /*******��ת�¼�&������������********/
  if(Feed_speed<15 && (speed_out>30||speed_out<-30))
    Block_motor_time_flag++;            //��ת��ʱ
  else Block_motor_time_flag=0;        
  if(Block_motor_time_flag>20||Block_motor_delay_flag)
  {                                     //��תʱ����ֵ��250ms
    Block_motor_time_flag=0;            //��ת��ʱ��λ
    Block_motor_delay_flag=1;           //��ת������λ
    Block_motor_duty_flag=1;            //��ת�¼���λ
    speed_out=0;                        //��ת�������
  }
  else
  {
    Block_motor_delay_flag=0;           //��ת������λ
    Block_motor_duty_flag=0;            //��ת�¼���λ
  }
  /**********˫����PWM����*************/
  PWM_CH5=(signed int)(125-(speed_out/2));
  PWM_CH2=(signed int)(125+(speed_out/2));
  /*******PWM�޷� Σ�գ��󶯣�*********/
  if(PWM_CH5>240)PWM_CH5=240;
  else if(PWM_CH5<10)PWM_CH5=10;
  else;
  if(PWM_CH2>240)PWM_CH2=240;
  else if(PWM_CH2<10)PWM_CH2=10;
  else;
  /*************PWM���****************/
  ftm_pwm_duty(FTM0,FTM_CH0,PWM_CH5);
  ftm_pwm_duty(FTM0,FTM_CH1,PWM_CH2);
}
/*****************ִ�л����ۺϿ��ƺ���*********************/
void CAR_Drive_duty(void)           //ִ�л����ۺϿ��ƺ���
{
  /************�������**************/
  if(gpio_get(PTE4))
   DJ_PID(40);                      //�������ֵ
  else DJ_PID(mid_point[img_y_control]);//�������
  /************�ٶȿ���**************/
  if(street_len>50)
  speed_need=speed_need_Boost;  
  else
  { 
    if(street_len>40&&street_len<50)
    speed_need=speed_need_L;            
    else
    speed_need=speed_need_normal;
  }
  /************�������**************/
  if(KEY_start_time_flag<100)//ͣ��
  {
    ftm_pwm_duty(FTM0,FTM_CH0,130);
    ftm_pwm_duty(FTM0,FTM_CH1,125);
  }
  else                      //PID����    
  {
    speed_PID(speed_need);                            
  }
}
/*******************************************************************************/
//                      ��һ���֣��������Ʋ���
//
//                      ��һ���֣�����������
/*******************************************************************************/
void  main(void)
{
  /**************��ʼ������***************/
  SCB->CPACR|=((3UL<<10*2)|(3UL<<11*2));//����Ӳ������
  IO_init();                            //I/O��ʼ��
  LCD_Init();                           //OLED��ʼ��
  len_init();                           //������ȳ�ʼ��
  camera_init(imgbuff);                 //����ͷ��ʼ��
  pit_init_ms(PIT0,10);                 //pit0��ʱ�ж�(���ڱ������ɼ�)
  NVIC_SetPriorityGrouping(4);          //�������ȼ�����,3bit
  NVIC_SetPriority(PORTC_IRQn,0);       //�������ȼ�PORTA
  NVIC_SetPriority(PORTD_IRQn,1);       //�������ȼ�PORTD
  NVIC_SetPriority(DMA0_IRQn,2);        //�������ȼ�DMA0
  NVIC_SetPriority(PIT0_IRQn,3);        //�������ȼ�PIT0
  set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);//����PORTD���жϸ�λ����
  set_vector_handler(PORTC_VECTORn ,PORTC_IRQHandler);//����PORTA���жϷ�����
  set_vector_handler(DMA0_VECTORn  ,DMA0_IRQHandler); //���� DMA0���жϷ�����
  set_vector_handler(PIT0_VECTORn  ,PIT0_IRQHandler); //���� PIT0 ���жϷ�����
  enable_irq(PORTD_IRQn);               //ʹ��PORTD�ж�
  enable_irq(PIT0_IRQn);                //ʹ��PIT0�ж�
  left_point[img_y_max]=0;
  right_point[img_y_max]=80;
  /**************��ѭ������***************/
  while(1)
  {
    //pit_time_start(PIT1);                     //ȡ�����ν�����������ʱ��
    camera_get_img();                           //����ͷ��ȡͼ��
    //pit_time_start(PIT1);                     //ȡ�����ν�������������ʱ��
    img_extract(img, imgbuff, CAMERA_SIZE);     //���ݽ�ѹ
    IMG_DUTY();                                 //��ͼ������
    CAR_Drive_duty();                           //ִ�л����ۺϿ���
    GUI_Duty();                                 //���Խ��溯��
    //code_Time_flag =pit_time_get_us(PIT1)/1000;//ȡ�����ν�����ʱ��
  }
}
/*******************************************************************************/
//                     ��һ���֣�����������
//
//                     ��һ���֣��жϺ�������
/*******************************************************************************/
/*****************����ͷ��/���ж�**********************/
void PORTC_IRQHandler()
{
    uint8  n;                    //���ź�
    uint32 flag;
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;           //���жϱ�־λ
    n = 6;                     //���ж�
    if(flag & (1 << n))         //PTA29�����ж�
    {camera_vsync();}
}
/********************DMA���ݴ����ж�*********************/
void DMA0_IRQHandler()
{
    camera_dma();
}
/*******************PIT0�����������ж�*******************/
void PIT0_IRQHandler()
{
    PIT_Flag_Clear(PIT0);
    FEED_COUNT();
}
/*****************PORTD�����������ж�********************/
void PORTD_IRQHandler(void)
{
    uint8  n = 0;              //���ź�
    n = 0;
    if(PORTD_ISFR & (1 << n))  //PTD0 �����ж�
    {
        PORTD_ISFR  = (1 << n);//д1���жϱ�־λ
        Feed_flag++;           //�������ɼ�����...
    }
}