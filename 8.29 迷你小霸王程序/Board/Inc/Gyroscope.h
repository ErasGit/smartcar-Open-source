/*
* @file         Gyroscope.h
  * @brief      ��Ǵ�������غ���ʵ��
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-7-31
*/

#ifndef __GYROSCOPE_H__
#define __GYROSCOPE_H__

#define MPU_SELF_TESTX_REG		0X0D	//�Լ�Ĵ���x
#define MPU_SELF_TESTY_REG		0X0E	//�Լ�Ĵ���y
#define MPU_SELF_TESTZ_REG		0X0F	//�Լ�Ĵ���z
#define MPU_SELF_TESTA_REG		0X10	//�Լ�Ĵ���a
#define MPU_SAMPLE_RATE_REG		0X19	//����ƽ�ʷ�Ƶ��
#define MPU_CFG_REG			0X1A	//���üĴ���
#define MPU_GYRO_CFG_REG		0X1B	//���������üĴ���
#define MPU_ACCEL_CFG_REG		0X1C	//���ٶȼ����üĴ���
#define MPU_MOTION_DET_REG		0X1F	//�˶���ֵⷧ���üĴ���
#define MPU_FIFO_EN_REG			0X23	//FIFOʹ�ܼĴ���
#define MPU_I2CMST_CTRL_REG		0X24	//IIC�������üĴ���

#define MPU_I2CMST_STA_REG		0X36	//IIC����״̬�Ĵ���
#define MPU_INTBP_CFG_REG		0X37	//�ж����üĴ���
#define MPU_INT_EN_REG			0X38	//�ж�ʹ�ܼĴ���
#define MPU_INT_STA_REG			0X3A	//�ж�״̬�Ĵ���

#define MPU_ACCEL_XOUTH_REG		0X3B	//X���λ���ٶȼĴ���
#define MPU_ACCEL_XOUTL_REG		0X3C	//X���λ���ٶȼĴ���
#define MPU_ACCEL_YOUTH_REG		0X3D	//Y���λ���ٶȼĴ���
#define MPU_ACCEL_YOUTL_REG		0X3E	//Y���λ���ٶȼĴ���
#define MPU_ACCEL_ZOUTH_REG		0X3F	//Z���λ���ٶȼĴ���
#define MPU_ACCEL_ZOUTL_REG		0X40	//Z���λ���ٶȼĴ���

#define MPU_TEMP_OUTH_REG		0X41	//�¶ȸ�λ�Ĵ���
#define MPU_TEMP_OUTL_REG		0X42	//�¶ȵ�λ�Ĵ���

#define MPU_GYRO_XOUTH_REG		0X43	//X���λ�����ǼĴ���
#define MPU_GYRO_XOUTL_REG		0X44	//X���λ�����ǼĴ���
#define MPU_GYRO_YOUTH_REG		0X45	//Y���λ�����ǼĴ���
#define MPU_GYRO_YOUTL_REG		0X46	//Y���λ�����ǼĴ���
#define MPU_GYRO_ZOUTH_REG		0X47	//Z���λ�����ǼĴ���
#define MPU_GYRO_ZOUTL_REG		0X48	//Z���λ�����ǼĴ���

#define MPU_I2CMST_DELAY_REG	        0X67	//IIC������ʱ����Ĵ���
#define MPU_SIGPATH_RST_REG		0X68	//�ź�ͨ����λ�Ĵ���
#define MPU_MDETECT_CTRL_REG	        0X69	//�˶������ƼĴ���
#define MPU_USER_CTRL_REG		0X6A	//�û����ƼĴ���
#define MPU_PWR_MGMT1_REG		0X6B	//��Դ����Ĵ���1
#define MPU_PWR_MGMT2_REG		0X6C	//��Դ����Ĵ���2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO�����Ĵ�����λ
#define MPU_FIFO_CNTL_REG		0X73	//FIFO�����Ĵ�����λ
#define MPU_FIFO_RW_REG			0X74	//FIFO��д�Ĵ���
#define MPU_DEVICE_ID_REG		0X75	//����ID�Ĵ���
 
#define MPU_ADDR			0XD0    //������ַ


uint8 MPU_Init(void);
uint8 MPU_Set_Gyro_Fsr(uint8 fsr);
uint8 MPU_Set_Accel_Fsr(uint8 fsr);
uint8 MPU_Set_LPF(uint16 lpf);
uint8 MPU_Set_Rate(uint16 rate);
int16 MPU_Get_Temperature(void);
uint8 MPU_Get_Gyroscope(int16 *gx,int16 *gy,int16 *gz);
uint8 MPU_Get_Accelerometer(int16 *ax,int16 *ay,int16 *az);


#endif