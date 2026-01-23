#include "modbus.h"

MODBUS modbus;
/* 用于保存读命令获得的数据 */
u16 ReadDateVal = 0;
u16 HolidingRegister[MAX_REGISTERS];
u8 RegCount;                          // 实际读取的寄存器数量
u8 DataReady;                 // 数据接收完成标志
volatile u8 modbus_busy = 0;

/**************************************************
函数名称：USART2_Init(u32 bound)
函数功能：串口2初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void USART2_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //使能USART2时钟
	
	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 						//PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA.2

	//USART2_RX	  GPIOA.3初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;						  //PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化GPIOA.3  

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//初始化NVIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;								 //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_Even;        //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure);     //初始化串口2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART2, ENABLE);                    //使能串口2 
}

/*
 * 函数名称： void RS485IOInit(void)
 * 入口参数：void
 * 返回  值：无
 * 功能说明：io口初始化，这里使用的是PD6
*/
void RS485IOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RS485_MODE_RCC, ENABLE);	 //使能PB,PE端口时钟
	
	GPIO_InitStructure.GPIO_Pin   = RS485_MODE_Pin;				//RS485模式控制引脚
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//翻转速度50MHz
	GPIO_Init(RS485_MODE_PROT, &GPIO_InitStructure);
	RS485_TX_EN;
}


/*
 * 函数名称： void Send_Data(u8 *buf,u8 len)
 * 入口参数：u8 *buf u8 len
 * 返回  值：无
 * 功能说明：串口发送数据				
 * 			 buf:发送区首地址
 *			 len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
*/
void Send_Data(u8 *buf,u8 len)
{
	u8 i;
	RS485_TX_EN=1;			//设置为发送模式
	delay_us(5);
	for(i=0;i<len;i++)		//循环发送数据
	{	
		USART_SendData(USART2,buf[i]);	    
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  		
		
	}
	delay_us(5);
	modbus.Modbus_RX_Cnt=0;	  
	RS485_TX_EN=0;				//设置为接收模式	
}

/*
 * 函数名称：u8 UartRead(u8 *buf, u8 len) 
 * 入口参数：u8 *buf u8 len
 * 返回  值：u8
 * 功能说明：计算接收的数据长度，并且将数据放到*buf数组中
*/             
u8 UartRead(u8 *buf, u8 len)  
{
	u8 i;
	if(len > modbus.Modbus_RX_Cnt)  		//指定读取长度大于实际接收到的数据长度时
	{
		len = modbus.Modbus_RX_Cnt; 		//读取长度设置为实际接收到的数据长度
	}
	
	for(i = 0;i < len;i ++)  				//拷贝接收到的数据到接收指针中
	{
		*buf = modbus.Modbus_RX_BUF[i];  	//将数据复制到buf中
		buf  ++;
	}
	modbus.Modbus_RX_Cnt = 0;              	//接收计数器清零
	return len;                   			//返回实际读取长度
}
/*
 * 函数名称：void UartRxMonitor(u8 ms)
 * 入口参数：u8 ms
 * 返回  值：无
 * 功能说明：在定时器中调用，用于监控数据接收,等待时间可调节
*/   
void UartRxMonitor(u8 ms) 					
{
	static u8 USART_RX_BKP = 0;  					//定义USART2_RC_BKP暂时存储诗句长度与实际长度比较
	static u8 idletmr      = 0;        				//定义监控时间
	
	if(modbus.Modbus_RX_Cnt > 0)					//接收计数器大于零时，监控总线空闲时间
	{
		if(USART_RX_BKP != modbus.Modbus_RX_Cnt) 	//接收计数器改变，即刚接收到数据时，清零空闲计时
		{
			USART_RX_BKP = modbus.Modbus_RX_Cnt;  	//赋值操作，将实际长度给USART2_RX_BKP
			idletmr      = 0;               		//将监控时间清零
		}
		else                              			//接收计数器未改变，即总线空闲时，累计空闲时间
		{
													//如果在一帧数据完成之前有超过3.5个字节时间的停顿，接收设备将刷新当前的消息并假定下一个字节是一个新的数据帧的开始
			if(idletmr < 5)                 		//空闲时间小于3ms时，持续累加
			{
				idletmr += ms;
				if(idletmr >= 5)            		//空闲时间达到3ms时，即判定为1帧接收完毕
				{
					modbus.Modbus_RX_Flag = 1;					//设置命令到达标志，帧接收完毕标志
				}
			}
		}
	}
	else
	{
		USART_RX_BKP = 0;
	}
}	

/****************************************************************************************************
 * 函数名称：void UartDriver(void)
 * 入口参数：void
 * 返回  值：无
 * 功能说明：串口驱动函数，检测数据帧的接收，调度功能函数，需在主循环中调用
 *           这里是主机，所以在功能调度函数里只需要得到读命令返回的数据，
 *           得到的数据根据自已的需要使用即可，这里是基于modbus协议，所以
 *           需要对照modbus协议去理解。计算不包含帧头
 ***************************************************************************************************/   
