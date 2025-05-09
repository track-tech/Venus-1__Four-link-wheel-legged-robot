#include "ICM42688_dev.h"
#include "tim.h"

int16_t Accel[3]={0};
int16_t Gyro[3]={0};
float Temperature =0.1f;
float LSB_ACC_GYRO[2]={0};

	/*修正值为浮点,乘好LSB*/
	float GyroCal[3]={0};
	float AccelCal[3]={0};

	float GyroCorrected[3]={0};
	float AccelCorrected[3]={0};


uint8_t ICM_42688_ReadReg(uint8_t Reg)
{
	static uint8_t txdata[2];
	static uint8_t rxdata[2];
	
	uint8_t Read = 0xFF;
	Reg |= 0x80;
	ICM42688_SPI_CS_LOW();
	txdata[0] = Reg;
	txdata[1] = 0xff;
	HAL_SPI_TransmitReceive(&hspi2,txdata,rxdata,2,0xffff);
	
	Read = rxdata[1];
	ICM42688_SPI_CS_HIGH();
	return Read;
}

void ICM_42688_ReadRegs(uint8_t Reg, uint8_t* Data, uint8_t num)
{
	static uint8_t txdata[20];
	static uint8_t rxdata[20];
	uint8_t i;
	

	Reg |= 0x80;
	ICM42688_SPI_CS_LOW();
	
	if(num <20)
	{
		txdata[0] = Reg;
		HAL_SPI_TransmitReceive(&hspi2,txdata,rxdata,1+num,0xffff);
	}
	
	for(i = 0; i < num; i++)
	{
		Data[i] = rxdata[i+1];
	}	
	ICM42688_SPI_CS_HIGH();
}

void ICM_42688_WriteReg(uint8_t Reg, uint8_t  Data)
{
	static uint8_t txdata[10];
	static uint8_t rxdata[10];
//	uint8_t i;
	
	ICM42688_SPI_CS_LOW();
	
	txdata[0] = Reg;
	txdata[1] = Data;
	HAL_SPI_TransmitReceive(&hspi2,txdata,rxdata,2,0xffff);
	
	ICM42688_SPI_CS_HIGH();
}

void ICM_42688_WriteRegs(uint8_t Reg, uint8_t  *Data, uint8_t num)
{
	static uint8_t txdata[20];
	static uint8_t rxdata[20];
	uint8_t i;
	
	ICM42688_SPI_CS_LOW();
	
	txdata[0] = Reg;

	for(i = 0; i < num; i++)
	{
		txdata[i+1] = Data[i];
	}	
	HAL_SPI_TransmitReceive(&hspi2,txdata,rxdata,1+num,0xffff);
	
	ICM42688_SPI_CS_HIGH();
}


void ICM_42688_ReadAccel(void)
{
	uint8_t AccelBuf[6];
	ICM_42688_ReadRegs(0x1F,AccelBuf,6);
	Accel[0] = (AccelBuf[0]<<8 | AccelBuf[1]);
	Accel[1] = (AccelBuf[2]<<8 | AccelBuf[3]);
	Accel[2] = (AccelBuf[4]<<8 | AccelBuf[5]);
}

void ICM_42688_ReadGyro(void)
{
	uint8_t GyroBuf[6];
	ICM_42688_ReadRegs(0x25,GyroBuf,6);
	Gyro[0] = (GyroBuf[0]<<8 | GyroBuf[1]);
	Gyro[1] = (GyroBuf[2]<<8 | GyroBuf[3]);
	Gyro[2] = (GyroBuf[4]<<8 | GyroBuf[5]);
}
int16_t raw_tmpww;
void ICM_42688_ReadTem(void)
{
//	uint8_t GyroBuf[6];
//	ICM_42688_ReadRegs(0x25,GyroBuf,6);
//	Gyro[0] = (GyroBuf[0]<<8 | GyroBuf[1]);
//	Gyro[1] = (GyroBuf[2]<<8 | GyroBuf[3]);
//	Gyro[2] = (GyroBuf[4]<<8 | GyroBuf[5]);

	
	uint8_t buf[2]={0};
	int16_t raw_tmp;

	ICM_42688_ReadRegs(0x1D,buf,2);
	
	raw_tmp = (int16_t)((buf[0]<<8)|(buf[1]));
	
	raw_tmpww = raw_tmp;
	
	//低通滤波器截止频率:100Hz
	//this->Temperature = 0.1*(((float)raw_tmp/132.48f)+25.0f) + 0.9*this->Temperature;
	Temperature = ((float)raw_tmp/1324.8f)+ 2.5f + 0.9f*Temperature;	
}

//IMU预初始化 用于完成加热动作
void IMU_PreInit(void)
{
	uint8_t buf = 0;
	/*指定Bank0*/
	ICM_42688_WriteReg(0x76,0x00);
	/*软重启*/
	ICM_42688_WriteReg(0x11,0x01);
	HAL_Delay(5);
	/*读取中断位 切换SPI*/
	buf = ICM_42688_ReadReg(0x2D);
	/*指定Bank0*/
	ICM_42688_WriteReg(0x76,0x00);
	/*Gyro设置*/
	ICM_42688_WriteReg(0x4F,0x06);//2000dps 1KHz
	/*Accel设置*/
	ICM_42688_WriteReg(0x50,0x06);//16G 1KHz
	/*电源管理*/
	ICM_42688_WriteReg(0x4E,0x0F);//ACC GYRO LowNoise Mode
}

