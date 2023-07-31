#include "my_usart.h"
uint16_t USART_RX_STA=0;       //接收状态标记	

uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲

 uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
 
/*************Printf重定向UART**************/
/*用途：
*使用printf将打印内容通过串口发送。 
*原理：
*将C语言库函数中的fputc 和fgetc函数重新定向到UART串口
*使用方法及注意事项：
*使用前，需要在main.c中添加头文件 "stdio.h""uart_printf.h" 如果是keil编译器，同时需要在选项卡中勾选“使用微库”
*
*默认使用USART1串口， 当需要使用其他串口的时候， 修改huart1 为目标串口， 
*建议使用CubeMX生成的代码，这样直接修改huart1后面的标号即可直接使用
******************************************/
///////////////////////////////////////////////////////////要在魔术棒里打开微库

/*****************  发送字符串（重新定向） **********************/
void Usart_SendString(uint8_t *str)
{
    unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&huart4,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到串口DEBUG_USART */
    HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, 1000);    
    
    return (ch);
}
 
///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
        
    int ch;
    HAL_UART_Receive(&huart4, (uint8_t *)&ch, 1, 1000);    
    return (ch);
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	#if find_road_moda
	if(huart->Instance==UART5)//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}

	}
	#endif
}



#include "stdint.h"
 
int32_t  Output=0;
void str2int(uint8_t* str, char flag, int32_t no, int32_t* Output)
{
	int32_t id_end, count, output;
	for (id_end = 0, count = 0; count != no; ++id_end)
	{
		if (str[id_end] == flag || str[id_end] == '\r' || str[id_end] == '\n' || str[id_end] == '\0')
			++count;
	}
	id_end -= 2;
	for (output = 0, count = 1; str[id_end] != flag && id_end >= 0; --id_end)
	{
		if (str[id_end] == '-')
			output *= -1;
		else if (str[id_end] == '+')
			;
		else
		{
			output += (str[id_end] - '0') * count;
			count *= 10;
		}
	}
	*Output = output;
}
