/*
***********************************************************************************************
*                                       QMC5883L����������
*
*                                      ʵ��QMC5883L�ײ�����
*
* Filename : QMC5883L.c
* Version : V1.00
* Programmer(s) : FengDuo
*
***********************************************************************************************
*/
#define QMC5883L_MOUDLE
//�ļ�����
/*
***********************************************************************************************
*                                         INCLUDE FILES
***********************************************************************************************
*/
#include "main.h"
#include "QMC5883L_dev.h"

/*
***********************************************************************************************
*                                         LOCAL DEFINES
***********************************************************************************************
*/
static QMC5883LSAMPLE_DEF QMC5883LSample;
/*
***********************************************************************************************
*                                   LOCAL FUNCTION PROTOTYPES
***********************************************************************************************
*/
static void         QMC5883L_IOInit(void);
static void         QMC5883L_Register_Write(CPU_INT08U addr,CPU_INT08U val);
static CPU_INT08U   QMC5883L_Register_Read(CPU_INT08U addr);
static void         QMC5883L_Read_XYZ(CPU_INT16S *x,CPU_INT16S *y,CPU_INT16S *z);
#ifdef QMC5883L_READ_TEMP
static void         QMC5883L_Read_TEMP(CPU_INT16S *temp);
#endif
static CPU_BOOLEAN  QMC5883L_Wait_Data_Update(CPU_INT08U max_cnt);
static CPU_BOOLEAN  QMC5883L_Read_Average(CPU_INT16S *x,CPU_INT16S *y,CPU_INT16S *z,CPU_INT08U times);
/*
***********************************************************************************************
                                         LOCAL FUNCTIONS
***********************************************************************************************
*/
/*
***********************************************************************************************
*                                     QMC5883L_IOInit
*
* Description: QMC5883L������ IO��ʼ��
*
* Argument(s): ��
*
* Return(s) :  ��
*
* Caller(s) :
*
***********************************************************************************************
*/
//static void QMC5883L_IOInit(void)
//{
//    GPIO_InitTypeDef  GPIO_InitStruct;
//    
//    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull      = GPIO_PULLUP;
//    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Pin       = QMC5883L_PIN_SCL |QMC5883L_PIN_SDA;
//    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
//    HAL_GPIO_Init(QMC5883L_PORT_SDA_SCL, &GPIO_InitStruct);
//}
// 
/*
***********************************************************************************************
*                                     QMC5883L_Register_Write
*
* Description: дQMC5883L�Ĵ���
*
* Argument(s):
* addr: IN, �Ĵ�����ַ
*  val: IN, Ҫд���ֵ
*
* Return(s) :  ��
*
* Caller(s) :
*
***********************************************************************************************
*/
static void QMC5883L_Register_Write(CPU_INT08U addr,CPU_INT08U val) 
{
    CPU_INT08U len = 0x01;
    QMC5883L_HardI2C_Register_Write(QMC5883L_ADDR_WRITE, addr, &val, len);
}
 
/*
***********************************************************************************************
*                                     QMC5883L_Register_Read
*
* Description: ��QMC5883L�Ĵ���
*
* Argument(s):
* addr: IN, �Ĵ�����ַ
*
* Return(s) : �Ĵ�����ȡֵ
*
* Caller(s) :
*
***********************************************************************************************
*/
static CPU_INT08U QMC5883L_Register_Read(CPU_INT08U addr) 
{
    CPU_INT08U temp = 0x00, len = 0x01;
    QMC5883L_HardI2C_Register_Read(QMC5883L_ADDR_READ, addr, &temp, len);
    return temp;
}
 
