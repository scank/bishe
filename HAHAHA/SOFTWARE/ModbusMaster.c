#include "ModbusMaster.h"
#include "MBMasterFunc.h"
#include "quene.h"
#include "MBMasterRtu.h"




typedef bool (*FChandler) (u16 *psreg,u16 *pdreg,Quene *pqbuf);

typedef struct __MMfunchandler
{
	u16 	funccode;
	FChandler	funchandler;
}MMfunchandler;


static MODEBUSMODE MasterMode=MB_MODE_NOMAL;//MB_MODE_TRANSIT;//

static u16 *pMBDataReg=(u16*)0;


static u16 uiDataReg[MASTER_DATAREG_MAX_LEN];


static u16 Sndreg[MASTER_SENDREG_MAX_LEN];
static u16 Slaveaddr=MB_SLAVE_ADRESS;

static MBERRCODE MBErrCode=MB_ERR_NONE;
static MBEVENT MBEvent=MM_EVENT_NONE;

static MFsndRWREGstru MMreadholdreg=
{
	Sndreg,
	&Slaveaddr,
	MB_FUNCCODE_READHOLDINGREG,
	(Quene*) 0 ,
	0,
	0
	
	
};

static MFsndRWREGstru MMwirtereg=
{
	Sndreg,
	&Slaveaddr,
	MB_FUNCCODE_WIRTEHOLDINGREG,
	(Quene*) 0 ,
	0,
	0
	
};

static MFsndWMULREGstru MMwirtemulreg=
{
	Sndreg,
	&Slaveaddr,
	MB_FUNCCODE_WIRTEHOLDINGREG_MUL,
	(Quene*) 0 ,
	0,
	0,
	0,
	(u16*)0

} ;

static MMfunchandler FCRecHANDLER[]=
{
		{MB_FUNCCODE_READHOLDINGREG,MF_REC_Readholdingreg},
		{MB_FUNCCODE_WIRTEHOLDINGREG,MF_REC_Wirtereg},
		{MB_FUNCCODE_WIRTEHOLDINGREG_MUL,MF_REC_WirteMulreg},
		{0,(void*)0}
			
};



static void MbRecComplete(void);
static void MbSndReady(void);

bool MB_ReadHoldReg(u16 uiSlaveAddr,u16 uiRegStartAddr,u16 uiRegCnt)
{
	Quene *pquene=(Quene*)0;
	
		if(MBEvent==MM_EVENT_NONE)
			{
	
				pquene=MBMR_GetSndbufaddr();
				
				
				if(pquene)
					{
						MMreadholdreg.pquene=pquene;
						*MMreadholdreg.pslaveaddr=uiSlaveAddr;
						MMreadholdreg.regaddr=uiRegStartAddr;
						MMreadholdreg.cntdata=uiRegCnt;
						
						pMBDataReg=uiDataReg;
						
						if(MF_SND_Readholdingreg(&MMreadholdreg))
							{
								MBErrCode=MB_ERR_NONE;
								MBEvent=MM_EVENT_SND_READY;
								return TRUE;
							}
						else
							return FALSE;
	
					}
				else
					{
						return FALSE;
					}
			}
		else
			{
				return FALSE;
			}

}

bool MB_WirteSingleReg(u16 uiSlaveAddr,u16 uiRegAddr,u16 uiData)
{
	Quene *pquene=(Quene*)0;
	
		if(MBEvent==MM_EVENT_NONE)
			{
				pquene=MBMR_GetSndbufaddr();
				
				if(pquene)
					{
						MMwirtereg.pquene=pquene;
						*MMwirtereg.pslaveaddr=uiSlaveAddr;
						MMwirtereg.regaddr=uiRegAddr;
						MMwirtereg.cntdata=uiData;
	
						
						if(MF_SND_Wirtereg(&MMwirtereg))
							{

								MBErrCode=MB_ERR_NONE;
								MBEvent=MM_EVENT_SND_READY;
								return TRUE;
							}
						else
							return FALSE;
	
					}
				else
					{
						return FALSE;
					}
			}
		else
			{
				return FALSE;
			}

}
bool MB_WirteMultReg(u16 uiSlaveAddr,u16 uiRegStartAddr,u16 uiRegCnt,u16 uiDataCnt,u16 *puiDataReg)
{
	Quene *pquene=(Quene*)0;
	
		if(MBEvent==MM_EVENT_NONE)
			{

				pquene=MBMR_GetSndbufaddr();
				
				if(pquene)
					{
						MMwirtemulreg.pquene=pquene;
						*MMwirtemulreg.pslaveaddr=uiSlaveAddr;
						MMwirtemulreg.regstartaddr=uiRegStartAddr;
						MMwirtemulreg.regcnt=uiRegCnt;
						MMwirtemulreg.datacnt=uiDataCnt;
						MMwirtemulreg.pdatareg=puiDataReg;
	
						
						if(MF_SND_WirteMulreg(&MMwirtemulreg))
							{

								MBErrCode=MB_ERR_NONE;
								MBEvent=MM_EVENT_SND_READY;
								return TRUE;
							}
						else
							return FALSE;
	
					}
				else
					{
						return FALSE;
					}
			}
		else
			{
				return FALSE;
			}

}




