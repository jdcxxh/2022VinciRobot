
#include "my_motor.h"


motor_measure_t MOTOR[2];

void motor_init(motor_measure_t *per,TIM_HandleTypeDef htim)
{
  per->speed=0;
	per->last_Encode=0;
	per->encode_count=0;
	per->torget_speed=0;
	per->now_Encode=0;
	per->htim=htim;

}
 
 int Read_Encoder(motor_measure_t *per)//读取编码器走过的总编码值  encoder1
{
	return __HAL_TIM_GetCounter(&per->htim)+65535*per->encode_count;
}

 
void speed_Encoder(motor_measure_t *per)
{
	
	
  per->now_Encode = Read_Encoder(per);
	per->speed=per->now_Encode-per->last_Encode;
	per->last_Encode=per->now_Encode;

	
}

/**
  *
	*@param 
	*/

 void motor_two_dirver(int16_t pwmA,int16_t pwmB)
{

				if(pwmA>=0)
				{	
	HAL_GPIO_WritePin(GPIOF,  GPIO_PIN_11,  GPIO_PIN_RESET );				
	HAL_GPIO_WritePin(GPIOF,  GPIO_PIN_12,  GPIO_PIN_SET );

	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, pwmA);
							
					
				}
			else                  
				{
	HAL_GPIO_WritePin(GPIOF,  GPIO_PIN_12,  GPIO_PIN_RESET );
	HAL_GPIO_WritePin(GPIOF,  GPIO_PIN_11,  GPIO_PIN_SET );
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, -pwmA);
 			
				
				}

					if(pwmB>=0) 
				{			HAL_GPIO_WritePin(GPIOF,  GPIO_PIN_14,  GPIO_PIN_RESET );		
					HAL_GPIO_WritePin(GPIOF,  GPIO_PIN_13,  GPIO_PIN_SET );
					
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pwmB);
					
				}
			else                 
				{

	HAL_GPIO_WritePin(GPIOF,  GPIO_PIN_13,  GPIO_PIN_RESET );
	HAL_GPIO_WritePin(GPIOF,  GPIO_PIN_14,  GPIO_PIN_SET );
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, -pwmB);
				}
	
	}
