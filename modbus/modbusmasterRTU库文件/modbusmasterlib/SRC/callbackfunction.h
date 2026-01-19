#ifndef _CALLBACK_FUNCTION_H
#define _CALLBACK_FUNCTION_H
//#include "stm32f10x.h"
#include "typedef.h"

extern void TIMER_Servic_Timer1_ISR(void);//called by timer1 interrupt

extern void MBMR_RecFrame(void);//called by usart1 rx interrupt
extern void MBMR_SendFrame(void);//called by usart1 tx interrupt

extern void USART1_RXTXSwitch(bool rxsta,bool txsta);

extern u16 USART1_RecData(void);

extern void USART1_SendData(u16 data);
extern void COM_Init(void);

extern void TIMER_ArrayInit(void);
extern void Modbus_Init(void);

extern void Timer1_Init(void);

extern void TIM1_INTDisable(void);
extern void TIM1_INTEnable(void);

#endif

