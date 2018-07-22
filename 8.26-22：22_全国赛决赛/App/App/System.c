#include "System.h"

extern const unsigned char gImage_123456[];

Key_Parameter TYPE[MaxLengh]={{"OV7725_CNST:",0,2},
                              {"Ramp_OFF_ON:",0,1},
                              {"Ramp_Speed :",0,5},
                              {"Ramp_Delay :",0,10},
                              {"S3010_P    :",0,1},
                              {"Annu_OFF_ON:",0,1},
                    };
Key_Parameter *Typemeber;

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //Ԥ��
uint8 Image_Send_Flag = 1;	// ͼ���Ϳ��ƣ�1Ϊ���ͣ�0Ϊ������
uint8 Send_OK = 0;

uint8 CNST_Value=60;
uint8 key_state,Key_Flag=0;
uint8 Img_Display;
void System_Init()
{
        uint32 i;
        uint8 j;
        /************************ ���� K60 �����ȼ�  ***********************/

        NVIC_SetPriorityGrouping(4);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�

        NVIC_SetPriority(PORTA_IRQn,0);         //�������ȼ�
        NVIC_SetPriority(PORTE_IRQn,1);         //�������ȼ�
        NVIC_SetPriority(DMA0_IRQn,2);          //�������ȼ�
        NVIC_SetPriority(PIT0_IRQn,3);          //�������ȼ�
//        NVIC_SetPriority(PIT1_IRQn,4);          //�������ȼ�
        /********************** PID������ʼ�� ***********************/
        PlacePID_Init(&S3010_PID);//���PID������ʼ��   ������δ����
        PlacePID_Init(&CS_PID);//���PID������ʼ��   ������δ����
        IncPID_Init(&Left_MOTOR_PID,Left_MOTOR);	//�����   PID������ʼ��
        IncPID_Init(&Right_MOTOR_PID,Right_MOTOR);	//�����   PID������ʼ��
        
        /*********************** ��� ��ʼ�� ***********************/  
        ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,S3010_MID);   //��ʼ�� ��� PWM
        
        /************************** LED ��ʼ��  ***************************/
        led_init(LED_MAX);
        /************************** OLED ��ʼ��  ***************************/
        OLED_IIC_Init();
        OLED_Init();
        OLED_Fill(0xff);//ȫ����
        /************************** ���뿪�� ��ʼ��  ***************************/
         Boma_Key_Init();
        /************************ ���ֲ��� ��ʼ��  ************************************/
        Parameter_Init();
        
        while(!Key_Flag)
        {
              KEY_Adjuet();
              if(Key_Flag)break;
        }
        CNST_Value = Typemeber->Value;        //ͬ������ͷ��ֵ
        Ramp_OFF_ON=(Typemeber+1)->Value;
        Ramp_Speed = (Typemeber+2)->Value;   //ͬ���µ��ٶ�
        Ramp_Delay = (Typemeber+3)->Value;   //ͬ���µ���ʱʱ��
        S3010[Set][0] = (Typemeber+4)->Value; //ͬ�����P
        Annu_OFF_ON  = (Typemeber+5)->Value;
        SCCB_WriteByte (OV7725_CNST, CNST_Value);
        
        if (Img_Display)OLED_Fill(0xff);//ȫ����
        else 
        {
            OLED_Fill(0xff);//ȫ����
            OLED_DrawBMP(32,0,96,8,(unsigned char *)gImage_123456);//����BMPλͼ��ʾ
        }
        led(LED_MAX,LED_ON);// ��LEDָʾ��		ȫ����ʼ���ɹ�
        /************************ ���ֲ��� ��ʼ�� ���� ************************************/
        
        
        
        
     
        
        /************************ ������ ��ʼ�� **********************************/        
