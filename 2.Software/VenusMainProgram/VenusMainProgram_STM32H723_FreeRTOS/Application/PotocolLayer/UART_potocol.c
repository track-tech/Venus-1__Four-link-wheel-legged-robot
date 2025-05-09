

#include "stm32h7xx_hal.h"
#include "usart.h"
#include "UART_potocol.h"
#include "FS_IA6B_dev.h"
#include "UART_drv.h"


/**
 * @brief ���ڻص�-������SBUS���� ���޸�ΪDMA����
 * 
 * @param huart �⴮�ڽṹ��
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    
    //��ֵ�BUG����Ҫ�������ģʽ�����������CPU����ʹ��---�ѽ��������Ϊ���жϴ�����û������UART1�жϽ���  
	int i;
	while(huart->Instance == UART4) //����Ǵ���1 
	{
		
		UART4_RX_BUF[UART4_RX_STA] = aRxBuffer1[0];
		if (UART4_RX_STA == 0 && UART4_RX_BUF[UART4_RX_STA] != 0x0F) {
			HAL_UART_Receive_IT(&huart4, (uint8_t *)aRxBuffer1, RXBUFFERSIZE);
			break; //֡ͷ���ԣ�����
		}
		UART4_RX_STA++;
		if (UART4_RX_STA > USART_REC_LEN) UART4_RX_STA = 0;  ///�������ݴ���,���¿�ʼ����
		if (UART4_RX_BUF[0] == 0x0F && UART4_RX_BUF[24] == 0x00 && UART4_RX_STA == 25)	//������һ֡����
		{
			update_sbus(UART4_RX_BUF);
			for (i = 0; i<25; i++)		//��ջ�����
				UART4_RX_BUF[i] = 0;
			UART4_RX_STA = 0;
		}
		HAL_UART_Receive_IT(&huart4, (uint8_t *)aRxBuffer1, RXBUFFERSIZE);		
//		HAL_GPIO_TogglePin(RedLED_GPIO_Port, RedLED_Pin);
		break;
	}
}


