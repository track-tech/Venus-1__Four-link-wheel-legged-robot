/**
 * @file control_task.h
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stm32h7xx_hal.h"

typedef struct{

	uint8_t jump;
	uint8_t move;

}Venus_MotionModeTypeDef;

typedef struct{

	int veer;
	int speed;
	
}Venus_ContorlTypeDef;


extern Venus_MotionModeTypeDef Venus_MotionMode;

extern Venus_ContorlTypeDef Venus_Contorl;
