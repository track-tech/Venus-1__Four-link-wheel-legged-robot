/*
Mahony�㷨��̬����ͷ�ļ�-�ο����µ�ַ
http://www.singularity-blog.top/2022/07/28/ICM42688P-DrvBySPI/#Mahony%E5%A7%BF%E6%80%81%E4%BC%B0%E8%AE%A1%E5%AE%9E%E7%8E%B0%E4%BB%A3%E7%A0%81
*/

float inv_sqrt(float x);

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
												 float ax, float ay, float az);

						   
/**
  * @brief  ��Ԫ��תŷ����
  * @note   �����ڲ�У����Ԫ����׼ȷ��
  * @param  pIMU : ��׼��λ�Ƶ�������������
  * @retval None
  */
//void trans_quaternionToEulerAngle(imu_rawData_t* pIMU);

void trans_quaternionToEulerAngle_simple(float q[4], float IMU_angle[3]);

float ErrorCompensation(void);
                         
                           