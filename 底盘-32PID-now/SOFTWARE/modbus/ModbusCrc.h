#ifndef __MODBUSCRC_H
#define __MODBUSCRC_H

#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "stdio.h"

unsigned int GetCRC16(u8 *puchMsg, u8 DataLen);
	
#endif