void UartDriver(void)
{
	unsigned int crc;
	unsigned char crch,crcl;
	static unsigned char len;
	static unsigned char buf[64];
	if (modbus.Modbus_RX_Flag == 1)            									//帧接收完成标志，即接收到一帧新数据
	{
		modbus.Modbus_RX_Flag = 0;           								//帧接收完成标志清零
		len       = UartRead(buf,sizeof(buf));   				//将接收到的命令读到缓冲区中
		crc       = GetCRC16(buf,len-2);       					//计算CRC校验值，除去CRC校验值
		crch=crc  >> 8;    										//crc高位
		crcl=crc  &  0xFF;										//crc低位

		if((buf[len-2] == crch) && (buf[len-1] == crcl))  		//判断CRC校验是否正确
		{
			if (buf[1] == 0x03)									//0x03 读命令
			{
				//if((buf[2] == 0x00) && (buf[3] <= 0x05))  		//寄存器地址支持0x0000~0x0005
				//{
					/* 通过上面的验证判断后 在这里可直接获取数据 保存在ReadDateVal中 */
					ReadDateVal = buf[3] * 256 + buf[4];		//modbus数据是16位的
					printf("数据为：%d",ReadDateVal);
				//}
			}
			/* 写命令不需要数据只需要应答即可 */
			
			/* 判断校验码正确后 无论是读还是写 都清零485忙标志，表示收到应答，释放485，可进行其它命令操作 */
			modbus.RS485Busy = 0;										
		}
		else	/* 如果校验值错误，执行错误处理函数 这里用串口1打印一条提示信息 也可自定义共它处理函数 */
			printf("接收出现错误\r\n");
	}
}

/****************************************************************************************************
 * 函数名称：void UartDriverMe(void)
 * 入口参数：void
 * 返回  值：无
 * 功能说明：串口驱动函数，检测数据帧的接收，调度功能函数，需在主循环中调用
 *           这里是主机，所以在功能调度函数里只需要得到读命令返回的数据，
 *           得到的数据根据自已的需要使用即可，这里是基于modbus协议，所以
 *           需要对照modbus协议去理解。这个可以全部接收到，想要的就自己拿
 ***************************************************************************************************/   
void UartDriverMe(void)
{
//	u8 i;
	unsigned int crc;
	unsigned char crch,crcl;
	static unsigned char len;
	static unsigned char buf[100];
	if (modbus.Modbus_RX_Flag == 1)            									//帧接收完成标志，即接收到一帧新数据
	{
		modbus.Modbus_RX_Flag = 0;   // 帧接收完成标志清零
		len = UartRead(buf, sizeof(buf));  // 将接收到的命令读到缓冲区中
		 
//      printf("\r\n收到Modbus响应(%d字节): ", len);// 调试输出
//		for(i = 0; i < len; i++) {
//            printf("%02X ", buf[i]);
//      }
//        printf("\r\n");
		
		crc       = GetCRC16(buf,len-2);       					//计算CRC校验值，除去CRC校验值
		crch=crc  >> 8;    										//crc高位
		crcl=crc  &  0xFF;										//crc低位
		if((buf[len-2] == crch) && (buf[len-1] == crcl))  // Modbus CRC低字节在前
        {
            //printf("CRC-gogogo\r\n");
            
            // 解析Modbus响应
			modbus.RS485Busy = 0;  // 释放总线
			modbus_busy = 0;
            ParseModbusResponse(buf, len);
        }
		else
        {
            printf("CRC-error now-CRC: %02X%02X, receive-CRC: %02X%02X\r\n", 
                   crch, crcl, buf[len-2], buf[len-1]);
        }
		
	}
}
/*
 * 函数名称：void ParseModbusResponse(u8 *buf, u8 len)
 * 入口参数：u8 *buf - 接收到的数据缓冲区
 *           u8 len - 数据长度
 * 返回  值：无
 * 功能说明：解析Modbus响应，存储寄存器数据
*/   
void ParseModbusResponse(u8 *buf, u8 len)
{
	u8 i;
	u8 data_index;
    u8 slave_addr = buf[0];     // 从机地址
    u8 func_code = buf[1];      // 功能码
    u8 byte_count = buf[2];     // 数据字节数
	//调试专用    
//    printf("addr: 0x%02X, ma: 0x%02X, count: %d\r\n", 
//           slave_addr, func_code, byte_count);
//	
//	for(i = 0; i < len; i++) {
//		printf("%02X ", buf[i]);
//	}
//	printf("\r\n");
    // 处理03功能码响应（读保持寄存器）
    if(func_code == 0x03)
    {
        // 检查数据长度是否匹配
        // 字节数 + 地址(1) + 功能码(1) + 字节数字段(1) + CRC(2) = 总长度
        if(len != (byte_count + 5)) {
            printf("data length not same! target: %d, fact: %d\r\n", byte_count+5, len);
            return;
        }
        
        // 计算寄存器数量（每个寄存器2字节）
		RegCount = byte_count / 2;
        
        if(RegCount > MAX_REGISTERS) {
            printf("count overflow max! receive: %d, max: %d\r\n", 
                   RegCount, MAX_REGISTERS);
            RegCount = MAX_REGISTERS;
        }
        
        // 提取并存储寄存器数据
        for(i = 0; i < RegCount; i++)
        {
            data_index = 3 + (i * 2);  // 数据从第3字节开始
            HolidingRegister[i] = (buf[data_index] << 8) | buf[data_index + 1];
            
        }
        
        // 设置数据就绪标志
        DataReady = 1;
        
    }
}




