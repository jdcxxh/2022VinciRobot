#include "sys.h"
#include "motor.h"
/************************************************

电子爱好者协会&&机电创新协会
代码制作：自动2003谢林袁
DATE:2021\5\29

************************************************/

void motor_GPIO_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_1|GPIO_Pin_0;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOA, &GPIO_InitStructure);		
	
 //GPIO_Init(GPIOE, &GPIO_InitStructure);	  

}


void forward(void)   //前进
{
	AIN1=0;AIN2=1;//z左上  正转
	BIN1=1;BIN2=0;//右上  正传
	CIN1=1;CIN2=0;//右下  正传
	DIN1=1;DIN2=0;//左下  正传
}

void back(void)
{
	AIN1=1;AIN2=0;//z左上  
	BIN1=0;BIN2=1;//右上  
	CIN1=0;CIN2=1;//右下  
	DIN1=0;DIN2=1;//左下  

}
	
	
void turn_left(void) //左转
{
  AIN1=0;AIN2=1;//z左上  
	BIN1=0;BIN2=1;//右上  
	CIN1=0;CIN2=1;//右下  
	DIN1=1;DIN2=0;//左下  
}


void turn_right(void) //右转
{
  AIN1=1;AIN2=0;//z左上  
	BIN1=1;BIN2=0;//右上  
	CIN1=1;CIN2=0;//右下  
	DIN1=0;DIN2=1;//左下  
}


void stop(void) //停止
{
   AIN1=0;AIN2=0;//z左上  
	BIN1=0;BIN2=0;//右上  
	CIN1=0;CIN2=0;//右下  
	DIN1=0;DIN2=0;//左下 
}
void YUANDI_(void) //停止
{
   AIN1=0;AIN2=0;//z左上  
	BIN1=0;BIN2=0;//右上  
	CIN1=0;CIN2=0;//右下  
	DIN1=0;DIN2=0;//左下 
}

















