#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"

void USART2_ConfigurationParam(u32 baudrate,u16 databit,u16 stopbit,u16 parity);

void USART2_RXTXSwitch(bool rxsta,bool txsta);


u16 USART2_RecData(void);

void USART2_SendData(u16 data);

void USART2_IRQHandler(void);


#endif
