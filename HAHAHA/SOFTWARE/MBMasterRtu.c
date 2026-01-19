#include "MBMasterRtu.h"
#include "usart.h"
#include "MBMasterTimer.h"
#include "ModbusMaster.h"
#include "mbcrc.h"



#define MASTER_BUFFER_MAX_LEN	256U




static u16 MasterBuf[MASTER_BUFFER_MAX_LEN];


static Quene Mbmasterbuf=
{
	MasterBuf,
	MASTER_BUFFER_MAX_LEN,
	0,
	0
	
};



static MRRecsta Rxstate=MR_RX_IDLE;
static MRSendsta Txstate=MR_TX_IDLE;

MRRecsta MBMR_GetRxState(void)
{
	return Rxstate;
}
void MBMR_SetRxState(MRRecsta rxsta)
{
	Rxstate=rxsta;
}
MRSendsta MBMR_GetTxState(void)
{
	return Txstate;
}
void MBMR_SetTxState(MRSendsta txsta)
{
	Txstate=txsta;
}

Quene *MBMR_GetRecbufaddr(void)
{
	Quene *pq=&Mbmasterbuf;
	u16 crctemp;
	if(pq->Tail-2>=0)
		{
			crctemp=pq->Parray[pq->Tail-1]<<8 | (pq->Parray[pq->Tail-2]&0x00ffu);
		}
	else
		{	
			return 0;
		}
	if(CRC16(pq->Parray,pq->Tail-2)==crctemp)
		{
			return pq;
		}
	else
		{
			return 0;
		}
	
}

Quene *MBMR_GetSndbufaddr(void)
{
	Quene *pq=&Mbmasterbuf;

	Quene_Clear(pq);
	if(Quene_IsEmpty(pq))
		{
			return pq;
		}
	else
		{
			return 0;
		}
}


void MBMR_RecFrame(void)//called by usart1 rx interrupt
{
	Quene *pbufs;

	pbufs=&Mbmasterbuf;
	switch(Rxstate)
		{
			case MR_RX_IDLE:
				
				break;
			case MR_RX_INIT:
				Quene_Clear(pbufs);
				Push_Quene(pbufs,USART2_RecData());
				Rxstate=MR_RX_REC;
				MRtimer2Enable();
				break;
			case MR_RX_REC:
				if(!Quene_IsFull(pbufs))
					{
						Push_Quene(pbufs,USART2_RecData());
						
					}
				else
					{
						Rxstate=MR_RX_ERR;
					}
				MRtimer2Enable();
				break;
			case MR_RX_ERR:
				MRtimer2Enable();
				break;

			default:

				break;
		}
}


void MBMR_SendFrame(void)//called by usart1 tx interrupt
{
	Quene *pbufs;
	pbufs=&Mbmasterbuf;


	switch(Txstate)
		{
			case MR_TX_IDLE:

				break;
			
			case MR_TX_SENDING:
				
				if(!Quene_IsEmpty(pbufs))
					{
						USART2_SendData(Pop_Quene(pbufs));
					}
				else
					{
						
						
						MB_SetMasterEvent(MM_EVENT_SND_COMP);
						Txstate=MR_TX_IDLE;
						Rxstate=MR_RX_INIT;
						USART2_RXTXSwitch(TRUE,FALSE);
											
					}
				MRSofttimerEnable();
				break;
			

			default:

				break;
		}
}


void MBMR_timer2T35ISR(void)//called by timer2 interrupte
{
	
	
	if(Rxstate!=MR_RX_IDLE)
		{
			
			if(Rxstate==MR_RX_REC)
				{
					MB_SetMasterEvent(MM_EVENT_REC_COMP);
				}
			else
				{
					MB_SetMasterEvent(MM_EVENT_NONE);
				}
			Rxstate=MR_RX_IDLE;
			USART2_RXTXSwitch(FALSE,TRUE);
			
		}
	
	
	MRtimer2Disable();

}

void MBMR_ResponsesTimeoutISR(void)
{
	if(MB_GetMasterEvent()==MM_EVENT_SND_COMP)
		{
			MB_SetMasterErrCode(MB_LOCAL_ERR_RESPONSES_TIMEOUT);
			MB_SetMasterEvent(MM_EVENT_NONE);
			USART2_RXTXSwitch(FALSE,TRUE);
			
		}
	
	MRSofttimerDisable();
}
