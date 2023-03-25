// misc.c
#include "stm32f4xx_hal.h"
#include "misc.h"

/* Cortex M3 Delay functions */

static __IO uint32_t TimingDelay = 0;

void Delay(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

/* STM32F10x TIM helper functions */

TIM_Direction TIM_ReadDirection(TIM_TypeDef* TIMx)
{
    return (TIMx->CR1 & TIM_CR1_DIR);
}

void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)
{
  /* Check the parameters */
  assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));
  if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
  {
    SysTick->CTRL |= SysTick_CLKSource_HCLK;
  }
  else
  {
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
  }

}

