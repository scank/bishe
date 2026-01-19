#ifndef _MBMASTERFUNC_H
#define _MBMASTERFUNC_H
#include "stm32f10x.h"
#include "quene.h"

typedef struct sndRWREGstruct
{
	u16 *psndreg;
	u16 *pslaveaddr;
	u16 funcode;
	Quene *pquene;
	u16 regaddr;
	u16 cntdata;	
}MFsndRWREGstru;

typedef struct sndWMULREGstruct
{
	u16 *psndreg;
	u16 *pslaveaddr;
	u16 funcode;
	Quene *pquene;
	u16 regstartaddr;
	u16 regcnt;	
	u16 datacnt;
	u16 *pdatareg;
}MFsndWMULREGstru;

//receive function
bool MF_REC_Readholdingreg(u16 *psndreg,u16 *pdatareg,Quene *pquenebuf);
bool MF_REC_Wirtereg(u16 *psndreg,u16 *pdatareg,Quene *pquenebuf);
bool MF_REC_WirteMulreg(u16 *psndreg,u16 *pdatareg,Quene *pquenebuf);



bool MF_SND_Readholdingreg(MFsndRWREGstru *preadholdstru);
bool MF_SND_Wirtereg(MFsndRWREGstru *pwirteregstru);
bool MF_SND_WirteMulreg(MFsndWMULREGstru *pwirtemulstru);


#endif
