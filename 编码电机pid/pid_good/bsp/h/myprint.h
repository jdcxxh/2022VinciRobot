
 
#ifndef _UART_PRINT_H
#define _UART_PRINT_H
 
#include "INCLUDES.h"

 
 
#ifndef _UART_PRINT_C
//������Ҫ���ⲿ���õ�uart.c�ļ��е�ȫ�ֱ���\����
 
#endif
extern UART_HandleTypeDef huart1;
 
void Usart_SendString(uint8_t *str);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
 
 

#endif
