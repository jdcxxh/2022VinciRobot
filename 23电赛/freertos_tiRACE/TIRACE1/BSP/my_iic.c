#include "my_iic.h"


/**
 *@brief 命令发送
 *@param  date 命令
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



//////////////U8G2/////////////


uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	switch(msg){
		
	case U8X8_MSG_GPIO_AND_DELAY_INIT:
	    break;
		
	case U8X8_MSG_DELAY_MILLI:
		HAL_Delay(arg_int);
	    break;
		
	case U8X8_MSG_GPIO_I2C_CLOCK:		
        break;							
		
    case U8X8_MSG_GPIO_I2C_DATA:			
        break;
		
	default:	
		return 0;
	}
	return 1; // command processed successfully.
}

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  static uint8_t buffer[32];		/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
  static uint8_t buf_idx;
  uint8_t *data;
 
  switch(msg){
		
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;      
      while( arg_int > 0 ){
				buffer[buf_idx++] = *data;
				data++;
				arg_int--;
			}      
    break;
			
    case U8X8_MSG_BYTE_INIT:
      /* add your custom code to init i2c subsystem */
    break;
		
    case U8X8_MSG_BYTE_START_TRANSFER:
      buf_idx = 0;
    break;
		
    case U8X8_MSG_BYTE_END_TRANSFER:
      HAL_I2C_Master_Transmit(&hi2c1,u8x8_GetI2CAddress(u8x8), buffer, buf_idx,1000);
    break;
		
    default:
      return 0;
  }
  return 1;
}





u8g2_t u8g2;
void u8g2_init(void)
{
u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2,U8G2_R0,u8x8_byte_hw_i2c,u8g2_gpio_and_delay_stm32);
u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
u8g2_SetPowerSave(&u8g2, 0); // wake up display
u8g2_ClearDisplay(&u8g2);
	
	
	
//u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_chinese1);
//u8g2_DrawCircle(&u8g2,60,30,20,U8G2_DRAW_ALL);
//u8g2_DrawUTF8(&u8g2,10,50,"你好,world");
//u8g2_SendBuffer(&u8g2);
//HAL_Delay(3000);





}
 


//void draw(u8g2_t *u8g2)
//{
//    u8g2_SetFontMode(u8g2, 1); /*字体模式选择*/
//    u8g2_SetFontDirection(u8g2, 0); /*字体方向选择*/
//    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); /*字库选择*/
//    u8g2_DrawStr(u8g2, 0, 20, "U");
//    
//    u8g2_SetFontDirection(u8g2, 1);
//    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
//    u8g2_DrawStr(u8g2, 21,8,"8");
//        
//    u8g2_SetFontDirection(u8g2, 0);
//    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
//    u8g2_DrawStr(u8g2, 51,30,"g");
//    u8g2_DrawStr(u8g2, 67,30,"\xb2");
//    
//    u8g2_DrawHLine(u8g2, 2, 35, 47);
//    u8g2_DrawHLine(u8g2, 3, 36, 47);
//    u8g2_DrawVLine(u8g2, 45, 32, 12);
//    u8g2_DrawVLine(u8g2, 46, 33, 12);
//  
//    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
//    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");
//}

void draw(void)
{
    u8g2_SetFontMode(&u8g2, 2); /*字体模式选择*/
    u8g2_SetFontDirection(&u8g2, 0); /*字体方向选择*/
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr); /*字库选择*/
//    u8g2_DrawStr(&u8g2, 0, 20, "U");
//    
//    u8g2_SetFontDirection(&u8g2, 1);
//    u8g2_SetFont(&u8g2, u8g2_font_inb30_mn);
//    u8g2_DrawStr(&u8g2, 21,8,"8");
//        
//    u8g2_SetFontDirection(&u8g2, 0);
//    u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
//    u8g2_DrawStr(&u8g2, 51,30,"g");
//    u8g2_DrawStr(&u8g2, 67,30,"\xb2");
//    
//    u8g2_DrawHLine(&u8g2, 2, 35, 47);
//    u8g2_DrawHLine(&u8g2, 3, 36, 47);
//    u8g2_DrawVLine(&u8g2, 45, 32, 12);
//    u8g2_DrawVLine(&u8g2, 46, 33, 12);
 u8g2_DrawStr(&u8g2, 0, 15,"1.led");
	u8g2_DrawStr(&u8g2, 0, 30,"2.pid");
	u8g2_SetDrawColor(&u8g2,1);
	u8g2_DrawStr(&u8g2, 0, 45,"3.speed_pid");

	u8g2_DrawStr(&u8g2, 0, 60,"4.positions_pid");
}

void draw1(void)
{
    u8g2_SetFontMode(&u8g2, 2); /*字体模式选择*/
    u8g2_SetFontDirection(&u8g2, 0); /*字体方向选择*/
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr); /*字库选择*/
//    u8g2_DrawStr(&u8g2, 0, 20, "U");
//    
//    u8g2_SetFontDirection(&u8g2, 1);
//    u8g2_SetFont(&u8g2, u8g2_font_inb30_mn);
//    u8g2_DrawStr(&u8g2, 21,8,"8");
//        
//    u8g2_SetFontDirection(&u8g2, 0);
//    u8g2_SetFont(&u8g2, u8g2_font_inb24_mf);
//    u8g2_DrawStr(&u8g2, 51,30,"g");
//    u8g2_DrawStr(&u8g2, 67,30,"\xb2");
//    
//    u8g2_DrawHLine(&u8g2, 2, 35, 47);
//    u8g2_DrawHLine(&u8g2, 3, 36, 47);
//    u8g2_DrawVLine(&u8g2, 45, 32, 12);
//    u8g2_DrawVLine(&u8g2, 46, 33, 12);
 u8g2_DrawStr(&u8g2, 7, 15,"SLH");
	u8g2_DrawStr(&u8g2, 7, 30,"2.pid");
	u8g2_SetDrawColor(&u8g2,1);
	u8g2_DrawStr(&u8g2, 7, 45,"3.speed_pid");

	u8g2_DrawStr(&u8g2, 7, 60,"4.TEST");
}
