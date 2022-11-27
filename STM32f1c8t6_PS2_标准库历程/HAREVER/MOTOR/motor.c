#include "sys.h"
#include "motor.h"
/************************************************

���Ӱ�����Э��&&���紴��Э��
�����������Զ�2003л��Ԭ
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


void forward(void)   //ǰ��
{
	AIN1=0;AIN2=1;//z����  ��ת
	BIN1=1;BIN2=0;//����  ����
	CIN1=1;CIN2=0;//����  ����
	DIN1=1;DIN2=0;//����  ����
}

void back(void)
{
	AIN1=1;AIN2=0;//z����  
	BIN1=0;BIN2=1;//����  
	CIN1=0;CIN2=1;//����  
	DIN1=0;DIN2=1;//����  

}
	
	
void turn_left(void) //��ת
{
  AIN1=0;AIN2=1;//z����  
	BIN1=0;BIN2=1;//����  
	CIN1=0;CIN2=1;//����  
	DIN1=1;DIN2=0;//����  
}


void turn_right(void) //��ת
{
  AIN1=1;AIN2=0;//z����  
	BIN1=1;BIN2=0;//����  
	CIN1=1;CIN2=0;//����  
	DIN1=0;DIN2=1;//����  
}


void stop(void) //ֹͣ
{
   AIN1=0;AIN2=0;//z����  
	BIN1=0;BIN2=0;//����  
	CIN1=0;CIN2=0;//����  
	DIN1=0;DIN2=0;//���� 
}
void YUANDI_(void) //ֹͣ
{
   AIN1=0;AIN2=0;//z����  
	BIN1=0;BIN2=0;//����  
	CIN1=0;CIN2=0;//����  
	DIN1=0;DIN2=0;//���� 
}

















