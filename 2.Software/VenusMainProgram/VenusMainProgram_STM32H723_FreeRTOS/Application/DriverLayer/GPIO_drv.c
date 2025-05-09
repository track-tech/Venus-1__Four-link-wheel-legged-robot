/**
 * @file GPIO_drv.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
 
 
#include "stm32h7xx_hal.h" 
#include "GPIO_drv.h"
#include "gpio.h"


void GPIO_Init(void)
{
    HAL_GPIO_WritePin(ENPower5V_GPIO_Port, ENPower5V_Pin, GPIO_PIN_SET);
}



