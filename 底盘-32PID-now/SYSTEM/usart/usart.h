#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "motor.h"
#include "modbus.h"
extern float fAcc[3], fGyro[3], fAngle[3]; // 加速度 角速度 角度
extern float temp;       // 温度
extern u8 Imu_Data_flag;
extern u8 buf3[11];
void parseWT61HexData(u8 *buf);

void USART_SendByte(USART_TypeDef* USARTx,uint16_t Data);

//void USART1_Init(u32 bound);
void USART3_Init(u32 bound);
void USART4_Init(u32 bound);
void USART5_Init(u32 bound);

//void USART1_Send_Byte(u8 Data);
//void USART1_Send_nByte(u8 *Data, u16 size);
//void USART1_Send_Str(u8 *Data);

//void USART2_Send_Byte(u8 Data);
//void USART2_Send_nByte(u8 *Data, u16 size);
//void USART2_Send_Str(u8 *Data);

void USART3_Send_Byte(u8 Data);
void USART3_Send_nByte(u8 *Data, u16 size);
void USART3_Send_Str(u8 *Data);

void UART4_Send_Byte(u8 Data);
void UART4_Send_nByte(u8 *Data, u16 size);
void UART4_Send_Str(u8 *Data);

void UART5_Send_Byte(u8 Data);
void UART5_Send_nByte(u8 *Data, u16 size);
void UART5_Send_Str(u8 *Data);


#endif


