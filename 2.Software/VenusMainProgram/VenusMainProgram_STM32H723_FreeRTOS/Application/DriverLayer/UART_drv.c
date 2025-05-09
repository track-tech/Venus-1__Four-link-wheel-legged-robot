/**
 * @file UART_drv.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "stm32h7xx_hal.h"
#include "usart.h"
#include "UART_drv.h"


uint8_t UART4_RX_BUF[USART_REC_LEN];    //接收缓冲,最大USART_REC_LEN个字节.
uint16_t UART4_RX_STA = 0;              //接收状态标记
uint8_t aRxBuffer1[RXBUFFERSIZE];		//HAL库使用的串口接收缓冲

void UART_Init(void)
{
    //开中断
    HAL_UART_Receive_IT(&huart4, (uint8_t *)aRxBuffer1, RXBUFFERSIZE);		
    
}



