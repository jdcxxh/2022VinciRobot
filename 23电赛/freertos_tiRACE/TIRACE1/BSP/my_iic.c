#include "my_iic.h"


/**
 *@brief √¸¡Ó∑¢ÀÕ
 *@param  date √¸¡Ó
 */
HAL_StatusTypeDef iic_write_one_cmd(uint8_t reg,uint8_t data)
{
	
	uint8_t pData=data;
	return HAL_I2C_Mem_Write(&hi2c1, 0x68, reg, I2C_MEMADD_SIZE_8BIT, &pData, 1, 0xffff );//(uint8_t *)
	
}
HAL_StatusTypeDef iic_write_len_cmd(uint8_t reg,uint8_t len ,uint8_t data)
{
	
	uint8_t pData=data;
	return HAL_I2C_Mem_Write(&hi2c1, 0x68, reg, I2C_MEMADD_SIZE_8BIT, &pData, len, 0xffff );//(uint8_t *)
	
}




HAL_StatusTypeDef iic_read_one_cmd(uint8_t reg,uint8_t *data)
{
	
	return HAL_I2C_Mem_Read(&hi2c1, 0x69,reg, I2C_MEMADD_SIZE_8BIT,data,1, 0xffff);
	
}

HAL_StatusTypeDef iic_read_len_cmd(uint8_t reg,uint8_t len,uint8_t *data)
{
	
	 return HAL_I2C_Mem_Read(&hi2c1, 0x69,reg, I2C_MEMADD_SIZE_8BIT,data,len, 0xffff);
	
}










