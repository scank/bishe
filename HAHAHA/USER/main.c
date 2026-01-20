#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "commonusart.h"	
#include "led.h"
#include "sys.h"
#include "ModbusMaster.h"
#include "Comunication.h"
#include "SystemInit.h"

#define READ_REG_COUNT 0x24  // 36个寄存器
void Process_Received_Data(void);
bool read_success;
u8 i;
int main(void)
{	
	
	System_Init();
	MB_SetMasterMode(MB_MODE_NOMAL);
	USART1_Init(115200);
	USART3_Init(115200);
	LED_Init();		
	TIM3_Int_Init(3599,19);
	while(1)
	{
		MB_MasterTask();
		if(flag_1 == 1)
		{
			//LED0 = 0;
			//printf("11");
			read_success  = MB_ReadHoldReg(0x01, 0x10, READ_REG_COUNT);
			if(read_success == 1)
			{
				printf("11");
			}
			else if(read_success == 0)
			{
				printf("00");
			}
			flag_1 = 0;
		}
		Process_Received_Data();
		
	}
}


// 处理接收到的数据函数
void Process_Received_Data(void)
{
	u16 data_buffer[READ_REG_COUNT + 1];  // +1用于存放数据数量
	u16 *p_data;
	u16 data_count;
	
	// 获取数据缓冲区地址和数据数量
	MB_GetBufAddr(&p_data, &data_count);
	
	if(data_count > 0)
	{
		printf("Received %d registers:\r\n", data_count);
		
		// 打印所有接收到的寄存器值
		for(i = 0; i < data_count; i++)
		{
			printf("Reg[0x%02X] = 0x%04X (%d)\r\n", 
				   0x10 + i,  // 寄存器地址
				   p_data[i + 1],  // 数据从索引1开始
				   p_data[i + 1]);
		}
		
	}
}