/*
***********************************************************************************************
*                                     QMC5883L_Read_XYZ
*
* Description: QMC5883L��ȡXYZ����ų�ǿ��
*
* Argument(s):
* x: IN, X�����ݵ�ַ
* y: IN, Y�����ݵ�ַ
* z: IN, Z�����ݵ�ַ
*
* Return(s) :  ��
*
* Caller(s) :
*
***********************************************************************************************
*/
static void QMC5883L_Read_XYZ(CPU_INT16S *x,CPU_INT16S *y,CPU_INT16S *z)
{
    CPU_INT08U buf[QMC5883L_XYZBUF_LEN];
    CPU_INT16S dataX,dataY,dataZ;
    
    QMC5883L_HardI2C_Register_Read(QMC5883L_ADDR_READ, QMC5883L_ADDR_XOUTL, buf, QMC5883L_XYZBUF_LEN);
    
    dataX=(CPU_INT16S)((buf[1]<<8)+buf[0]);
    dataY=(CPU_INT16S)((buf[3]<<8)+buf[2]);
    dataZ=(CPU_INT16S)((buf[5]<<8)+buf[4]);
 
    *x = dataX / QMC5883L_SENSITIVITY_8G;
    *y = dataY / QMC5883L_SENSITIVITY_8G;
    *z = dataZ / QMC5883L_SENSITIVITY_8G;
    //QMC5883LSample.XY_Angle = (atan2((CPU_FP32)QMC5883LSample.Y_Data,(CPU_FP32)QMC5883LSample.X_Data) * (180 / 3.14159265) + 180) ;
    //QMC5883LSample.XZ_Angle = (atan2((CPU_FP32)QMC5883LSample.Z_Data,(CPU_FP32)QMC5883LSample.X_Data) * (180 / 3.14159265) + 180) ;
    //QMC5883LSample.YZ_Angle = (atan2((CPU_FP32)QMC5883LSample.Z_Data,(CPU_FP32)QMC5883LSample.Y_Data) * (180 / 3.14159265) + 180) ;
}
 
/* ��ʱ�򿪣�fd,2019.11.19 */
#ifdef QMC5883L_READ_TEMP
/*
***********************************************************************************************
*                                     QMC5883L_Read_TEMP
*
* Description: QMC5883L��ȡ����¶�ֵ
*
* Argument(s):
* temp: IN, �¶����ݵ�ַ
*
* Return(s) :  ��
*
* Caller(s) :
*
***********************************************************************************************
*/
static void QMC5883L_Read_TEMP(CPU_INT16S *temp)
{
    CPU_INT08U buf[QMC5883L_TEMBUF_LEN];
    CPU_INT16S dataT;
    
    QMC5883L_HardI2C_Register_Read(QMC5883L_ADDR_READ, QMC5883L_ADDR_TEMPL, buf, QMC5883L_TEMBUF_LEN);
 
    dataT = (CPU_INT16S)((buf[1]<<8)+buf[0]); 
    *temp=  dataT / 100;
}
#endif
 
/*
***********************************************************************************************
*                                     QMC5883L_Wait_Data_Update
*
* Description: QMC5883L��ѯ�Ĵ������ݾ���
*
* Argument(s):
* max_cnt: IN, �����ѯ����
*
* Return(s) :  �ɹ�:TRUE,ʧ��:FALSE
*
* Caller(s) :
*
***********************************************************************************************
*/
static CPU_BOOLEAN QMC5883L_Wait_Data_Update(CPU_INT08U max_cnt)
{
    for(; max_cnt>0x00; max_cnt--)
    {
        if(QMC5883L_Register_Read(QMC5883L_ADDR_STATUS) & 0x01)
        {
            return TRUE;
        }
        HAL_Delay(1);
    }
    return FALSE;
}
 
