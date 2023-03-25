#include "stm32f10x.h"
#include "beep.h"


void beep_Init(void)
{
	GPIO_InitTypeDef  GPIO_beep;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	GPIO_beep.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PB.8 �˿�����
  GPIO_beep.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_beep.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_beep);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	
	
}
