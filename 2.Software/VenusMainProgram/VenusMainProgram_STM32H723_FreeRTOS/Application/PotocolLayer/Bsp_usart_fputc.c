/**
 * @file Bsp_usart_fputc.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief printf重映射
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */



#include "Bsp_usart_fputc.h"

extern UART_HandleTypeDef huart1;

/**
 * @brief		重写这个函数,重定向printf函数到串口
 * 
 * @param		None						
 * @retval		None
 * 
 * @attention	None		
 * 
 */
int fputc(int ch, FILE * f)
{
#ifdef HAL_USART
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);///<普通串口发送数据
#endif
    
#ifdef HAL_USART_DMA
    HAL_UART_Transmit_DMA(&huart1,(uint8_t *)&ch,1);///<DMA串口发送数据
#endif
    
#ifdef N_HAL_USART
    USART_SendData(USART1, (uint8_t) ch);///<标准库串口发送数据
#endif
    
#ifdef N_HAL_USART
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}///<等待发送完成
#else
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}///<等待发送完成
#endif
    
    return ch;
}

/**
  * @brief		重定向scanf函数到串口
  * 
  * @param		None						
  * @retval		None
  * 
  * @attention	None		
  * 
  */
int fgetc(FILE * F) 
{
	uint8_t ch = 0;
#ifdef HAL_USART
	HAL_UART_Receive(&huart1,&ch, 1, 0xffff);///<普通串口接收数据
#endif
    
#ifdef HAL_USART_DMA
    HAL_UART_Receive_DMA(&huart1,(uint8_t *)&ch,1);///<DMA串口发送数据
#endif
    
#ifdef N_HAL_USART
   	USART_SendData(USART1, (uint8_t) ch);///<标准库串口接收数据
#endif
   
#ifdef N_HAL_USART
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}///<等待发送完成
#else
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}///<等待发送完成
#endif
    
	return ch; 
}