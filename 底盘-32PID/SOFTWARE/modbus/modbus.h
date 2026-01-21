#ifndef __MODBUS_H
#define __MODBUS_H

#include "sys.h"
#include "stdio.h"
#include "delay.h"
#include "usart.h"
#include "Translation.h"

/* 485模式控制引脚设置 如需更换更改下面这几项即可 */
#define	RS485_MODE_PROT	GPIOD					//RS485模式控制端口
#define RS485_MODE_Pin	GPIO_Pin_7				//RS485模式控制引脚
#define RS485_MODE_RCC	RCC_APB2Periph_GPIOD	//RS485模式控制端口GPIO时钟
#define RS485_TX_EN		PDout(7)				//485模式控制.0,接收;1,发送
// 定义读取的最大寄存器数量
#define MAX_REGISTERS 36  // 对应0x24个寄存器

typedef enum {
    MODBUS_IDLE,      // 空闲状态
    MODBUS_SENDING,   // 发送中
    MODBUS_WAITING,   // 等待响应
    MODBUS_RECEIVING, // 接收中
} ModbusState_t;

typedef struct
{
    u8 Modbus_RX_BUF[100]; 		//modbus的接收数据缓冲区
	u8 Modbus_RX_Cnt;  	 		//modbus端口已经接收到的数据个数
	u8 Modbus_RX_Flag;	    	//modbus收到一帧数据的标志
	u8 RS485Busy;				// 主机发送数据后置1 接收到应答后清零

}MODBUS;



extern MODBUS modbus;
extern u16 HolidingRegister[MAX_REGISTERS];
extern u16 ReadDateVal;
extern u8 DataReady;

void USART2_Init(u32 bound);
void RS485IOInit(void);
void UartDriver(void);
void UartDriverMe(void);
void ParseModbusResponse(u8 *buf, u8 len);
void UartRxMonitor(u8 ms);
void RS485_RW_Opr(u8 ucAddr,u8 ucCmd,u16 ucReg,u16 uiDate);
unsigned int GetCRC16(u8 *puchMsg, u8 DataLen);

#endif
