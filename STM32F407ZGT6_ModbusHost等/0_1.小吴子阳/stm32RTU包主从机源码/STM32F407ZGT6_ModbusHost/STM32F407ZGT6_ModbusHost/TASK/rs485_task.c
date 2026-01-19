
#include "rs485_task.h"
#include "log/bsp_log.h"
#include "led/bsp_led.h"
#include "rs485/bsp_rs485.h"
#include "mbhost/bsp_mbhost.h"
#include "TimBase/bsp_TiMbase.h"

void RS485_Error_Repose(u8 *data)
{
    uint16_t crc = 0;
    uint16_t TxCount = 0;

    RS485_TX_BUF[TxCount++] = data[0];		/* 从站地址 */
    RS485_TX_BUF[TxCount++] = data[1]|0x80; /* 功能码 */
    RS485_TX_BUF[TxCount++] = data[1];	    /* 寄存器地址 高字节 */

    crc = MB_CRC16((uint8_t*)&RS485_TX_BUF, TxCount);
    RS485_TX_BUF[TxCount++] = crc;	          /* crc 低字节 */
    RS485_TX_BUF[TxCount++] = crc>>8;		  /* crc 高字节 */
    RS485_Send_Data((uint8_t *)&RS485_TX_BUF, TxCount);
}

void rs485_process(u8 *data)
{
    switch(data[1]) //功能码选择
    {
        case FUNCTION_CODE_01H:
        {
            LOG_INFO("=======================FUNCTION_CODE_01H.\r\n");
        }break;
        case FUNCTION_CODE_02H:
        {
            LOG_INFO("=======================FUNCTION_CODE_02H.\r\n");
        }break;
        default:    
            break;
    }
}

void rs485_task(void)
{
    MB_WriteCoil_05H(MB_SLAVEADDR, COIL1_ADDR, reg_value.DATA_05H); 
//    MB_ReadCoil_01H(MB_SLAVEADDR, MB_REG_ADDR, 0x01); 
    WaitTimeOut();              //等待从机响应,等待时间大概为200ms
    if((RS485_COUNT&0x80))      //接收到完整的一帧数据
    {
        if(RS485_RX_BUF[0] != MB_SLAVEADDR) //从机地址错误
        {
            memset(RS485_RX_BUF, 0, sizeof(RS485_RX_BUF));
            RS485_COUNT = 0;
            return ;
        }
        uint16_t crc_s = 0;
        uint16_t crc_c = 0;
        uint16_t RxCount = RS485_COUNT&0x7F;
        crc_c = MB_CRC16((uint8_t*)&RS485_RX_BUF, RxCount-2);
        crc_s = ((RS485_RX_BUF[RxCount-1]<<8) | RS485_RX_BUF[RxCount-2]);
        if(crc_c == crc_s) //CRC校验
        {
            rs485_process(RS485_RX_BUF);
        }
        
        memset(RS485_RX_BUF, 0, sizeof(RS485_RX_BUF));
        RS485_COUNT = 0;
    }
}
