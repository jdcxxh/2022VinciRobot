#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "FreeRTOS.h"
#include "task.h"
/************************************************
 ALIENTEK 探索者STM32F407开发板 FreeRTOS实验4-1
 FreeRTOS中断测试-库函数版本
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
#define INTERRUPT_TASK_PRIO		2
//任务堆栈大小	
#define INTERRUPT_STK_SIZE 		256  
//任务句柄
TaskHandle_t INTERRUPTTask_Handler;
//任务函数
void interrupt_task(void *p_arg);

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);					//初始化延时函数
	uart_init(115200);     				//初始化串口
	LED_Init();		        			//初始化LED端口
	TIM3_Int_Init(10000-1,8400-1);		//初始化定时器3，定时器周期1S
	TIM5_Int_Init(10000-1,8400-1);		//初始化定时器5，定时器周期1S
	
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
    //创建中断测试任务
    xTaskCreate((TaskFunction_t )interrupt_task,  			//任务函数
                (const char*    )"interrupt_task", 			//任务名称
                (uint16_t       )INTERRUPT_STK_SIZE,		//任务堆栈大小
                (void*          )NULL,						//传递给任务函数的参数
                (UBaseType_t    )INTERRUPT_TASK_PRIO,		//任务优先级
                (TaskHandle_t*  )&INTERRUPTTask_Handler); 	//任务句柄
	vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//中断测试任务函数 
void interrupt_task(void *pvParameters)
{
	static u32 total_num=0;
    while(1)
    {
		total_num+=1;
		if(total_num==5) 
		{
			printf("关闭中断.............\r\n");
			portDISABLE_INTERRUPTS();				//关闭中断
			delay_xms(5000);						//延时5s
			printf("打开中断.............\r\n");	//打开中断
			portENABLE_INTERRUPTS();
		}
        LED0=~LED0;
        vTaskDelay(1000);
    }
}  


