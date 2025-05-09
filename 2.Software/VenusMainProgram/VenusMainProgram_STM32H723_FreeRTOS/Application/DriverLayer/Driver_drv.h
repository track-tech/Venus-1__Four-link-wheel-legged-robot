/**
 * @file Driver_drv.h
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef __DRIVER_H
#define __DRIVER_H

#include "stm32h7xx_hal.h"
#include "stdbool.h"

#include "CAN_drv.h"
#include "GPIO_drv.h"
#include "IIC_drv.h"
#include "TIM_drv.h"
#include "UART_drv.h"

void Driver_Init(void);

#endif