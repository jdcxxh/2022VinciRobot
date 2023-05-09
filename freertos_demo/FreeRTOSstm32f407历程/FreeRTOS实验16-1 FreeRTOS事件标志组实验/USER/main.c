#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "lcd.h"
#include "key.h"
#include "beep.h"
#include "exti.h"
#include "string.h"
#include "malloc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
/************************************************
 ALIENTEK 探索者STM32F407开发板 FreeRTOS实验16-1
 FreeRTOS事件标志组实验-库函数版本
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
#define EVENTSETBIT_TASK_PRIO	2
//任务堆栈大小	
#define EVENTSETBIT_STK_SIZE 	256  
//任务句柄
TaskHandle_t EventSetBit_Handler;
//任务函数
void eventsetbit_task(void *pvParameters);

//任务优先级
#define EVENTGROUP_TASK_PRIO	3
//任务堆栈大小	
#define EVENTGROUP_STK_SIZE 	256  
//任务句柄
TaskHandle_t EventGroupTask_Handler;
//任务函数
void eventgroup_task(void *pvParameters);

//任务优先级
#define EVENTQUERY_TASK_PRIO	4
//任务堆栈大小	
#define EVENTQUERY_STK_SIZE 	256  
//任务句柄
TaskHandle_t EventQueryTask_Handler;
//任务函数
void eventquery_task(void *pvParameters);


////////////////////////////////////////////////////////
EventGroupHandle_t EventGroupHandler;	//事件标志组句柄

#define EVENTBIT_0	(1<<0)				//事件位
#define EVENTBIT_1	(1<<1)
#define EVENTBIT_2	(1<<2)
#define EVENTBIT_ALL	(EVENTBIT_0|EVENTBIT_1|EVENTBIT_2)

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
	EXTIX_Init();						//初始化外部中断
	LCD_Init();							//初始化LCD
	my_mem_init(SRAMIN);            	//初始化内部内存池
	
    POINT_COLOR = RED;
	LCD_ShowString(30,10,200,16,16,"ATK STM32F103/407");	
	LCD_ShowString(30,30,200,16,16,"FreeRTOS Examp 16-1");
	LCD_ShowString(30,50,200,16,16,"Event Group");
	LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,16,16,"2016/11/25");

	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,130,234,314);	//画矩形
	POINT_COLOR = BLUE;
	LCD_ShowString(30,110,220,16,16,"Event Group Value:0");
	
	
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
    //创建事件标志组
	EventGroupHandler=xEventGroupCreate();	 //创建事件标志组
	
	//创建设置事件位的任务
    xTaskCreate((TaskFunction_t )eventsetbit_task,             
                (const char*    )"eventsetbit_task",           
                (uint16_t       )EVENTSETBIT_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EVENTSETBIT_TASK_PRIO,        
                (TaskHandle_t*  )&EventSetBit_Handler);   	
    //创建事件标志组处理任务
    xTaskCreate((TaskFunction_t )eventgroup_task,             
                (const char*    )"eventgroup_task",           
                (uint16_t       )EVENTGROUP_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EVENTGROUP_TASK_PRIO,        
                (TaskHandle_t*  )&EventGroupTask_Handler);  
	//创建事件标志组查询任务
    xTaskCreate((TaskFunction_t )eventquery_task,             
                (const char*    )"eventquery_task",           
                (uint16_t       )EVENTQUERY_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EVENTQUERY_TASK_PRIO,        
                (TaskHandle_t*  )&EventQueryTask_Handler);    
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}


//设置事件位的任务
void eventsetbit_task(void *pvParameters)
{
	u8 key;
	while(1)
	{
		if(EventGroupHandler!=NULL)
		{
			key=KEY_Scan(0);
			switch(key)
			{
				case KEY1_PRES:
					xEventGroupSetBits(EventGroupHandler,EVENTBIT_1);
					break;
				case KEY2_PRES:
					xEventGroupSetBits(EventGroupHandler,EVENTBIT_2);
					break;	
			}
		}
        vTaskDelay(10); //延时10ms，也就是10个时钟节拍
	}
}

//事件标志组处理任务
void eventgroup_task(void *pvParameters)
{
	u8 num;
	EventBits_t EventValue;
	while(1)
	{

		if(EventGroupHandler!=NULL)
		{
			//等待事件组中的相应事件位
			EventValue=xEventGroupWaitBits((EventGroupHandle_t	)EventGroupHandler,		
										   (EventBits_t			)EVENTBIT_ALL,
										   (BaseType_t			)pdTRUE,				
										   (BaseType_t			)pdTRUE,
								           (TickType_t			)portMAX_DELAY);	
			printf("事件标志组的值:%d\r\n",EventValue);
			LCD_ShowxNum(174,110,EventValue,1,16,0);
			
			num++;
			LED1=!LED1;	
			LCD_Fill(6,131,233,313,lcd_discolor[num%14]);
		}
		else
		{
			vTaskDelay(10); //延时10ms，也就是10个时钟节拍
		}
	}
}

//事件查询任务
void eventquery_task(void *pvParameters)
{	
	u8 num=0;
	EventBits_t NewValue,LastValue;
	while(1)
	{
		if(EventGroupHandler!=NULL)
		{
			NewValue=xEventGroupGetBits(EventGroupHandler);	//获取事件组的
			if(NewValue!=LastValue)
			{
				LastValue=NewValue;
				printf("事件标志组的值:%d\r\n",NewValue);
				LCD_ShowxNum(174,110,NewValue,1,16,0);
			}
		}
		num++;
		if(num==10) 	//每500msLED0闪烁一次
		{
			num=0;
			LED0=!LED0;	
		}
		vTaskDelay(50); //延时50ms，也就是50个时钟节拍
	}
}

