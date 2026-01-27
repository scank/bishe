
#ifndef __BSP_LOG_H
#define __BSP_LOG_H

#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#ifndef DEBUG_INFO
#error "DEBUG_INFO undefined!"
#endif

#if(DEBUG_INFO&0x01)
    #define LOG_DEBUG(format,...) App_Printf("[%s:%05d][%s]"format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define LOG_DEBUG(format,...)
#endif

#if(DEBUG_INFO&0x02)
    #define LOG_INFO(format,...) printf("[%s:%05d][%s]"format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define LOG_INFO(format,...)
#endif

#if(DEBUG_INFO&0x04)
    #define LOG_WARNING(format,...) printf("[%s:%05d][%s]"format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define LOG_WARNING(format,...)
#endif

#if(DEBUG_INFO&0x08)
    #define LOG_ERR(format,...) printf("[%s:%05d][%s]"format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define LOG_ERR(format,...)
#endif

#if(DEBUG_INFO&0x10)
    #define LOG_FATAL(format,...) printf("[%s:%05d][%s]"format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define LOG_FATAL(format,...)
#endif

#if(DEBUG_INFO&0x20)
    #define LOG_PRINTF(format,...) printf("[%s:%05d][%s]"format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define LOG_PRINTF(format,...)
#endif

#endif

