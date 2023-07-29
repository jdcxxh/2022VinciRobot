#include "my_tim.h"

void my_tim_init(void)
{
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 0);	

}





void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if(htim->Instance==htim9.Instance)		 
 {
 	speed_Encoder(&MOTOR[0]);
 	speed_Encoder(&MOTOR[1]);
 
 
 
 }
	
	
  	 if(htim->Instance==htim1.Instance)		        
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1))
		{
		MOTOR[0].encode_count--;
		}
	  else
		{		
			MOTOR[0].encode_count++;		
		}

  } 
	if(htim->Instance==htim8.Instance)		        
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim8))
		{
			MOTOR[1].encode_count--;
		}
	  else
		{		
			MOTOR[1].encode_count++;		
		}
	}

}
 








