/**
 * @file Driver_drv.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Driver_drv.h"

void Driver_Init(void)
{
    GPIO_Init();
    TIM_Init();
    CAN_Init();
    UART_Init();
}


