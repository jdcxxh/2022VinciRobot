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
#include "timers.h"
/************************************************
 ALIENTEK 探索者STM32F407开发板 FreeRTOS实验15-1
 FreeRTOS软件定时器实验-库函数版本
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
#define TIMERCONTROL_TASK_PRIO	2
//任务堆栈大小	
#define TIMERCONTROL_STK_SIZE 	256  
//任务句柄
TaskHandle_t TimerControlTask_Handler;
//任务函数
void timercontrol_task(void *pvParameters);

////////////////////////////////////////////////////////
TimerHandle_t 	AutoReloadTimer_Handle;			//周期定时器句柄
TimerHandle_t	OneShotTimer_Handle;			//单次定时器句柄

void AutoReloadCallback(TimerHandle_t xTimer); 	//周期定时器回调函数
void OneShotCallback(TimerHandle_t xTimer);		//单次定时器回调函数

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
	LCD_ShowString(30,30,200,16,16,"FreeRTOS Examp 15-1");
	LCD_ShowString(30,50,200,16,16,"KEY_UP:Start Tmr1");
	LCD_ShowString(30,70,200,16,16,"KEY0:Start Tmr2");
	LCD_ShowString(30,90,200,16,16,"KEY1:Stop Tmr1 and Tmr2");

	LCD_DrawLine(0,108,239,108);		//画线
	LCD_DrawLine(119,108,119,319);		//画线
	
	POINT_COLOR = BLACK;
	LCD_DrawRectangle(5,110,115,314); 	//画一个矩形	
	LCD_DrawLine(5,130,115,130);		//画线
	
	LCD_DrawRectangle(125,110,234,314); //画一个矩形	
	LCD_DrawLine(125,130,234,130);		//画线
	POINT_COLOR = BLUE;
	LCD_ShowString(6,111,110,16,16,	 "AutoTim:000");
	LCD_ShowString(126,111,110,16,16,"OneTim: 000");
	
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
    //创建软件周期定时器
    AutoReloadTimer_Handle=xTimerCreate((const char*		)"AutoReloadTimer",
									    (TickType_t			)1000,
							            (UBaseType_t		)pdTRUE,
							            (void*				)1,
							            (TimerCallbackFunction_t)AutoReloadCallback); //周期定时器，周期1s(1000个时钟节拍)，周期模式
    //创建单次定时器
	OneShotTimer_Handle=xTimerCreate((const char*			)"OneShotTimer",
							         (TickType_t			)2000,
							         (UBaseType_t			)pdFALSE,
							         (void*					)2,
							         (TimerCallbackFunction_t)OneShotCallback); //单次定时器，周期2s(2000个时钟节拍)，单次模式					  
    //创建定时器控制任务
    xTaskCreate((TaskFunction_t )timercontrol_task,             
                (const char*    )"timercontrol_task",           
                (uint16_t       )TIMERCONTROL_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TIMERCONTROL_TASK_PRIO,        
                (TaskHandle_t*  )&TimerControlTask_Handler);    
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//TimerControl的任务函数
void timercontrol_task(void *pvParameters)
{
	u8 key,num;
	while(1)
	{
		//只有两个定时器都创建成功了才能对其进行操作
		if((AutoReloadTimer_Handle!=NULL)&&(OneShotTimer_Handle!=NULL))
		{
			key = KEY_Scan(0);
			switch(key)
			{
				case WKUP_PRES:     //当key_up按下的话打开周期定时器
					xTimerStart(AutoReloadTimer_Handle,0);	//开启周期定时器
					printf("开启定时器1\r\n");
					break;
				case KEY0_PRES:		//当key0按下的话打开单次定时器
					xTimerStart(OneShotTimer_Handle,0);		//开启单次定时器
					printf("开启定时器2\r\n");
					break;
				case KEY1_PRES:		//当key1按下话就关闭定时器
					xTimerStop(AutoReloadTimer_Handle,0); 	//关闭周期定时器
					xTimerStop(OneShotTimer_Handle,0); 		//关闭单次定时器
					printf("关闭定时器1和2\r\n");
					break;	
			}
		}
		num++;
		if(num==50) 	//每500msLED0闪烁一次
		{
			num=0;
			LED0=!LED0;	
		}
        vTaskDelay(10); //延时10ms，也就是10个时钟节拍
	}
}

//周期定时器的回调函数
void AutoReloadCallback(TimerHandle_t xTimer)
{
	static u8 tmr1_num=0;
	tmr1_num++;									//周期定时器执行次数加1
	LCD_ShowxNum(70,111,tmr1_num,3,16,0x80); 	//显示周期定时器的执行次数
	LCD_Fill(6,131,114,313,lcd_discolor[tmr1_num%14]);//填充区域
}

//单次定时器的回调函数
void OneShotCallback(TimerHandle_t xTimer)
{
	static u8 tmr2_num = 0;
	tmr2_num++;		//周期定时器执行次数加1
	LCD_ShowxNum(190,111,tmr2_num,3,16,0x80);  //显示单次定时器执行次数
	LCD_Fill(126,131,233,313,lcd_discolor[tmr2_num%14]); //填充区域
	LED1=!LED1;
    printf("定时器2运行结束\r\n");
}

