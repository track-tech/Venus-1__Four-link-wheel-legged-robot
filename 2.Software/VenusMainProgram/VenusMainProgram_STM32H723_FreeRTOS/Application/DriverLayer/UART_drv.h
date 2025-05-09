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

#define USART_REC_LEN  			100  	//�����������ֽ��� 200
#define RXBUFFERSIZE   			1 		//�����С

extern uint8_t UART4_RX_BUF[USART_REC_LEN];    //���ջ���,���USART_REC_LEN���ֽ�.
extern uint16_t UART4_RX_STA;              //����״̬���
extern uint8_t aRxBuffer1[RXBUFFERSIZE];		//HAL��ʹ�õĴ��ڽ��ջ���

void UART_Init(void);
