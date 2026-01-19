#include "sys.h"
#include "commonusart.h"	
#include "string.h"
u8 AxL,AxH,AyL,AyH,AzL,AzH;
u8 WxL,WxH,WyL,WyH,WzL,WzH;
u8 AnxL,AnxH,AnyL,AnyH,AnzL,AnzH;
int16_t myff;

float fAcc[3], fGyro[3], fAngle[3]; // 加速度 角速度 角度
float temp;       // 温度
u8 buf3[11]; // 临时缓冲区：存储串口接收的11字节原始十六进制数据

u8 cnt3 = 0; // 串口三接收计数

void USART_SendByte(USART_TypeDef* USARTx,uint16_t Data)//发送一个字节
{
	USARTx->DR=(Data & (uint16_t)0x01FF);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);
}	

/**************************************************
函数名称：USART1_Init(u32 bound)
函数功能：串口1初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void USART1_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 						//PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//初始化NVIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;								 //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure);     //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

/**************************************************
函数名称：USART3_Init(u32 bound)
函数功能：串口3初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void USART3_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //使能USART3时钟
	
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 						//PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);								//初始化GPIOB.10

	//USART3_RX	  GPIOB.11初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;						  //PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);								//初始化GPIOB.11  

	//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//初始化NVIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;								 //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure);     //初始化串口3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    //使能串口3 

}


/**************************************************
函数名称：USART4_Init(u32 bound)
函数功能：串口4初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void USART4_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); //使能USART4时钟
	
	//USART4_TX   GPIOC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 						//PC10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);								//初始化GPIOB.10

	//USART4_RX	  GPIOC.11初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;						  //PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);								//初始化GPIOB.11  

	//Usart4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//初始化NVIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;								 //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(UART4, &USART_InitStructure);     //初始化串口4
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(UART4, ENABLE);                    //使能串口4

}

/**************************************************
函数名称：USART5_Init(u32 bound)
函数功能：串口5初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void USART5_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);	//使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); //使能USART4时钟
	
	//USART5_TX   GPIOC.12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 						//PC12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);								//初始化GPIOC.12

	//USART5_RX	  GPIOD.2初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;						  //PD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);								//初始化GPIOD.2 

	//Usart5 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//初始化NVIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;								 //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(UART5, &USART_InitStructure);     //初始化串口5
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(UART5, ENABLE);                    //使能串口5

}

/**************************************************
函数名称：USART1_IRQHandler(void) 
函数功能：串口1中断函数-----ROS
入口参数：无
返回参数：无
***************************************************/
void USART1_IRQHandler(void)                	
{
	u8 temp;
	if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)
	{
		temp=USART_ReceiveData(USART1);
	}
}
	
void parseWT61HexData(u8 *buf)
{
	if(buf[0] != 0x55) return;
	if(buf[1] == 0x51) {
		AxH=buf[2];
		AxL=buf[3];
		myff=(AxH<<8)|AxL;
		fAcc[0]=myff/32768.0*16*9.8;

		AyH=buf[4];
		AyL=buf[5];
		myff=(AyH<<8)|AyL;
		fAcc[1]=myff/32768.0*16*9.8;

		AzH=buf[6];
		AzL=buf[7];
		myff=(AzH<<8)|AzL;
		fAcc[2]=myff/32768.0*16*9.8;
		
		temp = (int16_t)((buf[8]<<8)|buf[9])/100.0;
	}
	// else if(buf[1] == 0x52) {
	// 	// x
	// 	WxH=buf[2];
	// 	WxL=buf[3];
	// 	myff=(WxH<<8)|WxL;
	// 	fGyro[0]= myff/32768.0*2000;  
		
	// 	// y
	// 	WyH=buf[4];
	// 	WyL=buf[5];
	// 	myff=((WyH<<8)|WyL);
	// 	fGyro[1]=myff/32768.0*2000;  

	// 	// z
	// 	WzH=buf[6];
	// 	WzL=buf[7];
	// 	myff=((WzH<<8)|WzL);
	// 	fGyro[2]=myff/32768.0*2000; 
	// }
	else if(buf[1] == 0x53) {
		AnxH=buf[2];
		AnxL=buf[3];	
		myff=(AnxH<<8)|AnxL;
		fAngle[0]= myff/32768.0*180;  

		// y
		AnyH=buf[4];
		AnyL=buf[5];
		myff=((AnyH<<8)|AnyL);
		fAngle[1]=myff/32768.0*180; 

		// z
		AnzH=buf[6];
		AnzL=buf[7];
		myff=((AnzH<<8)|AnzL);
		fAngle[2]=myff/32768.0*180;  
	}
}
/**************************************************
函数名称：USART3_IRQHandler(void) 
函数功能：串口3中断函数----IMU
入口参数：无
返回参数：无
***************************************************/
void USART3_IRQHandler(void)                	
{
	u8 temp;
	if(USART_GetITStatus(USART3, USART_IT_RXNE)!=RESET)
	{
		temp=USART_ReceiveData(USART3);
		if (cnt3 == 0 && temp != 0x55) return;
		buf3[cnt3++] = temp;
		if (cnt3 == 11) {
		  parseWT61HexData(buf3);
		  cnt3 = 0; // 重置计数，准备接收下一帧
		}
		USART_ClearFlag(USART3, USART_IT_RXNE); //清除标志位；
	}
} 

