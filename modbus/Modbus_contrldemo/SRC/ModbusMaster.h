#ifndef _MODBUSMASTER_H
#define _MODBUSMASTER_H
#include "stm32f10x.h"
#include "ModbusCommon.h"
#include "quene.h"


typedef enum __MBEVENT
{
	MM_EVENT_NONE=0,
	MM_EVENT_REC_COMP,
	MM_EVENT_SND_READY,
	MM_EVENT_SND_SENDING,
	MM_EVENT_SND_COMP
}MBEVENT;



bool MB_ReadHoldReg(u16 uiSlaveAddr,u16 uiRegStartAddr,u16 uiRegCnt);
bool MB_WirteSingleReg(u16 uiSlaveAddr,u16 uiRegAddr,u16 uiData);
bool MB_WirteMultReg(u16 uiSlaveAddr,u16 uiRegStartAddr,u16 uiRegCnt,u16 uiDataCnt,u16 *puiDataReg);




void MB_GetRecData(u16 *puiDataReg,u16 uiRegStartAddr,u16 uiRegCnt);
void MB_GetBufAddr(u16 **ppuiBuf,u16 *puiBufCnt);

MBEVENT MB_GetMasterEvent(void);
void MB_SetMasterEvent(MBEVENT Evnet);

void MB_SetMasterErrCode(MBERRCODE Errcode);

MBERRCODE MB_GetMasterErrCode(void);
void  MB_SetMasterMode(MODEBUSMODE Mmode );
MODEBUSMODE MB_GetMasterMode(void);

void MB_MasterTask(void);

#endif
