
#include "rs485/bsp_rs485.h"	
	   
u8 RS485_TX_BUF[128];   //发送缓冲,最大128个字节.
u8 RS485_RX_BUF[128];   //接收缓冲,最大128个字节.
u8 RS485_COUNT = 0;     //接收到的数据长度
										 
void RS485_init(u32 bound)
{  	 
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟
    //串口2引脚复用映射
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2

    //USART2    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;  //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	    //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //初始化PA2，PA3
	
	//PG8推挽输出，485模式控制  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOG6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOG,&GPIO_InitStructure);               //初始化PG8
    RS485_TX_EN=0;				                        //初始化默认为接收模式	
	
    //USART2 初始化设置
    USART_InitStructure.USART_BaudRate = bound;                 //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART2, &USART_InitStructure);                       //初始化串口2
	
    USART_Cmd(USART2, ENABLE);                    //使能串口2	
    USART_ClearFlag(USART2, USART_FLAG_TC);       //清除中断标志
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接受中断
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启空闲中断

    //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	
}

//串口2接收中断服务函数
void USART2_IRQHandler(void)
{
	u8 res = 0;	    
    if((RS485_COUNT&0x80)==0)
    {
        if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
        {	 	
            res =(u8)USART_ReceiveData(USART2);         //(USART2->DR) 读取接收到的数据
            RS485_RX_BUF[RS485_COUNT++] = res;
        }  			
        else if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET) 
        {
            RS485_COUNT |= 0x80; //标志帧接收结束
            res = USART2->SR;
            res = USART2->DR;      //清除空闲中断标志
        }
    }
} 
	
void RS485_Send_Data(u8 *buf,u16 len)
{
	u8 i = 0;
	RS485_TX_EN = 1;	//设置为发送模式
    
  	for(i=0; i<len; i++)		
	{
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
        USART_SendData(USART2,buf[i]);                           //发送数据
	}	 
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	RS485_TX_EN = 0;	   //发送完设置为接收模式	
}






