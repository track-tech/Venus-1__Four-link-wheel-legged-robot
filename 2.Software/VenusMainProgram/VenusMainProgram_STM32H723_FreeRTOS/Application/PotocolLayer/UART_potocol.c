

#include "stm32h7xx_hal.h"
#include "usart.h"
#include "UART_potocol.h"
#include "FS_IA6B_dev.h"
#include "UART_drv.h"


/**
 * @brief 串口回调-适用于SBUS接收 待修改为DMA处理
 * 
 * @param huart 库串口结构体
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    
    //奇怪的BUG，需要进入调试模式后多重启几遍CPU才能使用---已解决，是因为在中断处理那没有重启UART1中断接收  
	int i;
	while(huart->Instance == UART4) //如果是串口1 
	{
		
		UART4_RX_BUF[UART4_RX_STA] = aRxBuffer1[0];
		if (UART4_RX_STA == 0 && UART4_RX_BUF[UART4_RX_STA] != 0x0F) {
			HAL_UART_Receive_IT(&huart4, (uint8_t *)aRxBuffer1, RXBUFFERSIZE);
			break; //帧头不对，丢掉
		}
		UART4_RX_STA++;
		if (UART4_RX_STA > USART_REC_LEN) UART4_RX_STA = 0;  ///接收数据错误,重新开始接收
		if (UART4_RX_BUF[0] == 0x0F && UART4_RX_BUF[24] == 0x00 && UART4_RX_STA == 25)	//接受完一帧数据
		{
			update_sbus(UART4_RX_BUF);
			for (i = 0; i<25; i++)		//清空缓存区
				UART4_RX_BUF[i] = 0;
			UART4_RX_STA = 0;
		}
		HAL_UART_Receive_IT(&huart4, (uint8_t *)aRxBuffer1, RXBUFFERSIZE);		
//		HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
		break;
	}
}


