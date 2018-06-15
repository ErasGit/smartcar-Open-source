

#include "headfile.h"


/*ϵͳ������ʼ������*/
void PIDStructurePointerInit(PID_Type_Def *PID_Balance, PID_Type_Def *PID_Speed, PID_Type_Def *PID_Path, PID_Type_Def *PID_Diff ){ //PID��ʼֵ����


  PID_Balance->Kp = 8;
  PID_Balance->Kd = 1.2;
  PID_Balance->out = 0;
  
  
  PID_Speed->Kp = 10;
  PID_Speed->Ki = 0.05;
  PID_Speed->out   = 0;
  
  
  PID_Path->Kp = 2.0;
  PID_Path->Kd = 0.0;
  PID_Path->offset = 1.0;
  PID_Path->out    = 0;
  

  PID_Diff->Kp = 15.0;
  PID_Diff->Kd = 2.0;
  PID_Diff->Ki = 0.0; 
  
  PID_Diff->offset = 0.35;
  PID_Diff->out    = 0;
  
  
  for(uint8 n=0; n<10; n++){
    
    PID_Balance->err_k[n] = 0;
    PID_Speed->err_k[n]   = 0;
    PID_Path->err_k[n]    = 0;
    PID_Diff->err_k[n]    = 0;  
    PID_Speed->out_k[n]   = 0;
  };
  
};



void SensorStructurePointerInit(Acc_Type_Def *Acc, Gyro_Type_Def *Gyro, Angle_Type_Def *Angle, Speed_Type_Def *Speed, Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB){ //�ṹ��ָ���ʼ����Ա

  
  /*Gyro*/  
  Gyro->X_Final=0;
  Gyro->X_Offset=0;
  Gyro->X_Value[0]=0;
  Gyro->X_Value[1]=0;
  Gyro->X_Value[2]=0;

  Gyro->Z_Final=0;
  Gyro->Z_Offset=0;
  Gyro->Z_Value[0]=0;
  Gyro->Z_Value[1]=0;
  Gyro->Z_Value[2]=0;
  
  InductorA->LeftFinal=0;
  InductorA->RightFinal=0;
  InductorA->LeftValue[0]=0;
  InductorA->LeftValue[1]=0;
  InductorA->LeftValue[2]=0;
  
  InductorB->LeftFinal=0;
  InductorB->RightFinal=0;
  InductorB->LeftValue[0]=0;
  InductorB->LeftValue[1]=0;
  InductorB->LeftValue[2]=0;

  //�Ƕ�Ŀ��ֵ  Ҳ����ƽ��ֵ  û��ת���ɽǶ�ֵ ���������Թ�ϵ��
  Angle->PiontValue = 2300;
  
  //�ٶ�Ŀ��ֵ  û��ת������ʵ�ٶ�  ���ٶ����Թ�ϵ
  Speed->PiontSpeed = 2000;

  
  Speed->PiontSpeedMax=5000;
 
  Speed->PiontSpeedMin=1500;

  
  
  
  //�˴�Ӧ�ճ����������� ����ڵ������Ϸ� ���ֵ ����뵼����Զ��Ϊ��Сֵ 
  InductorA->LeftMax  = 1200;
  InductorA->LeftMin  = 125;
  InductorA->RightMax = 1200;
  InductorA->RightMin = 125;
  
  
  InductorB->LeftMax = 1200;
  InductorB->LeftMin = 125;
  InductorB->RightMax = 1200;
  InductorB->RightMin = 125;
  

  
};


void CarBalanceSystemInit(Gyro_Type_Def *Gyro){//ϵͳ��ʼ��

  
  //uart_init(UART1,9600);
 
  gpio_init(H2,GPO,1);
  
  //����ADC�ܽŽӵĶ��� ��headfile.h���ж���
  adc_init(ADC0_SE0);	
  adc_init(ADC0_SE1);	
  adc_init(ADC0_SE2);	
  adc_init(ADC0_SE3);	
  adc_init(ADC0_SE4);	
  adc_init(ADC0_SE5);	
  adc_init(ADC0_SE6);
  adc_init(ADC0_SE7);
  
  adc_init(ADC0_SE8);		
  adc_init(ADC0_SE9);	
  adc_init(ADC0_SE10);	
  adc_init(ADC0_SE11);	
  adc_init(ADC0_SE12);	
  adc_init(ADC0_SE13);	
  adc_init(ADC0_SE14);	
  adc_init(ADC0_SE15);
  
 
  
  
  OLED_Init();
  
  OLED_ShowString(0,0,"SysInit Finish!");
  OLED_ShowString(0,16,"Car balance2.0V");
  OLED_Refresh_Gram();

  // ʹ������������  Ҫ��ʼ��IIC
  //IIC_init();
  
  
  //�˴���������ֵ��ȡ����Ҫ   ��������������Ĭ�Ͼ�����ֵ �����ȡ  
  // ���������������ֵΪ0  ��ô  �˴� ֱ�Ӹ�ֵ0����
  Gyro->Z_Offset = adc_once(ADC0_SE2, ADC_12bit);
  Gyro->X_Offset = adc_once(ADC0_SE3, ADC_12bit);
  
  
  systick_delay_ms(10);

  //����
  ftm_pwm_init(ftm2,ftm_ch2,13000,0);	
  ftm_pwm_init(ftm2,ftm_ch3,13000,0);		
  
  //�ҵ��  
  ftm_pwm_init(ftm2,ftm_ch4,13000,0);		
  ftm_pwm_init(ftm2,ftm_ch5,13000,0);	
  

  systick_delay_ms(10);  
  
  //ftm�ļ������ſɲ鿴KEA128_port_cfg.h�ļ��ڵ�FTM0_COUNT_PIN��FTM1_COUNT_PIN�ĺ궨���֪
  //һ��ftmͬһʱ��ֻ��ʵ��һ�ֹ��ܣ�������ڲ��پͲ����������PWM	
  ///////////////////////////////////////////////////////////////////////////////////////////////
  ftm_count_init(ftm0);   //��E0���������������м���    E0�ӱ�����LSB    
  gpio_init(E1,GPI,0);    //�����жϷ���                  C5�ӱ�����DIR
  port_pull(E1);          //IO����
  
  ftm_count_init(ftm1);   //��E7���������������м���    E7�ӱ�����LSB
  gpio_init(H6,GPI,0);    //�����жϷ���                  H5�ӱ�����DIR
  port_pull(H6);          //IO����
  //////////////////////////////////////////////////////////////////////////////////////////////
 

  //��ʱ���ж�
  pit_init(pit0,0x9000);//1.539ms
  
  systick_delay_ms(10);  
  
  

};



//����ֵ����   ����Ҫȡ��ַ�����β�
void AbsoluteValue(float *Value, float *Return){
  
  if( (*Value) <0) (*Return) = -1*(*Value);
  else *Value = *Return;
}

/*ԭʼ���ݻ�ȡ����*/
void GetAccelerationValue(Acc_Type_Def *Acc){ //��ȡACCԭʼ����
  
  Acc->Z_Value[0] = Acc->Z_Value[1];
  Acc->Z_Value[1] = Acc->Z_Value[2];
  Acc->Z_Value[2] = ACC_Z;//�����ǻ�ȡ��ǰ���ٶ�  �����䶼�� �ϴ�ֵ�����ϴ�ֵ

  //���ö��ֵ�����˲�  �������е�����Ч��
  Acc->Z_Final = 0.33*Acc->Z_Value[2]+0.33*Acc->Z_Value[1]+0.33*Acc->Z_Value[0];
  
};

void GetAngleSpeedValue(Gyro_Type_Def *Gyro){  //��ȡGyroԭʼ����
  
  Gyro->Z_Value[0] = Gyro->Z_Value[1];
  Gyro->Z_Value[1] = Gyro->Z_Value[2];
  Gyro->Z_Value[2] = GYRO_Z - Gyro->Z_Offset;//������������������Gyro->Z_Offset��ֵ  �ڳ�ʼ����ʱ��
  
  Gyro->X_Value[0] = Gyro->X_Value[1]; //ǰǰ��
  Gyro->X_Value[1] = Gyro->X_Value[2]; //ǰ��
  Gyro->X_Value[2] = GYRO_X - Gyro->X_Offset; //����
  
  
};


