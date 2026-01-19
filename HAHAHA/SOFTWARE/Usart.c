#include "Usart.h"
#include "MBMasterRtu.h"




#define	DE1_H	    GPIO_SetBits(GPIOD, GPIO_Pin_7)	 	// PD7 高电平
#define	DE1_L	    GPIO_ResetBits(GPIOD, GPIO_Pin_7)	 // PD7 低电平


#define	USART2_RX_EN	DE1_H

#define	USART2_TX_EN	DE1_L



static void USART2_GPIOConfiguration(void);



/*
********************************************************************************
** 函数名称 ： USART1_GPIOConfiguration(void)
** 函数功能 ： 端口初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
static void USART2_GPIOConfiguration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD , ENABLE);
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 // 选中管脚9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	 // 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 最高输出速率50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				 // 选择A端口

	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			  //选中管脚10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  //选择A端口	

	/* Configure DE1 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; 		  
	GPIO_InitStructure.GPIO_Mode =	GPIO_Mode_Out_PP;		 // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   // 最高输出速率50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);				   // 选择C端口

	USART2_TX_EN ;
}



/*
********************************************************************************
** 函数名称 ： USART2_ConfigurationParam(u32 baudrate,u16 databit,u16 stopbit,u16 parity)
** 函数功能 ： 串口1初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void USART2_ConfigurationParam(u32 baudrate,u16 databit,u16 stopbit,u16 parity)

{

	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	
	USART2_GPIOConfiguration();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE  );



	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;			// 时钟低电平活动
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;				// 时钟低电平
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;				// 时钟第二个边沿进行数据捕获
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;		// 最后一位数据的时钟脉冲不从SCLK输出
	/* Configure the USART2 synchronous paramters */
	USART_ClockInit(USART2, &USART_ClockInitStructure);	

	USART_InitStructure.USART_BaudRate =baudrate;						  // 波特率为：115200
	USART_InitStructure.USART_WordLength =databit;			  // 8位数据
	USART_InitStructure.USART_StopBits = stopbit;				  // 在帧结尾传输1个停止位
	USART_InitStructure.USART_Parity =parity;				  // 奇偶失能


	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流控制失能

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // 发送使能+接收使能
	/* Configure USART2 basic and asynchronous paramters */
	USART_Init(USART2, &USART_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	  /* Enable USART2 */
	USART_ClearFlag(USART2, USART_IT_RXNE); 			//清中断，以免一启用中断后立即产生中断
	USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);		//使能USART2中断源
	USART_ITConfig(USART2,USART_IT_TC,ENABLE);
	USART_Cmd(USART2, ENABLE);	

}


u16 USART2_RecData(void)
{
	
	return USART2->DR;	

}



void USART2_SendData(u16 data)
{
	
	USART2_TX_EN;
	USART_SendData(USART2,data); 


}


void USART2_RXTXSwitch(bool rxsta,bool txsta)
{
	if(rxsta)
		{
			USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);		//使能USART1中断源
			USART2_RX_EN;
		}
	else
		{
			USART_ITConfig(USART2,USART_IT_RXNE, DISABLE);		//使能USART1中断源
			USART2_TX_EN;
		}

	if(txsta)
		{
			USART_ITConfig(USART2,USART_IT_TC, ENABLE);		//使能USART1中断源
			USART2_TX_EN;
		}
	else
		{
			USART_ITConfig(USART2,USART_IT_TC, DISABLE);		//使能USART1中断源
			USART2_RX_EN;
		}
		
}



/*
********************************************************************************
** 函数名称 ： USART2_IRQHandler(void)
** 函数功能 ： 串口1中断处理函数
** 输	 入 ： 无
** 输	 出 ： 无
** 返	 回 ： 无
********************************************************************************
*/
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{	
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		USART_ClearFlag(USART2, USART_IT_RXNE);
		MBMR_RecFrame();
	
	}
	
	if(USART_GetITStatus(USART2,USART_IT_TC)!=RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_TC);
		USART_ClearFlag(USART2, USART_IT_TC);
		MBMR_SendFrame();	
	}


}