//IMU初始化 用于正常工作
void IMU42688_Init(void)
{
	uint8_t buf = 0;
	/*指定Bank0*/
	ICM_42688_WriteReg(0x76,0x00);
	/*软重启*/
	ICM_42688_WriteReg(0x11,0x01);
	HAL_Delay(5);
	/*读取中断位 切换SPI*/
	buf = ICM_42688_ReadReg(0x2D);
//	#ifdef qwDbug
//	/*打印IMU信息*/
//	rt_kprintf("\n/*****\nRST MSG: %d\nIMU Conect = %d\n*****/\n",buf,IMU->ReadReg(0x75));
//	#endif
	
	/*指定Bank0*/
	ICM_42688_WriteReg(0x76,0x00);
	/*中断输出设置*/
	ICM_42688_WriteReg(0x12,0x36);//INT1 INT2 脉冲模式，低有效
	/*Gyro设置*/
	ICM_42688_WriteReg(0x4F,0x06);//2000dps 1KHz
	/*Accel设置*/
	ICM_42688_WriteReg(0x50,0x06);//16G 1KHz
	/*LSB设置*/
	LSB_ACC_GYRO[0] = LSB_ACC_16G;
	LSB_ACC_GYRO[1] = LSB_GYRO_2000_R;
	/*Tem设置&Gyro_Config1*/
	ICM_42688_WriteReg(0x51,0x56);//BW 82Hz Latency = 2ms
	/*GYRO_ACCEL_CONFIG0*/
	ICM_42688_WriteReg(0x52,0x11);//1BW
	/*ACCEL_CONFIG1*/
	ICM_42688_WriteReg(0x53,0x0D);//Null
	/*INT_CONFIG0*/
	ICM_42688_WriteReg(0x63,0x00);//Null
	/*INT_CONFIG1*/
	ICM_42688_WriteReg(0x64,0x00);//中断引脚正常启用
	/*INT_SOURCE0*/
	ICM_42688_WriteReg(0x65,0x08);//DRDY INT1
	/*INT_SOURCE1*/
	ICM_42688_WriteReg(0x66,0x00);//Null
	/*INT_SOURCE3*/
	ICM_42688_WriteReg(0x68,0x00);//Null
	/*INT_SOURCE3*/
	ICM_42688_WriteReg(0x69,0x00);//Null
	
/*****抗混叠滤波器@536Hz*****/
	
	/*GYRO抗混叠滤波器配置*/
	/*指定Bank1*/
	ICM_42688_WriteReg(0x76,0x01);
	/*GYRO抗混叠滤波器配置*/
	ICM_42688_WriteReg(0x0B,0xA0);//开启抗混叠和陷波滤波器
	ICM_42688_WriteReg(0x0C,0x0C);//GYRO_AAF_DELT 12 (default 13)
	ICM_42688_WriteReg(0x0D,0x90);//GYRO_AAF_DELTSQR 144 (default 170)
	ICM_42688_WriteReg(0x0E,0x80);//GYRO_AAF_BITSHIFT 8 (default 8)
	
	/*ACCEL抗混叠滤波器配置*/
	/*指定Bank2*/
	ICM_42688_WriteReg(0x76,0x02);
	/*ACCEL抗混叠滤波器配置*/
	ICM_42688_WriteReg(0x03,0x18);//开启滤波器 ACCEL_AFF_DELT 12 (default 24)
	ICM_42688_WriteReg(0x04,0x90);//ACCEL_AFF_DELTSQR 144 (default 64)
	ICM_42688_WriteReg(0x05,0x80);//ACCEL_AAF_BITSHIFT 8 (default 6)

/*****自定义滤波器1号@111Hz*****/

	/*指定Bank0*/
	ICM_42688_WriteReg(0x76,0x00);
	/*滤波器顺序*/
	ICM_42688_WriteReg(0x51,0x12);//GYRO滤波器1st
	ICM_42688_WriteReg(0x53,0x05);//ACCEL滤波器1st
	/*滤波器设置*/
	ICM_42688_WriteReg(0x52,0x33);//111Hz 03

	
//	exti_flag_clear(EXTI_3);
//	exti_flag_clear(EXTI_4);
//	exti_interrupt_enable(EXTI_3);
//	exti_interrupt_enable(EXTI_4);
	
	/*指定Bank0*/
	ICM_42688_WriteReg(0x76,0x00);
	/*电源管理*/
	ICM_42688_WriteReg(0x4E,0x0F);//ACC GYRO LowNoise Mode
	
	HAL_Delay(1);
}


/**
 * @brief 用于更新陀螺仪
 * 
 */
void ICM42688_Update(void)
{
		ICM_42688_ReadReg(0x75);

		ICM_42688_ReadAccel();
//		if(Accel[0]==0x00 && Accel[1]==0x00 && Accel[2]==0x00)
//			IMU42688_Init();
//		if(Accel[0]==-32768 && Accel[1]==-32768 && Accel[2]==-32768)
//			IMU42688_Init();		
		
		
		ICM_42688_ReadGyro();
		ICM_42688_ReadTem();
		
		AccelCorrected[0] = ((float)Accel[0] + AccelCal[0]) * LSB_ACC_GYRO[0];
		AccelCorrected[1] = ((float)Accel[1] + AccelCal[1]) * LSB_ACC_GYRO[0];
		AccelCorrected[2] = ((float)Accel[2] + AccelCal[2]) * LSB_ACC_GYRO[0];
		GyroCorrected[0] = ((float)Gyro[0] + GyroCal[0]) * LSB_ACC_GYRO[1];
		GyroCorrected[1] = ((float)Gyro[1] + GyroCal[1]) * LSB_ACC_GYRO[1];
		GyroCorrected[2] = ((float)Gyro[2] + GyroCal[2]) * LSB_ACC_GYRO[1];			
		
    	// osDelay(50);
		
		
		 __HAL_TIM_SetCompare(&htim16, TIM_CHANNEL_1, 50);


}
