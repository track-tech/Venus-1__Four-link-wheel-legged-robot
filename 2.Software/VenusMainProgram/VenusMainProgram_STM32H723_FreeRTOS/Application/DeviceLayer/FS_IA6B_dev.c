/**
 * @file FS_IA6B_dev.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "FS_IA6B_dev.h"

SBUS_CH_Struct SBUS_CH;
Control_Map_struct Control_Map;

#define SBUS_TARGET_MIN 0
#define SBUS_RANGE_MIN 300
#define SBUS_SCALE_FACTOR 1.448f

#define ON  1
#define OFF 0
#define MIDDLE  2


/**
 * @brief 将sbus信号转化为通道值
 * 
 * @param buf 解析数组
 * @return uint8_t 
 */
uint8_t update_sbus(uint8_t *buf)
{
//    int i;
    if (buf[23] == 0)
    {
        SBUS_CH.ConnectState = 1;
        SBUS_CH.CH1 = ((int16_t)buf[ 1] >> 0 | ((int16_t)buf[ 2] << 8 )) & 0x07FF;
        SBUS_CH.CH2 = ((int16_t)buf[ 2] >> 3 | ((int16_t)buf[ 3] << 5 )) & 0x07FF;
        SBUS_CH.CH3 = ((int16_t)buf[ 3] >> 6 | ((int16_t)buf[ 4] << 2 ) | (int16_t)buf[ 5] << 10 ) & 0x07FF;
        SBUS_CH.CH4 = ((int16_t)buf[ 5] >> 1 | ((int16_t)buf[ 6] << 7 )) & 0x07FF;
        SBUS_CH.CH5 = ((int16_t)buf[ 6] >> 4 | ((int16_t)buf[ 7] << 4 )) & 0x07FF;
        SBUS_CH.CH6 = ((int16_t)buf[ 7] >> 7 | ((int16_t)buf[ 8] << 1 ) | (int16_t)buf[9] << 9 ) & 0x07FF;
        SBUS_CH.CH7 = ((int16_t)buf[ 9] >> 2 | ((int16_t)buf[10] << 6 )) & 0x07FF;
        SBUS_CH.CH8 = ((int16_t)buf[10] >> 5 | ((int16_t)buf[11] << 3 )) & 0x07FF;
        SBUS_CH.CH9 = ((int16_t)buf[12] << 0 | ((int16_t)buf[13] << 8 )) & 0x07FF;
        SBUS_CH.CH10 = ((int16_t)buf[13] >> 3 | ((int16_t)buf[14] << 5 )) & 0x07FF;
        SBUS_CH.CH11 = ((int16_t)buf[14] >> 6 | ((int16_t)buf[15] << 2 ) | (int16_t)buf[16] << 10 ) & 0x07FF;
        SBUS_CH.CH12 = ((int16_t)buf[16] >> 1 | ((int16_t)buf[17] << 7 )) & 0x07FF;
        SBUS_CH.CH13 = ((int16_t)buf[17] >> 4 | ((int16_t)buf[18] << 4 )) & 0x07FF;
        SBUS_CH.CH14 = ((int16_t)buf[18] >> 7 | ((int16_t)buf[19] << 1 ) | (int16_t)buf[20] << 9 ) & 0x07FF;
        SBUS_CH.CH15 = ((int16_t)buf[20] >> 2 | ((int16_t)buf[21] << 6 )) & 0x07FF;
        SBUS_CH.CH16 = ((int16_t)buf[21] >> 5 | ((int16_t)buf[22] << 3 )) & 0x07FF;

        SbusControlMap();       //映射转换

        return 1;
    }
    else 
    {
        SBUS_CH.ConnectState = 0;
        return 0;
    }
}

//-------------------用户映射区---------------------

uint16_t sbus_to_pwm(uint16_t sbus_value)
{
    float pwm;
    pwm = (float)SBUS_TARGET_MIN + (float)(sbus_value - SBUS_RANGE_MIN) * SBUS_SCALE_FACTOR;

    if (pwm > 1000) pwm = 1000;
    if (pwm < 0) pwm = 0;
    return (uint16_t)pwm;
}

uint16_t sbus_to_PwmServo(uint16_t sbus_value)
{
    float pwm;
    pwm = 0.12953*sbus_value + 20.98528;

    if (pwm > 250) pwm = 250;
    if (pwm < 50) pwm = 50;
    return (uint16_t)pwm;
}


/**
 * @brief 用于测试小米电机
 * 
 * @param sbus_value 
 * @return double 
 * 
 */

double sbus_to_MiMotor_Angle(uint16_t sbus_value)
{
    float pwm;
    pwm = 0.0013*sbus_value - 1.2977;
    return (double)pwm;
}

double sbus_to_MiMotor_Angle2(uint16_t sbus_value)
{
    float pwm;
//    pwm = 0.00131*sbus_value - 1.34977;
	pwm = 0.00023*sbus_value + 0.21379;
    return (double)pwm;
}
/**
 * @brief 用于映射遥控拨杆开关
 * 
 */
void SbusControlMap(){
    switch (SBUS_CH.CH5)
    {
    case 1799:
        Control_Map.SWA = ON;
        break;
    
    default:
        Control_Map.SWA = OFF;
        break;
    }

    switch (SBUS_CH.CH6)
    {
    case 1550:
        Control_Map.SWB = ON;
        break;

    default:
        Control_Map.SWB = OFF;
        break;
    }

    switch (SBUS_CH.CH7)
    {
    case 1024:
        Control_Map.SWC = MIDDLE;
        break;

    case 1807:
        Control_Map.SWC = ON;
        break;

    default:
        Control_Map.SWC = OFF;
        break;
    }

    switch (SBUS_CH.CH8)
    {
    case 779:
        Control_Map.SWD = ON;
        break;

    default:
        Control_Map.SWD = OFF;
        break;
    }

}





