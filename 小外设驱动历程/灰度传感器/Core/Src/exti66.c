#include "exti66.h"

void EXTI4_IRQHandler()
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);        
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);  
}





void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_Delay(10);
	
  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))
	{
	 HAL_GPIO_WritePin( GPIOC,  GPIO_PIN_13, GPIO_PIN_SET);
	
	}
  else
  { 
	 HAL_GPIO_WritePin( GPIOC,  GPIO_PIN_13, GPIO_PIN_RESET);

  }
}

