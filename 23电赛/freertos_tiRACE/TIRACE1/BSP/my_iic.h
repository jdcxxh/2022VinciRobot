#ifndef _my_iic_
#define _my_iic_

#include "includes.h"
#include "i2c.h"

#include "u8g2.h"
HAL_StatusTypeDef iic_write_one_cmd(uint8_t reg,uint8_t data);
HAL_StatusTypeDef iic_write_len_cmd(uint8_t reg,uint8_t len ,uint8_t data);
HAL_StatusTypeDef iic_read_one_cmd(uint8_t reg,uint8_t *data);
HAL_StatusTypeDef iic_read_len_cmd(uint8_t reg,uint8_t len,uint8_t *data);






uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);



void u8g2_init(void);
void draw(void);
void draw1(void);


















#endif