void GetInductorValue(Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB){ //��ȡinductorԭʼ����
  
  
  InductorA->LeftValue[0] = InductorA->LeftValue[1];
  InductorA->LeftValue[1] = InductorA->LeftValue[2];
  InductorA->LeftValue[2] = L1;//�����ǻ�ȡ��ǰ���ֵ  L1��haedfile.h�ļ����ж��� 
  
  InductorA->RightValue[0] = InductorA->RightValue[1];
  InductorA->RightValue[1] = InductorA->RightValue[2];
  InductorA->RightValue[2] = L2;//�����ǻ�ȡ��ǰ���ֵ  L2��haedfile.h�ļ����ж��� 
  
  InductorB->LeftValue[0] = InductorB->LeftValue[1];
  InductorB->LeftValue[1] = InductorB->LeftValue[2];
  InductorB->LeftValue[2] = L3;//�����ǻ�ȡ��ǰ���ֵ  L3��haedfile.h�ļ����ж��� 

  InductorB->RightValue[0] = InductorB->RightValue[1];
  InductorB->RightValue[1] = InductorB->RightValue[2];
  InductorB->RightValue[2] = L4;//�����ǻ�ȡ��ǰ���ֵ  L4��haedfile.h�ļ����ж��� 
  
  
};

void GetSpeedValue(Speed_Type_Def *Speed){ //��ȡSpeedԭʼ����
  
  
  for(uint8 n=0; n<15 ; n++){  //ѭ������
    
    //�洢ǰ14�ε�ֵ  
    Speed->ActualLiftPulse[n] = Speed->ActualLiftPulse[n+1]; //0~14λ��ֵ
   
    if( n==14 ){ //��ǰֵ 15��  ����ֵ
      Speed->ActualLiftPulse[n+1] = ftm_count_get(ftm1)*(-1); //���� 15λ��ֵ  
      
      ftm_count_clean(ftm1);  //���ftm1����ֵ  
    }//�˴�һ��Ҫ�޸ĺ� ��������ʵ�ʵ�Ӳ��  ��Ӧ����������  ��Ϊ�������Ƕ��Ű�װ��  ��ô
    //�ڳ�����ǰ�ߵ�ʱ��  �������෴��  ����Ҫ������һ����ֵ��������Ϊ����
    //�����������ͬ��������  ���Ƕ��Ű�װ��  
  }
  
  
  
  for(uint8 n=0; n<15 ; n++){  //ѭ������
    
    Speed->ActualRightPulse[n] = Speed->ActualRightPulse[n+1]; //0~14λ��ֵ
   
    if( n==14 ){
      Speed->ActualRightPulse[n+1] = ftm_count_get(ftm0);  //���� 15λ��ֵ
      ftm_count_clean(ftm0);  //���ftm0����ֵ  
    }
    
  }

  if(gpio_get(H6)) //��� ftm1��ת��   H6  E1��������ұ�������  ҪŪ��� ��Ҫ��ʱ��Ĺ��Ժ�
    //��߱������ķ���  ȥ�����ұ߱������� ������   �Ǿ��군��
    Speed->ActualLiftPulse[15] = Speed->ActualLiftPulse[15];
  else                
    Speed->ActualLiftPulse[15] = -Speed->ActualLiftPulse[15];             
  
  
  if(gpio_get(E1))  //��� ftm0��ת��  
    Speed->ActualRightPulse[15] = Speed->ActualRightPulse[15];
  else                
    Speed->ActualRightPulse[15] = -Speed->ActualRightPulse[15]; //�������ȡ�����㳵��ǰ��ʱ  ������ֵ�Ƿ�Ϊ��      
  
};


void GetButtonStatus(Button_Type_Def *Button){ //Buttonԭʼ���ݻ�ȡ
  
  Button->Up[0] = Button->Up[1];
  Button->Up[1] = Button->Up[2];
  Button->Up[2] = Button->Up[3];
  Button->Up[3] = Button->Up[4];
  Button->Up[4] = UP;//���Ĵ������µ�ֵ  ����ֵ  ��haedfile.h�ļ����ж���
  
  Button->Down[0] = Button->Down[1];
  Button->Down[1] = Button->Down[2];
  Button->Down[2] = Button->Down[3];
  Button->Down[3] = Button->Down[4];
  Button->Down[4] = DOWN;//���Ĵ������µ�ֵ  ����ֵ  ��haedfile.h�ļ����ж���
  
  Button->Life[0] = Button->Life[1];
  Button->Life[1] = Button->Life[2];
  Button->Life[2] = Button->Life[3];
  Button->Life[3] = Button->Life[4];
  Button->Life[4] = LIFE;//���Ĵ������µ�ֵ  ����ֵ  ��haedfile.h�ļ����ж���
  
  Button->Right[0] = Button->Right[1];
  Button->Right[1] = Button->Right[2];
  Button->Right[2] = Button->Right[3];
  Button->Right[3] = Button->Right[4];
  Button->Right[4] = RIGHT;//���Ĵ������µ�ֵ  ����ֵ  ��haedfile.h�ļ����ж���
  
  Button->Enter[0] = Button->Enter[1];
  Button->Enter[1] = Button->Enter[2];
  Button->Enter[2] = Button->Enter[3];
  Button->Enter[3] = Button->Enter[4];
  Button->Enter[4] = ENTER;//���Ĵ������µ�ֵ  ����ֵ  ��haedfile.h�ļ����ж���
 
};


void GetSwitchStatus(Switch_Type_Def *Switch){ //����ԭʼ���ݻ�ȡ
  
  Switch->Bm1[0] = Switch->Bm1[1];
  Switch->Bm1[1] = Switch->Bm1[2];
  Switch->Bm1[2] = SW1; //���뿪��  ���Ĵ������µ�ֵ  ��haedfile.h�ļ����ж���
  
  Switch->Bm2[0] = Switch->Bm2[1];
  Switch->Bm2[1] = Switch->Bm2[2];
  Switch->Bm2[2] = SW2; //���뿪��  ���Ĵ������µ�ֵ  ��haedfile.h�ļ����ж���
  
  Switch->Bm3[0] = Switch->Bm3[1];
  Switch->Bm3[1] = Switch->Bm3[2];
  Switch->Bm3[2] = SW3; //���뿪��  ���Ĵ������µ�ֵ  ��haedfile.h�ļ����ж���
  
  Switch->Bm4[0] = Switch->Bm4[1];
  Switch->Bm4[1] = Switch->Bm4[2];
  Switch->Bm4[2] = SW4; //���뿪��  ���Ĵ������µ�ֵ  ��haedfile.h�ļ����ж���
 
  Switch->Bm5[0] = Switch->Bm5[1];
  Switch->Bm5[1] = Switch->Bm5[2];
  Switch->Bm5[2] = SW5; //���뿪��  ���Ĵ������µ�ֵ  ��haedfile.h�ļ����ж���
  
  Switch->Bm6[0] = Switch->Bm6[1];
  Switch->Bm6[1] = Switch->Bm6[2];
  Switch->Bm6[2] = SW6; //���뿪��  ���Ĵ������µ�ֵ  ��haedfile.h�ļ����ж���
};

/*ԭʼ���ݴ�����*/
void GyroHighPassFilter(Gyro_Type_Def *Gyro){ //Gyro�˲�

  float K =0.58;//Ȩֵ�˲�  ���������
  
  Gyro->Z_Value[2] = K * Gyro->Z_Value[2] +  (1-K) * Gyro->Z_Value[1];
  
  Gyro->Z_Final = Gyro->Z_Value[2];
 

  Gyro->X_Value[2] = K * Gyro->X_Value[2] +  (1-K) * Gyro->X_Value[1];
  
  Gyro->X_Final = Gyro->X_Value[2];
  

  //Ȩֵ�˲� ȥ����Ƶ����
};


