#include "gpio.h"




void gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
   __HAL_RCC_GPIOG_CLK_ENABLE()     ;                        
                         

    gpio_init_struct.Pin = GPIO_PIN_13;                   
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            
    gpio_init_struct.Pull = GPIO_PULLUP;                    
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);        

//    gpio_init_struct.Pin = GPIO_PIN_10;               
//    HAL_GPIO_Init(GPIOG, &gpio_init_struct);       
//    
    HAL_GPIO_WritePin(GPIOG,  GPIO_PIN_13, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOG,  GPIO_PIN_10, GPIO_PIN_RESET); 
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
