#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "beep.h"
#include "pstwo.h"
#include "usart.h"
#include "timer.h"
#include "motor.h"
#include "servo.h"

/************************************************

电子爱好者协会&&机电创新协会
代码制作：自动2003谢林袁
DATE:2021\5\29

************************************************/
u8 key,z,x,v,b;       //PS2键值


 int main(void) 
 { 
//	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 delay_init();             
//	 TIM3_Int_Init(1999,719);     //定时器中断初始化（可自行发挥）
//	 beep_Init();                   //蜂鸣器初始化（可去掉）
//	 uart_init(115200);             //串口初始化
	 PS2_Init();                    //PS2初始化
//   led_Init();                    //LED初始化（PB5  PE5）
motor_GPIO_Init();             //电机引脚初始化
	 TIM1_PWM_Init(7200,0);
	TIM1_PWM2_Init(7200,0);
 TIM1_PWM3_Init(7200,0);
	TIM1_PWM4_Init(7200,0);

		TIM_SetCompare1(TIM1,3600);	  
	TIM_SetCompare2(TIM1,3600);	  
 TIM_SetCompare3(TIM1,3600);	  
	TIM_SetCompare4(TIM1,3600);	  
	 while(1)
{

		key=PS2_DataKey();     //PS2按下一个按键就会返回一个数值 可通过串口查看
//	z=PS2_AnologData(6);
//	x=PS2_AnologData(5);
//	v=PS2_AnologData(8);
//	b=PS2_AnologData(7);
//	
	
//大家可以在swtich里修改指令
	switch(key)
		{
		 case 5: forward(); break;
		 case 6:  turn_right();break;
		 case 7:  back(); break;
		 case 8:  turn_left();  break;
		 case 9:  break;
		 default :break;
		}

	delay_ms(20);  //一定要加延时
 stop(); 
 }
}
