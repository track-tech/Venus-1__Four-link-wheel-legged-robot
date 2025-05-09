/**
 * @file return_task.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 上位机传输任务
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "return_task.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"
#include "Device_dev.h"
#include "Bsp_usart_fputc.h"

#include "monitor_task.h"
#include "chassis_task.h"

#define SBUS_Monnitor 0
#define IMU_Monnitor 1


uint8_t RxCmd[100];

void StartReturnTask(void *argument)
{

	
	
    while(1){
		
		

		
#ifdef SBUS_Monnitor
		
			 printf("CH: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
					 SBUS_CH.CH1,SBUS_CH.CH2,SBUS_CH.CH3,SBUS_CH.CH4,SBUS_CH.CH5,
					 SBUS_CH.CH6,SBUS_CH.CH7,SBUS_CH.CH8,SBUS_CH.CH9,SBUS_CH.CH10,SBUS_CH.ConnectState);    
		
#endif
		
		
#ifdef SBUS_Monnitor
		
			printf("IMU:%f, %f, %f, %f\n", IMU_Angle[0], IMU_Angle[1], IMU_Angle[2], Temperature);
#endif

        osDelay(5);
    }

}


