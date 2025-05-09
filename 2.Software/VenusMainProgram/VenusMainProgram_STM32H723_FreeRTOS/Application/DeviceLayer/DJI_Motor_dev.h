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




#ifndef __TK_BSP_CAN
#define __TK_BSP_CAN

//#include "mytype.h"
#include "pid.h"

typedef uint8_t 	u8;
typedef uint16_t 	u16;
typedef uint32_t 	u32;

typedef int8_t 		s8;
typedef int16_t 	s16;
typedef int32_t		s32;

typedef volatile uint8_t 	vu8;
typedef volatile uint16_t 	vu16;
typedef volatile uint32_t 	vu32;

typedef volatile int8_t 	vs8;
typedef volatile int16_t 	vs16;
typedef volatile int32_t	vs32;


typedef union{
	s8 		s8_fmt[8];	//for angle and omega
	u8 		u8_fmt[8];	//for angle and omega
	char 	ch_fmt[8];	//
	s16		s16_fmt[4];
	u16		u16_fmt[4];
	s32 	s32_fmt[2];
	u32 	u32_fmt[2];
	float 	f32_fmt[2];
	double 	d64_fmt;
}data_convert_ut;	//for diaobi gyro


typedef enum
{

	CAN_2006Moto_ALL_ID = 0x200,
	CAN_2006Moto1_ID = 0x201,
	CAN_2006Moto2_ID = 0x202,
	CAN_2006Moto3_ID = 0x203,
	CAN_2006Moto4_ID = 0x204,
	
}CAN_Message_ID;



#define FILTER_BUF_LEN		5
/*电机结构体*/
typedef struct{
	int16_t	 	speed_rpm;
    float  	real_current;
    int16_t  	given_current;
    uint8_t  	hall;
	uint16_t 	angle;				//abs angle range:[0,8191]
	uint16_t 	last_angle;			//abs angle range:[0,8191]
	uint16_t	offset_angle;
	int32_t		round_cnt;
	int32_t		total_angle;
	u8			buf_idx;
	u16			angle_buf[FILTER_BUF_LEN];
	u16			fited_angle;
	u32			msg_cnt;
}moto_measure_t;

/**
 *	@brief	四轮底盘定义
 *	@class	device
 */
typedef enum {
    CHAS_LF,
    CHAS_RF,
    CHAS_LB,
    CHAS_RB,
    CHAS_MOTOR_CNT,
} chassis_motor_cnt_t;

/* Extern  ------------------------------------------------------------------*/
extern moto_measure_t  moto_chassis[4];

extern PID_TypeDef motor_pid[4];


//uint8_t FDCAN1_Send_Msg(u8* msg,u32 len);				//鐠嬪啰鏁ょ拠銉ュ毐閺佺増甯撮弨鑸垫?傛导姘?鍤?閻滄壆鈹栭崐纭风礄0閿涳拷

void get_moto_measure(moto_measure_t *ptr, uint8_t *Data);
void set_moto_current(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);

void Motor_PID_Speed_Init(void);
void MOTOR_Speed_Contorl(int16_t SP1, int16_t SP2, int16_t SP3, int16_t SP4);
void CAR_MOVE(int16_t X_Speed,int16_t Y_Speed,int16_t Yaw_Speed);

void CAR_PID_Postion_Init(void);

void get_total_angle(moto_measure_t *p);


//void my_can_filter_init(FDCAN_HandleTypeDef* hcan);
//void my_can_filter_init_recv_all(FDCAN_HandleTypeDef* _hcan);
//void can_filter_recv_special(FDCAN_HandleTypeDef* hcan, uint8_t filter_number, uint16_t filtered_id);
//void get_moto_measure(moto_measure_t *ptr, FDCAN_HandleTypeDef* hcan);
//void can_receive_onetime(FDCAN_HandleTypeDef* _hcan, int time);
//void set_moto_current(FDCAN_HandleTypeDef* hcan, s16 iq1, s16 iq2, s16 iq3, s16 iq4);


#endif
