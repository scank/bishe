#ifndef _QUENE_H
#define _QUENE_H
#include "stm32f10x.h"

typedef struct quene
{
	u16 *Parray;
	int Size;
	int Head;
	int Tail;
}Quene;


//Quene operation
int Quene_IsEmpty(Quene *Pquene);
int Quene_IsFull(Quene *Pquene);
int Push_Quene(Quene *Pquene,u16 byte);
int Pop_Quene(Quene *Pquene);
int Quene_Clear(Quene *Pquene);

#endif
