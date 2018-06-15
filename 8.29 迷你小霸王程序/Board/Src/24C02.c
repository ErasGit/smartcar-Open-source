/*
* @file         24C02.c
  * @brief      24C02��غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#include "common.h"
#include "24C02.h"
#include "MKL_i2c.h"
#include "MKL_SysTick.h"

/*
 *  @brief      ��ʼ��24c02
 *  @since      v1.0
*/
void flash_24c02_init(void)
{
  i2c_init(I2C0,FLASH_IIC_SPEED);
}

/*
 *  @brief      ��24c02��ȡһ���ֽ�
 *  @since      v1.0
 *  address     Ҫ��ȡ�ĵ�ַ(0~255)
*/
uint8 flash_24c02_read_byte(uint8 address)
{
  uint8 data = 0;
  systick_delay_ms(10);          //��д֮�����ʱ
  data = i2c_read_reg(I2C0, IIC_ADDRESS, address);
  return data;
}

/*
 *  @brief      ��24c02д��һ���ֽ�
 *  @since      v1.0
 *  data        Ҫд�������
 *  address     Ҫд�����ݵĵ�ַ(0~255)
*/
void flash_24c02_write_byte(uint8 data, uint8 address)
{
  systick_delay_ms(10);          //��д֮�����ʱ
  i2c_write_reg(I2C0, IIC_ADDRESS, address, data);
}

/*
 *  @brief      ��24c02�в���һ���ֽڣ�����0x00��
 *  @since      v1.0
 *  address     Ҫ�����ĵ�ַ(0~255)
*/
void flash_24c02_erase_byte(uint8 address)
{
  systick_delay_ms(10);          //��д֮�����ʱ
  i2c_write_reg(I2C0, IIC_ADDRESS, address, 0x00);
}
/*
 *  @brief      ��24c02������д�����ֽ�
 *  @since      v1.0
 *  address     Ҫд��ĵ�ַ(ע���ַ���)
 *  data        Ҫд�������
 *  lenth       Ҫд������ݵĳ���
*/
void flash_24c02_Write_lenth_byte(uint8 address, uint32 data, uint8 lenth)
{  	
	uint8 t=0;
	for(t=0;t<lenth;t++)
	{
            flash_24c02_write_byte((data>>(8*t))&0xff,address+t);
	}												    
}
/*
 *  @brief      ��24c02��������ȡ����ֽ�
 *  @since      v1.0
 *  address     Ҫ��ȡ�ĵ�ַ(ע���ַ���)
 *  lenth       Ҫ��ȡ�����ݵĳ���
*/
uint32 flash_24c02_read_lenth_byte(uint8 address,uint8 lenth)
{  	
	uint8 t;
	uint32 data=0;
	for(t=0;t<lenth;t++)
	{
		data<<=8;
		data+=flash_24c02_read_byte(address+lenth-t-1); 	 				   
	}
	return data;												    
}

/*
 *  @brief      ��24c02����ȫƬ������ȫ������0x00��
 *  @since      v1.0
 *  ȫ��������ʱ�ϳ�
*/
void flash_24c02_erase_all(void)
{
  uint8 address = 0;
  for(address = 0;address < 255;address ++)
  {
    systick_delay_ms(10);          //��д֮�����ʱ
    i2c_write_reg(I2C0, IIC_ADDRESS, address, 0x00);
  }
  systick_delay_ms(10);
  i2c_write_reg(I2C0, IIC_ADDRESS, 255, 0x00);  //255��������������
}