void AngleHardwareFilter(Angle_Type_Def *Angle){ //�Ƕ��˲�
  //��������ֱ�Ӳɼ��Ƕȵ�  ֻ��ģ�������Ǻͼ��ٶȼƽ���Ӳ���ں�ֱ�ӽ���Ƕ�ֵ
  
  Angle->ActualValue[0] = Angle->ActualValue[1];
  Angle->ActualValue[1] = Angle->ActualValue[2];
  Angle->ActualValue[2] = ANGLE; //�˴��ǻ�ȡ ���½Ƕ�ֵ  
  
  //�˲� Angle->FinalValueΪ����ֵ
  Angle->FinalValue = 0.6*Angle->ActualValue[2] + 0.2*Angle->ActualValue[1] + 0.2*Angle->ActualValue[0];
  

};

void AngleSoftwareFilter(Acc_Type_Def *Acc, Gyro_Type_Def *Gyro , Angle_Type_Def *Angle){//�Ƕ��˲�
 //������������������������ ͨ����������ں��˲�����Ƕ�  
  
  Acc->Z_Final = 0.9*Acc->Z_Value[2] + 0.05*Acc->Z_Value[1] + 0.05*Acc->Z_Value[0];//Ȩֵ��ͨ�˲�
  
  Gyro->Z_Final = 0.92*Gyro->Z_Value[2] + 0.05*Gyro->Z_Value[1] + 0.03*Gyro->Z_Value[0];//Ȩֵ��ͨ�˲�
  
  //���ٶ�  ���ٶȲɼ�
  
  
  //�˴�������Ҫ����  �廪������ķ���ȥ���ڲ���  �����η�
  Angle->FinalValue = 0.92*(Angle->FinalValue + 0.05*Gyro->Z_Final) + 0.08*(Acc->Z_Final);//�ں��˲�

  
};


void InductorSoftwareFilter(Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB, Angle_Type_Def *Angle, Switch_Type_Def *Switch){//���ֵ��һ�˲� 

  /*Ȩֵ�˲� ��������  */
  InductorA->LeftFinal = 0.8*InductorA->LeftValue[2] + 0.15*InductorA->LeftValue[1] + 0.05*InductorA->LeftValue[0];//-InductorA->LeftMin;
  InductorA->RightFinal = 0.8*InductorA->RightValue[2] + 0.15*InductorA->RightValue[1] + 0.05*InductorA->RightValue[0];//-InductorA->RightMin;
  
  InductorB->LeftFinal = 0.8*InductorB->LeftValue[2] + 0.15*InductorB->LeftValue[1] + 0.05*InductorB->LeftValue[0];
  InductorB->RightFinal = 0.8*InductorB->RightValue[2] + 0.15*InductorB->RightValue[1] + 0.05*InductorB->RightValue[0];


 

  /*���ݹ�һ����0~1000*/
  InductorA->LeftFinal  =  1000*( (float)(InductorA->LeftFinal-InductorA->LeftMin)/(InductorA->LeftMax-InductorA->LeftMin) ); 
  InductorA->RightFinal  = 1000*( (float)(InductorA->RightFinal-InductorA->RightMin)/(InductorA->RightMax-InductorA->RightMin) );
  
  InductorB->LeftFinal  =  1000*( (float)(InductorB->LeftFinal-InductorB->LeftMin)/(InductorB->LeftMax-InductorB->LeftMin) ); 
  InductorB->RightFinal  = 1000*( (float)(InductorB->RightFinal-InductorB->RightMin)/(InductorB->RightMax-InductorB->RightMin) );
  
  if(InductorA->LeftFinal>999)InductorA->LeftFinal=999;
  if(InductorA->LeftFinal<  0)InductorA->LeftFinal=  0;
  if(InductorA->RightFinal>999)InductorA->RightFinal=999;
  if(InductorA->RightFinal< 0)InductorA->RightFinal=   0;
  
  
  if(InductorB->LeftFinal>999)InductorB->LeftFinal=999;
  if(InductorB->LeftFinal<  0)InductorB->LeftFinal=  0;
  if(InductorB->RightFinal>999)InductorB->RightFinal=999;
  if(InductorB->RightFinal< 0)InductorB->RightFinal=   0;
  
  //���AB  A �ǿ����ĵ��  B�ǿ��ڲ���    RL��������
  
  InductorA->AndValues = InductorA->LeftFinal+InductorA->RightFinal;//��к�ֵ
  
  InductorA->LeftDerivative = InductorA->LeftValue[2] - InductorA->LeftValue[0]; //���б仯��
  
  InductorA->RightDerivative= InductorA->RightValue[2] - InductorA->RightValue[0];//�ҵ�б仯��
  
  InductorB->LeftDerivative = InductorB->LeftValue[2] - InductorB->LeftValue[0];
  
};


void SpeedSoftwareFilter(Speed_Type_Def *Speed){   //�ٶȵ�ͨ�˲�
    
  /*��������*/
  Speed->ActualLiftPulse[15]=0.33*Speed->ActualLiftPulse[15] + 0.33*Speed->ActualLiftPulse[14] + 0.33*Speed->ActualLiftPulse[13];
  Speed->ActualRightPulse[15]=0.33*Speed->ActualRightPulse[15] + 0.33*Speed->ActualRightPulse[14] + 0.33*Speed->ActualRightPulse[13];
  // 6 2 2
  
  //ʵ���ٶ� = R+L
  Speed->ActualSpeed[0] = Speed->ActualSpeed[1];
  Speed->ActualSpeed[1] = Speed->ActualSpeed[2];
  Speed->ActualSpeed[2] = 
    (Speed->ActualRightPulse[0]+Speed->ActualLiftPulse[0])+
    (Speed->ActualRightPulse[1]+Speed->ActualLiftPulse[1])+                    
    (Speed->ActualRightPulse[2]+Speed->ActualLiftPulse[2])+
    (Speed->ActualRightPulse[3]+Speed->ActualLiftPulse[3])+
    (Speed->ActualRightPulse[4]+Speed->ActualLiftPulse[4])+
    (Speed->ActualRightPulse[5]+Speed->ActualLiftPulse[5])+
    (Speed->ActualRightPulse[6]+Speed->ActualLiftPulse[6])+
    (Speed->ActualRightPulse[7]+Speed->ActualLiftPulse[7])+
    (Speed->ActualRightPulse[8]+Speed->ActualLiftPulse[8])+
    (Speed->ActualRightPulse[9]+Speed->ActualLiftPulse[9])+
    (Speed->ActualRightPulse[10]+Speed->ActualLiftPulse[10])+
    (Speed->ActualRightPulse[11]+Speed->ActualLiftPulse[11])+
    (Speed->ActualRightPulse[12]+Speed->ActualLiftPulse[12])+
    (Speed->ActualRightPulse[13]+Speed->ActualLiftPulse[13])+
    (Speed->ActualRightPulse[14]+Speed->ActualLiftPulse[14])+
    (Speed->ActualRightPulse[15]+Speed->ActualLiftPulse[15]);
  
  //�˲�
  Speed->ActualSpeed[2] = 0.85*Speed->ActualSpeed[2]+0.1*Speed->ActualSpeed[1]+0.05*Speed->ActualSpeed[0]; 
  //7 2 1
  
  
  //ʵ�ʲ���  = L-R
  Speed->ActualDifferential[0] = Speed->ActualDifferential[1];
  Speed->ActualDifferential[1] = Speed->ActualDifferential[2];
  Speed->ActualDifferential[2] = Speed->ActualLiftPulse[15]-Speed->ActualRightPulse[15];
  Speed->ActualDifferential[2] = 0.8*Speed->ActualDifferential[2]+0.15*Speed->ActualDifferential[1]+0.05*Speed->ActualDifferential[0];
  //ʵ�ʲ���
  
  
  Speed->ActualInstantaneous[0] = Speed->ActualInstantaneous[1];  
  Speed->ActualInstantaneous[1] = Speed->ActualInstantaneous[2];
  Speed->ActualInstantaneous[2] = (Speed->ActualLiftPulse[15]+Speed->ActualRightPulse[15] + (Speed->ActualLiftPulse[14]+Speed->ActualRightPulse[14])) ;
  //˲ʱ�ٶȼ���
  
  Speed->ActualAcceleration = Speed->ActualInstantaneous[2] - Speed->ActualInstantaneous[0];
  //���ٶȼ���
  
  
  
};