//        	while (MPU6050_Init());		// �����ǳ�ʼ���ɹ�����0
//	
//	for (i = 0; i < 400; i++)
//	{
//		for (j = 0; j < 5; j++)
//		{
//			MPU6050_GetData(&GYRO, &ACC);	// ��ȡ����������
//			Data_Filter();					// ��ԭʼ���ݻ����˲�
//			DELAY_US(100);
//		}
//		//Get_Attitude();	// ��̬����
//	}

  	
	
	
	/************************ ��� ��ʼ��  ***********************/
	ftm_pwm_init(MOTOR_FTM, Left_MOTOR1_PWM,MOTOR_HZ,0);      //��ʼ�� �����1 PWM
	ftm_pwm_init(MOTOR_FTM, Left_MOTOR2_PWM,MOTOR_HZ,0);      //��ʼ�� �����2 PWM 
        ftm_pwm_init(MOTOR_FTM, Right_MOTOR1_PWM,MOTOR_HZ,0);      //��ʼ�� �����1 PWM
	ftm_pwm_init(MOTOR_FTM, Right_MOTOR2_PWM,MOTOR_HZ,0);      //��ʼ�� �����2 PWM 
	/************************ ��������  ***********************/   
	ftm_quad_init(FTM1);                                    //FTM1 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
	ftm_quad_init(FTM2);                                    //FTM2 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
                                                               //ʹ�ö�ʱ��0������10msִ��һ��
        
        /************************ ����ͷ ��ʼ��  ***********************/
        camera_init(p_imgbuff1);                                   //����ͷ��ʼ������ͼ��ɼ��� imgbuff ��ַ
        //�����жϷ�����
        set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //���� PORTA ���жϷ�����Ϊ PORTA_IRQHandler
        set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //���� DMA0 ���жϷ�����Ϊ DMA0_IRQHandler
        
          	/************************ ����ģ�� NRF ��ʼ��  ***********************/

//        while(!nrf_init());
//        //�����жϷ�����
//        set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);                //���� PORTE ���жϷ�����Ϊ PORTE_VECTORn
//        enable_irq(PORTE_IRQn);
//        nrf_msg_init();                                                     //����ģ����Ϣ��ʼ��
        
        //����ͷģ�飬��Ҫ ���� ������� ���԰�ģ�飬����������� ����ͼ��ʧ�ܶ�������������
//        i = 20;
//        while(i--)
//        {
//          nrf_msg_tx(COM_RETRAN,nrf_tx_buff);                              //���Ͷ�� �� �����ȥ��������� ���ն˵Ļ�������
//        }



	/******************** ��ʱ�� ��ʼ��  *********************/ 
        pit_init_ms(PIT0,10);                                               //pit ��ʱ�ж�(���ڰ�����ʱɨ��)
        set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //���� PIT0 ���жϷ�����Ϊ PIT0_IRQHandler
        enable_irq(PIT0_IRQn);							  //ʹ���ж�
        
//        pit_init_ms(PIT1,20);                                               //pit ��ʱ�ж�(���ڰ�����ʱɨ��)
//        set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);                  //���� PIT0 ���жϷ�����Ϊ PIT0_IRQHandler
//        enable_irq(PIT1_IRQn);							  //ʹ���ж�
  
	/************************ ͼ��ɼ� **************************/
//	camera_get_img();//�Ȳɼ�һ��ͼ��
        
      /************************ LED ��ʼ��  ************************************/
      led(LED_MAX,LED_OFF);// �ر�LEDָʾ��	

}


void data_send()
{		  
	com_e     com;
	nrf_result_e nrf_result;
	//uint8 imgtxflag = 0;    //0��ʾ����ͼ��
  
  	/************************ ���߷��ͺͽ�������  ***********************/
  	        do
        {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
            if(nrf_result == NRF_RESULT_RX_VALID)
            {
                switch(com)
                {
                    case COM_IMG:
                        //��ʾ �� ��Ļ��
//#if     ( CAMERA_COLOR == 1 )                                       //�Ҷ�����ͷ
//                        LCD_Img_gray_Z(site,size, imgbuff,imgsize);
//#elif   ( CAMERA_COLOR == 0 )                                       //�ڰ�����ͷ
//                        LCD_Img_Binary_Z(site,size, imgbuff,imgsize);
//#endif
                        break;

                    case COM_VAR:

                        break;
                    default:
                        break;
                }
            }
        }while(nrf_result != NRF_RESULT_RX_NO);         //���ղ������� ���˳�

        //��ͼ�� ���ͳ�ȥ
        nrf_msg_tx(COM_IMG,nrf_tx_buff);

        while(nrf_tx_state() == NRF_TXING);             //�ȴ��������

	 var_syn(VAR2);
	 var_syn(VAR3);
//	 var_syn(VAR4);
//	 var_syn(VAR5);
//	 var_syn(VAR6);
}

/*************************����***************************************/

