
#ifndef __BSP_LED_H
#define __BSP_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sys.h"

#define LED0 PEout(3)	//DS0
#define LED1 PEout(4)	//DS1	
#define LED2 PGout(9)	//DS2	

void LED_Init(void);

#ifdef __cplusplus
extern "C" {
#endif

#endif

