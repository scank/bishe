#include "SystemInit.h"
#include "callbackfunction.h"
#include "MBmasterTimer.h"
#include "Timers.h"
//#include "SystemTimer.h"





void Modbus_Init(void)
{

	TIMER_ArrayInit();
	Timer1_Init();

	MRtimerInit();

	COM_Init();



}


