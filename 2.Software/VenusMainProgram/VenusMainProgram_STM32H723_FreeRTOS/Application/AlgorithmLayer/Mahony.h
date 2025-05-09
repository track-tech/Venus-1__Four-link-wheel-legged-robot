/*
Mahony算法姿态解算头文件-参考文章地址
http://www.singularity-blog.top/2022/07/28/ICM42688P-DrvBySPI/#Mahony%E5%A7%BF%E6%80%81%E4%BC%B0%E8%AE%A1%E5%AE%9E%E7%8E%B0%E4%BB%A3%E7%A0%81
*/

float inv_sqrt(float x);

/**
  * @brief  Mahony融合算法（6轴版）
  * @note   只负责迭代环路，初始化另外提前做
  * @param  q : 四元数（数组）
  * @param  gx,gy,gz : 陀螺仪数据（标准dps单位）
  * @param  ax,ay,az : 加速度数据（标准g单位）
  * @retval None
  */
void MahonyFilter_6Axis(float q[4], 
	                       float gx, float gy, float gz,
												 float ax, float ay, float az);

						   
/**
  * @brief  四元数转欧拉角
  * @note   函数内不校验四元数的准确性
  * @param  pIMU : 标准单位制的陀螺仪数据区
  * @retval None
  */
//void trans_quaternionToEulerAngle(imu_rawData_t* pIMU);

void trans_quaternionToEulerAngle_simple(float q[4], float IMU_angle[3]);

float ErrorCompensation(void);
                         
                           