/*
***********************************************************************************************
*                                     QMC5883L_Read_Average
*
* Description: QMC5883L��ȡXYZ����ų�ǿ��ƽ��ֵ
*
* Argument(s):
* x: IN, X�����ݵ�ַ
* y: IN, Y�����ݵ�ַ
* z: IN, Z�����ݵ�ַ
* times: IN, ��ȡ����
*
* Return(s) :  ��
*
* Caller(s) :
*
***********************************************************************************************
*/
static CPU_BOOLEAN QMC5883L_Read_Average(CPU_INT16S *x,CPU_INT16S *y,CPU_INT16S *z,CPU_INT08U times)
{
    CPU_INT08U i,j;
    CPU_INT16S tx,ty,tz;
    CPU_INT08U  temp_serial[10],tmp_serial;
    CPU_INT16S  temp_x[10],temp_y[10],temp_z[10];
    CPU_INT32S  tmp = 0;
    
    *x=0;
    *y=0;
    *z=0;
    
    if(times)          
    {
        for(i=0;i<times;i++)
        {
            if(!QMC5883L_Wait_Data_Update(10))
            {
                return FALSE;
            }
            QMC5883L_Read_XYZ(&tx,&ty,&tz);
            temp_x[i]=tx;
            temp_y[i]=ty;
            temp_z[i]=tz;
        }
    }
    
    /**********************************����X��***********************************/
    for(j=0; j<times; j++) 
    {      
      	temp_serial[j] = j;	
    }
    
    for(i=times; i>0; i--)
    {
        for(j=0; j<(i-1); j++)
        {
            if(temp_x[temp_serial[j]] > temp_x[temp_serial[j+1]])
            {
                tmp_serial = temp_serial[j];
                temp_serial[j] = temp_serial[j+1];
                temp_serial[j+1] = tmp_serial;            
            }
        }
    }	
    
    for(i=1;i<times-1;i++) 
    { 
        tmp += temp_x[temp_serial[i]];
    }
    
    *x = tmp/(times-2);
    
    /**********************************����Y��***********************************/
    tmp = 0;
    
    for(j=0; j<times; j++) 
    {      
      	temp_serial[j] = j;	
    }
    
    for(i=times; i>0; i--)
    {
        for(j=0; j<(i-1); j++)
        {
            if(temp_y[temp_serial[j]] > temp_y[temp_serial[j+1]])
            {
                tmp_serial = temp_serial[j];
                temp_serial[j] = temp_serial[j+1];
                temp_serial[j+1] = tmp_serial;            
            }
        }
    }	
    
    for(i=1;i<times-1;i++) 
    { 
        tmp += temp_y[temp_serial[i]];
    }
    
    *y = tmp/(times-2);
    
    /**********************************����Z��***********************************/
    tmp = 0;
    
    for(j=0; j<times; j++) 
    {      
      	temp_serial[j] = j;	
    }
    
    for(i=times; i>0; i--)
    {
        for(j=0; j<(i-1); j++)
        {
            if(temp_z[temp_serial[j]] > temp_z[temp_serial[j+1]])
            {
                tmp_serial = temp_serial[j];
                temp_serial[j] = temp_serial[j+1];
                temp_serial[j+1] = tmp_serial;            
            }
        }
    }	
    
    for(i=1;i<times-1;i++) 
    { 
        tmp += temp_z[temp_serial[i]];
    }
    
    *z = tmp/(times-2);
 
    return TRUE;
}
/*
***********************************************************************************************
*                                    GLOBAL FUNCTIONS
***********************************************************************************************
*/
/*
***********************************************************************************************
*                                     QMC5883L_Init
*
* Description: QMC5883L�����Ƴ�ʼ��
*
* Argument(s): ��
*
* Return(s) :  ��ʼ�����, �ɹ�TRUE��ʧ��FALSE
*
* Caller(s) :
*
***********************************************************************************************
*/
CPU_BOOLEAN QMC5883L_Init(void)
{
    CPU_INT08U uErrTime=0;
    QMC5883L_IOInit();
    QMC5883L_POWER_ON();
 
    /* ��ȡ����ID */
     while(QMC5883L_Register_Read(QMC5883L_ADDR_CHIPID) != QMC5883L_CHIPID_VALUE)
    {
        uErrTime++;
        if(uErrTime>250)
        {
            return FALSE;
        }
    }
    QMC5883L_Register_Write(QMC5883L_ADDR_CFGA ,QMC5883L_CFGA_VALUE_STANDBY);   /* OSR = 512;RNG = 8G(0x1d RNG=8G);ODR=200Hz;MODE:����ģʽ*/
    QMC5883L_Register_Write(QMC5883L_ADDR_CFGC ,QMC5883L_CFGC_VALUE);
    QMC5883L_Register_Write(QMC5883L_ADDR_CFGD ,QMC5883L_CFGD_VALUE);
    QMC5883L_Register_Write(QMC5883L_ADDR_PERIORC ,QMC5883L_PERIORC_VALUE);
 
    if(QMC5883L_Register_Read(QMC5883L_ADDR_CFGA) != QMC5883L_CFGA_VALUE_STANDBY)
    {
        return FALSE;
    }
    return TRUE;
}
 