void ButtonLowPassFilter(Button_Type_Def *Button){ //������ͨ�˲�
  
  Button->Up_Final = Button->Up[0] || Button->Up[1] || Button->Up[2] || Button->Up[3] || Button->Up[4]; 
  //��һ��1  ����ֵ����1 ȫ��Ϊ0 ����ֵ��Ϊ0
  if(Button->Up_Final==1) Button->Up_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Button->Up_Final = 1; 
  
  
  Button->Down_Final = Button->Down[0] || Button->Down[1] || Button->Down[2] || Button->Down[3] || Button->Down[4]; 
  //��һ��1  ����ֵ����1 ȫ��Ϊ0 ����ֵ��Ϊ0
  if(Button->Down_Final==1) Button->Down_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Button->Down_Final = 1; 
  
  
  Button->Life_Final = Button->Life[0] || Button->Life[1] || Button->Life[2] || Button->Life[3] || Button->Life[4]; 
  //��һ��1  ����ֵ����1 ȫ��Ϊ0 ����ֵ��Ϊ0
  if(Button->Life_Final==1) Button->Life_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Button->Life_Final = 1; 
  
  
  Button->Right_Final = Button->Right[0] || Button->Right[1] || Button->Right[2] || Button->Right[3] || Button->Right[4]; 
  //��һ��1  ����ֵ����1 ȫ��Ϊ0 ����ֵ��Ϊ0
  if(Button->Right_Final==1) Button->Right_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Button->Right_Final = 1; 
  
  
  Button->Enter_Final = Button->Enter[0] || Button->Enter[1] || Button->Enter[2] || Button->Enter[3] || Button->Enter[4]; 
  //��һ��1  ����ֵ����1 ȫ��Ϊ0 ����ֵ��Ϊ0
  if(Button->Enter_Final==1) Button->Enter_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Button->Enter_Final = 1; 
  
};

void SwitchLowPassFilter(Switch_Type_Def *Switch){ //���ص�ͨ�˲�
  
  Switch->Bm1_Final = Switch->Bm1[0] || Switch->Bm1[1] || Switch->Bm1[2]; 
  if(Switch->Bm1_Final==1) Switch->Bm1_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Switch->Bm1_Final = 1; 
  
  Switch->Bm2_Final = Switch->Bm2[0] || Switch->Bm2[1] || Switch->Bm2[2];
  if(Switch->Bm2_Final==1) Switch->Bm2_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Switch->Bm2_Final = 1; 
  
  Switch->Bm3_Final = Switch->Bm3[0] || Switch->Bm3[1] || Switch->Bm3[2];
  if(Switch->Bm3_Final==1) Switch->Bm3_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Switch->Bm3_Final = 1; 
  
  Switch->Bm4_Final = Switch->Bm4[0] || Switch->Bm4[1] || Switch->Bm4[2];
  if(Switch->Bm4_Final==1) Switch->Bm4_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Switch->Bm4_Final = 1; 
  
  Switch->Bm5_Final = Switch->Bm5[0] || Switch->Bm5[1] || Switch->Bm5[2];
  if(Switch->Bm5_Final==1) Switch->Bm5_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Switch->Bm5_Final = 1; 
  
  Switch->Bm6_Final = Switch->Bm6[0] || Switch->Bm6[1] || Switch->Bm6[2];
  if(Switch->Bm6_Final==1) Switch->Bm6_Final = 0;//ȡ��    ��1Ϊ��Чֵ
  else Switch->Bm6_Final = 1; 
};


void OutOfControlSignal(Speed_Type_Def *Speed, PID_Type_Def *PID){   //�ٶ�ʧ���źż��
  
  if( (Speed->ActualSpeed[2]-Speed->ActualSpeed[1]) > 400  &&  Speed->ActualSpeed[2]>10000){ 
    //���ٶȹ��� ���ֿ�ת

    
    ftm_pwm_duty(ftm2,ftm_ch2,0);
    ftm_pwm_duty(ftm2,ftm_ch3,0);
    ftm_pwm_duty(ftm2,ftm_ch4,0);
    ftm_pwm_duty(ftm2,ftm_ch5,0);
//    
    
    
    while(1){
      
//     led(LED0,LED_ON);  //�������ǵ�Ӳ�����ñ�����
      
      systick_delay_ms(80);
      
//      led(LED0,LED_OFF);
      
      systick_delay_ms(80);
    
    };

  }

};



/*��������������*/
void BalanceFeedbackControl(PID_Type_Def *PID, Angle_Type_Def *Angle, Gyro_Type_Def *Gyro, Ramp_Type_Def *Ramp){ //ֱ����������
  
  PID->err = (Angle->PiontValue-Angle->FinalValue);//Ӳ��
  //ƫ��  =  �Ƕ�Ŀ��-�Ƕȷ���ֵ
  
  
  PID->out =   1000 + PID->Kp * PID->err + PID->Kd * Gyro->Z_Final;
  //1000��Ϊ�˸�һ��PWM�е�   Gyro->Z_Final���� ֱ���Ľ��ٶ�  Ҳ����D����  PD
  
  //һ�������  ��ֱ��ʱ   �ȸ�һ��P����  D��0   ����������   ������ֲ�������ת  
  //����� pid�����෴    ��(Angle->PiontValue-Angle->FinalValue)  ���������λ��  ����
  
  

};


void DirectionFeedbackControl(PID_Type_Def *PID, Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB, Gyro_Type_Def *Gyro, Speed_Type_Def *Speed, Ramp_Type_Def *Ramp){ //����������

  
  InductorA->RightFinal = PID->offset*InductorA->RightFinal;  
  InductorA->LeftFinal = (2-PID->offset)*InductorA->LeftFinal;
  //������ƫ
  

  //��Ҫ������ A���
  
  PID->err = 10.0*(sqrt(1.0*InductorA->RightFinal)-sqrt(1.0*InductorA->LeftFinal) )/ (InductorA->RightFinal+InductorA->LeftFinal);
  
  
  
   
  if( InductorA->AndValues < 25 && Ramp->Flag[0]!=1) {//�����һ� �Ҳ����µ�
   
    PID->err += 0.55*PID->err_k[6]; //�һ�ǿ��

  }
  else if( InductorA->AndValues < 50 && Ramp->Flag[0]!=1) {//�����һ�
   
    PID->err += 0.50*PID->err_k[6]; //�һ�ǿ��
  
  }
  else if( InductorA->AndValues < 75 && Ramp->Flag[0]!=1) {//�����һ�
   
    PID->err += 0.45*PID->err_k[7]; //�һ�ǿ��
  
  }
  else if( InductorA->AndValues < 100 && Ramp->Flag[0]!=1) {//�����һ�
   
    PID->err += 0.40*PID->err_k[7]; //�һ�ǿ��
  
  }
  else if( InductorA->AndValues < 125 && Ramp->Flag[0]!=1) {//�����һ�
   
    PID->err += 0.35*PID->err_k[8]; //�һ�ǿ��
  
  }
  else if( InductorA->AndValues< 150 && Ramp->Flag[0]!=1) {//�����һ�
   
    PID->err += 0.30*PID->err_k[8]; //�һ�ǿ��
  
  }
  else if( InductorA->AndValues < 175 && Ramp->Flag[0]!=1) {//�����һ�
   
    PID->err += 0.25*PID->err_k[9]; //�һ�ǿ��9
  
  }
  else if( InductorA->AndValues < 200 && Ramp->Flag[0]!=1) {//�����һ�200
   
    PID->err += 0.2*PID->err_k[9]; //�һ�ǿ��9
  
  }
  
  if(InductorA->AndValues<150)  PID->err = 0;
  

 
  PID->out = PID->Kp * PID->err * Speed->ActualInstantaneous[2] + PID->Kd * Gyro->X_Final; 
  //ת��PID������� Gyro->X_Final; ��ת����ٶ� Ҳ��D���� 
  
  
  Speed->PiontDifferential = (int16)(PID->out);  //�������ٻ�Ŀ��ֵ
  //��ת��  ������������ֵ  ������Ŀ�껷
  
  for(uint8 n=0; n<9 ; n++){  //ѭ������
    
    PID->err_k[n] = PID->err_k[n+1]; //0~8λ��ֵ
   
    if( n==8 )
      PID->err_k[n+1] = PID->err;  //���� 9λ��ֵ
    
  }
  
};