/*
 * 函数名称：void RS485_RW_Opr(u8 ucAddr,u8 ucCmd,u16 ucReg,u16 uiDate)
 * 入口参数：u8 ucAddr,u8 ucCmd,u16 ucReg,u16 uiDate
 * 			 ucAddr：从机地址
 *			 ucCmd ：功能码 03->读	06->写
 *			 ucReg ：寄存器地址
 *			 uiDate：写操作即是发送的数据 读操作即是读取数据个数
 * 返回  值：无
 * 功能说明：485读写操作函数
*/   
void RS485_RW_Opr(u8 ucAddr,u8 ucCmd,u16 ucReg,u16 uiDate)
{
	unsigned int crc;
	unsigned char crcl;
	unsigned char crch;	
	unsigned char ucBuf[8];
	
//	if(modbus_busy) return;  // 忙则退出
	modbus_busy = 1;
	
	ucBuf[0] = ucAddr;				/* 从机地址 */
	ucBuf[1] = ucCmd;				/* 命令 06 写 03 读 */
	ucBuf[2] = ucReg >> 8;			/* 寄存器地址高位 */
	ucBuf[3] = ucReg;				/* 寄存器地址低位 */
	ucBuf[4] = uiDate >> 8;			/* 数据高8位 */
	ucBuf[5] = uiDate;				/* 数据低8位 */
	crc      = GetCRC16(ucBuf,6);   /* 计算CRC校验值 */
	crch     = crc >> 8;    		/* crc高位 */
	crcl     = crc &  0xFF;			/* crc低位 */
	ucBuf[6] = crch;				/* 校验高8位 */
	ucBuf[7] = crcl;				/* 校验低8位 */
	
	Send_Data(ucBuf,8);				/* 发送数据 */
	modbus.RS485Busy = 1;					/* 发送完成后 忙标志置位 */
}

/*
 * 函数名称：void USART2_IRQHandler(void)
 * 入口参数：无
 * 返回  值：无
 * 功能说明：串口接收中断处理函数
*/   
void USART2_IRQHandler(void)
{
	unsigned char Temp;	    
 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 	//接收到数据
	{	 
		Temp = USART_ReceiveData(USART2); 					//读取接收到的数据
		USART_ClearITPendingBit(USART2, USART_IT_RXNE); //清除标志位；
		if(modbus.Modbus_RX_Cnt < 100)
		{
			modbus.Modbus_RX_BUF[modbus.Modbus_RX_Cnt] = Temp;			//记录接收到的值
			modbus.Modbus_RX_Cnt ++;								//接收数据增加1 
		} 
	}  											 
} 
/*
 * 函数名称：unsigned int GetCRC16(u8 *puchMsg, u8 DataLen)
 * 入口参数：无
 * 返回  值：无
 * 功能说明：嘻嘻，抄来的
*/ 
unsigned int GetCRC16(u8 *puchMsg, u8 DataLen)
{
	u16 index;
	u8 CRCH = 0xFF;//高八位
	u8 CRCL = 0xFF;//低八位
	const u8 TabH[] = 
		{
			0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
			0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
			0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
			0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
			0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
			0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
			0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
			0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
			0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
			0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
			0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
			0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
			0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
			0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
			0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
			0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
			0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
			0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
			0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
			0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
			0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
			0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
			0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
			0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
			0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
			0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
		};
	const u8 TabL[] =
		{
			0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
			0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
			0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
			0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
			0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
			0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
			0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
			0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
			0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
			0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
			0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
			0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
			0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
			0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
			0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
			0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
			0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
			0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
			0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
			0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
			0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
			0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
			0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
			0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
			0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
			0x43, 0x83, 0x41, 0x81, 0x80, 0x40
		};
	while(DataLen--)
	{
		index = CRCH^ *puchMsg++; // 计算CRC
        CRCH = CRCL ^ TabH[index];
        CRCL = TabL[index];
	}
	return ((CRCH << 8) | CRCL);
}





