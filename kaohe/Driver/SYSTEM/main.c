
#include "includes.h"
int main(void)
{  
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    gpio_init();
   
    while (1)
    {
			 HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
        delay_ms(200);
    }
}
