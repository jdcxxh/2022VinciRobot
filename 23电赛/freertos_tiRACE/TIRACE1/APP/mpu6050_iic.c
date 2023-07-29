#include "mpu6050_iic.h"


	
	
	
HAL_StatusTypeDef MPU_Set_Gyro_Fsr(uint8_t fsr)
{
  //设置陀螺仪满量程范围
	return iic_write_one_cmd( MPU_GYRO_CFG_REG , fsr<<3 ); 
}

/**********************************************
函数名称：MPU_Set_Accel_Fsr
函数功能：设置MPU6050加速度传感器满量程范围
函数参数：fsr:0,±2g;1,±4g;2,±8g;3,±16g
函数返回值：0,设置成功  其他,设置失败
**********************************************/
HAL_StatusTypeDef MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return iic_write_one_cmd(MPU_ACCEL_CFG_REG  , fsr<<3 );  //设置加速度传感器满量程范围  
}

/**********************************************
函数名称：MPU_Set_LPF
函数功能：设置MPU6050的数字低通滤波器
函数参数：lpf:数字低通滤波频率(Hz)
函数返回值：0,设置成功  其他,设置失败
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
	return iic_write_one_cmd(MPU_CFG_REG,data);//设置数字低通滤波器  
}

/**********************************************
函数名称：MPU_Set_Rate
函数功能：设置MPU6050的采样率(假定Fs=1KHz)
函数参数：rate:4~1000(Hz)  初始化中rate取50
函数返回值：0,设置成功  其他,设置失败
**********************************************/
HAL_StatusTypeDef MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=iic_write_one_cmd(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);											//自动设置LPF为采样率的一半
}

 

/**********************************************
函数名称：MPU_Get_Gyroscope
函数功能：得到陀螺仪值(原始值)
函数参数：gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
函数返回值：0,读取成功  其他,读取失败
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
函数名称：MPU_Get_Accelerometer
函数功能：得到加速度值(原始值)
函数参数：ax,ay,az:加速度传感器x,y,z轴的原始读数(带符号)
函数返回值：0,读取成功  其他,读取失败
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
  iic_write_one_cmd(MPU_PWR_MGMT1_REG , 0X80);//复位MPU6050
	HAL_Delay(100);
	iic_write_one_cmd(MPU_PWR_MGMT1_REG  ,0X00  ); //唤醒MPU6050 
	
	
	
	
	
		
  

		MPU_Set_Gyro_Fsr(3);										//陀螺仪传感器,±2000dps
		MPU_Set_Accel_Fsr(0);										//加速度传感器,±2g
		MPU_Set_Rate(50);												//设置采样率50Hz
		iic_write_one_cmd(MPU_INT_EN_REG,0X00);		//关闭所有中断
		iic_write_one_cmd(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
		iic_write_one_cmd(MPU_FIFO_EN_REG,0X00);		//关闭FIFO
		iic_write_one_cmd(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
		
		iic_read_one_cmd(MPU_DEVICE_ID_REG,&res);
		if(res==MPU_ADDR)												//器件ID正确,即res = MPU_ADDR = 0x68
		{
		iic_write_one_cmd(MPU_PWR_MGMT1_REG,0X01);		//设置CLKSEL,PLL X轴为参考
		iic_write_one_cmd(MPU_PWR_MGMT2_REG,0X00);		//加速度与陀螺仪都工作
		MPU_Set_Rate(50);													//设置采样率为50Hz
 	}else return 1;    //地址设置错误,返回1
	return 0;					 //地址设置正确,返回0

}
	
	
	
	
	
 



