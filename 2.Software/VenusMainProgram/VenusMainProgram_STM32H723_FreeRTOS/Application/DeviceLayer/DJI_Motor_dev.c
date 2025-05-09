/******************************************************************************
/// @brief
/// @copyright Copyright (c) 2017 <dji-innovations, Corp. RM Dept.>
/// @license MIT License
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction,including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense,and/or sell
/// copies of the Software, and to permit persons to whom the Software is furnished
/// to do so,subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
*******************************************************************************/

#include "fdcan.h"
#include "DJI_Motor_dev.h"
#include "string.h"
#include "pid.h"
#include "CAN_drv.h"


//求绝对值
#define ABS(x)	( (x>0) ? (x) : (-x) )

moto_measure_t moto_chassis[4];//4 chassis moto
PID_TypeDef motor_pid[4];

/*******************************************************************************************
  * @Func		void get_moto_measure(moto_measure_t *ptr, FDCAN_HandleTypeDef* hfdcan1)
  * @Brief		接收2006电机通过CAN发过来的信息
  * @Param		
  * @Retval		None
  * @Date     	2023/3/12
  * @author   	张百川
 *******************************************************************************************/
void get_moto_measure(moto_measure_t *ptr, uint8_t *Data)
{

	ptr->last_angle = ptr->angle;
	ptr->angle 			= (Data[0]<<8 | Data[1]) ;
	ptr->speed_rpm  	= (Data[2]<<8 | Data[3]);
	ptr->real_current 	= (Data[4]<<8 | Data[5])*5.f/16384.f;

	ptr->hall = Data[6];
	
	
	if(ptr->angle - ptr->last_angle > 4096)
		ptr->round_cnt --;
	else if (ptr->angle - ptr->last_angle < -4096)
		ptr->round_cnt ++;
	ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;
}

/**
*@bref 电机上电角度=0， 之后用这个函数更新3510电机的相对开机后（为0）的相对角度。
	*/
void get_total_angle(moto_measure_t *p){
	
	int res1, res2, delta;
	if(p->angle < p->last_angle){			//可能的情况
		res1 = p->angle + 8192 - p->last_angle;	//正转，delta=+
		res2 = p->angle - p->last_angle;				//反转	delta=-
	}else{	//angle > last
		res1 = p->angle - 8192 - p->last_angle ;//反转	delta -
		res2 = p->angle - p->last_angle;				//正转	delta +
	}
	//不管正反转，肯定是转的角度小的那个是真的
	if(ABS(res1)<ABS(res2))
		delta = res1;
	else
		delta = res2;

	p->total_angle += delta;
	p->last_angle = p->angle;
}

/*******************************************************************************************
  * @Func	void set_moto_current(uint16_t iq1, uint16_t iq2, uint16_t iq3, uint16_t iq4
  * @Brief    通过CAN发送电流值给C610电调-电机空载启动电流0.35A
  * @Param		
  * @Retval		None
  * @Date     2023/3/21
 *******************************************************************************************/
void set_moto_current(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4){

		uint8_t canbuf[8];
	
		canbuf[0] = iq1 >> 8;
		canbuf[1] = iq1;
		canbuf[2] = iq2 >> 8;
		canbuf[3] = iq2;	  
		canbuf[4] = iq3 >> 8;
		canbuf[5] = iq3;		
		canbuf[6] = iq4 >> 8;
		canbuf[7] = iq4;
	  
		FDCAN2_Send_Msg(canbuf, FDCAN_DLC_BYTES_8);
    
}	

/**
 *	@brief	底盘电机速度环PID初始化
 *	@class	device
 */

void Motor_PID_Speed_Init(void){
	
  for(int i=0; i<4; i++)
  {	

    pid_init(&motor_pid[i]);
//	motor_pid[i].f_param_init(&motor_pid[i], PID_Speed, 16384, 5000, 10, 0, 8000, 0, 10, 0.3, 0);//速度环
	motor_pid[i].f_param_init(&motor_pid[i], PID_Speed, 20000, 5000, 10, 0, 8000, 0, 20, 1.5, 0);//速度环
	  
//	get_total_angle(&moto_chassis[i]);	//电机初始化角获得
  }
  
}

/**
 *	@brief	底盘电机速度控制函数(已加入速度环控制) 
 *	@class	device
 */

void MOTOR_Speed_Contorl(int16_t SP1, int16_t SP2, int16_t SP3, int16_t SP4){

	int16_t SPEED[4] = {SP1,SP2,SP3,SP4};
	
	for(int i=0;i<4;i++){
	//			if(i == 0 || i == 3)     motor_pid[i].target = -CAR_X.output;
	//			else motor_pid[i].target = CAR_X.output;
		motor_pid[i].target = SPEED[i];
		motor_pid[i].f_cal_pid(&motor_pid[i],moto_chassis[i].speed_rpm);    //根据设定值进行PID计算。
	}
	set_moto_current(motor_pid[0].output, motor_pid[1].output, motor_pid[2].output, motor_pid[3].output);
	memset(SPEED,0,sizeof(SPEED));

}