void DifferentialFeedbackControl(PID_Type_Def *PID, Speed_Type_Def *Speed, Gyro_Type_Def *Gyro){//���ٷ�������
     
  //PID->offsetֻ��һ����������  PID->offset=2     
  
  int16 SpeedDifferentialValue = PID->offset * Speed->ActualDifferential[2] + (1-PID->offset)*(-0.18*Gyro->X_Final);
  //  �õ�����ֵ  �Ǳ�������   ת����ٶȹ�ͬ�������ٷ���ֵ
  
  
  //Speed->PiontDifferential = 0;
  //����Ŀ��-  ����ʵ��ֵ
  PID->err = ( Speed->PiontDifferential -  SpeedDifferentialValue);

  PID->integral += PID->err;
  
  //����������
  if(PID->integral>50000)PID->integral=50000;
  
  if(PID->integral<-50000)PID->integral=-50000;
  
  if(Speed->ActualSpeed[2]< 50)PID->integral = 0;
  //���ٻ�������
  
  PID->out =  PID->Kp * PID->err + PID->Kd * ( SpeedDifferentialValue + PID->err_k[0]);// + PID->Ki * PID->integral;
  //PID����
  
  for(uint8 n=0; n<9 ; n++){  //ѭ������
    
    PID->err_k[n] = PID->err_k[n+1]; //0~8λ��ֵ
   
    if( n==8 )
      PID->err_k[n+1] = PID->err;  //���� 9λ��ֵ
    
  }
  
 
  PID->err_k[0] = SpeedDifferentialValue;//����һ��PID->err_k[0]��  �����ϴ�ֵ ����
  

  //�����������    
  if(PID->out>2000)PID->out=2000;//1800
  if(PID->out<-2000)PID->out=-2000;


  
};

void SpeedFeedbackControl(PID_Type_Def *PID, Speed_Type_Def *Speed, Ramp_Type_Def *Ramp, Switch_Type_Def *Switch){ //�ٶȷ�������
  

  PID->out_k[9] = PID->out;//�������һ�����ֵ
  
  PID->out_k[0] = 0;
  
  
  
  PID->err = Speed->ActualSpeed[2] - Speed->PiontSpeed;
  
  
  PID->integral = PID->err_k[0]+PID->err_k[1]+PID->err_k[2]+PID->err_k[3]+PID->err_k[4]+
  
          PID->err_k[5]+PID->err_k[6]+PID->err_k[7]+PID->err_k[8]+PID->err_k[9]+PID->err;


 
  PID->out = PID->Kp * PID->err;// + PID->Ki * PID->integral;
  
  PID->out *= 0.05; //��С�ٶȻ����
  
  
  if(PID->out >  4800) PID->out= 4800; //������ͽǶ�4200
  
  if(PID->out < -1800) PID->out= -1800;//ǰ����ͽǶ�-1800
 
  

  
  for(uint8 n=0; n<9 ; n++){  //ѭ������
    
    PID->err_k[n] = PID->err_k[n+1]; //0~8λ��ֵ
   
    if( n==8 )
      PID->err_k[n+1] = PID->err;  //���� 9λ��ֵ
    
  }
  

};

void PWMFeedbackControl(PWM_Type_Def *PWM, PID_Type_Def *PID_Balance, PID_Type_Def *PID_Speed, PID_Type_Def *PID_Path, PID_Type_Def *PID_Diff, Speed_Type_Def *Speed, Switch_Type_Def *Switch){ //PWM���ӷ�������
  
  float DiffK=1;
 
    
  PID_Speed->out_k[0] += (PID_Speed->out - PID_Speed->out_k[9]) * 0.0625;//�ֶγ�1/16������
  
  //PID_Speed->out_k[9]���ϴ�ֵ //PID_Speed->out�Ǳ���ֵ  PID_Speed->out_k[0] ��ƽ���������ֵ 
  //���ӵ�ʱ���   ƽ���������ֵ + �ϴε�ֵ �õ�16�������� ÿ4ms����Ӧ����ֵ
  
  PWM->PiontValue  = (int16)(PID_Balance->out)+ (int16)(PID_Speed->out_k[0] + PID_Speed->out_k[9]) ;
  //�����л�������
  
  
  PWM->Differential = (int16)(PID_Diff->out);
 //����PWM
  
  
  if(PWM->Differential>0){
    
    PWM->LiftValue = PWM->PiontValue + (DiffK) * PWM->Differential;
  
    PWM->RightValue = PWM->PiontValue - (2-DiffK) * PWM->Differential;      
    
  }
  
  if(PWM->Differential<0){ //����С����ʱ  ��߸�Ŀ��ռ�ձ�+ ϵ��*����
    
    PWM->LiftValue = PWM->PiontValue + (2-DiffK) * PWM->Differential;
  
    PWM->RightValue = PWM->PiontValue - (DiffK) * PWM->Differential;      
    
  }
  
  
  if(PWM->Differential==0){  //����=0ʱ  ���Ҹ�һ����Ŀ��ռ�ձ�
    
    PWM->LiftValue = PWM->PiontValue;
    
    PWM->RightValue = PWM->PiontValue;
  }
};

