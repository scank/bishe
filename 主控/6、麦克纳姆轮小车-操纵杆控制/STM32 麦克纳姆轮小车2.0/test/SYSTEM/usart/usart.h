#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include <stdio.h>



void USART1_Config(u32 bound);
void USART2_Config(u32 bound);
void USART3_Config(u32 bound);
void USART_Send_Char(USART_TypeDef* USARTx,u8 c);     //×Ö½Ú
void USART_Send_string(USART_TypeDef* USARTx,unsigned char *str);  //×Ö·û´®
//printf("\r\n This is a USART1_printf demo %d \r\n",i);  //´®¿Ú1µÄ×Ö·û´®
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...); //×Ö·û´®

#endif /* __USART_H */
