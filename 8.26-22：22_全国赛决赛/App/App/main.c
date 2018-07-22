#include "common.h"
#include "include.h"

#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)         //������������������ȷ����ȫ

uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);     //ͼ���ַ

/*** ���Բ��� ***/
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;
extern uchar Annulus_Flag;
uint8 display_temp[20];
/*********** main����***********/
  
//aaa

void  main(void)
{	
    uchar i;
    Point = 40;   //�����ߣ�����Ϊ40����Ƭ����Ϊ��39  //�Ȳ���  
    
//    /************************ flash���ݶ���  ***********************/
//    uint32  data32;
//    uint16  data16;
//    uint8   data8;
    
    DisableInterrupts;                 //��ֹȫ���ж� 
    System_Init();		              //��ʼ������ģ��		
    SCCB_WriteByte (OV7725_CNST, CNST_Value);	  //�ı�ͼ����ֵ	����50	����60
    EnableInterrupts;				  //ʹ��ȫ���ж�
    ov7725_eagle_img_flag = IMG_START;//��ʼ�ɼ�ͼ��
	enable_irq(PORTA_IRQn);           //����PTA���ж�
//    /************************ ��ʼ��flash  ***********************/
//    flash_init();                                       //��ʼ��flash
//    flash_erase_sector(SECTOR_NUM);                     //��������
//    if( 1 == flash_write(SECTOR_NUM, 0, 0x12345678) )   //д�����ݵ�������ƫ�Ƶ�ַΪ0������һ��д��4�ֽ�
//        //if����������Ƿ�д��ɹ���д��ɹ��˾Ͷ�ȡ
//    {
//        data32 =    flash_read(SECTOR_NUM, 0, uint32);  //��ȡ4�ֽ�
//        printf("һ�ζ�ȡ32λ������Ϊ��0x%08x\n", data32);
//
//        data16 =    flash_read(SECTOR_NUM, 0, uint16);  //��ȡ2�ֽ�
//        printf("һ�ζ�ȡ16λ������Ϊ��0x%04x\n", data16);
//
//        data8  =    flash_read(SECTOR_NUM, 0, uint8);   //��ȡ1�ֽ�
//        printf("һ�ζ�ȡ8λ������Ϊ��0x%02x\n", data8);
//    }
  	while(1)
  	{
        /************************ ͼ��������ʾ   ***********************/  
        if (Img_Display)
        {
                Display_img();
                OLED_ShowLongNum(100,0,Line_Count);
                OLED_ShowLongNum(100,1,Point);
                OLED_ShowLongNum(100,2,Foresight);
                OLED_ShowLongNum(100,3,Annulus_Nums);
                OLED_ShowLongNum(100,4,Annulus_Small);
                OLED_ShowLongNum(100,5,Annulus_Mode);

                sprintf(display_temp,"%4d",Fictitious_Beeline_Num);
                OLED_ShowStr(90,6,display_temp, 1);
                /************************ ͼ�����ݷ���   ***********************/  
//                if(Image_Send_Flag && Send_OK) //ѡ���Ƿ���ͼ����Ϣ����λ��
//                {
//                    Send_OK = 0;
//                                if (Annulus_Flag)
//                                {
//                                    for(i=0;i<79;i++)
//                                    {
//                                        img[1*80+i]=0;
//                                        img[3*80+i]=0;
//                                        img[5*80+i]=0;
//                                    }
//                                }
//                    img_recontract(img, imgbuff,CAMERA_SIZE); //ͼ��ѹ��
//        //             save_var(VAR3, Track_Line_Num);
//                    data_send();							   //��������λ��
//        //            Data_Send_Scope();
//        //            vcan_sendimg(imgbuff,CAMERA_SIZE);			//��������λ��
//                }
                /************************ ͼ�����ݷ��ͽ���   ***********************/ 
               
        }
        //OLED_ShowLongNum(100,2,Foresight);
		/************************ �������ݷ���   ***********************/        
		

	}
}




