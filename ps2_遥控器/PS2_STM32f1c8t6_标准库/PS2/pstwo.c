#include "pstwo.h"
#include "usart.h"

u16 Handkey;
u8 Comd[2]={0x01,0x42};	//开始命令。请求数据
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
u16 MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
	
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
	
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1,
	
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//按键值与按键明

//手柄接口初始化    输入  DI->PB12 
//                  输出  DO->PB13    CS->PB0  CLK->PB1
void PS2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTC时钟

	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;//PB12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB12设置成输入	默认下拉  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB12
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13	;//PB13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB13
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB0
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PB1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB1
	
}

//向手柄发送命令
void PS2_Cmd(u8 CMD)
{
	//volatile是易变型变量，是防止编译器优化代码时假设这个变量的值，保证每次小心地重新读取值。
	volatile u16 ref=0x01;
	Data[1] = 0;
	//ref的变化是一个八位二进制数中唯一一个1的位置变化，从最低位到最高位移动，从0000 0001到1000 0000。
	for(ref=0x01;ref<0x0100;ref<<=1)//
	{
	//当ref中1的位置对应CMD中得位置上也为1时，结果为1；当ref中1的位置对应CMD中得位置上为0时，结果为0。CMD的其他位则不影响此结果。
		if(ref&CMD)
		{
			DO_H;                   //输出以为控制位
		}
		else DO_L;
		//这个结果为1时，DO_H即输出1，这个结果为0时，DO_L即输出0。因此for循环八次，DO的结果就是将CMD的每一位传送了过去。
		
		CLK_H;                        //时钟拉高
		delay_us(50);
		CLK_L;
		delay_us(50);
		CLK_H;    //手动拉出一个下降沿使DO和DI得以同时传送
		/*
		接下来又对接收到的DI进行判断：当DI为1时，运用按位或操作，根据Data[1]初始值为0000 0000，
		以及按位或的定义，不难理解ref | Data[1]得到新的Data[1]的过程是：ref里的唯一的1以值不变
		位置不变的形式给到结果的二进制数中，比如某一次循环Data[1] = 0000 0010，ref = 0000 1000,
		且DI=1，则ref | Data[1]=0000 1010。而这个给1的操作，只有这一bit的DI=1时才会进行；若DI=0，
		则ref只进行1的移位，不给予，但其实也就相当于这一位ref是给予了0给Data[1]。所以其实判断DI并
		执行从句的这一步在整个for循环后的结果即是将8 bit的DI按位保存到Data[1]。		
		*/
		if(DI)
			Data[1] = ref|Data[1];//运用或运算按位存入Data[1]的8位
	}
}
//判断是否为红灯模式
//返回值；0，红灯模式
//		  其他，其他模式
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//读取手柄数据
void PS2_ReadData(void)
{
	//volatile是易变型变量，是防止编译器优化代码时假设这个变量的值，保证每次小心地重新读取值。
	volatile u8 byte=0;
	volatile u16 ref=0x01;

	CS_L;

	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据

	for(byte=2;byte<9;byte++)          //开始接受数据
	{
		for(ref=0x01;ref<0x0100;ref<<=1)
		{
			CLK_H;
			CLK_L;
			delay_us(50);
			CLK_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(50);
	}
	CS_H;	
}

//对读出来的PS2的数据进行处理      只处理了按键部分         默认数据是红灯模式  只有一个按键按下时
//按下为0， 未按下为1
u8 PS2_DataKey()
{
	u8 index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3]; //这是16个按键  按下为0， 未按下为1
	for(index=0;index<16;index++)//循环16次，看哪一个按键被按下
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //没有任何按键按下
}

//得到一个摇杆的模拟量	 范围0~256
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}




