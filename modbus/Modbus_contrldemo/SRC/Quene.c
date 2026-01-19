#include "Quene.h"


/*
********************************************************************************
** 函数名称 ： Quene_IsEmpty(Quene *Pquene)
** 函数功能 ： 判断队列是否空
** 输    入	： *Pquene:队列结构体指针
** 输    出	： 无
** 返    回	： true or false
********************************************************************************
*/

int Quene_IsEmpty(Quene *Pquene)
{
	if(Pquene->Head==Pquene->Tail)
		return 1;
	else
		return 0;

}
/*
********************************************************************************
** 函数名称 ： Quene_IsFull(Quene *Pquene)
** 函数功能 ： 判断队列是否满
** 输    入	： *Pquene:队列结构体指针
** 输    出	： 无
** 返    回	： true or false
********************************************************************************
*/


int Quene_IsFull(Quene *Pquene)
{
	int n;
	n=Pquene->Tail-Pquene->Head;
	
	if(n==-1||n==Pquene->Size-1)
		return 1;
	else
		return 0;
}
/*
********************************************************************************
** 函数名称 ： Push_Quene(Quene *Pquene,u16 byte)
** 函数功能 ： 队列入栈
** 输    入	： *Pquene:队列结构体指针，byte:待入栈数据
** 输    出	： 无
** 返    回	： true or false
********************************************************************************
*/


int Push_Quene(Quene *Pquene,u16 byte)
{
	if(!Quene_IsFull(Pquene))
		{
		Pquene->Parray[Pquene->Tail]=byte;
		Pquene->Tail++;
		if(Pquene->Tail>=Pquene->Size)
			Pquene->Tail=0;
		return 1;
		}
	else
		{
		return 0;
		}
}
/*
********************************************************************************
** 函数名称 ： Pop_Quene(Quene *Pquene)
** 函数功能 ： 队列出栈
** 输    入	： *Pquene:队列结构体指针
** 输    出	： 无
** 返    回	： 返回出栈数据
********************************************************************************
*/


int Pop_Quene(Quene *Pquene)
{
	u16 n=0;
	if(!Quene_IsEmpty(Pquene))
		{
		n=Pquene->Parray[Pquene->Head];
		Pquene->Head++;
		if(Pquene->Head>=Pquene->Size)
			Pquene->Head=0;
		return n;
		}
	else
		{
		return -1;
		}
}
/*
********************************************************************************
** 函数名称 ： Quene_Clear(Quene *Pquene)
** 函数功能 ： 队列清空
** 输    入	： *Pquene:队列结构体指针
** 输    出	： 无
** 返    回	： 返回true
********************************************************************************
*/


int Quene_Clear(Quene *Pquene)
{
	Pquene->Head=0;
	Pquene->Tail=0;
	return 1;
}

