/**
  * @author  Liu heng
  * 卡尔曼滤波器来自RoboMaster论坛  
  */
  
#ifndef _KALMAN_H
#define _KALMAN_H


typedef struct {
    float X_last; //上一时刻的最优结果  X(k-|k-1)
    float X_mid;  //当前时刻的预测结果  X(k|k-1)
    float X_now;  //当前时刻的最优结果  X(k|k)
    float P_mid;  //当前时刻预测结果的协方差  P(k|k-1)
    float P_now;  //当前时刻最优结果的协方差  P(k|k)
    float P_last; //上一时刻最优结果的协方差  P(k-1|k-1)
    float kg;     //kalman增益
    float A;      //系统参数
		float B;
    float Q;
    float R;
    float H;
}extKalman_t;

//创建滤波器
void KalmanCreate(extKalman_t *p,float T_Q,float T_R);

//往滤波器塞数据
float KalmanFilter(extKalman_t* p,float dat);


//清空卡尔曼滤波器
void KalmanClear(extKalman_t *p);

#endif
