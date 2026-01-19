#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"

void USART1_ConfigurationParam(u32 baudrate,u16 databit,u16 stopbit,u16 parity);

void USART1_RXTXSwitch(bool rxsta,bool txsta);


u16 USART1_RecData(void);

void USART1_SendData(u16 data);

void USART1_IRQHandler(void);


#endif
