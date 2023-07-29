#ifndef _my_iic_
#define _my_iic_

#include "includes.h"
#include "i2c.h"

HAL_StatusTypeDef iic_write_one_cmd(uint8_t reg,uint8_t data);
HAL_StatusTypeDef iic_write_len_cmd(uint8_t reg,uint8_t len ,uint8_t data);
HAL_StatusTypeDef iic_read_one_cmd(uint8_t reg,uint8_t *data);
HAL_StatusTypeDef iic_read_len_cmd(uint8_t reg,uint8_t len,uint8_t *data);































#endif
