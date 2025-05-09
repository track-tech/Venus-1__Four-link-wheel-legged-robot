//=============================================================================================
// MahonyAHRS.h
//=============================================================================================
//
// Madgwick's implementation of Mayhony's AHRS algorithm.
// See: http://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/
//
// Date			Author			Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=============================================================================================
#ifndef MahonyAHRS_h
#define MahonyAHRS_h
void Mahony_update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void Mahony_Init(float sampleFrequency);
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
void Mahony_computeAngles(void);
void MahonyAHRSinit(float ax, float ay, float az, float mx, float my, float mz);
float getRoll(void);
float getPitch(void);
float getYaw(void);
float getRollRadians(void);
float getPitchRadians(void);
float getYawRadians(void);

extern float roll_mahony, pitch_mahony, yaw_mahony;


#endif
