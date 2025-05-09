#ifndef __YC_ICM42688_H__
#define __YC_ICM42688_H__
#include "main.h"
#include "main.h"
#include "spi.h"

#include "gpio.h"


#define ICM42688_SPI_CS_LOW()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET)
#define ICM42688_SPI_CS_HIGH()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET)


/*G = 9.8011 in Dalian*/
#define LSB_ACC_16G		0.0047856934f
#define LSB_ACC_8G		0.0023928467f
#define LSB_ACC_4G		0.0011964233f
#define LSB_ACC_2G		0.00059821167f

/*Turn Into Radian*/
#define LSB_GYRO_2000_R	0.0010652644f
#define LSB_GYRO_1000_R	0.00053263222f
#define LSB_GYRO_500_R	0.00026631611f
#define LSB_GYRO_250_R	0.00013315805f
#define LSB_GYRO_125D_R	0.000066579027f


	/*ä¿?æ­£å€¼ä¸ºæµ?ç‚?,ä¹˜å¥½LSB*/
extern float GyroCal[3];
extern float AccelCal[3];

extern float GyroCorrected[3];
extern float AccelCorrected[3];


extern int16_t Accel[3];
extern int16_t Gyro[3];
extern float Temperature;
extern float LSB_ACC_GYRO[2];


uint8_t ICM_42688_ReadReg(uint8_t Reg);
void ICM_42688_ReadRegs(uint8_t Reg, uint8_t* Data, uint8_t num);
void ICM_42688_WriteReg(uint8_t Reg, uint8_t  Data);
void ICM_42688_WriteRegs(uint8_t Reg, uint8_t  *Data, uint8_t num);
void ICM_42688_ReadAccel(void);
void ICM_42688_ReadGyro(void);
void ICM_42688_ReadTem(void);

void IMU42688_Init(void);

void ICM42688_Update(void);





#endif