/**************************************************
函数名称：UART4_IRQHandler(void) 
函数功能：串口4中断函数
入口参数：无
返回参数：无
***************************************************/
void UART4_IRQHandler(void) //中断处理函数；
{
	u8 temp;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET) //判断是否发生中断；
	{
		temp=USART_ReceiveData(UART4); //接收数据；
		USART_ClearFlag(UART4, USART_IT_RXNE); //清除标志位；
	} 
}

/**************************************************
函数名称：UART5_IRQHandler(void) 
函数功能：串口5中断函数
入口参数：无
返回参数：无
***************************************************/
void UART5_IRQHandler(void) //中断处理函数；
{
	u8 temp;    
	if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET) //判断是否发生中断；
	{
		USART_ClearFlag(UART5, USART_IT_RXNE); //清除标志位；
		temp=USART_ReceiveData(UART5); //接收数据；
	}  
	USART_ClearFlag(UART5, USART_IT_RXNE); //清除标志位；
}

/*************************串口发送函数*************************************/
/*
串口一
*/
void USART1_Send_Byte(u8 Data) {
	USART_SendData(USART1, Data);
	return;
}

void USART1_Send_nByte(u8 *Data, u16 size) {
	u16 i = 0;
	for(i=0; i<size; i++) {
		USART_SendData(USART1, Data[i]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
	}
	return;
}

void USART1_Send_Str(u8 *Data) {
	while(*Data) {
		USART_SendData(USART1, *Data++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
	}
	return;
}
/*
串口二
*/
//void USART2_Send_Byte(u8 Data) {
//	USART_SendData(USART2, Data);
//	return;
//}

//void USART2_Send_nByte(u8 *Data, u16 size) {
//	u16 i = 0;
//	for(i=0; i<size; i++) {
//		USART_SendData(USART2, Data[i]);
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
//	}
//	return;
//}

//void USART2_Send_Str(u8 *Data) {
//	while(*Data) {
//		USART_SendData(USART2, *Data++);
//		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
//	}
//	return;
//}
/*
串口三
*/
void USART3_Send_Byte(u8 Data) {
	USART_SendData(USART3, Data);
	return;
}

void USART3_Send_nByte(u8 *Data, u16 size) {
	u16 i = 0;
	for(i=0; i<size; i++) {
		USART_SendData(USART3, Data[i]);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
	}
	return;
}

void USART3_Send_Str(u8 *Data) {
	while(*Data) {
		USART_SendData(USART3, *Data++);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
	}
	return;
}
/*
串口四
*/
void UART4_Send_Byte(u8 Data)
{
	USART_SendData(UART4,Data);
	while( USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );
}

void UART4_Send_nByte(u8 *Data, u16 size) {
	u16 i = 0;
	for(i=0; i<size; i++) {
		USART_SendData(UART4, Data[i]);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET); 
	}
	return;
}

void UART4_Send_Str(u8 *Data) {
	while(*Data) {
		USART_SendData(UART4, *Data++);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET); 
	}
	return;
}
/*
串口五
*/
void UART5_Send_Byte(u8 Data)
{
	USART_SendData(UART5,Data);
	while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
}

void UART5_Send_nByte(u8 *Data, u16 size) {
	u16 i = 0;
	for(i=0; i<size; i++) {
		USART_SendData(UART5, Data[i]);
		while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET); 
	}
	return;
}

void UART5_Send_Str(u8 *Data) {
	while(*Data) {
		USART_SendData(UART5, *Data++);
		while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET); 
	}
	return;
}

/**************************************************
函数名称：fputc(int ch,FILE *f)
函数功能：串口重定向
入口参数：无
返回参数：无
***************************************************/
#pragma import(__use_no_semihosting)

struct __FILE
{
	int handle;
};

FILE __stdout;
void _sys_exit(int x)
{
	x=x;
}

int fputc(int ch,FILE *f)
{
	USART_SendData(USART1,(uint8_t) ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	return (ch);	
}

int fgetc(FILE *f)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
	return ((int)USART_ReceiveData(USART1));	
}