void Boma_Key_Init(void)
{
     gpio_init(PTE0, GPI, 0);
     port_init_NoALT(PTE0, PULLDOWN);         //���ָ��ò��䣬�����ı�����ѡ��
          gpio_init(PTE1, GPI, 0);
     port_init_NoALT(PTE1, PULLDOWN);         //���ָ��ò��䣬�����ı�����ѡ��
          gpio_init(PTE2, GPI, 0);
     port_init_NoALT(PTE2, PULLDOWN);         //���ָ��ò��䣬�����ı�����ѡ��
          gpio_init(PTE3, GPI, 0);
     port_init_NoALT(PTE3, PULLDOWN);         //���ָ��ò��䣬�����ı�����ѡ��
          gpio_init(PTE4, GPI, 0);
     port_init_NoALT(PTE4, PULLDOWN);         //���ָ��ò��䣬�����ı�����ѡ��
          gpio_init(PTE5, GPI, 0);
     port_init_NoALT(PTE5, PULLDOWN);         //���ָ��ò��䣬�����ı�����ѡ��
          gpio_init(PTE6, GPI, 0);
     port_init_NoALT(PTE6, PULLDOWN);         //���ָ��ò��䣬�����ı�����ѡ��
          gpio_init(PTE7, GPI, 0);
     port_init_NoALT(PTE7, PULLDOWN);         //���ָ��ò��䣬�����ı�����ѡ��
     
     //����
    key_init(KEY_U);
    key_init(KEY_D);
    key_init(KEY_L);
    key_init(KEY_R);
        
}


void Parameter_Init(void)
{
    if (gpio_get(PTE7) == KEY_UP)  //�����Ƿ���ʾͼ��
    {
            Img_Display = 1;
    }
    else
    {
            Img_Display = 0;
    }
     if(gpio_get(PTE0) == KEY_UP)          //��һ����·
    {
        Annulus_Count[0] = 1;
    }
    else 
    {
        Annulus_Count[0] = 0;
    }
    if(gpio_get(PTE1) == KEY_UP)           //�ڶ�����·
    {
       Annulus_Count[1] = 1;
    }
    else
    {
        Annulus_Count[1] = 0;
    }
    if(gpio_get(PTE2) == KEY_UP)           //��������·
    {
      Annulus_Count[2] = 1;
    }
    else
    {
      Annulus_Count[2] = 0;
    }
    if(gpio_get(PTE3) == KEY_UP)           //���ĸ���·
    {
      Hazard_Left_Right=1;
    }
    else
    {
      Hazard_Left_Right=0;
    }
    if(Hazard_Left_Right)Point_Mid=Point_Right;
    else Point_Mid=Point_Left;
//    if(gpio_get(PTE3) == KEY_DOWN&&gpio_get(PTE4) == KEY_DOWN)
//    {
//          SCCB_WriteByte (OV7725_CNST, 50);	//�ı�ͼ����ֵ	����50	����60	
//    }
//    else if(gpio_get(PTE3) == KEY_DOWN&&gpio_get(PTE4) == KEY_UP)
//    {
//          SCCB_WriteByte (OV7725_CNST, 53);	//�ı�ͼ����ֵ	����50	����60	
//    }
//    else if(gpio_get(PTE3) == KEY_UP&&gpio_get(PTE4) == KEY_DOWN)
//    {
//          SCCB_WriteByte (OV7725_CNST, 56);	//�ı�ͼ����ֵ	����50	����60	
//    }
//     else if(gpio_get(PTE3) == KEY_UP&&gpio_get(PTE4) == KEY_UP)
//    {
//          SCCB_WriteByte (OV7725_CNST, 60);	//�ı�ͼ����ֵ	����50	����60	
//    }
    if (gpio_get(PTE4) == KEY_DOWN)
    {
            if(gpio_get(PTE5) == KEY_DOWN&&gpio_get(PTE6) == KEY_DOWN)     //�ٶ��趨
            {
                  Speed_Set = 1;
                  Set = 0; 
            }
            else if(gpio_get(PTE5) == KEY_DOWN&&gpio_get(PTE6) == KEY_UP)
            {
                  Speed_Set = 2;
                  Set = 1; 
            }
            else if(gpio_get(PTE5) == KEY_UP&&gpio_get(PTE6) == KEY_DOWN)
            {
                  Speed_Set = 3;
                  Set = 1;
            }
             else if(gpio_get(PTE5) == KEY_UP&&gpio_get(PTE6) == KEY_UP)
            {
                  Speed_Set = 4;
                  Set = 0;
            }
    }
    if (gpio_get(PTE4) == KEY_UP)
    {
            if(gpio_get(PTE5) == KEY_DOWN&&gpio_get(PTE6) == KEY_DOWN)     //�ٶ��趨
            {
                  Speed_Set = 5;
                  Set = 1;
            }
            else if(gpio_get(PTE5) == KEY_DOWN&&gpio_get(PTE6) == KEY_UP)
            {
                  Speed_Set = 6;
                  Set = 1;
            }
            else if(gpio_get(PTE5) == KEY_UP&&gpio_get(PTE6) == KEY_DOWN)
            {
                  Speed_Set = 7;
                  Set = 1;
            }
             else if(gpio_get(PTE5) == KEY_UP&&gpio_get(PTE6) == KEY_UP)
            {
                  Speed_Set = 8;
                  Set = 1;
            }
    }
     
//     if(gpio_get(PTE7) == KEY_UP)           //���ư���λ
//    {
//        key
//    }
//    else
//    {
//    
//    }
        Typemeber=TYPE;
        OLED_ShowStr(0,0,Typemeber->name, 1);
        OLED_ShowStr(0,1,(Typemeber+1)->name, 1);
        OLED_ShowStr(0,2,(Typemeber+2)->name, 1);
        OLED_ShowStr(0,3,(Typemeber+3)->name, 1);
        OLED_ShowStr(0,4,(Typemeber+4)->name, 1);
        OLED_ShowStr(0,5,(Typemeber+5)->name, 1);
        
        Typemeber->Value=CNST_Value;
        (Typemeber+1)->Value=Ramp_OFF_ON;
        (Typemeber+2)->Value=Ramp_Speed;
        (Typemeber+3)->Value=Ramp_Delay;
        (Typemeber+4)->Value=S3010[Set][0];
        (Typemeber+5)->Value=Annu_OFF_ON;
}