void ButtonFunctionControl(PID_Type_Def *PID_Balance, PID_Type_Def *PID_Speed, PID_Type_Def *PID_Path, PID_Type_Def *PID_Diff, Angle_Type_Def *Angle, Speed_Type_Def *Speed, Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB, Button_Type_Def *Button, uint8 *Page){ //��������ʵ��
  //OLED  ���� ����PID�����õ�   
  
  
  if(Button->Life_Final==1)if(*Page>0)(*Page)--;
  
  if(Button->Right_Final==1)if(*Page<135)(*Page)++;

  //page0
  if(*Page<=5){  //UP  kp
    
    if(Button->Up_Final==1 ){
 
      if(PID_Balance->Kp<30) PID_Balance->Kp = PID_Balance->Kp+0.01;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Balance->Kp>0) PID_Balance->Kp = PID_Balance->Kp-0.01;
    }

  }
 
  if(*Page<=10 && *Page>5){  //UP  kd
    
    if(Button->Up_Final==1){
  
      if(PID_Balance->Kd<30) PID_Balance->Kd = PID_Balance->Kd+0.01;
    }
        
    if(Button->Down_Final==1){
    
      if(PID_Balance->Kd>0) PID_Balance->Kd = PID_Balance->Kd-0.01;
    }
    
  }
  
  if(*Page<=15 && *Page>10){  // angle piont
    
    if(Button->Up_Final==1){
  
      if(Angle->PiontValue<2400) Angle->PiontValue = Angle->PiontValue+1;
    }
        
    if(Button->Down_Final==1){
    
      if(Angle->PiontValue>1600) Angle->PiontValue = Angle->PiontValue-1;
    }
    
  }
   
  if(*Page<=20 && *Page>15){  //speed kp
    
    
    if(Button->Up_Final==1){
   
      if(PID_Speed->Kp<30)PID_Speed->Kp = PID_Speed->Kp+0.1;

    }
      
    
    if(Button->Down_Final==1){      
  
      if(PID_Speed->Kp>0)PID_Speed->Kp = PID_Speed->Kp-0.1;
    
    }
    
   
  }
 
  if(*Page<=25 && *Page>20){  //speed ki
    
    
    if(Button->Up_Final==1){ 
  
      if(PID_Speed->Ki<10)PID_Speed->Ki = PID_Speed->Ki+0.001;
    }
        
    
    if(Button->Down_Final==1){  
 
      if(PID_Speed->Ki>0)PID_Speed->Ki = PID_Speed->Ki-0.001;
    }

    
  }
  
  if(*Page<=30 && *Page>25){  //speeed piont
    
    if(Button->Up_Final==1){
  
      if(Speed->PiontSpeed<10000)Speed->PiontSpeed = Speed->PiontSpeed+10;
    }
        
    if(Button->Down_Final==1){

      if(Speed->PiontSpeed>0)Speed->PiontSpeed = Speed->PiontSpeed-10;
    }
    
    
  }

  
  
  
  //page1
  if(*Page<=35 && *Page>30){  //path kp
    
    if(Button->Up_Final==1){

      if(PID_Path->Kp<30)PID_Path->Kp = PID_Path->Kp+0.01;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Path->Kp>0)PID_Path->Kp = PID_Path->Kp-0.01;
    }

    
  }
  
  if(*Page<=40 && *Page>35){  //path kd 
    
    
    if(Button->Up_Final==1){

      if(PID_Path->Kd<30)PID_Path->Kd = PID_Path->Kd+0.01;
    }
        
    if(Button->Down_Final==1){
   
      if(PID_Path->Kd>0)PID_Path->Kd = PID_Path->Kd-0.01;
    }

  }
  
  if(*Page<=45 && *Page>40){  //path piont
    
    
    if(Button->Up_Final==1){
      
      if(PID_Path->offset<2)PID_Path->offset = PID_Path->offset+0.001;
    
    }
      
    if(Button->Down_Final==1){
    
      if(PID_Path->offset>0)PID_Path->offset = PID_Path->offset-0.001;
    
    }
    
    
  }
  
  if(*Page<=50 && *Page>45){  //diff kp
    
    if(Button->Up_Final==1){

      if(PID_Diff->Kp<30)PID_Diff->Kp = PID_Diff->Kp+0.01;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Diff->Kp>0)PID_Diff->Kp = PID_Diff->Kp-0.01;
    }

    
  }
  
  if(*Page<=55 && *Page>50){  //diff kd
    
    if(Button->Up_Final==1){

      if(PID_Diff->Kd<30)PID_Diff->Kd = PID_Diff->Kd+0.01;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Diff->Kd>0)PID_Diff->Kd = PID_Diff->Kd-0.01;
    }

    
  }
   
  if(*Page<=60 && *Page>55){  //diff kd
    
    if(Button->Up_Final==1){

      if(PID_Diff->Ki<30)PID_Diff->Ki = PID_Diff->Ki+0.01;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Diff->Ki>0)PID_Diff->Ki = PID_Diff->Ki-0.01;
    }

    
  }
  
  if(*Page<=65 && *Page>60){  //diff offset
    
    if(Button->Up_Final==1){

      if(PID_Diff->offset<1)PID_Diff->offset = PID_Diff->offset+0.001;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Diff->offset>0)PID_Diff->offset = PID_Diff->offset-0.001;
    }

    
  }
  
  
 
  
  
  //page2
  if(*Page<=70 && *Page>65){ //�������ֵ
    
    if(Button->Up_Final==1 && InductorA->LeftMax < 4000)InductorA->LeftMax ++;
        
    
    if(Button->Down_Final==1 && InductorA->LeftMax > 100)InductorA->LeftMax --;
    
    
  }
  
  if(*Page<=75 && *Page>70){ //������Сֵ
    
    
    if(Button->Up_Final==1 && InductorA->LeftMin < 4000)InductorA->LeftMin ++;
        
    
    if(Button->Down_Final==1 && InductorA->LeftMin > 100)InductorA->LeftMin --;
    
  }
  
  if(*Page<=80 && *Page>75){ //�ҵ�����ֵ
    
    if(Button->Up_Final==1 && InductorA->RightMax < 4000)InductorA->RightMax ++;
        
    
    if(Button->Down_Final==1 && InductorA->RightMax > 100)InductorA->RightMax --;
    
    
  }
  
  if(*Page<=85 && *Page>80){ //�ҵ����Сֵ
    
    
    if(Button->Up_Final==1 && InductorA->RightMin< 4000)InductorA->RightMin ++;
        
    
    if(Button->Down_Final==1 && InductorA->RightMin> 100)InductorA->RightMin --;
    
  }
  
  if(*Page<=90 && *Page>85){ //�м������ֵ
    
    
    if(Button->Up_Final==1 && InductorB->LeftMax< 4000)InductorB->LeftMax ++;
        
    
    if(Button->Down_Final==1 && InductorB->LeftMax> 100)InductorB->LeftMax --;
    
  }
  
  if(*Page<=95 && *Page>90){//�м�����Сֵ
    
    
    if(Button->Up_Final==1 && InductorB->LeftMin< 4000)InductorB->LeftMin ++;
        
    
    if(Button->Down_Final==1 && InductorB->LeftMin> 100)InductorB->LeftMin --;
    
  }
  
  if(*Page<=100 && *Page>95){//����ٶ�
    
    
    if(Button->Up_Final==1 && Speed->PiontSpeedMax<10000)Speed->PiontSpeedMax ++;
        
    
    if(Button->Down_Final==1 && Speed->PiontSpeedMax >0)Speed->PiontSpeedMax --;
    
  }
  
  if(*Page<=105 && *Page>100){//��С�ٶ�
  
    if(Button->Up_Final==1 && Speed->PiontSpeedMin < Speed->PiontSpeedMax)Speed->PiontSpeedMin ++;
        
    
    if(Button->Down_Final==1 && Speed->PiontSpeedMin > 0)Speed->PiontSpeedMin --;
    
    
  }

  
//  
//  //page3
//  if(*Page<=110 && *Page>105){//���پ���
//    
//    if(Button->Up_Final==1 && Speed->SlowDistance < 6000)Speed->SlowDistance ++;
//        
//    
//    if(Button->Down_Final==1 && Speed->SlowDistance > 0)Speed->SlowDistance --;
//  }
//  
//  if(*Page<=115 && *Page>110){//���پ���
//    
//    if(Button->Up_Final==1 && Speed->AccDistance < 6000)Speed->AccDistance ++;
//        
//    
//    if(Button->Down_Final==1 && Speed->AccDistance > 0)Speed->AccDistance --;
//  }
//  
//  if(*Page<=120 && *Page>115){//�������
//    
//    if(Button->Up_Final==1 && Speed->FaultDistance < 150000)Speed->FaultDistance+=100;
//        
//    
//    if(Button->Down_Final==1 && Speed->FaultDistance > 100)Speed->FaultDistance-=100;
//  }
  
};


