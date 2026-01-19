#ifndef _TYPEDEF_H
#define _TYPEDEF_H
typedef int  s32;
typedef short s16;
typedef signed char  s8;
typedef unsigned int  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef enum 
{
	DISABLE = 0,
	ENABLE = !DISABLE
} FunctionalState;

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

#endif
