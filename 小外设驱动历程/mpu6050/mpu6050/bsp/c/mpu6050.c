#include "mpu6050.h"
  

/**********************************************
函数名称：MPU_Init
函数功能：初始化MPU6050
函数参数：无
函数返回值：0,初始化成功  其他,初始化失败
**********************************************/
u8 MPU_Init(void)
{ 

  u8 pdata1=0x80;
	u8 pdata2=0x00;
	u8 pdata3=3<<3;
	u8 pdata4=0<<3;
  u8 pdata5=0X00;
	u8 pdata6=0X00;
	u8 pdata7=0X00;
	u8 pdata8=0X80;
	u8 pdata9=0x00;
	u8 pdata10=0x01;
	u8 pdata11=0x00;
	
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_PWR_MGMT1_REG, 1, &pdata1, 1, HAL_MAX_DELAY);//MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050          (MPU_ADDR<<1)|0
  HAL_Delay(100);                                                                        //delay_ms(100);
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_PWR_MGMT1_REG, 1, &pdata2, 1, HAL_MAX_DELAY);//MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_GYRO_CFG_REG, 1, &pdata3, 1, HAL_MAX_DELAY);						//MPU_Set_Gyro_Fsr(3);										//陀螺仪传感器,±2000dps																																								//MPU_Set_Gyro_Fsr(3);										//陀螺仪传感器,±2000dps
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_ACCEL_CFG_REG, 1, &pdata4, 1, HAL_MAX_DELAY);    //MPU_Set_Accel_Fsr(0);										//加速度传感器,±2g
	MPU_Set_Rate(50);												//设置采样率50Hz
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_INT_EN_REG, 1, &pdata5, 1, HAL_MAX_DELAY); //MPU_Write_Byte(MPU_INT_EN_REG,0X00);		//关闭所有中断
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_USER_CTRL_REG, 1, &pdata6, 1, HAL_MAX_DELAY); //MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_FIFO_EN_REG, 1, &pdata7, 1, HAL_MAX_DELAY); //MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);		//关闭FIFO





  HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_INTBP_CFG_REG, 1, &pdata8, 1, HAL_MAX_DELAY);//MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_DEVICE_ID_REG, 1, &pdata9, 1, HAL_MAX_DELAY); 
	//res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(pdata9==MPU_ADDR)												//器件ID正确,即res = MPU_ADDR = 0x68
	{
		
	HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_PWR_MGMT1_REG, 1, &pdata10, 1, HAL_MAX_DELAY); //MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);		//设置CLKSEL,PLL X轴为参考
		
		
		
		
		HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W,MPU_PWR_MGMT2_REG, 1, &pdata11, 1, HAL_MAX_DELAY);//MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);		//加速度与陀螺仪都工作
		MPU_Set_Rate(50);													//设置采样率为50Hz
 	}else return 1;    //地址设置错误,返回1
	return 0;					 //地址设置正确,返回0
}

/**********************************************
函数名称：MPU_Set_Gyro_Fsr
函数功能：设置MPU6050陀螺仪传感器满量程范围
函数参数：fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
函数返回值：0,设置成功  其他,设置失败
**********************************************/
//u8 MPU_Set_Gyro_Fsr(u8 fsr)
//{
//	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3); //设置陀螺仪满量程范围
//}

/**********************************************
函数名称：MPU_Set_Accel_Fsr
函数功能：设置MPU6050加速度传感器满量程范围
函数参数：fsr:0,±2g;1,±4g;2,±8g;3,±16g
函数返回值：0,设置成功  其他,设置失败
**********************************************/
//u8 MPU_Set_Accel_Fsr(u8 fsr)
//{
//	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3); //设置加速度传感器满量程范围  
//}

/**********************************************
函数名称：MPU_Set_LPF
函数功能：设置MPU6050的数字低通滤波器
函数参数：lpf:数字低通滤波频率(Hz)
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_CFG_REG, 1, &data, 1, HAL_MAX_DELAY); //MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器  
}

/**********************************************
函数名称：MPU_Set_Rate
函数功能：设置MPU6050的采样率(假定Fs=1KHz)
函数参数：rate:4~1000(Hz)  初始化中rate取50
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	
	data=HAL_I2C_Mem_Write(&hi2c1, ADDRESS_W, MPU_SAMPLE_RATE_REG, 1, &data, 1, HAL_MAX_DELAY);     //data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);											//自动设置LPF为采样率的一半
}

/**********************************************
函数名称：MPU_Get_Temperature
函数功能：得到温度传感器值
函数参数：无
函数返回值：温度值(扩大了100倍)
**********************************************/
short MPU_Get_Temperature(void)
{
   u8 buf[2]; 
   short raw;
	 float temp;
	
	 HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R,MPU_TEMP_OUTH_REG, 1, buf, 2, HAL_MAX_DELAY);//MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
   raw=((u16)buf[0]<<8)|buf[1];
   temp=36.53+((double)raw)/340;
   return temp*100;
}

/**********************************************
函数名称：MPU_Get_Gyroscope
函数功能：得到陀螺仪值(原始值)
函数参数：gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
函数返回值：0,读取成功  其他,读取失败
**********************************************/
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
  u8 buf[6],res;
	
	res=HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R, MPU_GYRO_XOUTH_REG, 1, buf, 6, HAL_MAX_DELAY);//res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];
		*gy=((u16)buf[2]<<8)|buf[3];
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
  return res;
}

/**********************************************
函数名称：MPU_Get_Accelerometer
函数功能：得到加速度值(原始值)
函数参数：ax,ay,az:加速度传感器x,y,z轴的原始读数(带符号)
函数返回值：0,读取成功  其他,读取失败
**********************************************/
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=HAL_I2C_Mem_Read(&hi2c1, ADDRESS_R,MPU_ACCEL_XOUTH_REG, 1, buf, 6, HAL_MAX_DELAY);//res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}


uint8_t mpu_6050_addr;
void I2C_Addr_Search(void)
{
	for(mpu_6050_addr = 0x00;mpu_6050_addr <= 0xff;mpu_6050_addr++)
	{
		if(HAL_I2C_IsDeviceReady(&hi2c1,mpu_6050_addr,1,100)==HAL_OK)
			break;
	}
		
}

