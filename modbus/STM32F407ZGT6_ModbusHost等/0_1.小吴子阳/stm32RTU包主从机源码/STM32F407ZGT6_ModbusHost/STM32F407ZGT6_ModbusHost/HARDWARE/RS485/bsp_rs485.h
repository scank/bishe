
#ifndef __BSP_RS485_H
#define __BSP_RS485_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"	 			
#include "usart.h"

#define RS485_TX_EN		PGout(6)	//485模式控制.0,接收;1,发送.

extern u8 RS485_RX_BUF[128]; 	    //接收缓冲,最大128个字节
extern u8 RS485_TX_BUF[128]; 	    //接收缓冲,最大128个字节
extern u8 RS485_COUNT;   		    //接收到的数据长度
													 
void RS485_init(u32 bound);
void RS485_Send_Data(u8 *buf, u16 len);

#ifdef __cplusplus
}
#endif

 
#endif	   
















