#ifndef __ROS_H
#define __ROS_H

#include "sys.h"
#include "led.h"
#include "string.h"
#include "modbus.h"
#include "Translation.h"

#pragma pack(push, 1)
// STM32发送给ROS的数据包
typedef struct {
    uint8_t header;     // 0x01
    
    float acc_x;        // X加速度
    float acc_y;        // Y加速度
    float acc_z;        // Z加速度
    float roll;         // 滚转角
    float pitch;        // 俯仰角
    float yaw;          // 偏航角
    int32_t speed_fl;   // 前左轮实际转速，RPM
    int32_t speed_fr;   // 前右轮实际转速，RPM
    int32_t speed_bl;   // 后左轮实际转速，RPM
    int32_t speed_br;   // 后右轮实际转速，RPM
    
    uint8_t footer;     // 0xFF
} ROS_TX_Packet_t;      // 总大小: 1 + 6*4 + 4*4 + 1 = 46字节

// ROS发送给STM32的数据包
typedef struct {
    int32_t target_fl;  		// 前左轮目标转速，RPM
    int32_t target_fr; 	 		// 前右轮目标转速，RPM
    int32_t target_bl;  		// 后左轮目标转速，RPM
    int32_t target_br;  		// 后右轮目标转速，RPM
} ROS_RX_Packet_t;      // 总大小: 1 + 4*4 + 1 = 18字节
#pragma pack(pop)

extern ROS_TX_Packet_t ros_tx_packet;
extern ROS_RX_Packet_t ros_rx_packet;

void USART1_Init(u32 bound);

void SendROSData(void);
void Process_RX_Packet(void);

void USART1_Send_Byte(u8 Data);
void USART1_Send_nByte(u8 *Data, u16 size);
void USART1_Send_Str(u8 *Data);



#endif
