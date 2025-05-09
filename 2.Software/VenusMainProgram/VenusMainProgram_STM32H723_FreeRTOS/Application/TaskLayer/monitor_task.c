/**
 * @file monitor_task.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief �豸�������-���ڴ�������ʵʱ����
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
                    

#define cheat 1  						//����ģʽ ȥ����С��gyroֵ
#define correct_Time_define 1000    	//�ϵ�ȥ0Ʈ 1000��ȡƽ��
#define temp_times 300       			//̽���¶���ֵ

//pid_type_def Temperature_PID={0};
float Temperature_PID_Para[3]={1600,50,40};
float H723_Temperature=0.f;
unsigned int adc_v;
float adcx;

float gyro[3], accel[3], temp; 			//������ԭʼֵ
float gyro_correct[3]={0};  			//0Ʈ��ʼֵ
float RefTemp = 40;   					//Destination
float roll,pitch,yaw=0;					//ŷ����
uint8_t attitude_flag=2;
uint32_t correct_times=0;

float IMU_Data[4] = {1.0f, 0.0f, 0.0f, 0.0f};	//��ʼ��
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
//					MahonyAHRSinit(accel[0],accel[1],accel[2],0,0,0);   //mahony�ϵ���ٳ�ʼ��
//				}
//				if(attitude_flag==2)  //ekf����̬����
//				{
//					gyro[0]-=gyro_correct[0];   //��ȥ������0Ʈ
//					gyro[1]-=gyro_correct[1];
//					gyro[2]-=gyro_correct[2];

////					gyro[0]+=gyro_correct[0];   //��ȥ������0Ʈ
////					gyro[1]+=gyro_correct[1];
////					gyro[2]+=gyro_correct[2];
//					
//					#if cheat              //���� ������yaw���ȶ� ȥ���Ƚ�С��ֵ
////						if(fabsf(gyro[2])<0.003f)
////							gyro[2]=0;
//						if(fabsf(gyro[2])<0.02)
//							gyro[2]=-0.0;						
////							gyro[2]=-0.00026;						

//					#endif
//					//===========================================================================
//						//ekf��̬���㲿��
//					//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);
////					IMU_QuaternionEKF_Update(gyro[0],gyro[1],gyro[2],accel[0],accel[1],accel[2]);
//					//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET);
//					//===============================================================================	
//						
//					//=================================================================================
//					//mahony��̬���㲿��
//					//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);
//					Mahony_update(gyro[0],gyro[1],gyro[2],accel[0],accel[1],accel[2],0,0,0);
//					Mahony_computeAngles(); //�Ƕȼ���
//					//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET);
//					//=============================================================================
//					//ekf��ȡ��̬�ǶȺ���
////					pitch=Get_Pitch(); //���pitch
////					roll=Get_Roll();//���roll
////					yaw=Get_Yaw();//���yaw
//					//==============================================================================
//				}
//				else if(attitude_flag==1)   //״̬1 ��ʼ1000�ε�������0Ʈ��ʼ��
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

                    
                    
                    