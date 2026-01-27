
#ifndef __BSP_MBHOST_H
#define __BSP_MBHOST_H

#include "sys.h"
#include "usart.h"
#include "delay.h"

#define MB_SLAVEADDR        0x0001 //从机地址
#define MB_REG_ADDR         0x0001 //寄存器地址（离散、线圈）
#define HoldingReg          0x0010 //保持寄存器
#define InputRegReg         0x0020 //输入寄存器

#define TIME_OVERRUN        100    // 定义超时时间 ms

#define COIL1_ADDR          0x01
#define COIL2_ADDR          0x02
#define COIL3_ADDR          0x03
#define COIL4_ADDR          0x04
#define COIL5_ADDR          0x05

#define FUNCTION_CODE_01H   0x01
#define FUNCTION_CODE_02H   0x02
#define FUNCTION_CODE_03H   0x03
#define FUNCTION_CODE_04H   0x04
#define FUNCTION_CODE_05H   0x05
#define FUNCTION_CODE_06H   0x06

typedef struct
{
    uint16_t DATA_01H;
    uint16_t DATA_02H;
    uint16_t DATA_03H;
    uint16_t DATA_04H;
    uint16_t DATA_05H;
    uint16_t DATA_06H;
    uint8_t DATA_10H[64];
}REG_DATA;

extern REG_DATA reg_value;

void WaitTimeOut(void);
uint16_t MB_CRC16(uint8_t *pushMsg,uint8_t usDataLen);
void MB_ReadCoil_01H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_WriteCoil_05H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadInput_02H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadInputReg_04H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadHoldingReg_03H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_WriteHoldingReg_06H(uint8_t _addr, uint16_t _reg, uint16_t _data);
void MB_WriteNumHoldingReg_10H(uint8_t _addr, uint16_t _reg, uint16_t _num,uint8_t *_databuf);

#endif 