void MB_GetRecData(u16 *puiDataReg,u16 uiRegStartAddr,u16 uiRegCnt)
{
	u16 i=0;
	for(i=1;i<uiRegCnt+1;i++)
		{
			puiDataReg[uiRegStartAddr]=uiDataReg[i];
			uiRegStartAddr++;
		}
}

void MB_GetBufAddr(u16 **ppuiBuf,u16 *puiBufCnt)

{
	*ppuiBuf=uiDataReg;
	*puiBufCnt=uiDataReg[0];
}

static void MbRecComplete(void)
{
	Quene *pquene=(Quene*)0;
	u16		fcode=0;
	u16  n=0;

	pquene=MBMR_GetRecbufaddr();
	if(pquene)
		{
			if(MasterMode==MB_MODE_NOMAL)
				{
					if(Pop_Quene(pquene)==Slaveaddr)
						{
							fcode=Pop_Quene(pquene);
							if(fcode<MB_ERR_MASK)
								{
									while(FCRecHANDLER[n].funccode!=0 && FCRecHANDLER[n].funccode!=fcode)
										{
											n++;
										}
									if(FCRecHANDLER[n].funccode==fcode && FCRecHANDLER[n].funchandler!=0)
										{
											if(pMBDataReg)
												{
													if(FCRecHANDLER[n].funchandler(Sndreg,pMBDataReg,pquene))
														{
															MBErrCode=MB_ERR_NONE;
														}
													else
														{
															MBErrCode=MB_LOCAL_ERR_RETURNDATA_ERR;
														}
												}
											else
												{
													MBErrCode=MB_LOCAL_ERR_ILLEGAL_DATAREG_ADDR;
												}
										}
									else
										{
											MBErrCode=MB_LOCAL_ERR_ILLEGAL_FUNCTION_ADDR;
										}
								}
							else
								{
									MBErrCode=Pop_Quene(pquene);
								}
						}
					else
						{
							MBErrCode=MB_LOCAL_ERR_SLAVE_ADDR_ERR;
						}
				}
		
		}
	else
		{
			MBErrCode=MB_LOCAL_ERR_INVALID_BUF_DATA;
		}
	
} 

static void MbSndReady(void)
{

	MBMR_SetTxState(MR_TX_SENDING);
	USART2_RXTXSwitch(FALSE,TRUE);
	MBMR_SendFrame();

}

MBEVENT MB_GetMasterEvent(void)
{
	return MBEvent;
}

void MB_SetMasterEvent(MBEVENT Evnet)
{
	MBEvent=Evnet;
}

void MB_SetMasterErrCode(MBERRCODE Errcode)
{
	MBErrCode=Errcode;
}

MBERRCODE MB_GetMasterErrCode(void)
{
	return MBErrCode;
}

void  MB_SetMasterMode(MODEBUSMODE Mmode )
{
	MasterMode=Mmode;
	MBMR_SetRxState(MR_RX_INIT);
	MBMR_SetTxState(MR_TX_IDLE);
	USART2_RXTXSwitch(FALSE,TRUE);
	MBEvent=MM_EVENT_NONE;
	MBErrCode=MB_ERR_NONE;
}

MODEBUSMODE MB_GetMasterMode(void)
{
	return MasterMode;
}

void MB_MasterTask(void)
{
	switch(MBEvent)
		{
			case MM_EVENT_NONE:
				break;

			case MM_EVENT_REC_COMP:
				MbRecComplete();
				MBEvent=MM_EVENT_NONE;
				break;
	
			case MM_EVENT_SND_READY:
				MbSndReady();
				MBEvent=MM_EVENT_SND_SENDING;
				break;
				
			case MM_EVENT_SND_SENDING:
				MBMR_SetTxState(MR_TX_SENDING);
				break;	
			case MM_EVENT_SND_COMP:
				MBMR_SetTxState(MR_TX_IDLE);
				break;

			default:

				break;
		}
}


