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

���Ӱ�����Э��&&���紴��Э��
�����������Զ�2003л��Ԭ
DATE:2021\5\29

************************************************/
u8 key,z,x,v,b;       //PS2��ֵ


 int main(void) 
 { 
//	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 delay_init();             
//	 TIM3_Int_Init(1999,719);     //��ʱ���жϳ�ʼ���������з��ӣ�
//	 beep_Init();                   //��������ʼ������ȥ����
//	 uart_init(115200);             //���ڳ�ʼ��
	 PS2_Init();                    //PS2��ʼ��
//   led_Init();                    //LED��ʼ����PB5  PE5��
motor_GPIO_Init();             //������ų�ʼ��
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

		key=PS2_DataKey();     //PS2����һ�������ͻ᷵��һ����ֵ ��ͨ�����ڲ鿴
//	z=PS2_AnologData(6);
//	x=PS2_AnologData(5);
//	v=PS2_AnologData(8);
//	b=PS2_AnologData(7);
//	
	
//��ҿ�����swtich���޸�ָ��
	switch(key)
		{
		 case 5: forward(); break;
		 case 6:  turn_right();break;
		 case 7:  back(); break;
		 case 8:  turn_left();  break;
		 case 9:  break;
		 default :break;
		}

	delay_ms(20);  //һ��Ҫ����ʱ
 stop(); 
 }
}
