#ifndef _MBMASTER_RTU_H
#define _MBMASTER_RTU_H
#include "stm32f10x.h"
#include "quene.h"

typedef enum recstate
{
	MR_RX_IDLE=0,
	MR_RX_INIT,
	MR_RX_REC,
	MR_RX_ERR
}MRRecsta;

typedef enum sendstate
{
	MR_TX_IDLE=0,
	MR_TX_SENDING

}MRSendsta;

MRRecsta MBMR_GetRxState(void);
void MBMR_SetRxState(MRRecsta rxsta);
MRSendsta MBMR_GetTxState(void);
void MBMR_SetTxState(MRSendsta txsta);


void MBMR_RecFrame(void);//called by usart1 rx interrupt
void MBMR_SendFrame(void);//called by usart1 tx interrupt
void MBMR_timer2T35ISR(void);//called by timer2 interrupte
void MBMR_ResponsesTimeoutISR(void);


Quene *MBMR_GetRecbufaddr(void);
Quene *MBMR_GetSndbufaddr(void);


#endif
