/**
 * @file Device_dev.h
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "MI_Motor_dev.h"
#include "DJI_Motor_dev.h"
#include "FS_IA6B_dev.h"
#include "ICM42688_dev.h"


extern PID_TypeDef Venus_pitch;
extern PID_TypeDef Venus_pitch_Speed;
extern PID_TypeDef Venus_pitch_AngelAccel;
 
extern PID_TypeDef Venus_Position;
extern PID_TypeDef Venus_Position_2;


extern PID_TypeDef Venus_roll;



void Device_Init(void);





