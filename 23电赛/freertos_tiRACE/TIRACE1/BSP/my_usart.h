 
#ifndef _UART_PRINT_H
#define _UART_PRINT_H
 
#include "includes.h"
#include  "stdio.h"
 #include  "usart.h"
 	#define RXBUFFERSIZE   1 //缓存大小
	#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  
#ifndef _UART_PRINT_C
//放置需要被外部调用的uart.c文件中的全局变量\函数
 
#endif
extern UART_HandleTypeDef huart4;
 
void Usart_SendString(uint8_t *str);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
 void   str2int(uint8_t* str, char flag, int32_t no, int32_t* Output);
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目

 



#endif
