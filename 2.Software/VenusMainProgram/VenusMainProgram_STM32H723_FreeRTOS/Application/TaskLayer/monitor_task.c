/**
 * @file monitor_task.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 设备监控任务-用于处理传感器实时数据
 * @version 0.1
 * @date 2024-03-10
 * 
 
 * @copyright Copyright (c) 2024
 * 
 */

#include "monitor_task.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"
#include "Device_dev.h"
#include "Bsp_usart_fputc.h"
//#include "MahonyAHRS.h"                    
#include "Mahony.h"                    
                    

#define cheat 1  						//作弊模式 去掉较小的gyro值
#define correct_Time_define 1000    	//上电去0飘 1000次取平均
#define temp_times 300       			//探测温度阈值

//pid_type_def Temperature_PID={0};
float Temperature_PID_Para[3]={1600,50,40};
float H723_Temperature=0.f;
unsigned int adc_v;
float adcx;

float gyro[3], accel[3], temp; 			//陀螺仪原始值
float gyro_correct[3]={0};  			//0飘初始值
float RefTemp = 40;   					//Destination
float roll,pitch,yaw=0;					//欧拉角
uint8_t attitude_flag=2;
uint32_t correct_times=0;

float IMU_Data[4] = {1.0f, 0.0f, 0.0f, 0.0f};	//初始化
float IMU_Angle[3];  					//yaw pitch roll

void INS_Task(void);  					//1khz


void StartMonitorTask(void *argument)
{

    while(1){
        
		INS_Task();
		
        osDelay(1);
    }

}
                    
void INS_Task(void){

	    ICM42688_Update();

	
		MahonyFilter_6Axis(IMU_Data, 
									GyroCorrected[0], GyroCorrected[1], GyroCorrected[2],
												 AccelCorrected[0], AccelCorrected[1], AccelCorrected[2]);	  
        trans_quaternionToEulerAngle_simple(IMU_Data, IMU_Angle);

}


//static uint8_t first_mahony=0; 
//void INS_Task(void)  //1khz
//{
//    static uint32_t count = 0;
//        ICM42688_Update();
//		gyro[0] = GyroCorrected[0];	
//		gyro[1] = GyroCorrected[1];	
//		gyro[2] = GyroCorrected[2];	
//	
//		accel[0] = AccelCorrected[0];
//		accel[1] = AccelCorrected[1]; 
//		accel[2] = AccelCorrected[2]; 
//	

////		Mahony_update(GyroCorrected[0],GyroCorrected[1],GyroCorrected[2],AccelCorrected[0],AccelCorrected[1],AccelCorrected[2],0,0,0);
//    // ins update
//				if(first_mahony==0)
//				{
//					first_mahony++;
//					MahonyAHRSinit(accel[0],accel[1],accel[2],0,0,0);   //mahony上电快速初始化
//				}
//				if(attitude_flag==2)  //ekf的姿态解算
//				{
//					gyro[0]-=gyro_correct[0];   //减去陀螺仪0飘
//					gyro[1]-=gyro_correct[1];
//					gyro[2]-=gyro_correct[2];

////					gyro[0]+=gyro_correct[0];   //减去陀螺仪0飘
////					gyro[1]+=gyro_correct[1];
////					gyro[2]+=gyro_correct[2];
//					
//					#if cheat              //作弊 可以让yaw很稳定 去掉比较小的值
////						if(fabsf(gyro[2])<0.003f)
////							gyro[2]=0;
//						if(fabsf(gyro[2])<0.02)
//							gyro[2]=-0.0;						
////							gyro[2]=-0.00026;						

//					#endif
//					//===========================================================================
//						//ekf姿态解算部分
//					//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);
////					IMU_QuaternionEKF_Update(gyro[0],gyro[1],gyro[2],accel[0],accel[1],accel[2]);
//					//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET);
//					//===============================================================================	
//						
//					//=================================================================================
//					//mahony姿态解算部分
//					//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);
//					Mahony_update(gyro[0],gyro[1],gyro[2],accel[0],accel[1],accel[2],0,0,0);
//					Mahony_computeAngles(); //角度计算
//					//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET);
//					//=============================================================================
//					//ekf获取姿态角度函数
////					pitch=Get_Pitch(); //获得pitch
////					roll=Get_Roll();//获得roll
////					yaw=Get_Yaw();//获得yaw
//					//==============================================================================
//				}
//				else if(attitude_flag==1)   //状态1 开始1000次的陀螺仪0飘初始化
//				{
//						//gyro correct
//						gyro_correct[0]+=	gyro[0];
//						gyro_correct[1]+=	gyro[1];
//						gyro_correct[2]+=	gyro[2];
//						correct_times++;
//						if(correct_times>=correct_Time_define)
//						{
//							gyro_correct[0]/=correct_Time_define;
//							gyro_correct[1]/=correct_Time_define;
//							gyro_correct[2]/=correct_Time_define;
//							attitude_flag=2; //go to 2 state
//						}
//				}
//    

//}
//

                    
                    
                    