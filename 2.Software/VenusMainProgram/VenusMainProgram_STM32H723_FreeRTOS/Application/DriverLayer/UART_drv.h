/**
 * @file UART_drv.h
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stm32h7xx_hal.h"

#define USART_REC_LEN  			100  	//定义最大接收字节数 200
#define RXBUFFERSIZE   			1 		//缓存大小

extern uint8_t UART4_RX_BUF[USART_REC_LEN];    //接收缓冲,最大USART_REC_LEN个字节.
extern uint16_t UART4_RX_STA;              //接收状态标记
extern uint8_t aRxBuffer1[RXBUFFERSIZE];		//HAL库使用的串口接收缓冲

void UART_Init(void);
