#include "myprint.h"
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

 
 
 
/*****************  发送字符串（重新定向） **********************/
void Usart_SendString(uint8_t *str)
{
    unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&huart1,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}
///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到串口DEBUG_USART */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);    
    
    return (ch);
}
 
///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
        
    int ch;
    HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 1000);    
    return (ch);
}




//HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
