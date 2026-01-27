#include "ROS.h"

// 全局数据包
ROS_TX_Packet_t ros_tx_packet;
ROS_RX_Packet_t ros_rx_packet;

u8 ros_rx_state = 0;
u8 ros_rx_buffer[20];
u8 ros_rx_index = 0;
u8 ros_rx_complete = 0;

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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级3
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
函数名称：SendROSData(void)
函数功能：发送ROS数据到上位机
入口参数：无
返回参数：无
***************************************************/
void SendROSData(void)
{
	ros_tx_packet.header = 0x01;
	ros_tx_packet.footer = 0xFF;
	
	ros_tx_packet.acc_x = fAcc[0];
	ros_tx_packet.acc_y = fAcc[1];
	ros_tx_packet.acc_z = fAcc[2];
	ros_tx_packet.roll = fAngle[0];
	ros_tx_packet.pitch = fAngle[1];
	ros_tx_packet.yaw = fAngle[2];

	// 填充四轮实际转速（从MotorSpeed数组获取）
	ros_tx_packet.speed_fl = MotorSpeed[0];  // 前左
	ros_tx_packet.speed_fr = MotorSpeed[1];  // 前右
	ros_tx_packet.speed_bl = MotorSpeed[2];  // 后左
	ros_tx_packet.speed_br = MotorSpeed[3];  // 后右
	USART1_Send_nByte((uint8_t*)&ros_tx_packet, sizeof(ROS_TX_Packet_t));
}

/**************************************************
函数名称：Process_RX_Packet(void)
函数功能：接收ROS控制指令
入口参数：data为上位机发送回来的数据，len为长度
返回参数：无
***************************************************/
// 处理接收到的数据包
void Process_RX_Packet(void)
{
    if(ros_rx_complete)
    {
        ros_rx_complete = 0;
        ros_rx_state = 0;
        ros_rx_index = 0;
		
		ros_rx_packet.target_fl = 0;
		ros_rx_packet.target_fr = 0;
		ros_rx_packet.target_bl = 0;
		ros_rx_packet.target_br = 0;
		
		ros_rx_packet.target_fl |= ((int32_t)ros_rx_buffer[1] << 0);
		ros_rx_packet.target_fl |= ((int32_t)ros_rx_buffer[2] << 8);
		ros_rx_packet.target_fl |= ((int32_t)ros_rx_buffer[3] << 16);
		ros_rx_packet.target_fl |= ((int32_t)ros_rx_buffer[4] << 24);
		
		ros_rx_packet.target_fr |= ((int32_t)ros_rx_buffer[5] << 0);
		ros_rx_packet.target_fr |= ((int32_t)ros_rx_buffer[6] << 8);
		ros_rx_packet.target_fr |= ((int32_t)ros_rx_buffer[7] << 16);
		ros_rx_packet.target_fr |= ((int32_t)ros_rx_buffer[8] << 24);
		
		ros_rx_packet.target_bl |= ((int32_t)ros_rx_buffer[9] << 0);
		ros_rx_packet.target_bl |= ((int32_t)ros_rx_buffer[10] << 8);
		ros_rx_packet.target_bl |= ((int32_t)ros_rx_buffer[11] << 16);
		ros_rx_packet.target_bl |= ((int32_t)ros_rx_buffer[12] << 24);
		
		ros_rx_packet.target_br |= ((int32_t)ros_rx_buffer[13] << 0);
		ros_rx_packet.target_br |= ((int32_t)ros_rx_buffer[14] << 8);
		ros_rx_packet.target_br |= ((int32_t)ros_rx_buffer[15] << 16);
		ros_rx_packet.target_br |= ((int32_t)ros_rx_buffer[16] << 24);
	
    }
}

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
		switch(ros_rx_state)
        {
			
            case 0: // 等待帧头
                if(temp == 0x01)
                {
                    ros_rx_buffer[0] = temp;
                    ros_rx_index = 1;
                    ros_rx_state = 1;  // 切换到接收数据状态
                }
                break;
                
            case 1: // 接收数据中
                if(ros_rx_index < 20)  // 防止缓冲区溢出
                {
                    ros_rx_buffer[ros_rx_index] = temp;
                    ros_rx_index++;
                    
                    // 检查是否收到完整数据包（18字节）
                    if(ros_rx_index >= 18)
                    {
                        // 检查帧尾
                        if(ros_rx_buffer[17] == 0xFF)
                        {
                            ros_rx_complete = 1;  // 设置完成标志
                            ros_rx_state = 0;     // 重置状态，准备接收下一帧
                        }
                        else
                        {
                            // 帧尾错误，重新开始
                            ros_rx_state = 0;
                            ros_rx_index = 0;
                        }
                    }
                }
                else
                {
                    // 缓冲区溢出，重新开始
                    ros_rx_state = 0;
                    ros_rx_index = 0;
                }
                break;
        }
			
	}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE); //清除标志位；
} 

