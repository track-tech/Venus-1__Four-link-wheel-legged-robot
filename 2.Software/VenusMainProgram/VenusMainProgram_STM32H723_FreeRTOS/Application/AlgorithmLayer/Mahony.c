/*
Mahony�㷨��̬����-�ο����µ�ַ
http://www.singularity-blog.top/2022/07/28/ICM42688P-DrvBySPI/#Mahony%E5%A7%BF%E6%80%81%E4%BC%B0%E8%AE%A1%E5%AE%9E%E7%8E%B0%E4%BB%A3%E7%A0%81
*/

#include "stm32h7xx_hal.h"
#include "Mahony.h"
#include "math.h"


#define sampleFreq	1000.0f			// �����ʣ� Hz ��
#define twoKpDef	(2.0f * 0.5f)	// 2 * proportional gain
#define twoKiDef	(2.0f * 0.0f)	// 2 * integral gain

volatile float twoKp = twoKpDef;											// 2 * proportional gain (Kp)
volatile float twoKi = twoKiDef;											// 2 * integral gain (Ki)
volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	// �������

/******************************************************************
**  �㷨ʵ��
*******************************************************************/

/**
  * @brief  ���ټ��㷴ƽ����
  * @note   1/sqrt(x)
  * @param  x : �������
  * @retval y : ��ƽ�������
  */
float inv_sqrt(float x) 
{
	float halfx = 0.5f * x;
	float y     = x;
	long  i     = *(long*)&y;
	
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	
	return y;
}

/**
  * @brief  Mahony�ں��㷨��6��棩
  * @note   ֻ���������·����ʼ��������ǰ��
  * @param  q : ��Ԫ�������飩
  * @param  gx,gy,gz : ���������ݣ���׼dps��λ��
  * @param  ax,ay,az : ���ٶ����ݣ���׼g��λ��
  * @retval None
  */
void MahonyFilter_6Axis(float q[4], 
	                       float gx, float gy, float gz,
												 float ax, float ay, float az)
{
	float normFactor;
	float halfvx, halfvy, halfvz;	// ���Ƶ��������ٶ�ʸ����half��ʾ��ֵ
	float halfex, halfey, halfez; 	// �������
	float q0 = q[0],q1 = q[1], q2 = q[2], q3 = q[3];
	
	float q0q0 = q[0]*q[0];
	float q0q1 = q[0]*q[1];
	float q0q2 = q[0]*q[2];
	float q0q3 = q[0]*q[3];
	float q1q1 = q[1]*q[1];
	float q1q2 = q[1]*q[2];
	float q1q3 = q[1]*q[3];
	float q2q2 = q[2]*q[2];   
	float q2q3 = q[2]*q[3];
	float q3q3 = q[3]*q[3];  

	// ֻ�ڼ��ٶȼ�������Чʱ�Ž�������
	if((ax != 0.0f) || (ay != 0.0f) || (az != 0.0f)) 
	{
		// �����ٶȼƵõ���ʵ���������ٶ�����v��λ��
		normFactor = inv_sqrt(ax * ax + ay * ay + az * az); //����ģ������
		ax *= normFactor;
		ay *= normFactor;
		az *= normFactor;        

		// ͨ����Ԫ������ת���󣩵õ������������ٶ��ڻ�������ϵ�µ�����ֵ 
		// ע�⣬����ʵ�����Ǿ��������*1/2���ڿ�ͷ��Kp Ki�ĺ궨���Ϊ2*���棬��������Ŀ���Ǽ��ٳ˷�������
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;	// q0q0 - q1q1 - q2q2 + q3q3 ���Ż��棨|q| =1��
	
		// ����ʵ���������ٶ�����������ֵ���������������ٶ�����������ֵ�������
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		if(twoKi > 0.0f) 
		{
			// ���ֹ���
			integralFBx += twoKi * halfex * (1.0f / sampleFreq);	
			integralFBy += twoKi * halfey * (1.0f / sampleFreq);
			integralFBz += twoKi * halfez * (1.0f / sampleFreq);

			// ������
			gx += integralFBx;
			gy += integralFBy;
			gz += integralFBz;
		}
		else {
			integralFBx = 0.0f;	
			integralFBy = 0.0f;
			integralFBz = 0.0f;
		}

		// ������
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}
	
	// ��Ԫ�� ΢�ַ���
	gx *= (0.5f * (1.0f / sampleFreq));
	gy *= (0.5f * (1.0f / sampleFreq));
	gz *= (0.5f * (1.0f / sampleFreq));
	
	q[0] += (-q1 * gx - q2 * gy - q3 * gz);
	q[1] += (q0 * gx + q2 * gz - q3 * gy);
	q[2] += (q0 * gy - q1 * gz + q3 * gx);
	q[3] += (q0 * gz + q1 * gy - q2 * gx); 
	
	// ��λ����Ԫ��
	normFactor = inv_sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	q[0] *= normFactor;
	q[1] *= normFactor;
	q[2] *= normFactor;
	q[3] *= normFactor;
    
}

/**
  * @brief  ��Ԫ��תŷ����
  * @note   �����ڲ�У����Ԫ����׼ȷ��
  * @param  pIMU : ��׼��λ�Ƶ�������������
  * @retval None
  */
//void trans_quaternionToEulerAngle(imu_rawData_t* pIMU)
//{
//	float q[4];
//	for(int i =0; i <4; i++)
//	{
//		q[i] = pIMU->quaternion_body2Earth[i];
//	}
//	/* yaw    -pi----pi */
//	pIMU->yaw = -atan2(2*q[1]*q[2] + 2*q[0]*q[3], -2*q[2]*q[2] - 2*q[3]*q[3] + 1)* 57.3; 
//	/* pitch  -pi/2----pi/2 */
//	pIMU->pitch = -asin(-2*q[1]*q[3] + 2*q[0]*q[2])* 57.3;
//	/* roll   -pi----pi  */	
//	pIMU->roll =  atan2(2*q[2]*q[3] + 2*q[0]*q[1], -2*q[1]*q[1] - 2*q[2]*q[2] + 1)* 57.3;
//}



void trans_quaternionToEulerAngle_simple(float q[4], float IMU_angle[3])
{
//	float q[4];
//	for(int i =0; i <4; i++)
//	{
//		q[i] = pIMU->quaternion_body2Earth[i];
//	}
    static int i = 1;
    
	/* yaw    -pi----pi */
	IMU_angle[0] = -atan2(2*q[1]*q[2] + 2*q[0]*q[3], -2*q[2]*q[2] - 2*q[3]*q[3] + 1)* 57.3; //0.0002759711
	/* pitch  -pi/2----pi/2 */
	IMU_angle[1] = -asin(-2*q[1]*q[3] + 2*q[0]*q[2])* 57.3;
	/* roll   -pi----pi  */	
	IMU_angle[2] =  atan2(2*q[2]*q[3] + 2*q[0]*q[1], -2*q[1]*q[1] - 2*q[2]*q[2] + 1)* 57.3;
    
    i++;
}



float ErrorCompensation(void){

        uint16_t TimeSave = 0;
        float errorclean = 0;
        uint16_t OnecFlag = 0;
        if(OnecFlag == 0){
        
            TimeSave = HAL_GetTick();
            OnecFlag = 1;
        }
        else if(OnecFlag == 1){
        
            errorclean = -0.0006299387*(HAL_GetTick() - TimeSave) - 172.0349196;
            
        }
        
        return errorclean;

}



