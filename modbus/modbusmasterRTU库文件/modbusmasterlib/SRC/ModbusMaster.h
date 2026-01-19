#ifndef _MODBUSMASTER_H
#define _MODBUSMASTER_H

#include "typedef.h"
#include "ModbusCommon.h"



typedef enum __MBEVENT
{
	MM_EVENT_NONE=0,
	MM_EVENT_REC_COMP,
	MM_EVENT_SND_READY,
	MM_EVENT_SND_SENDING,
	MM_EVENT_SND_COMP
}MBEVENT;



extern bool MB_ReadHoldReg(u16 uiSlaveAddr,u16 uiRegStartAddr,u16 uiRegCnt);
extern bool MB_WirteSingleReg(u16 uiSlaveAddr,u16 uiRegAddr,u16 uiData);
extern bool MB_WirteMultReg(u16 uiSlaveAddr,u16 uiRegStartAddr,u16 uiRegCnt,u16 uiDataCnt,u16 *puiDataReg);




extern void MB_GetRecData(u16 *puiDataReg,u16 uiRegStartAddr,u16 uiRegCnt);
extern void MB_GetBufAddr(u16 **ppuiBuf,u16 *puiBufCnt);

extern MBEVENT MB_GetMasterEvent(void);
extern void MB_SetMasterEvent(MBEVENT Evnet);

extern void MB_SetMasterErrCode(MBERRCODE Errcode);

extern MBERRCODE MB_GetMasterErrCode(void);
extern void  MB_SetMasterMode(MODEBUSMODE Mmode );
extern MODEBUSMODE MB_GetMasterMode(void);

extern void MB_MasterTask(void);

#endif
