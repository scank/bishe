#include "Comunication.h"
#include "Quene.h"
#include "USART.h"
#include "ModbusMaster.h"

static u32 BaudRate[8]={9600,19200,38400,56000,57600,115200,128000,256000};
static u16 ParityrAra[4]={USART_Parity_No,USART_Parity_Odd,USART_Parity_Even,USART_Parity_No};
static u16 DataBit[2]={USART_WordLength_9b,USART_WordLength_8b};
static u16 StopBit[2]={USART_StopBits_2,USART_StopBits_1};


void COM_Init(void)
{
	USART2_ConfigurationParam(BaudRate[5]\
								,DataBit[1]\
								,StopBit[0]\
								,ParityrAra[0]);

}


void COM_Task(void)
{
	
	MB_MasterTask();
		
}




