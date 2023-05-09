#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "string.h"
#include "malloc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/************************************************
 ALIENTEK 探索者STM32F407开发板 FreeRTOS实验14-3
 FreeRTOS优先级翻转实验-HAL库版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//任务优先级
#define START_TASK_PRIO			1
//任务堆栈大小	
#define START_STK_SIZE 			256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LOW_TASK_PRIO			2
//任务堆栈大小	
#define LOW_STK_SIZE 			256  
//任务句柄
TaskHandle_t LowTask_Handler;
//任务函数
void low_task(void *pvParameters);

//任务优先级
#define MIDDLE_TASK_PRIO 		3
//任务堆栈大小	
#define MIDDLE_STK_SIZE  		256 
//任务句柄
TaskHandle_t MiddleTask_Handler;
//任务函数
void middle_task(void *pvParameters);

//任务优先级
#define HIGH_TASK_PRIO 			4
//任务堆栈大小	
#define HIGH_STK_SIZE  			256 
//任务句柄
TaskHandle_t HighTask_Handler;
//任务函数
void high_task(void *pvParameters);

//二值信号量句柄
SemaphoreHandle_t BinarySemaphore;	//二值信号量

//LCD刷屏时使用的颜色
int lcd_discolor[14]={	WHITE, BLACK, BLUE,  BRED,      
						GRED,  GBLUE, RED,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);					//初始化延时函数
	uart_init(115200);     				//初始化串口
	LED_Init();		        			//初始化LED端口
	KEY_Init();							//初始化按键
	BEEP_Init();						//初始化蜂鸣器
	LCD_Init();							//初始化LCD
	my_mem_init(SRAMIN);            	//初始化内部内存池
    
    POINT_COLOR = RED;
	LCD_ShowString(30,10,200,16,16,"ATK STM32F103/407");	
	LCD_ShowString(30,30,200,16,16,"FreeRTOS Examp 14-3");
	LCD_ShowString(30,50,200,16,16,"Priority Overturn");
	LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,16,16,"2016/11/25");
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
	//创建二值信号量
	BinarySemaphore=xSemaphoreCreateBinary();
	//二值信号量创建成功以后要先释放一下
	if(BinarySemaphore!=NULL)xSemaphoreGive(BinarySemaphore);	
	
    //创建高优先级任务
    xTaskCreate((TaskFunction_t )high_task,             
                (const char*    )"high_task",           
                (uint16_t       )HIGH_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )HIGH_TASK_PRIO,        
                (TaskHandle_t*  )&HighTask_Handler);   
    //创建中等优先级任务
    xTaskCreate((TaskFunction_t )middle_task,     
                (const char*    )"middle_task",   
                (uint16_t       )MIDDLE_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )MIDDLE_TASK_PRIO,
                (TaskHandle_t*  )&MiddleTask_Handler); 
	//创建低优先级任务
    xTaskCreate((TaskFunction_t )low_task,     
                (const char*    )"low_task",   
                (uint16_t       )LOW_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LOW_TASK_PRIO,
                (TaskHandle_t*  )&LowTask_Handler);
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//高优先级任务的任务函数
void high_task(void *pvParameters)
{
	u8 num;
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,110,115,314); 	//画一个矩形	
	LCD_DrawLine(5,130,115,130);		//画线
	POINT_COLOR = BLUE;
	LCD_ShowString(6,111,110,16,16,"High Task");
	
	while(1)
	{
		vTaskDelay(500);	//延时500ms，也就是500个时钟节拍	
		num++;
		printf("high task Pend Sem\r\n");
		xSemaphoreTake(BinarySemaphore,portMAX_DELAY);	//获取二值信号量
		printf("high task Running!\r\n");
		LCD_Fill(6,131,114,313,lcd_discolor[num%14]); 	//填充区域
		LED1=!LED1;
		xSemaphoreGive(BinarySemaphore);				//释放信号量
		vTaskDelay(500);	//延时500ms，也就是500个时钟节拍  
	}
}

//中等优先级任务的任务函数
void middle_task(void *pvParameters)
{
	u8 num;
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(125,110,234,314); //画一个矩形	
	LCD_DrawLine(125,130,234,130);		//画线
	POINT_COLOR = BLUE;
	LCD_ShowString(126,111,110,16,16,"Middle Task");
	while(1)
	{
		num++;
		printf("middle task Running!\r\n");
		LCD_Fill(126,131,233,313,lcd_discolor[13-num%14]); //填充区域
		LED0=!LED0;
        vTaskDelay(1000);	//延时1s，也就是1000个时钟节拍	
	}
}

//低优先级任务的任务函数
void low_task(void *pvParameters)
{
	static u32 times;

	while(1)
	{
		xSemaphoreTake(BinarySemaphore,portMAX_DELAY);	//获取二值信号量
		printf("low task Running!\r\n");
		for(times=0;times<20000000;times++)				//模拟低优先级任务占用二值信号量
		{
			taskYIELD();								//发起任务调度
		}
		xSemaphoreGive(BinarySemaphore);				//释放二值信号量
		vTaskDelay(1000);	//延时1s，也就是1000个时钟节拍	
	}
}