/*
***********************************************************************************************
*                                     QMC5883L_Soft_Reset
*
* Description: QMC5883L��������λ
*
* Argument(s): ��
*
* Return(s) :  ��
*
* Caller(s) :
*
***********************************************************************************************
*/
void QMC5883L_Soft_Reset(void)
{
    QMC5883L_Register_Write(QMC5883L_ADDR_CFGB ,QMC5883L_CFGB_VALUE_REBOOT);
    HAL_Delay(50);
}
 
/*
***********************************************************************************************
*                                     QMC5883L_Hard_Reset
*
* Description: QMC5883L������Ӳ��λ
*
* Argument(s): ��
*
* Return(s) :  ��
*
* Caller(s) :
*
***********************************************************************************************
*/
void QMC5883L_Hard_Reset(void)
{
    QMC5883L_POWER_OFF();
    HAL_Delay(1000);
    QMC5883L_POWER_ON();
}
/*
***********************************************************************************************
*                                     QMC5883L_AUTO_Adjust
*
* Description: QMC5883LУ׼
*
* Argument(s): ��
*
* Return(s) :  ��
*
* Caller(s) :
*
***********************************************************************************************
*/
CPU_BOOLEAN QMC5883L_Adjust_Magnetic(void)
{
    CPU_INT08U  err;
    CPU_BOOLEAN res;
    CPU_INT16S  x,y,z;
 
//    OSSemPend(App_SensorSem, OS_TICKS_PER_SEC, &err);                           /* �����ź��� */
//    if(err == OS_ERR_NONE)
    if(err == 1)
    {
        /* QMC5883L��������ģʽ */
        QMC5883L_Register_Write(QMC5883L_ADDR_CFGA ,QMC5883L_CFGA_VALUE_CONTINUE);
        /* ��ȡ10��X,Y,Z��������Ĵų�ǿ��ֵ��������ȡƽ�� */
        if(!QMC5883L_Read_Average(&x,&y,&z,10))
        {
//            OSSemPost(App_SensorSem);                                           /* �ͷ��ź��� */
            res = FALSE;
        }
        QMC5883LSave.X_Offset = x;
        QMC5883LSave.Y_Offset = y;
        QMC5883LSave.Z_Offset = z;
        /* QMC5883L��������ģʽ */
        QMC5883L_Register_Write(QMC5883L_ADDR_CFGA ,QMC5883L_CFGA_VALUE_STANDBY);
//        OSSemPost(App_SensorSem);                                               /* �ͷ��ź��� */
 
        res = TRUE;
    }
    else
    {
        res = FALSE;
    }
    return res;
}
 
