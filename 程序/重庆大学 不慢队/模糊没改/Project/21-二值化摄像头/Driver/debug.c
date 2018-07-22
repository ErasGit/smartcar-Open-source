#include "common.h"
#include "include.h"
float Variable[20];
float Control_Para[15];
float Voltage;

int   Start_Cnt=0;
uint8 Para_Index_Limit=7;       //һҳ�����7���������
uint8 Page_Index=2,Para_Index=1,Light_Tower_Index=0,Para_Checked=0,OLED_Refresh=1,Camera_Drawed;
uint8 Uart_Send=0,SendPara,stop_contorl,send_data_contorl=0,SendSD,SD_Save=0,beep=0,SD_Write_info=0;
float Step[6]={0.0001,0.001,0.01,0.1,1.0,10.0};   //Ĭ�ϵ��ڲ���Ϊ0.01
unsigned char Step_Index=2;
//��Ӧ��ͬ��ҳ��
char Para_Name[7][12]={"PID_ANGLE.P\0","PID_ANGLE.D\0","PID_SPEED.P\0",
"PID_SPEED.I\0","PID_SPEED.D\0","PID_DIREC.P\0","PID_DIREC.D\0"};
char Debug_Mode[4][12]={"Normal  \0","UpRight\0","NoSpeed\0","NoDirec\0"};  //����ģʽ ���� ֱ�� û���ٶ� ��û�з���

char Para_Name1[7][12]={"Set_Speed\0","Set_Angle\0","Acc_Offset\0","Fuzzy_kp",
"Fuzzy_kd","STurnSpeed\0","BTurnAngle\0"};

void my_putchar(char temp)
{
      uart_putchar(UART0,temp); //����ʵ�ʵĴ��ں����޸�
}
/*����֪ͨ��λ���µ�һ�����ݿ�ʼ��Ҫ�������ݱ��뷢����*/
void Send_Begin()
{
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0xae);
}
void Variable_update()
{
  Variable[0]=Angle;
  Variable[1]=Car_Angle;
  Variable[2]=Angle_Speed;
  Variable[3]=PID_ANGLE.OUT;
   Variable[4]= PID_SPEED.OUT;
  Variable[5]=SpeedControlOutNew;
  Variable[6]=CarSpeed;
}
void Para_Update()
{
  PID_ANGLE.P=Control_Para[0];
  PID_ANGLE.D=Control_Para[1];
  PID_SPEED.P=Control_Para[2];
  PID_SPEED.I=Control_Para[3];
  PID_SPEED.D=Control_Para[4];
  PID_TURN.P=Control_Para[5];
  PID_TURN.D=Control_Para[6];
  SetSpeed=Control_Para[7];
  
}
void OLED_Draw_UI()  //��������
{ 
   uint8 i;
   if(Page_Index<=1) 
   {
     Voltage=adc_once(ADC0_SE14,ADC_12bit);
     Voltage=Voltage*Vol_ratio; //ת��Ϊʵ�ʵ�ѹ
     OLED_P6x8Str(0,0,"Voltage=");                          //��ʾ��ص�ѹ
     OLED_PrintValueF(48, 0,Voltage,2);                     
     OLED_PrintValueF(72, 0,Step[Step_Index],5);            //��ʾ���ڲ���ֵ
     if(Para_Index==7)
     {
        reverse=1; 
         OLED_P6x8Str(116,0,"EE"); 
        reverse=0;
     }
     else  
     {
        OLED_P6x8Str(116,0,"EE"); 
     }
     OLED_Set_Pos(122,7);
     OLED_P6x8Char(Page_Index+48);                         //д��ҳ�����
   }
  /////////////////////////////////////////////////////////��0ҳ  PID����
  if(Page_Index==0)                
  {
    for(i=0;i<7;i++)
    {
      if(i==Para_Index&&Para_Checked==false)
      {
       reverse=1;
       OLED_P6x8Str(0,i+1,Para_Name[i]);   //����������ת��ʾ
       reverse=0;
      }
      else OLED_P6x8Str(0,i+1,Para_Name[i]);

      
      
      if(i==Para_Index&&Para_Checked)
      {
        reverse=1;
        OLED_PrintValueF(72, i+1,Control_Para[i],5);
        reverse=0;
      }
      else  OLED_PrintValueF(72, i+1,Control_Para[i],5);
      
      OLED_Set_Pos(116,i+1);

    }
  }
  /////////////////////////////////////////////////////////��1ҳ   ��������
    else if(Page_Index==1)
  {  
     for(i=0;i<7;i++)
    {
     if(i==Para_Index&&Para_Checked==false)
      {
       reverse=1;
       OLED_P6x8Str(0,i+1,Para_Name1[i]);   //����������ת��ʾ
       reverse=0;
      }
      else OLED_P6x8Str(0,i+1,Para_Name1[i]);  
     
      if(i==Para_Index&&Para_Checked)
      {
        reverse=1;
        OLED_PrintValueF(72, i+1,Control_Para[i+7],5);
        reverse=0;
      }
      else  OLED_PrintValueF(72, i+1,Control_Para[i+7],5);    
    
      OLED_Set_Pos(116,i+1);
    }    
    }
    /////////////////////////////////////////////////////////��3ҳ ״̬��ʾ
  else if(Page_Index==2)
  {
    OLED_P6x8Str(0,0,"CarAngle");
    OLED_PrintValueF(72,0,Car_Angle,3);
    
    OLED_P6x8Str(0,1,"GyroSpeed");
    OLED_PrintValueF(72, 1,Angle_Speed,3);
    
//    OLED_P6x8Str(0,5,"CarSpeed");
//    OLED_PrintValueF(72, 5,CarSpeed,3);
    OLED_P6x8Str(0,5,"Gyro_Y");
    OLED_PrintValueF(72, 5,Gyro_Y,3);
    
//    OLED_P6x8Str(0,2,"Distance");
//    OLED_PrintValueF(72, 2,Distance,4);
     OLED_P6x8Str(0,2,"LeftM");
    OLED_PrintValueF(72, 2,LeftMotorOut,4);
    
    
//    OLED_P6x8Str(0,3,"RunTime");
//    OLED_PrintValueF(72, 3,RunTime,4);
    OLED_P6x8Str(0,3,"PID");
    OLED_PrintValueF(72, 3,PID_ANGLE.pout,4);
//    
//    OLED_P6x8Str(0,4,"Average_Spd");
//    OLED_PrintValueF(72, 4,AverageSpeed,4);   
    OLED_P6x8Str(0,4,"shu");
    OLED_PrintValueF(72, 4,shuzhi,4);   
    
    reverse=0;
     
  } 
  ////////////////////////////////////////////////////////////��4ҳ ��������ʾ
  else if(Page_Index==3) 
  {
    OLED_Draw_camera();
  }
  
}


