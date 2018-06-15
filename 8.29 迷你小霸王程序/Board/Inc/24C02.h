/*
* @file         24C02.h
  * @brief      24C02��غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#ifndef __24C02_H__
#define __24C02_H__

#define FLASH_IIC_SPEED 100*1000        //IIC��������
#define IIC_ADDRESS 0X50        //24c02Ӳ��7bit��ַ

void flash_24c02_init(void);            //��ʼ��24c02
uint8 flash_24c02_read_byte(uint8 address);             //��24c02��ȡһ���ֽ�
void flash_24c02_write_byte(uint8 data, uint8 address);         //��24c02д��һ���ֽ�
void flash_24c02_erase_byte(uint8 address);              //��24c02�в���һ���ֽڣ�����0x00��
void flash_24c02_erase_all(void);                //��24c02����ȫƬ������ȫ������0x00��
void flash_24c02_Write_lenth_byte(uint8 address, uint32 data, uint8 lenth);     //��24c02������д�����ֽ�
uint32 flash_24c02_read_lenth_byte(uint8 address,uint8 lenth);          //��24c02��������ȡ����ֽ�
#endif