/*
***********************************************************************************************
*                                     QMC5883L_Read_MagDensity
*
* Description: QMC5883L��ȡ�ų�ǿ��
*
* Argument(s): ��
*
* Return(s) :  ��������ȡ���ݽ��
*
* Caller(s) :
*
***********************************************************************************************
*/
QMC5883L_RESULT_SAMPLE QMC5883L_Read_MagDensity(void)
{
    CPU_INT08U i,param_vali;
    CPU_INT16U MagDensity = 0x00;
    CPU_INT16S fx = 0x00, fy = 0x00, fz = 0x00;
    CPU_INT16S  x = 0x00,  y = 0x00,  z = 0x00;
    CPU_INT16S Temp = 0x00;
    SENSOR_DATA_ELEMENT_T Q_Sample;
    /* ��鴫���� */
    if(QMC5883L_Register_Read(QMC5883L_ADDR_CHIPID) != QMC5883L_CHIPID_VALUE)
    {
        QMC5883LSample.HardFault = TRUE;
        return QMC5883L_DATA_ERROR_ADDR;
    }
    else
    {
        QMC5883LSample.HardFault = FALSE;
    }
    /* QMC5883L��������ģʽ */
    QMC5883L_Register_Write(QMC5883L_ADDR_CFGA ,QMC5883L_CFGA_VALUE_CONTINUE);
    for(i=0;i<2;i++)
    {
        if(QMC5883L_Wait_Data_Update(6))
        {
            QMC5883L_Read_XYZ(&x,&y,&z);
        }
        else
        {
            QMC5883L_Register_Write(QMC5883L_ADDR_CFGA ,QMC5883L_CFGA_VALUE_STANDBY);
            return QMC5883L_DATA_ERROR_TIMEOUT;
        }
    }
    /* QMC5883L��������ģʽ */
    QMC5883L_Register_Write(QMC5883L_ADDR_CFGA ,QMC5883L_CFGA_VALUE_STANDBY);
 
#ifdef QMC5883L_READ_TEMP
    QMC5883L_Read_TEMP(&Temp);
#endif
    if(((x<3) && (x>-3)) && ((y<3) && (y>-3)) && ((z<3) && (z>-3)))
    {
        param_vali = FALSE;
        QMC5883LSample.DetectInvalid = TRUE;
    }
    else
    {
        param_vali = TRUE;
        QMC5883LSample.DetectInvalid = FALSE;
    }
    
    QMC5883LSample.X_Data = x;
    QMC5883LSample.Y_Data = y;
    QMC5883LSample.Z_Data = z;
    
    fx = QMC5883LSample.X_Data - QMC5883LSave.X_Offset;
    fy = QMC5883LSample.Y_Data - QMC5883LSave.Y_Offset;
    fz = QMC5883LSample.Z_Data - QMC5883LSave.Z_Offset;
    MagDensity = (CPU_INT16U)(sqrt(fx*fx+fy*fy+fz*fz));
    
    Q_Sample.Bx    = fx;
    Q_Sample.By    = fy;
    Q_Sample.Bz    = fz;
    Q_Sample.Temp  = Temp;
    Q_Sample.BNorm = MagDensity;
    Q_Enqueue(&Q_Sample,&Q_CtlM);
    
    if(!param_vali)
    {
        return QMC5883L_DATA_ERROR_VALUE;
    }
    return QMC5883L_DATA_SUCCESS;
}
 
/*
***********************************************************************************************
*                                     QMC5883L_HardState
*
* Description: ��ȡ������Ӳ���Ƿ���
*
* Argument(s): ��
*
* Return(s) :  ������Ӳ���𻵽��
*
* Caller(s) :
*
***********************************************************************************************
*/
CPU_BOOLEAN QMC5883L_HardState(void)
{
    return QMC5883LSample.HardFault;
}
 
/*
***********************************************************************************************
*                                     QMC5883L_DetectState
*
* Description: ��ȡ�������ɼ��Ƿ�ʧЧ
*
* Argument(s): ��
*
* Return(s) :  �������ɼ�ʧЧ���
*
* Caller(s) :
*
***********************************************************************************************
*/
CPU_BOOLEAN QMC5883L_DetectState(void)
{
    return QMC5883LSample.DetectInvalid;
}