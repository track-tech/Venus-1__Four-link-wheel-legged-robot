/**
 * @file control_task.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 外部控制任务
 * @version 0.1
 * @date 2024-03-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "control_task.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"
#include "Device_dev.h"
#include "Bsp_usart_fputc.h"

#include "Mahony.h" 
#include "monitor_task.h"



uint16_t count_jump = 0;
uint8_t test_flag = 0;
uint16_t last_value = 0;

Venus_MotionModeTypeDef Venus_MotionMode;
Venus_ContorlTypeDef Venus_Contorl;

void Venus_Jump_Motion();
void Venus_Control(Venus_ContorlTypeDef* Venus_Contorl);

uint8_t keyreset = 0;

void StartControlTask(void *argument)
{
	osDelay(1000);
	
	while (1)
	{

		Venus_Control(&Venus_Contorl);
		
		if(Control_Map.SWB == 1 && Venus_MotionMode.jump == 0 && keyreset == 1){
			
			keyreset = 0;
			Venus_pitch.target = -9.5;
			while(IMU_Angle[1] >= -9.5)		osDelay(2);

			Venus_MotionMode.jump = 1;
			Venus_Jump_Motion();
			Venus_MotionMode.jump = 0;

		}
		else if(Control_Map.SWB == 0){
		
			Venus_MotionMode.jump = 0;
			keyreset = 1;
			
		}
				
		osDelay(5);
	}
}


/**
 * @brief 跳跃测试函数
 * 
 */
void Venus_Jump_Motion()
{
	static uint16_t count_jump_inside = 0;
	

		MI_motor_LocationControl(&MI_Motor_1,  0.2, 40, 2);
		MI_motor_LocationControl(&MI_Motor_2, -0.2, 40, 2);
		while(MI_Motor_1.RxCAN_info.angle >= 0.22) 		osDelay(2);
;
	
	
		MI_motor_Control(&MI_Motor_1,  12,  0.55, 10, 150, 1);
		osDelay(2);
		MI_motor_Control(&MI_Motor_2, -12, -0.55, 10, 150, 1);	
	
		while(MI_Motor_1.RxCAN_info.angle <= 0.54)		osDelay(2);

		MI_motor_LocationControl(&MI_Motor_1,  0.22, 200, 2);
		osDelay(2);
		MI_motor_LocationControl(&MI_Motor_2, -0.22, 200, 2);

		while(MI_Motor_1.RxCAN_info.angle >= 0.23)		osDelay(2);

		Venus_pitch.target = -1.5;

		MI_motor_LocationControl(&MI_Motor_1,  0.4, 30, 2);
		MI_motor_LocationControl(&MI_Motor_2, -0.4, 30, 2);
		
		
		for(int i = 0;i<3;i++){
			osDelay(2);
			MI_motor_LocationControl(&MI_Motor_1,  0.32, 40, 2);
			osDelay(2);
			MI_motor_LocationControl(&MI_Motor_2, -0.32, 40, 2);	
		
		}
		
}


void Venus_Control(Venus_ContorlTypeDef* Venus_Contorl)
{
	
	if(SBUS_CH.CH1 >= 1024 ){
		
		Venus_Contorl->veer = 7.82473*SBUS_CH.CH1 - 8012.52352;	
	}
	else{
		
		Venus_Contorl->veer = 8.19672*SBUS_CH.CH1 - 7311.47424;	
	}

	if(SBUS_CH.CH2 >= 1026 ){
		
		Venus_Contorl->speed = 13.33333*SBUS_CH.CH2 - 13653.32992;	
	}
	else if(SBUS_CH.CH2 <= 1000){
		
		Venus_Contorl->speed = 13.0719*SBUS_CH.CH2 - 13071.9;	
	}
	else{
		Venus_Contorl->speed = 0;
	
	}

}
