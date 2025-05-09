/**
 * @file Device_dev.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */



#include "fdcan.h"
#include "Device_dev.h"
#include "pid.h"
//#include "MahonyAHRS.h"

PID_TypeDef Venus_pitch;
PID_TypeDef Venus_pitch_Speed;
PID_TypeDef Venus_pitch_AngelAccel;

PID_TypeDef Venus_Position;
PID_TypeDef Venus_Position_2;

PID_TypeDef Venus_roll;



void RobotPID_Init(void)
{
	

//	void (*f_param_init)(struct _PID_TypeDef *pid,  //PID参数初始化
//				   PID_ID id,
//				   uint16_t maxOutput,
//				   uint16_t integralLimit,
//				   float deadband,
//				   uint16_t controlPeriod,
//					int16_t max_err,     
//					int16_t  target,
//				   float kp,
//				   float ki,
//				   float kd);	
		pid_init(&Venus_pitch);
		pid_init(&Venus_pitch_Speed);
		pid_init(&Venus_pitch_AngelAccel);

		pid_init(&Venus_roll);		//翻滚角

	
		//整体定位
		pid_init(&Venus_Position);
		pid_init(&Venus_Position_2);
	
	
//		Venus_pitch.f_param_init(&Venus_pitch, PID_Speed, /* maxOutput */16384, /* integralLimit */5000, /* deadband */10, /* controlPeriod */0, 
//									/* max_err */8000, /* target */6, /* kp */500, /* ki */0.0, /* kd */0);


//角度
		Venus_pitch.f_param_init(&Venus_pitch, PID_Position, /* maxOutput */16000, /* integralLimit */5000, /* deadband */0, /* controlPeriod */0, 
									/* max_err */0, /* target */-1.5, /* kp */3000*0.6, /* ki */0, /* kd */30000*0.6);


//轮速度
		Venus_pitch_Speed.f_param_init(&Venus_pitch_Speed, PID_Speed, /* maxOutput */16000, /* integralLimit */5000, /* deadband */0, /* controlPeriod */0, 
									/* max_err */0, /* target */0, /* kp */0.6, /* ki */0.003, /* kd */0);	

//角速度
//		Venus_pitch_Speed.f_param_init(&Venus_pitch_Speed, PID_Speed, /* maxOutput */15000, /* integralLimit */5000, /* deadband */0, /* controlPeriod */0, 
//									/* max_err */0, /* target */0, /* kp */20000, /* ki */0, /* kd */0);	

//角加速度
		Venus_pitch_AngelAccel.f_param_init(&Venus_pitch_AngelAccel, PID_Position, /* maxOutput */15000, /* integralLimit */20000, /* deadband */0, /* controlPeriod */0, 
									/* max_err */0, /* target */0, /* kp */1.1, /* ki */0, /* kd */0);
									


////定位用
////		Venus_Position.f_param_init(&Venus_Position, PID_Position, /* maxOutput */10, /* integralLimit */8000, /* deadband */0, /* controlPeriod */0, 
////									/* max_err */0, /* target */0, /* kp */0.0001, /* ki */0, /* kd */0.0009);

//////定位用-用于实现里程计和自适应重心-原
//		Venus_Position.f_param_init(&Venus_Position, PID_Position, /* maxOutput */15, /* integralLimit */8000, /* deadband */0, /* controlPeriod */0, 
//									/* max_err */0, /* target */0, /* kp */0.0015, /* ki */0, /* kd */0.05);

////定位用-用于实现里程计和自适应重心-原
		Venus_Position.f_param_init(&Venus_Position, PID_Position, /* maxOutput */450, /* integralLimit */8000, /* deadband */0, /* controlPeriod */0, 
									/* max_err */0, /* target */0, /* kp */10, /* ki */0, /* kd */5);


//		Venus_Position_2.f_param_init(&Venus_Position_2, PID_Position, /* maxOutput */1000, /* integralLimit */5000, /* deadband */0, /* controlPeriod */0, 
//									/* max_err */0, /* target */0, /* kp */1, /* ki */0, /* kd */0);

//横滚角稳定-悬挂

		Venus_roll.f_param_init(&Venus_roll, PID_Position, /* maxOutput */1, /* integralLimit */8000, /* deadband */0, /* controlPeriod */0, 
									/* max_err */0, /* target */-2, /* kp */0.05, /* ki */0, /* kd */0);



//转向环



}



void Device_Init(void)
{

    //小米电机初始化
    MI_motor_Init(&MI_Motor_1, &hfdcan1, 1);
    MI_motor_Init(&MI_Motor_2, &hfdcan1, 2);
	
	MI_motor_Enable(&MI_Motor_1);
	HAL_Delay(2);
	MI_motor_Enable(&MI_Motor_2);
    //M2006电机PID结构体初始化
    Motor_PID_Speed_Init();
		
    //IMU初始化
    IMU42688_Init();
//	Mahony_Init(1000);  //mahony姿态解算初始化

	RobotPID_Init();

}