/*
 * �����뿪�ص�ֵ
 */
void Read_Switch() 
{
  if(gpio_get(SW1)==0)  //���뿪��1����
  {
   Uart_Send=true;
  }
  else           
  {
    Uart_Send=false;
  }

  if(gpio_get(SW3)==0)   //���뿪��3����
  {
       
  } 
  else 
  {
    
  }
  
  if(gpio_get(SW4)==0)  //���뿪��4����
  {

      
  }
  else 
  {
    
  }
 

} 

/*
 * ��ⰴ���Ƿ���
 */
void Check_BottonPress()
{
  
      //��ʾ����
     if(BT_SHOW_IN==0)
   {
      //ȥ��
       BEEP_ON;
       DELAY_MS(40);
      BEEP_OFF;
      if(BT_SHOW_IN==0)
      {    
          if(OLED_Refresh==false)
        {
         OLED_Init();
         OLED_Refresh=true;
        }
        else
        {
          OLED_Refresh=false;
          OLED_Fill(0x00);       
        }
      }
      while(BT_SHOW_IN==0);  //ֱ�������ɿ�������
      DELAY_MS(30);
   } 
   
   //����1 yes
   if(BT_YES_IN==0) 
   {
     //ȥ��
       BEEP_ON;
       DELAY_MS(30);
       BEEP_OFF;
      if(BT_YES_IN==0&&OLED_Refresh)
     {    
       if(Para_Index==7) 
       { 
         EEPROM_Save();
         Para_Index=0; 
       }
       else
       {
 
         if(Para_Checked==false&&((Page_Index==1)||(Page_Index==0))) Para_Checked=true;
         else Para_Checked=false;       

       }
      }
      else
      {
        if(Stop==true)
       { 
         Start_Cnt=1000;
         Starting=true;
         Stop=false; 
         //�������ٶȿ��Ʊ�������
         SpeedControlOutOld=0;
         SpeedControlOutNew=0;
         SpeedControlIntegral=0;  
         PID_SPEED.OUT=0;
         Distance=0;
         RunTime=0;
         ControlSpeed=0;
       }
         else
       {
        Stop=true;
        CarStopedJustNow=true;     //С����ֹͣ
       }
        
      }
      while(BT_YES_IN==0); //ֱ�������ɿ�������
   }
   //����2 Left_L
     if(BT_LEFT_IN==0&&OLED_Refresh)
   {
      //ȥ��
       BEEP_ON;
       DELAY_MS(30);
       BEEP_OFF;
      if(BT_LEFT_IN==0)
      {
        if(Para_Checked) 
        {
          if(Step_Index==5) 
          Step_Index=5;   //���Ĳ���Ϊ10
          else Step_Index++;
        }
        else 
        { 
          Para_Index=0;
          if(Page_Index==0) Page_Index=3; //������û��ѡ�е�ʱ�򣬰����Ҽ���ҳ
          else Page_Index--;
          OLED_Fill(0);//���� 
        }
      }
      while(BT_LEFT_IN==0);//ֱ�������ɿ�������
   } 
   //����6 Right_L
     if(BT_RIGHT_IN==0&&OLED_Refresh)
   {
      //ȥ��
       BEEP_ON;
       DELAY_MS(30);
       BEEP_OFF;
      if(BT_RIGHT_IN==0)
      {
        if(Para_Checked) 
        {
          if(Step_Index==0) 
           Step_Index=0;//��С�Ĳ���Ϊ0.0001
          else
          {
            Step_Index--;
          }
        }
        else 
        { 
          Para_Index=0;
          if(Page_Index==3) Page_Index=0;
          else Page_Index++;
         OLED_Fill(0);//���� 
        }
      }
      while(BT_RIGHT_IN==0);      //ֱ�������ɿ�������
   }
   //����3 up
     if(BT_UP_IN==0&&OLED_Refresh)
   {
       BEEP_ON;
       DELAY_MS(30);
       BEEP_OFF;
      if(BT_UP_IN==0)
      {
   
          if(Para_Checked==false)
          {
           if(Para_Index==0) Para_Index=Para_Index_Limit;
           else Para_Index-=1;
          }
          else
          {
              if(Page_Index==0&&Para_Index<=6)                    //�޸ĵ�0ҳ�Ĳ���
            {
              Control_Para[Para_Index]+=Step[Step_Index];
            }
            
            if(Page_Index==1&&Para_Index<=6)                    //�޸ĵ�1ҳ�Ĳ���
            {
              Control_Para[Para_Index+7]+=Step[Step_Index];
            } 
            Para_Update();
          }
      }  
      while(BT_UP_IN==0);//ֱ�������ɿ�������  
   }
   //����4 down
     if(BT_DOWN_IN==0&&OLED_Refresh)
   {
       BEEP_ON;
       DELAY_MS(30);
       BEEP_OFF;
      if(BT_DOWN_IN==0)
      {
          if(Para_Checked==false)
          {             
            if(Para_Index==Para_Index_Limit)Para_Index=0;   //��ֹ��ų�����Χ
            else  Para_Index+=1; 
          }
           else 
           {
              if(Page_Index==0&&Para_Index<=6)                    //�޸ĵ�0ҳ�Ĳ���
            {
              Control_Para[Para_Index]-=Step[Step_Index];
            }
             
             if(Page_Index==1&&Para_Index<=6)                    //�޸ĵ�1ҳ�Ĳ���
            {
              Control_Para[Para_Index+7]-=Step[Step_Index];
            }
            Para_Update();
          }
      }

      while(BT_DOWN_IN==0);  //ֱ�������ɿ�������
   }
}

