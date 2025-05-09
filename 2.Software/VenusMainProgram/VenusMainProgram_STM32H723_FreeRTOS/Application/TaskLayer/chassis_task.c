/**
 * @file chassis_task.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief �����˶�����
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */


//
#include "chassis_task.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"
#include "Device_dev.h"
#include "math.h"
//#include "MahonyAHRS.h" 

//ͬ������
#include "control_task.h"

//�㷨��
#include "Mahony.h" 
#include "monitor_task.h"

//DSP��ѧ��
#include "arm_math.h"
#include "arm_const_structs.h"


// void Test_Task_Init(void);
void Venus_BlanceCal(void);
void Venus_RollCal(void);
void Venus_Wheel_odometer(void);

void Venus_Jump_Motion(void);

#define Wheel_odometer 1	//��̼ƿ���


void StartChassisTask(void *argument)
{
	osDelay(1000);
	
    while(1){
		
		if(Control_Map.SWA == 1){
			if(MI_Motor_AutoMachineZero() == 1){ 
				Venus_BlanceCal();
//				Venus_RollCal();
				Venus_Wheel_odometer();

			}
		}		
		else{
			MOTOR_Speed_Contorl(0, 0, 0, 0); //�����ֵ��

		}
		
		osDelay(1);

    }

}




/**
 * @brief �Զ�У׼��е��㣨С�׵����
 * 
 */
uint8_t MI_Motor_AutoMachineZero(void)
{
	 static uint8_t count_1 = 0, count_2 = 0;
	 static uint8_t flag_1 = 0, flag_2 = 0;

	if( !(flag_1 == 1 && flag_2 == 1) ){
		
		MOTOR_Speed_Contorl(0, 0, 0, 0); //�����ֵ��
//motor1		
		if(flag_1 == 0){
			MI_motor_SpeedControl(&MI_Motor_1, -3, 3);
		}
		//�ж��Ƿ񵽴��е��λ		
		if(MI_Motor_1.RxCAN_info.torque < -7.5 && flag_1 == 0){
			osDelay(200);
			if(MI_Motor_1.RxCAN_info.torque < -7.5){
				for(uint8_t i =0;i<5;i++){
					MI_motor_SetMechPositionToZero(&MI_Motor_1);
					osDelay(2);			
				}
				
			}
			flag_1 = 1;
			
		}	
//		if(flag_1 == 1) MI_motor_LocationControl(&MI_Motor_1, 0.5, 40, 2);
		osDelay(2);
		
//motor2
		if(flag_2 == 0){
			MI_motor_SpeedControl(&MI_Motor_2, 3, 3);
		}
		//�ж��Ƿ񵽴��е��λ		
		if(MI_Motor_2.RxCAN_info.torque > 7.5 && flag_2 == 0){
			osDelay(200);
			if(MI_Motor_2.RxCAN_info.torque > 7.5 ){			
				for(uint8_t i =0;i<5;i++){
					MI_motor_SetMechPositionToZero(&MI_Motor_2);
					osDelay(2);
				}
			}
			flag_2 = 1;
		}	
		
		if(flag_1 == 1)  MI_motor_LocationControl(&MI_Motor_1,  0.45, 40, 2);
		osDelay(2);
		if(flag_2 == 1)  MI_motor_LocationControl(&MI_Motor_2, -0.45, 40, 2);
		osDelay(2);
		
		//�ϵ��ʼ�����Խ�
		for(int i=0; i<1; i++)
		{	
			get_total_angle(&moto_chassis[i]);	//�ֵ����ʼ���ǻ��
		}
		
		return 0;	
	}
	else
		return 1;
}



static uint16_t count;
static float last_Accel;
/**
 * @brief ����pitchƽ�Ᵽ�ּ��㲿��
 * 
 */
