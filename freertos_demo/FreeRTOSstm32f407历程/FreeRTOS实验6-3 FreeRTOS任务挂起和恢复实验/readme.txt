实验器材:
	探索者STM32F407开发板
	
实验目的:
	学习使用FreeRTOS的任务挂起函数vTaskSuspend()、任务级恢复函数vTaskResume()和
	中断级任务恢复函数xTaskResumeFromISR()。
	
硬件资源:
	1,DS0(连接在PF9)，DS1(连接在PF10上)
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面) 
	3,ALIENTEK 2.8/3.5/4.3/7寸LCD模块(仅支持MCU屏) 
	4,按键KEY0(PE4)/KEY1(PE3)/KEY2(PE2)/KEY_UP(PA0,也称之为WK_UP)	
	5,外部中断线4(KEY0键)。
	
实验现象:
	按下KEY_UP按键以后就会挂起任务task1_task，按下KEY1键的话就会恢复任务task1_task的运行。
	按下KEY2键会挂起任务task2_task,按下KEY0键会恢复任务task2_task()的运行。

注意事项:
	无. 

参考资料：STM32F407 FreeRTOS开发手册.pdf 第六章


-------------------------------------------------------------------------------------------

◆其他重要连接：
  开发板光盘资料下载地址（视频+文档+源码等）：http://www.openedv.com/posts/list/13912.htm


◆友情提示：如果您想以后及时免费的收到正点原子所有开发板资料更新增加通知，请关注微信公众平台：
 2种添加方法：（动动手提升您的学习效率，惊喜不断哦）
（1）打开微信->添加朋友->公众号->输入“正点原子”->点击关注
（2）打开微信->添加朋友->输入“alientek_stm32"->点击关注
 具体微信添加方法，请参考帖子：http://www.openedv.com/posts/list/45157.htm
 


						

						淘宝店铺： http://openedv.taobao.com
						           http://eboard.taobao.com
						公司网站：www.alientek.com
						技术论坛：www.openedv.com
                                                微信公众平台：正点原子
						电话：020-38271790
						传真：020-36773971
						广州市星翼电子科技有限公司
						正点原子@ALIENTEK
						     2016-11月