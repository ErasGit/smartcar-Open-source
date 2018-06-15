/*
* @file         IIC_sim.c
  * @brief      ģ��iic��غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#include "common.h"
#include "IIC_sim.h"
#include "MKL_gpio.h"

static void IIC_delay(volatile uint16_t time)
{
    while(time)
    {
        time--;
    }
}
  
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Init(void)
*��������:		��ʼ��I2C��Ӧ�Ľӿ����š�
*******************************************************************************/
void IIC_Init(void)
{			    
    gpio_init  (PTB9, GPO, 1); //��ʼ��SCL0
    gpio_init  (PTB7, GPO, 1); //��ʼ��SDA0
    port_init(PTB9 , ALT1 | PULLUP);
    port_init(PTB7 , ALT1 | PULLUP);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Start(void)
*��������:		����IIC��ʼ�ź�
*******************************************************************************/
void IIC_Start(void)
{
	SDA_DDR_OUT();
	SDA_H();	  	  
	SCL_H();
	IIC_DELAY();
 	SDA_L();//START:when CLK is high,DATA change form high to low 
	IIC_DELAY();
	SCL_L();
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Stop(void)
*��������:	    //����IICֹͣ�ź�
*******************************************************************************/	  
void IIC_Stop(void)
{
	SDA_DDR_OUT();
	SCL_L();
	SDA_L();//STOP:when CLK is high DATA change form low to high
 	IIC_DELAY();
	SCL_H(); 
	SDA_H();
	IIC_DELAY();							   	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC_Wait_Ack(void)
*��������:	    �ȴ�Ӧ���źŵ��� 
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
*******************************************************************************/
uint8 IIC_Wait_Ack(void)
{
	uint8 ucErrTime=0;
	SDA_DDR_OUT(); 
	SDA_H(); IIC_DELAY();	   
	SCL_H(); IIC_DELAY();
        SDA_DDR_IN();
	while(SDA_IN())
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC_Stop();
			return 1;
		}
	  IIC_DELAY();
	}
	SCL_L();//����?����?3?0 	   
	return 0;  
} 

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Ack(void)
*��������:	    ����ACKӦ��
*******************************************************************************/
void IIC_Ack(void)
{
	SCL_L();
	SDA_DDR_OUT();
	SDA_L();
	IIC_DELAY();
	SCL_H();
	IIC_DELAY();
	SCL_L();
}
	
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_NAck(void)
*��������:	    ����NACKӦ��
*******************************************************************************/	    
void IIC_NAck(void)
{
	SCL_L();
	SDA_DDR_OUT();
	SDA_H();
	IIC_DELAY();
	SCL_H();
	IIC_DELAY();
	SCL_L();
}					 				     

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Send_Byte(u8 txd)
*��������:	    IIC����һ���ֽ�
*******************************************************************************/		  
void IIC_Send_Byte(uint8 txd)
{                        
    uint8 t=8;   
    SDA_DDR_OUT(); 	    
    SCL_L();
    while(t--)
    {
        if(txd&0x80)
        {
            SDA_H();
        }
        else
        {
            SDA_L();
        }
        txd<<=1; 
        IIC_DELAY();   
	SCL_H();
	IIC_DELAY(); 
	SCL_L();	
	//IIC_delay();
    }
} 	 
   
/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IIC_Read_Byte(unsigned char ack)
*��������:	    //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK 
*******************************************************************************/ 
uint8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_DDR_IN();
    for(i=0;i<8;i++ )
    {
        SCL_L(); 
        IIC_DELAY();
	SCL_H();
        receive<<=1;
        if(SDA_IN())receive++;   
	IIC_DELAY(); 
    }					 
    if (ack)
        IIC_Ack(); 
    else
        IIC_NAck();
    return receive;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	I2C_Addr  Ŀ���豸��ַ
		addr	   �Ĵ�����ַ
����   ��������ֵ
*******************************************************************************/ 
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	IIC_Start();	
	IIC_Send_Byte(I2C_Addr);	   

	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
 
	IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte(I2C_Addr+1); 	   
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);	   
        IIC_Stop();

	return res;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ����� length��ֵ
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫ�����ֽ���
		*data  ���������ݽ�Ҫ��ŵ�ָ��
����   ���������ֽ�����
*******************************************************************************/
uint8 IICreadBytes(uint8 dev, uint8 reg, uint8 length, uint8 *data)
{
    uint8 count = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev);	   
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   
    IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte(dev+1);  
	IIC_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  
		 	else  data[count]=IIC_Read_Byte(0);	 
	}
    IIC_Stop();
    return count;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*��������:	    ������ֽ�д��ָ���豸 ָ���Ĵ���
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫд���ֽ���
		*data  ��Ҫд�����ݵ��׵�ַ
����   �����Ƿ�ɹ�
*******************************************************************************/ 
uint8 IICwriteBytes(uint8 dev, uint8 reg, uint8 length, uint8* data)
{
  
 	uint8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   
        IIC_Wait_Ack();	  
	for(count=0;count<length;count++){
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
	 }
	IIC_Stop();

    return 1; //status == 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		u8 IICreadByte(u8 dev, u8 reg, u8 *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		*data  ���������ݽ�Ҫ��ŵĵ�ַ
����   1
*******************************************************************************/
uint8 IICreadByte(uint8 dev, uint8 reg, uint8 *data)
{
	*data=I2C_ReadOneByte(dev, reg);
    return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*��������:	    д��ָ���豸 ָ���Ĵ���һ���ֽ�
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		data  ��Ҫд����ֽ�
����   1
*******************************************************************************/
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
{
    return IICwriteBytes(dev, reg, 1, &data);
}