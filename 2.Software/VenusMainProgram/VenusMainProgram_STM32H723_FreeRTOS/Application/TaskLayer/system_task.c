/**
 * @file system_task.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 系统任务
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "system_task.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"


void StartSystemTask(void *argument){

    while(1){

        BuzzerWarning();
		osDelay(2);

    }
    
}



