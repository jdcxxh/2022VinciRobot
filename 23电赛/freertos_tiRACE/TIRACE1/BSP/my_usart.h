 
#ifndef _UART_PRINT_H
#define _UART_PRINT_H
 
#include "includes.h"
#include  "stdio.h"
 #include  "usart.h"
 	#define RXBUFFERSIZE   1 //�����С
	#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  
#ifndef _UART_PRINT_C
//������Ҫ���ⲿ���õ�uart.c�ļ��е�ȫ�ֱ���\����
 
#endif
extern UART_HandleTypeDef huart4;
 
void Usart_SendString(uint8_t *str);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
 void   str2int(uint8_t* str, char flag, int32_t no, int32_t* Output);
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ

 



#endif
