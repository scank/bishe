#include "MBMasterFunc.h"
#include "mbcrc.h"
#include "ModbusCommon.h"

#define READ_HOLD_REG_ADDR_OFFSET_HI		2U
#define READ_HOLD_REG_ADDR_OFFSET_LO		3U

//receive code function
bool MF_REC_Readholdingreg(u16 *psndreg,u16 *pdatareg,Quene *pquenebuf)
{
	u16 i,n=0;
	u16 startaddr;

	startaddr=0;
	n=Pop_Quene(pquenebuf);
	pdatareg[startaddr++]=n/2;
	for(i=0;i<n;i++)
		{
			pdatareg[startaddr]=Pop_Quene(pquenebuf)<<8|(Pop_Quene(pquenebuf)&0xff);
			startaddr++;
			i=i+1;
		}
	return TRUE;
}

bool MF_REC_Wirtereg(u16 *psndreg,u16 *pdatareg,Quene *pquenebuf)
{
	u16 i,n=8;
	bool btemp=TRUE;
	for(i=2;i<n;i++)
		{
			if(Pop_Quene(pquenebuf)!=psndreg[i])
				{
					btemp=FALSE;
					break;
				}
		}
	return btemp;
}


bool MF_REC_WirteMulreg(u16 *psndreg,u16 *pdatareg,Quene *pquenebuf)
{
	u16 i,n=5;
	bool btemp=TRUE;
	for(i=2;i<n;i++)
		{
			if(Pop_Quene(pquenebuf)!=psndreg[i])
				{
					btemp=FALSE;
					break;
				}
		}
	return btemp;
}




bool MF_SND_Readholdingreg(MFsndRWREGstru *preadholdstru)
{
	u16 n=0,i=0;
	u16 crctemp=0;

	preadholdstru->psndreg[n++]=*preadholdstru->pslaveaddr;
	preadholdstru->psndreg[n++]=preadholdstru->funcode;
	preadholdstru->psndreg[n++]=preadholdstru->regaddr>>8;
	preadholdstru->psndreg[n++]=preadholdstru->regaddr&0x00ffu;
	preadholdstru->psndreg[n++]=preadholdstru->cntdata>>8;
	preadholdstru->psndreg[n++]=preadholdstru->cntdata&0x00ffu;

	crctemp=CRC16(preadholdstru->psndreg,n);
	preadholdstru->psndreg[n++]=crctemp&0x00ffu;
	preadholdstru->psndreg[n++]=crctemp>>8;
	for(i=0;i<n;i++)
		{
			if(Push_Quene(preadholdstru->pquene,preadholdstru->psndreg[i])==FALSE)
				return FALSE;
		}
		return TRUE;

}

bool MF_SND_Wirtereg(MFsndRWREGstru *pwirteregstru)
{
	u16 n=0,i=0;
	u16 crctemp=0;

	pwirteregstru->psndreg[n++]=*pwirteregstru->pslaveaddr;
	pwirteregstru->psndreg[n++]=pwirteregstru->funcode;
	pwirteregstru->psndreg[n++]=pwirteregstru->regaddr>>8;
	pwirteregstru->psndreg[n++]=pwirteregstru->regaddr&0x00ffu;
	pwirteregstru->psndreg[n++]=pwirteregstru->cntdata>>8;
	pwirteregstru->psndreg[n++]=pwirteregstru->cntdata&0x00ffu;

	crctemp=CRC16(pwirteregstru->psndreg,n);
	pwirteregstru->psndreg[n++]=crctemp&0x00ffu;
	pwirteregstru->psndreg[n++]=crctemp>>8;
	for(i=0;i<n;i++)
		{
			if(Push_Quene(pwirteregstru->pquene,pwirteregstru->psndreg[i])==FALSE)
				return FALSE;
		}
		return TRUE;
}

bool MF_SND_WirteMulreg(MFsndWMULREGstru *pwirtemulstru)
{
	u16 n=0;
	u16 i=0;
	u16 crctemp=0;
	if(pwirtemulstru->datacnt<MASTER_SENDREG_MAX_LEN-11)
		{

			pwirtemulstru->psndreg[n++]=*pwirtemulstru->pslaveaddr;
			pwirtemulstru->psndreg[n++]=pwirtemulstru->funcode;
			pwirtemulstru->psndreg[n++]=pwirtemulstru->regstartaddr>>8;
			pwirtemulstru->psndreg[n++]=pwirtemulstru->regstartaddr&0x00ffu;
			pwirtemulstru->psndreg[n++]=pwirtemulstru->regcnt>>8;
			pwirtemulstru->psndreg[n++]=pwirtemulstru->regcnt&0x00ffu;
			pwirtemulstru->psndreg[n++]=pwirtemulstru->datacnt>>8;
			pwirtemulstru->psndreg[n++]=pwirtemulstru->datacnt&0x00ffu;
			for(i=0;i<pwirtemulstru->datacnt;i++)
				{
					pwirtemulstru->psndreg[n++]=pwirtemulstru->pdatareg[pwirtemulstru->regstartaddr+i]>>8;
					pwirtemulstru->psndreg[n++]=pwirtemulstru->pdatareg[pwirtemulstru->regstartaddr+i]&0x00ffu;
				}
			crctemp=CRC16(pwirtemulstru->psndreg,n);
			pwirtemulstru->psndreg[n++]=crctemp&0x00ffu;
			pwirtemulstru->psndreg[n++]=crctemp>>8;

			for(i=0;i<n;i++)
				{
					if(Push_Quene(pwirtemulstru->pquene,pwirtemulstru->psndreg[i])==FALSE)
						return FALSE;
				}
				return TRUE;
		}
	else
		{
			return FALSE;
		}

}