/*�����������*/
void PrintParameterToOLED(PID_Type_Def *PID_Balance, PID_Type_Def *PID_Speed, PID_Type_Def *PID_Path, PID_Type_Def *PID_Diff, Angle_Type_Def *Angle, Speed_Type_Def *Speed , PWM_Type_Def *PWM, Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB, Curve_Type_Def *Curve, uint8 Page){    //��ӡ������OLED��

  uint8 mode[10];
  static uint8 Last=0;
  
  if(Page<=30){    
  
    if(Page<=5) mode[0] = 0;
    else mode[0] = 1;
    
    if(Page<=10 && Page>5) mode[1] =0;
    else mode[1] = 1;
    
    if(Page<=15 && Page>10) mode[2] =0;
    else mode[2] = 1;
    
    if(Page<=20 && Page>15) mode[3] =0;
    else mode[3] = 1;
    
    if(Page<=25 && Page>20) mode[4] =0;
    else mode[4] = 1;
    
    if(Page<=30 && Page>25) mode[5] =0;
    else mode[5] = 1;
    
    if( Last!=1 ){
      OLED_Fill(0,0,127,63,0);
      Last = 1;
    }
    OLED_ShowString(0,0,"UP P    D         ");
    OLED_ShowNum(36,0,(int)(PID_Balance->Kp*10),3,12,mode[0]);  
    OLED_ShowNum(72,0,(int)(PID_Balance->Kd*10),3,12,mode[1]); 
    OLED_ShowString(0,12 ,"AnglePiont        "); 
    OLED_ShowNum(96,12,(int)(Angle->PiontValue),4,12,mode[2]);  
    
    OLED_ShowString(0,24 ,"Speed P   I        "); 
    OLED_ShowNum(56,24,(int)(PID_Speed->Kp*10),3,12,mode[3]);  
    OLED_ShowNum(96,24,(int)(PID_Speed->Ki*1000),3,12,mode[4]);  
    OLED_ShowString(0,36 ,"SpeedPiont        ");
    OLED_ShowNum(96,36,(int)(Speed->PiontSpeed),4,12,mode[5]);
      

    OLED_ShowNum(32,48,(int)(InductorA->LeftFinal),3,12,0);  
    OLED_ShowNum(56,48,(int)(InductorA->RightFinal),3,12,0);
    OLED_ShowNum(86,48,(int)(InductorB->LeftFinal),3,12,0); 
  }  
  
  else if(Page>30 && Page<=65){
    
    if(Page<=35 && Page>30) mode[0] = 0;
    else mode[0] = 1;
    
    if(Page<=40 && Page>35) mode[1] =0;
    else mode[1] = 1;
    
    if(Page<=45 && Page>40) mode[2] =0;
    else mode[2] = 1;
    
    if(Page<=50 && Page>45) mode[3] =0;
    else mode[3] = 1;
    
    if(Page<=55 && Page>50) mode[4] =0;
    else mode[4] = 1;
    
    if(Page<=60 && Page>55) mode[5] =0;
    else mode[5] = 1;
    
    if(Page<=65 && Page>60) mode[6] =0;
    else mode[6] = 1;
    
    
    
    if( Last!=2 ){
      OLED_Fill(0,0,127,63,0);
      Last = 2;
    }
    
    OLED_ShowString(0,0,"Path P    D         ");
    OLED_ShowNum(48,0,(int)(PID_Path->Kp*10),3,12,mode[0]);  
    OLED_ShowNum(96,0,(int)(PID_Path->Kd*100),3,12,mode[1]); 
    OLED_ShowString(0,12 ,"PathPiont        "); 
    OLED_ShowNum(84,12,(int)(PID_Path->offset*100),3,12,mode[2]);  
    
      
    OLED_ShowString(0,24 ,"Diff P     D        "); 
    OLED_ShowNum(56,24,(int)(PID_Diff->Kp*10),3,12,mode[3]);  
    OLED_ShowNum(96,24,(int)(PID_Diff->Kd*10),3,12,mode[4]);  
    OLED_ShowString(0,36 ,"Diff I     S        ");
    OLED_ShowNum(56,36,(int)(PID_Diff->Ki*10),3,12,mode[5]);
    OLED_ShowNum(96,36,(int)(PID_Diff->offset*100),3,12,mode[6]); //�������������ںϱ���
      
      

    OLED_ShowNum(32,48,(int)(InductorA->LeftFinal),3,12,0);  
    OLED_ShowNum(56,48,(int)(InductorA->RightFinal),3,12,0);
    OLED_ShowNum(86,48,(int)(InductorB->LeftFinal),3,12,0); 
      
  }
  
  else if(Page>65 && Page<=105){
    
    if(Page<=70 && Page>65) mode[0] = 0;
    else mode[0] = 1;
    
    if(Page<=75 && Page>70) mode[1] =0;
    else mode[1] = 1;
    
    if(Page<=80 && Page>75) mode[2] =0;
    else mode[2] = 1;
    
    if(Page<=85 && Page>80) mode[3] =0;
    else mode[3] = 1;
    
    if(Page<=90 && Page>85) mode[4] =0;
    else mode[4] = 1;
    
    if(Page<=95 && Page>90) mode[5] =0;
    else mode[5] = 1;
    
    if(Page<=100 && Page>95) mode[6] =0;
    else mode[6] = 1;
    
    if(Page<=105 && Page>100) mode[7] =0;
    else mode[7] = 1;
    
    
    if( Last!=3 ){
      OLED_Fill(0,0,127,63,0);
      Last = 3;
    }
    
    OLED_ShowString(0,0   ,"LAMAX");
    OLED_ShowNum(38,0,(int)(InductorA->LeftMax),4,12,mode[0]);
    OLED_ShowString(68,0  ,"LAMIN");
    OLED_ShowNum(108,0,(int)(InductorA->LeftMin),3,12,mode[1]);
    
    
    OLED_ShowString(0,12  ,"RAMAX");
    OLED_ShowNum(38,12,(int)(InductorA->RightMax),4,12,mode[2]);
    OLED_ShowString(68,12 ,"RAMIN");
    OLED_ShowNum(108,12,(int)(InductorA->RightMin),3,12,mode[3]);
    
    
    
    OLED_ShowString(0,24  ,"ZBMAX");
    OLED_ShowNum(38,24,(int)(InductorB->LeftMax),4,12,mode[4]);
    OLED_ShowString(68,24 ,"ZBMIN");
    OLED_ShowNum(108,24,(int)(InductorB->LeftMin),3,12,mode[5]);
    
    
    OLED_ShowString(0,36 ,"SpeedMax");
    OLED_ShowNum(84,36,(uint32)(Speed->PiontSpeedMax),4,12,mode[6]);
    
    OLED_ShowString(0,48 ,"SpeedMin");
    OLED_ShowNum(84,48,(uint32)(Speed->PiontSpeedMin),4,12,mode[7]);
    
  }
  
  OLED_Refresh_Gram();

};



void MotorControlFlow(PWM_Type_Def *PWM, Ramp_Type_Def *Ramp){ //���ֱ�ӿ���
  

  //��������� �ڴ˴� ��*PWM��ֵ ���������ע��

  //PWM->LiftValue = 1200;//��������  ��ǰ��
  //PWM->LiftValue = 1000;
  //PWM->LiftValue = 800;
  
  //PWM->RightValue = 1200;//�������� ��ǰ��
  //PWM->RightValue = 1000;
  //PWM->RightValue = 800;

  //����ұ߶��� 1000��ʱ�� Ӧ���ǲ�ת��   
  
  if(PWM->LiftValue>(PWMMAX))PWM->LiftValue = (PWMMAX);
  if(PWM->LiftValue<(PWMMIN))PWM->LiftValue = (PWMMIN);
  if(PWM->RightValue>(PWMMAX))PWM->RightValue = (PWMMAX);
  if(PWM->RightValue<(PWMMIN))PWM->RightValue = (PWMMIN);
  
  //�˴������ȵ�����  ��֤ PWMֵ�� >1000ʱ  ��ǰ��
  //PWMֵ< 1000 �Ǻ���   ���� ���ֱ���һ��
  //�ܸ�Ӳ�� �͸�������  �� �������  ���� 
  //��ͬ������  1200  ��ߵ��������ǰ  �ұ��������� ��ʱ�� �����ҵ����
  //��������������  ��1200  �������� ��ת 20%ռ�ձ�  ������ת�ٺܿ� ˵��
  //�������򲻶�   ������������� ͨ��  ���� ֱ�Ӹ����� 
  
  if(PWM->LiftValue>1000){ //����ǰ��  1000~2000
     ftm_pwm_duty(ftm2,ftm_ch2,0);
     ftm_pwm_duty(ftm2,ftm_ch3,PWM->LiftValue-1000); 
  }
  else if(PWM->LiftValue<1000){ //�����Ǻ���  0~1000
     ftm_pwm_duty(ftm2,ftm_ch2,1000-PWM->LiftValue); 
     ftm_pwm_duty(ftm2,ftm_ch3,0);    
  }  //������� �˴���������  ���� ����ת�� �������Ӳ������������  ע�� �����������ĵ���  
   

  if(PWM->RightValue>1000){ //������ǰ��  1000~2000
     ftm_pwm_duty(ftm2,ftm_ch4,0);
     ftm_pwm_duty(ftm2,ftm_ch5,PWM->RightValue-1000); 
  }
  else if(PWM->RightValue<1000){ //�����Ǻ���  0~1000
     ftm_pwm_duty(ftm2,ftm_ch4,1000-PWM->RightValue);
     ftm_pwm_duty(ftm2,ftm_ch5,0);   
  }   //������� �˴���������  ���� �����ת�� �������Ӳ������������  ע�� �����������ĵ���   
  
};