void Send_Variable()
{
  uint8 i=0,ch=0;
  float temp=0;
  uint8 Variable_num=10;
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0xad);
  my_putchar(Variable_num);
 for(i=0;i<Variable_num;i++)
  {
    temp=Variable[i];
    ch=BYTE0(temp);
      my_putchar(ch);
    ch=BYTE1(temp);
      my_putchar(ch);
    ch=BYTE2(temp);
      my_putchar(ch);
    ch=BYTE3(temp);
      my_putchar(ch);
  }
     my_putchar(0x0d);
}

void Send_SD()
{
 int i;
 SD_ReadDisk(SD_Buff,Block_Index*2+BLOCK_BEGIN);
 SD_ReadDisk(&(SD_Buff[512]),Block_Index*2+BLOCK_BEGIN+1);
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0xff);
 for(i=0;i<1024;i++) 
 {
   my_putchar(SD_Buff[i]); 
   if(i>=3)
   {
     if(SD_Buff[i-2]==0xAA)
     {
         if(SD_Buff[i-1]==0xBB)
       {
            if(SD_Buff[i]==0xCC)
           {
             i=i;
                break; //�����������Ч��û��Ҫ����
           }
       }
     }
   }
 }
}

void Modify_Parameter(uint8 *buff)
{
   uint8 i=0,addr=0;
   float temp;
   uint8 Parameter_num=14; //14���ɸĲ���
  /*          �޸Ĳ�������         */
   for(i=0;i<Parameter_num;i++)
  {
       BYTE0(temp)=*(uint8*)(buff+addr);
       addr++;
       BYTE1(temp)=*(uint8*)(buff+addr);
       addr++;
       BYTE2(temp)=*(uint8*)(buff+addr);
       addr++;
       BYTE3(temp)=*(uint8*)(buff+addr);
       addr++;
       Control_Para[i]=temp;
   }
   
}

