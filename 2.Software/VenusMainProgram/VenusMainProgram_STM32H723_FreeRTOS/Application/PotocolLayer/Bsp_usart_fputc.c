/**
 * @file Bsp_usart_fputc.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief printf��ӳ��
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */



#include "Bsp_usart_fputc.h"

extern UART_HandleTypeDef huart1;

/**
 * @brief		��д�������,�ض���printf����������
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
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);///<��ͨ���ڷ�������
#endif
    
#ifdef HAL_USART_DMA
    HAL_UART_Transmit_DMA(&huart1,(uint8_t *)&ch,1);///<DMA���ڷ�������
#endif
    
#ifdef N_HAL_USART
    USART_SendData(USART1, (uint8_t) ch);///<��׼�⴮�ڷ�������
#endif
    
#ifdef N_HAL_USART
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}///<�ȴ��������
#else
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}///<�ȴ��������
#endif
    
    return ch;
}

/**
  * @brief		�ض���scanf����������
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
	HAL_UART_Receive(&huart1,&ch, 1, 0xffff);///<��ͨ���ڽ�������
#endif
    
#ifdef HAL_USART_DMA
    HAL_UART_Receive_DMA(&huart1,(uint8_t *)&ch,1);///<DMA���ڷ�������
#endif
    
#ifdef N_HAL_USART
   	USART_SendData(USART1, (uint8_t) ch);///<��׼�⴮�ڽ�������
#endif
   
#ifdef N_HAL_USART
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}///<�ȴ��������
#else
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}///<�ȴ��������
#endif
    
	return ch; 
}