uint8 timer3ms = 0, timer4_5ms=0,timer24ms=0 ,timer64ms=0;

int main(void){
  
  //���ڱ�����ջ��϶�  ���Թ��̱ȽϷ���  ����һ��ʼ��ʱ�� ���ε�������
  //ֻ����������Ļ���  ƽ�⳵��Ҫ���Ƕ�����������Ŀ���  ������һ������
  //MotorControlFlow������ֱ�ӿ��Ƶ����  ���������Ҫ���Ժ�  ��Ҫ�����
  //ռ�ձ�0~1000�˺�   1000~2000��ǰ  ͨ������������  �� ���������  ����
  //���������� ch2 3 4 5 ��  23˳��  45˳����  �ﵽ���յ�Ч��  ������� 
  //��1000��������ǰ��   1000һ�������  2000��ǰ���   0�������
  
  //�����������������Ժ�  �ٵ��ڱ���������  ������MotorControlFlow������
  //ֱ�Ӹ�  PWM->LiftValue = 1200;PWM->RightValue = 1200;  ����������̶�ռ�ձ�
  //ת��  ��ʱ�� ͨ��IAR���ߵ��Թ۲�  ����������ֵ �Ƿ���ȷ  �Ƿ��Ӧ��������
  //(����ת����Ӧ�������  �ҵ��ת����Ӧ�ұ߱�����  ���ܴ�)
  //������ֹһ������۲��Ƿ����Ǹ�����ı�����ת���½�  ȷ�� �������ķ���һ�£�
  
  //������ �� ����������Ժ�ͷ����˺ܶ�  ����������ֱ��  ǰ���������ǲ����Ѿ�������
  //���ǵ���ֱ����ʱ��  Ҫ�����������ر� ��������  ����PWMFeedbackControl �ǰ����л����
  //���ϵ����PWM�ϵĺ��� ����  PWM->PiontValue  = (int16)(PID_Balance->out)+ (int16)(PID_Speed->out_k[0] + PID_Speed->out_k[9]) ;
  //��仰�ǰ�  ����������� ������һ��  ����ֻ��Ҫ���� ֱ����  ��������ֱ���Ƚ�˳��
  //���л���һ�� ���� ҲҪ��0  ��ֱ������Ӱ��   
  
  //��ֱ��������Ϻ�  �ٶȻ���ֱ�����ں��ǱȽ��ѵ�  ֱ�����ٶ��ǲ��� ��Ҫ���ĵ��ԣ�Ҳ����
  //�����ٶȻ��ĵ���  ֱ�ӽ���ת��  ����ٻ�    ����֮���Ǵ���PID�Ĺ�ϵ  
  
  //��������ж�  �ܷ� ����  ��ס��ADC��ѭ����  ˵����ջ���  ����KEA128.ICF�ļ���  ��ջֵ ����  ������12K����
  
 
 
  
  get_clk();              //��ȡʱ��Ƶ�� ����ִ��
  
  uint8 Page=0;
  
  PID_Type_Def  PID_Balance, PID_Speed, PID_Path, PID_Diff;
  
  RUN_Type_Def       RUN;
  Stop_Type_Def      Stop;
  Ramp_Type_Def      Ramp; 
  Curve_Type_Def     Curve;
  Acc_Type_Def       Acc;
  Gyro_Type_Def      Gyro;
  Angle_Type_Def     Angle;
  PWM_Type_Def       PWM;
  Speed_Type_Def     Speed;
  Button_Type_Def    Button;
  Switch_Type_Def    Switch;
  Inductor_Type_Def  InductorA, InductorB;
 
  
  SensorStructurePointerInit( &Acc, &Gyro, &Angle, &Speed, &InductorA, &InductorB );//������������ʼ��
  PIDStructurePointerInit( &PID_Balance, &PID_Speed, &PID_Path, &PID_Diff );//PID������ʼ��
  CarBalanceSystemInit( &Gyro ); //ϵͳ��ʼ��  
  


  
  RUN.Flag[4] = 1;//���г����־λ
  
  RUN.Flag[5] = 0;//���������־λ
  
  Stop.Flag[0] = 0;//ֹͣ�߱�־λ��ʱ������־λ
  
  Stop.Flag[1] = 0;//ֹͣ��־λ
  
  Ramp.Flag[0] = 0;//�ӽ��µ���־λ
  
  Ramp.Flag[1] = 0;//���µ���־λ
  
  Curve.Flag[1] = 0;//�����ٱ�־λ
  
  Curve.Flag[2] = 0;//�����ٱ�־λ
  
  Curve.Flag[3] = 0;//������Ŀ
  
  while(1){

    if( timer4_5ms  ){
      
        gpio_set(H2,1);//
      
      GetAccelerationValue( &Acc );  //���ٶ�ԭʼ���ݲɼ�
      GetAngleSpeedValue( &Gyro );   //���ٶ�ԭʼ���ݲɼ�
      GetSpeedValue( &Speed );       //��ȡԭʼ�ٶ�����
      GetInductorValue( &InductorA, &InductorB );//��е�ѹԭʼ���ݲɼ�
      
      GyroHighPassFilter( &Gyro );  //���ٶ��˲�����
      AngleHardwareFilter( &Angle );//�Ƕ��˲�����
      SpeedSoftwareFilter( &Speed );//�ٶȵݹ�Ȩֵ�˲�
      InductorSoftwareFilter( &InductorA, &InductorB, &Angle, &Switch);//����˲�����

      BalanceFeedbackControl( &PID_Balance, &Angle, &Gyro, &Ramp );//ƽ��ȷ�������
      DirectionFeedbackControl( &PID_Path, &InductorA, &InductorB, &Gyro, &Speed, &Ramp );//·����������
      DifferentialFeedbackControl( &PID_Diff, &Speed, &Gyro);//���ٷ�������

      PWMFeedbackControl( &PWM, &PID_Balance, &PID_Speed, &PID_Path, &PID_Diff, &Speed, &Switch );//PWM���� ֱ�� ���� �ٶ�PWM
      MotorControlFlow( &PWM, &Ramp ); //���PWM�������
      
        gpio_set(H2,0);//
      
      timer4_5ms = 0;//���ʱ���־λ
    }
    
    //�����������Ժ�Ͳ���ִ��  ��Ϊ�����Ƚϴ� Ӱ��PID��������
    if( Speed.ActualSpeed[2]<100 && timer24ms && timer4_5ms != 1 ){ 
      
      GetButtonStatus( &Button ); //��ȡ����״̬
      ButtonLowPassFilter( &Button );//�жϰ���״̬
      
      
      ButtonFunctionControl( &PID_Balance, &PID_Speed, &PID_Path, &PID_Diff, &Angle, &Speed, &InductorA, &InductorB, &Button, &Page );//ʵ�ְ���״̬
      
      GetSwitchStatus( &Switch ); //��ȡ����״̬
      SwitchLowPassFilter( &Switch );//�жϿ���״̬

      timer24ms = 0;

    };
    
    
    if(timer64ms && timer4_5ms != 1 ){  
      
      if(Speed.ActualSpeed[2]<100)PrintParameterToOLED( &PID_Balance, &PID_Speed, &PID_Path, &PID_Diff, &Angle, &Speed, &PWM, &InductorA, &InductorB, &Curve, Page );
      //�˻��������� �������в�ִ��
     
      SpeedFeedbackControl( &PID_Speed, &Speed, &Ramp, &Switch ); //�ٶȻ���������
      

       timer64ms = 0;
    };
    
    
  }
  
}




uint8 count = 0;

void PIT_CH0_IRQHandler(void)
{
 
  if( count<84 ) count++;
  else count = 0;
   
  
//  if( count%2==0 ) timer3ms = 1;
  
  if( count%3==0 ) timer4_5ms = 1;
  
  if( count%16==0 ) timer24ms = 1;
  
  if( count%42==0 ) timer64ms=1;
 
  
  PIT_FlAG_CLR(pit0);//����жϱ�־λ
  

  
}