void Send_Parameter()
{
  uint8 i=0,ch=0;
  float temp=0;
  uint8 Parameter_num=14;  //14���ɸĲ���
  
 
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0xab);
  my_putchar(Parameter_num);
  for(i=0;i<Parameter_num;i++)
  { 
     temp=Control_Para[i];
    ch=BYTE0(temp);
    my_putchar(ch);
    ch=BYTE1(temp);
    my_putchar(ch);
    ch=BYTE2(temp);
    my_putchar(ch);
    ch=BYTE3(temp);
    my_putchar(ch);
  }
    my_putchar(0X0b);//֡β
}


void UART0_RX_IRQHandler()
{
  static uint8 recv;
  static uint8 data_cnt=0;
  static uint8 Recv_Buff[100];
  static uint8 Data_Receiving=false,Block_Index_Receiving=false;
  if(uart_query(UART0)==1)  uart_getchar (UART0,(char*)(&recv));  //����ʵ�ʵĴ������޸�
  /**********�������ڽ���������λ���Ĳ�������*********/
  if(Data_Receiving||Block_Index_Receiving)
  {
    if(Data_Receiving)
    { 
      if(data_cnt<56)
      {
       Recv_Buff[data_cnt]= recv;
       data_cnt++;
      }
      else
      {
        data_cnt=0;    //�ﵽ֡��
        Data_Receiving=false;
        if(recv==0XAB)  //֡β
        {
           Modify_Parameter(Recv_Buff);
           SendPara=1;      //�����ش���ȷ�ϲ����޸����
        }
      }
     }
     if(Block_Index_Receiving)
     {
        if(data_cnt<1)
       {
        Recv_Buff[data_cnt]= recv;
        data_cnt++;
       }
       else
       {
         Recv_Buff[data_cnt]= recv;
         BYTE0(Block_Index)=*(uint8*)(Recv_Buff);
         BYTE1(Block_Index)=*(uint8*)(Recv_Buff+1);
         SendSD=true;
         
         data_cnt=0;    //
         Block_Index_Receiving=false;
       }
     }
  }
  else
  {
    beep=1;
    
    switch(recv)         //�жϹ�����
     { 
        case 0x30:           //��ģ��ͣ���� �յ����ݺ���Կ���С����������ֹͣ
          if(stop_contorl==0)
            stop_contorl=1;
            /***********�����������ֹͣС��������ٶ�����Ϊ�㣬�رյ����PWM��************/
          else 
            stop_contorl=0;
            /***********���������������С����ʹС����������************/
        break;
        
        case 0x31:           //���ݷ��Ϳ���
          if(send_data_contorl==0)
            send_data_contorl=1;   
           else 
            send_data_contorl=0;  
           /*********�������ݷ���ʱҪռ�ò���CPU��Դ������ʱ�䣬С������ʱ�����鷢�����ݣ����Կ��Թرշ���*********/
        break;
        
        case 0x32:           //��ȡ����
         if(SendPara==0)
         SendPara=1;
         break;        
        case 0X33:             //�޸Ĳ���
          Data_Receiving=true;
          data_cnt=0;
        break;
        case 0X34:             //�������
          EEPROM_Save();
        break;
        
        case 0X35:             //SD�����ݱ��濪��
         if(SD_Save==0)
         {
           SD_Save=1;
           Block_Index=0;
           SD_Write_info=0;
         }
         else 
         {
           SD_Write_info=1;
           SD_Save=0; 
         }
        break;
        
        case 0X36:             //��ȡSD�����ݱ�����ʼ��������Ϣ
           Block_Index=0;
           SendSD=true;
        break;
        
        case 0X37:             //SD����һ֡
         
            if(SD_OK)
         {
           Block_Index--;
           SendSD=true;
         }
    
        beep=0;         
        break;
        
        case 0X38:             //SD����һ֡
          
         if(SD_OK)
         {
           Block_Index++;
           SendSD=true;
         }
           beep=0;   
        break;
        
        case 0X39:             //������ת
         Block_Index_Receiving=true;
         data_cnt=0;
        break;
        
        
        default:           //
         break;
      }
  }
  
  uart_rx_irq_en(UART0);//ʹ�ܴ��ڽ����жϣ���ֹ�������жϱ��ر� 
    
}