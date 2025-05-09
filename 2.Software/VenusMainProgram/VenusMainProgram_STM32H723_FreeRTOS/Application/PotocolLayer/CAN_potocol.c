/**
 * @file CAN_potocol.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief CANЭ���
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "fdcan.h"
#include "CAN_drv.h"
#include "CAN_potocol.h"
#include "MI_Motor_dev.h"
#include "DJI_Motor_dev.h"


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
//	uint8_t i=0;
	uint8_t rxdata[8];
	uint8_t rxdata2[8];
    uint32_t Motor_Can_ID;
    
//	uint8_t DataAnalysis[8];
	static uint16_t TimeCount = 0;

	if(hfdcan == &hfdcan1){
        if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE)!=RESET)   //FIFO0�������ж�
        {
                //��ȡFIFO0�н��յ�������
                HAL_FDCAN_GetRxMessage(&hfdcan1,FDCAN_RX_FIFO0,&CAN_RxHeader_MI,rxdata2);
            if(CAN_RxHeader_MI.Identifier == 0x02800102)
                MI_motor_RxDecode(&MI_Motor_1.RxCAN_info, rxdata2);
            if(CAN_RxHeader_MI.Identifier == 0x02800201)
                MI_motor_RxDecode(&MI_Motor_2.RxCAN_info, rxdata2);
            
                HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
        }   
    }
    
	if(hfdcan == &hfdcan2){
        if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE)!=RESET)   //FIFO0�������ж�
        {
            //��ȡFIFO0�н��յ�������
            HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0,&fdcan2_RxHeader,rxdata);

    //			if(fdcan2_RxHeader.Identifier == 0x201)
    //			{
    //				if(++TimeCount >= 500)													//�Ե��1K�Ļش����ʣ�����պö���500ms��ӡһ��
    //				{
    //					TimeCount = 0;
    //					//printf("\r\n");
    ////					printf("ת�ӻ�е�Ƕ�:%d\r\nת��ת��:%d\r\nʵ�����ת��:%f\r\n\r\n", ((rxdata[0] << 8) + rxdata[1]),  
    ////																						((rxdata[2] << 8) + rxdata[3]),
    ////																						((rxdata[4] << 8) + rxdata[5])*5.f/16384.f);
    //					//����VOFA+��λ��Э��
    //					printf("MOTOR1: %d, %d, %f\n", ((rxdata[0] << 8) + rxdata[1]),  
    //												   ((rxdata[2] << 8) + rxdata[3]),
    //												   ((rxdata[4] << 8) + rxdata[5])*5.f/16384.f);					
    //				}
    //			}

                get_moto_measure(&moto_chassis[fdcan2_RxHeader.Identifier - CAN_2006Moto1_ID], rxdata);

                HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
        }   
    }    
    
}



