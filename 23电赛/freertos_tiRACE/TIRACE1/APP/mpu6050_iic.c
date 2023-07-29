#include "mpu6050_iic.h"


	
	
	
HAL_StatusTypeDef MPU_Set_Gyro_Fsr(uint8_t fsr)
{
  //���������������̷�Χ
	return iic_write_one_cmd( MPU_GYRO_CFG_REG , fsr<<3 ); 
}

/**********************************************
�������ƣ�MPU_Set_Accel_Fsr
�������ܣ�����MPU6050���ٶȴ����������̷�Χ
����������fsr:0,��2g;1,��4g;2,��8g;3,��16g
��������ֵ��0,���óɹ�  ����,����ʧ��
**********************************************/
HAL_StatusTypeDef MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return iic_write_one_cmd(MPU_ACCEL_CFG_REG  , fsr<<3 );  //���ü��ٶȴ����������̷�Χ  
}

/**********************************************
�������ƣ�MPU_Set_LPF
�������ܣ�����MPU6050�����ֵ�ͨ�˲���
����������lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
��������ֵ��0,���óɹ�  ����,����ʧ��
**********************************************/
HAL_StatusTypeDef MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return iic_write_one_cmd(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}

/**********************************************
�������ƣ�MPU_Set_Rate
�������ܣ�����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
����������rate:4~1000(Hz)  ��ʼ����rateȡ50
��������ֵ��0,���óɹ�  ����,����ʧ��
**********************************************/
HAL_StatusTypeDef MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=iic_write_one_cmd(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);											//�Զ�����LPFΪ�����ʵ�һ��
}

 

/**********************************************
�������ƣ�MPU_Get_Gyroscope
�������ܣ��õ�������ֵ(ԭʼֵ)
����������gx,gy,gz:������x,y,z���ԭʼ����(������)
��������ֵ��0,��ȡ�ɹ�  ����,��ȡʧ��
**********************************************/
HAL_StatusTypeDef MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
  uint8_t buf[6];
	HAL_StatusTypeDef res;
	
	res=iic_read_len_cmd(MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];
		*gy=((uint16_t)buf[2]<<8)|buf[3];
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
  return res;
}

/**********************************************
�������ƣ�MPU_Get_Accelerometer
�������ܣ��õ����ٶ�ֵ(ԭʼֵ)
����������ax,ay,az:���ٶȴ�����x,y,z���ԭʼ����(������)
��������ֵ��0,��ȡ�ɹ�  ����,��ȡʧ��
**********************************************/
HAL_StatusTypeDef MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[6];
	HAL_StatusTypeDef res;  
	res=iic_read_len_cmd(MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];  
		*ay=((uint16_t)buf[2]<<8)|buf[3];  
		*az=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;
}

 
 
 
		
uint8_t mpu6050_init(void)
{
	uint8_t res;
	HAL_Delay(100);
  iic_write_one_cmd(MPU_PWR_MGMT1_REG , 0X80);//��λMPU6050
	HAL_Delay(100);
	iic_write_one_cmd(MPU_PWR_MGMT1_REG  ,0X00  ); //����MPU6050 
	
	
	
	
	
		
  

		MPU_Set_Gyro_Fsr(3);										//�����Ǵ�����,��2000dps
		MPU_Set_Accel_Fsr(0);										//���ٶȴ�����,��2g
		MPU_Set_Rate(50);												//���ò�����50Hz
		iic_write_one_cmd(MPU_INT_EN_REG,0X00);		//�ر������ж�
		iic_write_one_cmd(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
		iic_write_one_cmd(MPU_FIFO_EN_REG,0X00);		//�ر�FIFO
		iic_write_one_cmd(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
		
		iic_read_one_cmd(MPU_DEVICE_ID_REG,&res);
		if(res==MPU_ADDR)												//����ID��ȷ,��res = MPU_ADDR = 0x68
		{
		iic_write_one_cmd(MPU_PWR_MGMT1_REG,0X01);		//����CLKSEL,PLL X��Ϊ�ο�
		iic_write_one_cmd(MPU_PWR_MGMT2_REG,0X00);		//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);													//���ò�����Ϊ50Hz
 	}else return 1;    //��ַ���ô���,����1
	return 0;					 //��ַ������ȷ,����0

}
	
	
	
	
	
 



