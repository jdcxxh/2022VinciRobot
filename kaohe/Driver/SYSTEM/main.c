
#include "includes.h"
int main(void)
{  
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    delay_init(168);                        /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    gpio_init();
   
    while (1)
    {
			 HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
        delay_ms(200);
    }
}