void Venus_BlanceCal(void)
{
		
	//�Ƕȵ���&Ť����ؼ��
	if(fabs(IMU_Angle[1]) < 60 && MI_Motor_1.RxCAN_info.torque > 1.1 && MI_Motor_2.RxCAN_info.torque < -1.1 && Venus_MotionMode.jump != 1){
		
		
		
//������������
//		Venus_pitch.f_cal_pid(&Venus_pitch, IMU_Angle[1]);    //�����趨ֵ����PID���㡣-ֱ����

//		Venus_pitch_Speed.target = Venus_pitch.output;
//		Venus_pitch_Speed.f_cal_pid(&Venus_pitch_Speed, moto_chassis[1].speed_rpm / 36);    //�ٶȻ�		
//		
//		Venus_pitch_AngelAccel.target = Venus_pitch_Speed.output;
//		
//		Venus_pitch_AngelAccel.f_cal_pid(&Venus_pitch_AngelAccel, AccelCorrected[0]);    //�Ǽ��ٶ�
//		last_Accel = AccelCorrected[0];
//				
//		Venus_Position.f_cal_pid(&Venus_Position, ((moto_chassis[0].total_angle - moto_chassis[1].total_angle)/2) / 36);    //λ��


//		Venus_pitch_Speed.target = 0;
		
		Venus_pitch_Speed.f_cal_pid(&Venus_pitch_Speed, moto_chassis[0].speed_rpm / 36);    //�ٶȻ�		
		Venus_pitch.target =  Venus_pitch_Speed.output;
		Venus_pitch.f_cal_pid(&Venus_pitch, IMU_Angle[1]);    //�����趨ֵ����PID���㡣-ֱ����

		
//		Venus_pitch_AngelAccel.target = Venus_pitch_Speed.output;
//		Venus_pitch_AngelAccel.f_cal_pid(&Venus_pitch_AngelAccel, AccelCorrected[0]);    //�Ǽ��ٶ�
//		last_Accel = AccelCorrected[0];
				
		Venus_Position.f_cal_pid(&Venus_Position, ((moto_chassis[0].total_angle - moto_chassis[1].total_angle)/2) / 36);    //λ��

//		if(Wheel_odometer  == 1 && Control_Map.SWD && count > 100) Venus_pitch.target =  -1 - Venus_Position.output - Venus_Contorl.speed*0.0003;
//		if(Wheel_odometer  == 1 && Control_Map.SWD && count > 100) Venus_pitch.target =  -1 - Venus_Position.output;

		if(Wheel_odometer  == 1 && Control_Map.SWD && count > 100) Venus_pitch_Speed.target = - Venus_Position.output;


		if(count < 110) count++;
		
		//ǿ�ȽǶȻ�ǰ��
		if(Control_Map.SWC == 1) Venus_pitch_Speed.target =  - Venus_Contorl.speed*0.001;
		


		//�Ƕ�+���ٶ� ����   -����ת��
//		MOTOR_Speed_Contorl(Venus_pitch_Speed.output + Venus_Contorl.veer , 
//											-(Venus_pitch_Speed.output) + Venus_Contorl.veer, 0, 0);

		MOTOR_Speed_Contorl(Venus_pitch.output + Venus_Contorl.veer , 
											-(Venus_pitch.output) + Venus_Contorl.veer, 0, 0);


	}
	else{
			MOTOR_Speed_Contorl(0, 0, 0, 0);
	} 

}

/**
 * @brief ����ƽ������
 * 
 */
#define POW(x) x*x

/**
 * @brief ����߶�ֱ�ӹ���
 * 
 * @param Motor_Angle �ؽ���ת��(������)
 * @return float VirtualHeight(����߶�)
 */
float Venus_HighVirtuallinkLength(float Motor_Angle)
{
	float VirtualHeight;

	VirtualHeight = (float)sqrt( POW(154.87) + POW(154.61) 
								- 2*154.87*154.61 * arm_cos_f32( 2*( Motor_Angle + 0.201586/*��е����ʼˮƽ�ǶȲ�*/ ) ) );

	return VirtualHeight;
}

/**
 * @brief roll�ȶ�����
 * 
 */
void Venus_RollCal(void)
{

	if(fabs(IMU_Angle[1]) < 15  && Venus_MotionMode.jump != 1 && (MI_Motor_1.RxCAN_info.torque > 1.5 || MI_Motor_2.RxCAN_info.torque < -1.5) ){

		
		Venus_roll.f_cal_pid(&Venus_roll, IMU_Angle[2]);    //�����趨ֵ����PID���㡣-ֱ����
		if(fabs(Venus_roll.output) > 0.44){
			if(Venus_roll.output > 0) Venus_roll.output = 0.44;
			else Venus_roll.output = -0.44;
		}
		if(IMU_Angle[2]<0){
			MI_motor_LocationControl(&MI_Motor_1, 0.45 + Venus_roll.output, 40, 2);
			MI_motor_LocationControl(&MI_Motor_2, -0.45 + Venus_roll.output, 40, 2);

		}
		else{
			MI_motor_LocationControl(&MI_Motor_2, -0.45 + Venus_roll.output, 40, 2);
			MI_motor_LocationControl(&MI_Motor_1, 0.45 + Venus_roll.output, 40, 2);

		}

	}
	
}

void Venus_Wheel_odometer(void)
{
	
	if(	 MI_Motor_1.RxCAN_info.torque < 2 && MI_Motor_2.RxCAN_info.torque > -2 || Venus_Contorl.veer != 0 || Venus_Contorl.speed != 0 ){

			Venus_Position.target =  ((moto_chassis[0].total_angle - moto_chassis[1].total_angle)/2) / 36 ;

		}

}
