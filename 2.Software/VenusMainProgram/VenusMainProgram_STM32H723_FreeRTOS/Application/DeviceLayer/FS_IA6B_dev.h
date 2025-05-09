/**
 * @file FS_IA6B_dev.h
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stm32h7xx_hal.h"

typedef struct
{
	uint16_t CH1;//通道1数值
	uint16_t CH2;//通道2数值
	uint16_t CH3;//通道3数值
	uint16_t CH4;//通道4数值
	uint16_t CH5;//通道5数值
	uint16_t CH6;//通道6数值
    uint16_t CH7;//通道7数值
    uint16_t CH8;//通道8数值
    uint16_t CH9;//通道9数值
    uint16_t CH10;//通道10数值
    uint16_t CH11;//通道11数值
    uint16_t CH12;//通道12数值
    uint16_t CH13;//通道13数值
    uint16_t CH14;//通道14数值
    uint16_t CH15;//通道15数值
    uint16_t CH16;//通道16数值
	uint8_t ConnectState;//遥控器与接收器连接状态 0=未连接，1=正常连接
}SBUS_CH_Struct;

extern SBUS_CH_Struct SBUS_CH;

uint8_t update_sbus(uint8_t *buf);

uint16_t sbus_to_pwm(uint16_t sbus_value);

uint16_t sbus_to_PwmServo(uint16_t sbus_value);

double sbus_to_MiMotor_Angle(uint16_t sbus_value);
double sbus_to_MiMotor_Angle2(uint16_t sbus_value);

/**
 * @brief 控制映射结构体(适用于FS-i6x)
 * 
 */
typedef struct 
{
    uint16_t L_UpDown;
    uint16_t L_LeftRight;
    uint16_t R_UpDown;
    uint16_t R_LeftRight;
    uint8_t SWA;
    uint8_t SWB;
    uint8_t SWC;
    uint8_t SWD;
    uint16_t VRR;
    uint16_t VRB;
}Control_Map_struct;

extern Control_Map_struct Control_Map;

void SbusControlMap();