uint8 wait_key_down()
{
    uint8 ret_key=0xff;
    uint16 key_time=0;
    while(1)
    {
      if(key_check(KEY_U)==KEY_DOWN)
          ret_key=KEY_U;
      else if(key_check(KEY_D)==KEY_DOWN)
          ret_key=KEY_D;
      else if(key_check(KEY_L)==KEY_DOWN)
          ret_key=KEY_L;
      else if(key_check(KEY_R)==KEY_DOWN)
          ret_key=KEY_R;
      if(ret_key!=0xff)
      {
          key_state=KEY_DOWN;
          while(key_get(ret_key)==KEY_DOWN)
          {
              DELAY_MS(1);
              key_time++;
          }
          if(key_time>300)
          {
              key_state=KEY_HOLD;
          }
          break;
      }
    }
    return ret_key;
}


void KEY_Adjuet(void)
{
  uint8 key=0;
  static uint8 i=0;
 
  key = wait_key_down();
  
  switch (key)
  {
    case KEY_U:
    {
        if(i>=MaxLengh)i=0;
        Typemeber=TYPE+i;
        OLED_ShowStr(0,7,"              ", 1);
        OLED_ShowStr(0,7,Typemeber->name, 1);
        OLED_ShowLongNum(72,7,Typemeber->Value);
        i++;
        OLED_ShowLongNum(72,i-1,0);
        OLED_ShowLongNum(72,i-1,Typemeber->Value);
    }
    break;
    case KEY_D:
    {
      Typemeber->Value+=Typemeber->Step;
      OLED_ShowLongNum(72,7,0);
      OLED_ShowLongNum(72,7,Typemeber->Value);
      OLED_ShowLongNum(72,i-1,0);
      OLED_ShowLongNum(72,i-1,Typemeber->Value);
    }
    break;
  case KEY_L:
    {
        Typemeber->Value-=Typemeber->Step;
        OLED_ShowLongNum(72,7,0);
        OLED_ShowLongNum(72,7,Typemeber->Value);
        OLED_ShowLongNum(72,i-1,0);
        OLED_ShowLongNum(72,i-1,Typemeber->Value);
    }
    break;
    case KEY_R:
    {
      Key_Flag=1;
      Typemeber=TYPE;
      i=0;
    }
    break;
    
  }

}