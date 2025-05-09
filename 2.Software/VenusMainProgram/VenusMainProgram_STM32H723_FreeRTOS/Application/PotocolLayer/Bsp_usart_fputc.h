/**
 * @file Bsp_usart_fputc.h
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief printf重映射
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef _BSP_USART_FPUTC_C_
#define _BSP_USART_FPUTC_C_

#include "stdio.h" 			///<添加头文件
#include "stm32h7xx_hal.h"	///<添加头文件

///选择串口发送数据的类型
#define HAL_USART			///<HAL普通串口发送数据
//#define HAL_USART_DMA		///<HAL DMA 串口发送数据
//#define N_HAL_USART 		///<标准库串口发送数据

#endif

